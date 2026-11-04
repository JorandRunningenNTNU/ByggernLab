
#include "sam.h"
#include "can.h"
#include "can_interface.h"
#include <stdio.h>
#include <stdbool.h>

void can_setup(){
	// Timing
	// TQ = 2(BRP+1)/F_CPU = 2(20 + 1) / 84 MHz = 500 ns
	// SJW = 1 TQ
	// Sync = 1 TQ
	// Prop = 2 TQ
	// PS1 = 3 TQ
	// PS2 = 2 TQ
	can_init((CanInit){.brp = 104, // en gruppe som fikk til lab 6 brukte 104 ikke 20
		               .sjw = 0,
		               .propag = 1,
					   .phase1 = 2,
					   .phase2 = 1,
					   .smp = 0}, 1, 1);
}

void can_printmsg(CanMsg m){
    printf("CanMsg(id:%d, length:%d, data:{", m.id, m.length);
    if(m.length){
        printf("%d", m.byte[0]);
    }
    for(uint8_t i = 1; i < m.length; i++){
        printf(", %d", m.byte[i]);
    }
    printf("})\n");
}


#define txMailbox 0
#define rxMailbox 1


void can_init(CanInit init, uint8_t rxInterrupt, uint8_t txInterrupt){
    // Disable CAN
    CAN0->CAN_MR &= ~CAN_MR_CANEN; 
    
    // Clear status register by reading it
    __attribute__((unused)) uint32_t ul_status = CAN0->CAN_SR;     
    
    // Disable interrupts on CANH and CANL pins
    PIOA->PIO_IDR = PIO_PA8A_URXD | PIO_PA9A_UTXD;
    
    // Select CAN0 RX and TX in PIOA
    PIOA->PIO_ABSR &= ~(PIO_PA1A_CANRX0 | PIO_PA0A_CANTX0);
    
    // Disable the Parallel IO (PIO) of the Rx and Tx pins so that the peripheral controller can use them
    PIOA->PIO_PDR = PIO_PA1A_CANRX0 | PIO_PA0A_CANTX0;
    
    // Enable pull up on CANH and CANL pin
    PIOA->PIO_PUER = (PIO_PA1A_CANRX0 | PIO_PA0A_CANTX0);    
    
    // Enable Clock for CAN0 in PMC
    // DIV = 1 (can clk = MCK/2), CMD = 1 (write), PID = 2B (CAN0)
    PMC->PMC_PCR = PMC_PCR_EN | (1 << PMC_PCR_DIV_Pos) | PMC_PCR_CMD | (ID_CAN0 << PMC_PCR_PID_Pos); 
    PMC->PMC_PCER1 |= 1 << (ID_CAN0 - 32);
    
    //Set baudrate, Phase1, phase2 and propagation delay for can bus. Must match on all nodes!
    CAN0->CAN_BR = init.reg; 
    


    // Configure mailboxes
    // transmit
    //CAN0->CAN_MB[txMailbox].CAN_MID = CAN_MID_MIDE; // ønkser ikke extended ID
    CAN0->CAN_MB[txMailbox].CAN_MMR = CAN_MMR_MOT_MB_TX;
    
    // receive
    CAN0->CAN_MB[rxMailbox].CAN_MAM = 0; // Accept all messages
    //CAN0->CAN_MB[rxMailbox].CAN_MID = CAN_MID_MIDE; // ønsker ikke extended ID
    CAN0->CAN_MB[rxMailbox].CAN_MMR = CAN_MMR_MOT_MB_RX;
    CAN0->CAN_MB[rxMailbox].CAN_MCR |= CAN_MCR_MTCR;
    if(rxInterrupt){
        // Enable interrupt on receive
        CAN0->CAN_IER |= (1 << rxMailbox); 
        // Enable interrupt in NVIC 
        NVIC_EnableIRQ(ID_CAN0);
    }
	
	if(txInterrupt){
		// Enable interrupt on receive
		CAN0->CAN_IER |= (1 << rxMailbox);
		// Enable interrupt in NVIC
		NVIC_EnableIRQ(ID_CAN0);
	}
	
	if(rxInterrupt){
		// Enable interrupt on receive
		CAN0->CAN_IER = (1u << rxMailbox);    // RX mailbox interrupt
	}

	if(rxInterrupt || txInterrupt){
		// All interrupts
		NVIC_EnableIRQ(CAN0_IRQn);            // one call is enough
	}

    // Enable CAN
    CAN0->CAN_MR |= CAN_MR_CANEN;
}


