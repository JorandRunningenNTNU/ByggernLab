#include "../../include/drivere/adc.h"

#define ADC (*(volatile uint8_t *) 0x13FF)

void setupADC(){
	// sette opp timer på PD4
	DDRD |= (1 << 4); // gjøre PD4 til output
	TCCR3A |= (1 << 6);
	TCCR3B |= (1 << 3) | (1 << 0); // CTC modus | ingen prescaling
	OCR3A = 0;
	
	// bruker reversert logikk, pga PA0 koblet til D7
	ADC = (1 << 3) | (0 << 7) | (0 << 6);  // inh | A0 | A1
	ADC = (1 << 3) | (0 << 7) | (1 << 6);
	ADC = (1 << 3) | (1 << 7) | (0 << 6);
	ADC = (1 << 3) | (1 << 7) | (1 << 6);
}

uint8_t reverseBitOrder(uint8_t data){
	uint8_t dataReversed = 0;
	for (uint8_t i = 0; i<8; i++){
		dataReversed |= (data & 1) << (7-i);
		data >>= 1;
	}
	return dataReversed;
}


void readADC(uint8_t *p_data){
	ADC = 0; // trigger måling
	while(PIND & (1 << 3)){} // venter på at ADC-en blir busy
	while(!(PIND & (1 << 3))){} // vente til måling er ferdig 
	
	// legge alle målingene inn i p_data
	for (uint8_t i = 0; i<4; i++){
		p_data[i] =  reverseBitOrder(ADC);
	}	
}


