#include "sam.h"
#include <stdio.h>
#include "adc.h"
#include "our_timer.h"
#include "can.h"
#include "can_interface.h"

volatile uint16_t adc_value;
volatile uint8_t  adc_new_sample = 0;
static volatile uint8_t death_message_sent = 0;

#define DEATH_THRESHOLD 200

void adc_setup(void){
	PMC->PMC_PCER1 = PMC_PCER1_PID37; //(1<<5); // aktiverer klokkke for adc
	ADC->ADC_CR = ADC_CR_SWRST; // (1<<0); // restarter adc
	
	//0x031F0110 Konfigurer ADC, 10-bit resolution
	ADC->ADC_MR =	(1u << 4) | 
					ADC_MR_PRESCAL(2) |
					ADC_MR_STARTUP_SUT960 |
					ADC_MR_SETTLING_AST3 |
					ADC_MR_TRACKTIM(3); 
	ADC->ADC_CHER = ADC_CHER_CH6; // (1<<6); // enabler adc channel 1?
	
	ADC->ADC_IER = ADC_IER_EOC6;      // EOC for channel 6 (or (1<<24) for DRDY on any channel)
	NVIC_EnableIRQ(ADC_IRQn);         // optional: NVIC_SetPriority(ADC_IRQn, x);
	
	cooler_timer_t timer_config = {
		.callback = adc_start_read,
		.frequency = 100
	};
	
	timer_channel_init(&timer_config,1);	
}

uint32_t adc_read(void){
	ADC->ADC_CR = (1<<1); // starter konvertering
	while(!(ADC->ADC_ISR & ADC_ISR_DRDY));
	uint16_t data = (uint16_t)ADC->ADC_LCDR;
	data &= ~(0x3f<<10); // setter alle bit unntatt de 10 nederste til 0.
	return (uint32_t)data;
}

void adc_start_read(void){
	ADC->ADC_CR = (1<<1); // starter konvertering
	adc_new_sample = 0;
}

uint32_t adc_read_newest_result(void){
	uint16_t data = (uint16_t)ADC->ADC_LCDR;
	data &= ~(0x3f<<10); // setter alle bit unntatt de 10 nederste til 0.
	return (uint32_t)data;
}



void ADC_Handler(void){
	if (ADC->ADC_ISR & ADC_ISR_EOC6){
		adc_value = ADC->ADC_CDR[6] & 0x3FF;   // reading CDR clears EOC6
		adc_new_sample = 1;
		if(adc_value < DEATH_THRESHOLD){
			if(!death_message_sent){
				CanMsg m = {
					.id = CAN_DEATH_ID,
					.length = 1,
					.byte = 1
				};
				death_message_sent = can_tx_enqueue(m);
			}
		}else{
			death_message_sent = 0;
		}
	}
}