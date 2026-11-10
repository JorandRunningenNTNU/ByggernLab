#include "sam.h"
#include <stdio.h>
#include "adc.h"
#include "our_timer.h"
#include "can.h"
#include "can_interface.h"

#define ADC_MR_LOWRES (1u << 4)

#define DEATH_THRESHOLD  (850u << 2)  // 3400
#define ALIVE_THRESHOLD  (900u << 2)  // 3600

#define ADC_CHANNEL 6

adc_return_t adc_setup(void){
	PMC->PMC_PCER1 = PMC_PCER1_PID37;	// aktiverer klokkke for adc
	ADC->ADC_CR = ADC_CR_SWRST;			// restarter adc
	
	//konfigurer ADC
	ADC->ADC_MR =	ADC_MR_PRESCAL(2) |		//Vi kjører den raskt med margin (84MHz /((1+1)*2) < ADC_CLKMAX
					ADC_MR_STARTUP_SUT960 |	//Vi gir den god tid til å starte opp
					ADC_MR_SETTLING_AST3 |	// Vi bytter aldri kanal så denne er minimal
					ADC_MR_TRACKTIM(1) |	// Opampen har lav utgangsimpedans så vi kan ha lite trackingtid
					ADC_MR_TRANSFER(1) |		// Litt transfer tid
					ADC_MR_TRGEN_EN |
					ADC_MR_FREERUN;
	

	ADC->ADC_EMR =  ADC_EMR_CMPALL | 
					ADC_EMR_CMPMODE_LOW;
					
	ADC->ADC_CWR = ADC_CWR_HIGHTHRES((uint16_t)ALIVE_THRESHOLD) | ADC_CWR_LOWTHRES((uint16_t)DEATH_THRESHOLD);
	
	ADC->ADC_IDR = 0xFFFFFFFF;			// disable alle interrupts
	ADC->ADC_IER = ADC_IER_COMPE;      // enable comparrisson interrupt for kanal 6
	NVIC_EnableIRQ(ADC_IRQn);
	NVIC_SetPriority(ADC_IRQn, 2);
	
	ADC->ADC_CHER = ADC_CHER_CH6; // (1<<6); // enabler adc channel 1
	ADC->ADC_CR = ADC_CR_START;
	
	return ADC_ALL_GOOD;
}

uint32_t adc_read_newest_result(void){
	uint16_t data = (uint16_t)ADC->ADC_LCDR;
	//data &= ~(0x3f<<10); // setter alle bit unntatt de 10 nederste til 0.
	return (uint32_t)data;	
}


void ADC_Handler(void){
	uint32_t ISR = ADC->ADC_ISR;
	if (ISR & ADC_ISR_COMPE){
		if((ADC->ADC_EMR & ADC_EMR_CMPMODE_Msk) == ADC_EMR_CMPMODE_LOW){
				CanMsg m = {
					.id = CAN_DEATH_ID,
					.length = 1,
					.byte = {1}
				};
				if(can_tx_try(m)){
					ADC->ADC_EMR &= ~ADC_EMR_CMPMODE_Msk;
					ADC->ADC_EMR |= ADC_EMR_CMPMODE_HIGH;
				}
		}else{
			ADC->ADC_EMR &= ~ADC_EMR_CMPMODE_Msk;
			ADC->ADC_EMR |= ADC_EMR_CMPMODE_LOW;
		}
	}
}