#include "uart.h"
#include <avr/interrupt.h>
#include <util/atomic.h>

#define UART_TX_BUFFER_SIZE 128

volatile uint8_t tx_buffer[UART_TX_BUFFER_SIZE];
volatile uint8_t tx_head = 0;
volatile uint8_t tx_tail = 0;

void sendByte(unsigned char data){
	// vente på at bufferen er klar til å skrives til
	while(!(UCSR0A & (1<<5)));
	
	UDR0 =  data;
}

ISR(USART0_UDRE_vect)
{
	if (tx_head == tx_tail) {
		// Nothing left to send
		UCSR0B &= ~(1 << UDRIE0);
	}
	else {
		UDR0 = tx_buffer[tx_tail];
		tx_tail = (tx_tail + 1) % UART_TX_BUFFER_SIZE;
	}
}

void sendByteINT(unsigned char c){
	uint8_t next;

	next = (tx_head + 1) % UART_TX_BUFFER_SIZE;

	// Vent på plass i bufferen
	while (next == tx_tail) {
		if (!(SREG & (1 << SREG_I))) {
			// Interrupts er av, så ISR-en kan ikke tømme bufferen.
			// Gjør det manuelt.
			if (UCSR0A & (1 << UDRE0)) {
				UDR0 = tx_buffer[tx_tail];
				tx_tail = (tx_tail + 1) % UART_TX_BUFFER_SIZE;
			}
		}
		// Hvis interrupts er på, tømmer ISR-en bufferen av seg selv
	}
	
	tx_buffer[tx_head] = c;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		tx_head = next;
		UCSR0B |= (1 << UDRIE0);
	}
}

unsigned char uart_read_byte(){
	int n = 10000;
	while(!(UCSR0A & (1<<7)) || (n--));
	
	// returnerer 254 om ingen ting leses
	if (n == 0){
		return 254;
	}
	
	return UDR0;
}



//''''''''''''' sette opp printf '''''''''''''''''''''''
static int uart_putchar(char c, FILE *stream){
	if (c == '\n') {
		sendByteINT('\r');
	}

	sendByteINT((unsigned char) c);
	return 0;
}


static int uart_getchar(FILE *stream){
	return uart_read_byte();
}


void setupUART(){
	// baud-rate 9600
	UBRR0H = 0;
	UBRR0L = 31;
		
	// enable reciver og transmitter
	UCSR0B |= (1<<TXEN0)|(1<<RXEN0);
		
	sei();
	
	//setup printf
	FILE *uart = fdevopen(uart_putchar, uart_getchar);
	stdout = uart;
	stdin = uart;
}