void can_tx(CanMsg m){
    while(!(CAN0->CAN_MB[txMailbox].CAN_MSR & CAN_MSR_MRDY)){}
    
    // bruker bare 8 bit ID, og det er de øverste 8 av 11 bitene
    CAN0->CAN_MB[txMailbox].CAN_MID = CAN_MID_MIDvA((m.id<<3)) /*| CAN_MID_MIDE*/ ;
        
    // Coerce maximum 8 byte length
    m.length = m.length > 8 ? 8 : m.length;
    
    //  Put message in can data registers
    CAN0->CAN_MB[txMailbox].CAN_MDL = m.dword[0];
    CAN0->CAN_MB[txMailbox].CAN_MDH = m.dword[1];
        
    // Set message length and mailbox ready to send
    CAN0->CAN_MB[txMailbox].CAN_MCR = (m.length << CAN_MCR_MDLC_Pos) | CAN_MCR_MTCR;
}

#define TXQ_SIZE 8
static CanMsg           txq[TXQ_SIZE];
static volatile uint8_t q_head, q_tail;   // head: written by producer, tail: written by ISR

bool can_tx_enqueue(CanMsg m){            // single producer only
	uint8_t next = (q_head + 1) % TXQ_SIZE;
	if(next == q_tail) return false;      // full
	txq[q_head] = m;
	q_head = next;
	CAN0->CAN_IER = (1u << txMailbox);    // enable TX-ready interrupt
	NVIC_SetPendingIRQ(CAN0_IRQn);        // kick-start the sending
	return true;
}

uint8_t can_rx(CanMsg* m){
    if(!(CAN0->CAN_MB[rxMailbox].CAN_MSR & CAN_MSR_MRDY)){
        return 0;
    }

    // Get message ID, bruker bare 8 bit ID, og det er de øverste 8 av 11 bitene
    m->id = ((uint8_t)((CAN0->CAN_MB[rxMailbox].CAN_MID & CAN_MID_MIDvA_Msk) >> CAN_MID_MIDvA_Pos)>>3);
        
    // Get data length
    m->length = (uint8_t)((CAN0->CAN_MB[rxMailbox].CAN_MSR & CAN_MSR_MDLC_Msk) >> CAN_MSR_MDLC_Pos);
    
    // Get data from CAN mailbox
    m->dword[0] = CAN0->CAN_MB[rxMailbox].CAN_MDL;
    m->dword[1] = CAN0->CAN_MB[rxMailbox].CAN_MDH;
                
    // Reset for new receive
    CAN0->CAN_MB[rxMailbox].CAN_MMR = CAN_MMR_MOT_MB_RX;
    CAN0->CAN_MB[rxMailbox].CAN_MCR |= CAN_MCR_MTCR;
    return 1;
}

bool can_tx_try(CanMsg m){
	if(!(CAN0->CAN_MB[txMailbox].CAN_MSR & CAN_MSR_MRDY)){
		return false;                       // busy: caller decides what to do
	}
	m.length = m.length > 8 ? 8 : m.length;
	CAN0->CAN_MB[txMailbox].CAN_MID = CAN_MID_MIDvA(m.id << 3);
	CAN0->CAN_MB[txMailbox].CAN_MDL = m.dword[0];
	CAN0->CAN_MB[txMailbox].CAN_MDH = m.dword[1];
	CAN0->CAN_MB[txMailbox].CAN_MCR = (m.length << CAN_MCR_MDLC_Pos) | CAN_MCR_MTCR;
	return true;
}


// Example CAN interrupt handler
void CAN0_Handler(void){
	char can_sr = CAN0->CAN_SR;
	
	const uint32_t rxbit = 1u << rxMailbox;
	const uint32_t txbit = 1u << txMailbox;
	
	// TX: send next queued frame if the mailbox is free
	if(CAN0->CAN_IMR & txbit){            // only while TX interrupt is enabled
		if(q_tail != q_head){
			if(can_tx_try(txq[q_tail])){  // returns false if mailbox still busy
				q_tail = (q_tail + 1) % TXQ_SIZE;
			}
		}
		if(q_tail == q_head){
			CAN0->CAN_IDR = txbit;        // stops the interrupt when the queue is empty
		}
	}
	
	// RX interrupt handler
	if(can_sr & rxbit){
		CanMsg msg;
		can_rx(&msg);
		can_interface_handle_message(&msg);
		can_printmsg(msg);
	}
	
	if(can_sr & CAN_SR_MB0){
		// Disable interrupt
		CAN0->CAN_IDR = CAN_IER_MB0;
	}
	
	NVIC_ClearPendingIRQ(ID_CAN0);
}