#include <avr/io.h>
#include <stdlib.h>
#include "..\include\drivere\spi.h"

// PB1 Can
// PB3 Display
// PB4 IO 

void setupSPI(){
	DDRB |= (1 << 4) | (1 << 3) | (1 << 1); // PBn som output
	
	/////////////// IKKKE FERDIG /////////////////////////////////////////////
}

void slectSlaveSPI(SPIselect slave){
	PORTB |= (1 << 4) | (1 << 3) | (1 << 1);
	
	if (slave = CAN){PORTB &= 0xfd;}
	if (slave = Display){PORTB &= 0xf7;}
	if (slave = IO){PORTB &= 0xef;}
}