#include <avr/io.h>
#include <stdlib.h>
#include "..\..\include\drivere\spi.h"
#include "..\..\include\sleep.h"
#include "..\..\include\drivere\uart.h"

// PB1 Can
// PB3 Display
// PB4 IO 

static void unSelectAllSlavesSPI();
static void selectSlaveSPI(SPIselect slave);

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

void writeByteSPI(uint8_t data, SPIselect slave){
	selectSlaveSPI(slave);
	SPDR = data;
	while(!(SPSR & (1 << 7))){}
	unSelectAllSlavesSPI();
}

void writeSPI(uint8_t* data, uint16_t n, SPIselect slave){
	// Bruker ikke writeByteSPI, siden den da kaller slaveSelect for hver byte
	selectSlaveSPI(slave);
	for(uint16_t i = 0; i<n; i++){
		SPDR = data[i];
		while(!(SPSR & (1 << 7))){}
	}
	unSelectAllSlavesSPI();
}

uint8_t readByteSPI(SPIselect slave){
	writeByteSPI(0, slave);
	return SPDR;
}

void readSPI(uint8_t* data, uint16_t n, SPIselect slave){
	selectSlaveSPI(slave);
	for(uint16_t i = 0; i<n; i++){
		SPDR = 0;
		while(!(SPSR & (1 << 7))){}
		data[i] = SPDR;
	}
	unSelectAllSlavesSPI();
}
