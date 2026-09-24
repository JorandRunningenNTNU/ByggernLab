#include <avr/io.h>
#include <stdlib.h>
#include "..\..\include\drivere\spi.h"
#include "..\..\include\sleep.h"
#include "..\..\include\drivere\uart.h"

// PB1 Can
// PB3 Display
// PB4 IO 

void setupSPI(){
	DDRB |= (1 << 7) | (1 << 5) | (1 << 4) | (1 << 3) | (1 << 1); // PBn som output
	unSelectAllSlavesSPI();
	
	// valgte innstillinger
	// MSB sendes først
	// klokken er lav i ideal state
	// Sampler på leading edge
	// SCK frekvens er f_osc/4, antar f_osc er den interne frekvensen
	SPCR |= (1 << 4); // setter atmega til master
	SPCR |= (1 << 6); // enable SPI
	
}

void unSelectAllSlavesSPI(){
	PORTB |= (1 << 4) | (1 << 3) | (1 << 1);
}

void selectSlaveSPI(SPIselect slave){
	unSelectAllSlavesSPI();
	
	if (slave == CAN){PORTB &= ~(1 << 1);}
	if (slave == Display){PORTB &= ~(1 << 3);}
	if (slave == IO){PORTB &= ~(1 << 4);}
}

void writeByteSPI(uint8_t data){
	SPDR = data;
	while(!(SPSR & (1 << 7))){}
}

void writeSPI(uint8_t* data, uint16_t n){
	for(uint16_t i = 0; i<n; i++){
		writeByteSPI(data[i]);
	}
}

uint8_t readByteSPI(){
	writeByteSPI(0);
	return SPDR;
}

void readSPI(uint8_t* data, uint16_t n){
	for(uint16_t i = 0; i<n; i++){
		data[i] = readByteSPI();
	}
}
