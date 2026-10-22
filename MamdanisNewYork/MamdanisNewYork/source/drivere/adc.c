#include "sam.h"
#include <stdio.h>
#include "../../include/drivere/adc.h"

void adc_setup(){
	PMC->PMC_PCER1 = (1<<5); // aktiverer klokkke for adc
	ADC->ADC_CR = (1<<0); // restarter adc
	ADC->ADC_MR = 0x031F0110;  // Konfigurer ADC, 10-bit resolution
	ADC->ADC_CHER = (1<<6); // enabler adc channel 1
}

uint32_t adc_read(){
	ADC->ADC_CR = (1<<1); // starter konvertering
	while(!(ADC->ADC_ISR & (1<<24)));
	uint16_t data = (uint16_t)ADC->ADC_LCDR;
	data &= ~(0x3f<<10); // setter alle bit unntatt de 10 nederste til 0.
	return (uint32_t)data;
}