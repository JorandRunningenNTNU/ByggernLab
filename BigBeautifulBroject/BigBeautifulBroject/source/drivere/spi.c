#include <avr/io.h>
#include <stdlib.h>
#include "..\include\drivere\spi.h"

// PB1 Can
// PB3 Display
// PB4 IO 

void setupSPI(){
	DDRB |= (1 << 7) | (1 << 5) | (1 << 4) | (1 << 3) | (1 << 1); // PBn som output
	PORTB |= (1 << 4) | (1 << 3) | (1 << 1); // Setter alle slaveSelect høy, så ingen ingen enheter kobles til
	
	// valgte innstillinger
	// MSB sendes først
	// klokken er lav i ideal state
	// Sampler på leading edge
	// SCK frekvens er f_osc/4, antar f_osc er den interne frekvensen
	SPCR |= (1 << 4); // setter atmega til master
	
}

void slectSlaveSPI(SPIselect slave){
	PORTB |= (1 << 4) | (1 << 3) | (1 << 1);
	
	if (slave = CAN){PORTB &= 0xfd;}
	if (slave = Display){PORTB &= 0xf7;}
	if (slave = IO){PORTB &= 0xef;}
}