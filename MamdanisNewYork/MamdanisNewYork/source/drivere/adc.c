#include "sam.h"
#include <stdio.h>
#include "adc.h"
#include "our_timer.h"
#include "can.h"
#include "can_interface.h"

#define ADC_MR_LOWRES (1u << 4)

volatile uint16_t adc_value;
volatile uint8_t  adc_new_sample = 0;
static volatile uint8_t death_message_sent = 0;

#define DEATH_THRESHOLD 200

adc_return_t adc_setup(const uint32_t frequency){
	PMC->PMC_PCER1 = PMC_PCER1_PID37; // aktiverer klokkke for adc
	ADC->ADC_CR = ADC_CR_SWRST; // restarter adc
	
	//konfigurer ADC
	ADC->ADC_MR =	ADC_MR_LOWRES | //10-bit resolution
					ADC_MR_PRESCAL(2) | 
					ADC_MR_STARTUP_SUT960 |
					ADC_MR_SETTLING_AST3 |
					ADC_MR_TRACKTIM(3); 
	ADC->ADC_CHER = ADC_CHER_CH6; // (1<<6); // enabler adc channel 1
	
	ADC->ADC_IER = ADC_IER_EOC6;      // enable end of conversion interrupt for kanal 6
	NVIC_EnableIRQ(ADC_IRQn);         // todo: NVIC_SetPriority(ADC_IRQn, x);
	
	if(timer_channel_init(frequency, adc_start_read,1) == TIM_ALL_GOOD){
		return ADC_ALL_GOOD;
	}
	return ADC_INIT_ERROR;
}

uint32_t adc_read(void){
	ADC->ADC_CR = ADC_CR_START; // starter konvertering
	while(!(ADC->ADC_ISR & ADC_ISR_DRDY));
	uint16_t data = (uint16_t)ADC->ADC_LCDR;
	data &= ~(0x3f<<10); // setter alle bit unntatt de 10 nederste til 0.
	return (uint32_t)data;
}

adc_return_t adc_start_read(void){
	ADC->ADC_CR = ADC_CR_START; // starter konvertering
	adc_new_sample = 0;
}

uint32_t adc_read_newest_result(void){
	uint16_t data = (uint16_t)ADC->ADC_LCDR;
	data &= ~(0x3f<<10); // setter alle bit unntatt de 10 nederste til 0.
	return (uint32_t)data;
}



void ADC_Handler(void){
	if (ADC->ADC_ISR & ADC_ISR_EOC6){
		adc_value = ADC->ADC_CDR[6] & 0x3FF;   // lesing af CDR6 clearer EOC6
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