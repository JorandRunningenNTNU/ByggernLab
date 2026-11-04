#include "sam.h"
#include <stdio.h>
#include "adc.h"

volatile uint16_t adc_value;
volatile uint8_t  adc_new_sample = 0;

void adc_setup(){
	PMC->PMC_PCER1 = (1<<5); // aktiverer klokkke for adc
	ADC->ADC_CR = (1<<0); // restarter adc
	ADC->ADC_MR = 0x031F0110;  // Konfigurer ADC, 10-bit resolution
	ADC->ADC_CHER = (1<<6); // enabler adc channel 1
	
	ADC->ADC_IER = ADC_IER_EOC6;      // EOC for channel 6 (or (1<<24) for DRDY on any channel)
	NVIC_EnableIRQ(ADC_IRQn);         // optional: NVIC_SetPriority(ADC_IRQn, x);
}

uint32_t adc_read(){
	ADC->ADC_CR = (1<<1); // starter konvertering
	while(!(ADC->ADC_ISR & (1<<24)));
	uint16_t data = (uint16_t)ADC->ADC_LCDR;
	data &= ~(0x3f<<10); // setter alle bit unntatt de 10 nederste til 0.
	return (uint32_t)data;
}

void ADC_Handler(void){
	if (ADC->ADC_ISR & ADC_ISR_EOC6){
		adc_value = ADC->ADC_CDR[6] & 0x3FF;   // reading CDR clears EOC6
		adc_new_sample = 1;
	}
}