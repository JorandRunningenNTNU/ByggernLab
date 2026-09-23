#include <avr/io.h>
#include <stdlib.h>
#include "..\..\include\drivere\display.h"
#include "..\..\include\drivere\spi.h"

void setupDisplay(){
	DDRB |= (1 << 2);
	
	uint8_t command[2];
	
	command[0] = 0xAE;
	sendCommand(command, 1); // Turns display off (resets) for configuration
	
	command[0] = 0xD5; // Set display clock divide ratio/oscillator frequency
	command[1] = 0xF0; // 31 oscillator value 1 divide ratio.
	sendCommand(command, 2);
	
	command[0] = 0xA8; // Set command
	command[1] = 0x3F ;// 1/64 duty (64 rows)
	sendCommand(command, 2);

	command[0] = 0xD3; // Set display offset
	command[1] = 0x00; // Offset 0
	sendCommand(command, 2);
	
	command[0] = 0x40;
	sendCommand(command, 1); // Start line 0
	
	command[0] = 0xA0;
	sendCommand(command, 1); // Segment remap
	
	command[0] = 0xC0;
	sendCommand(command, 1); // COM scan direction
	
	command[0] = 0xDA; // Set COM pins hardware configuration
	command[1] = 0x2;
	sendCommand(command, 2);
	
	command[0] = 0x81; // Set contrast
	command[1] = 0xFF; // Contrast value
	sendCommand(command, 2);
	
	command[0] = 0xA6;
	sendCommand(command, 1); // Normal display
	command[0] = 0xA4;
	sendCommand(command, 1); // Display follows ram
	command[0] = 0xAF;
	sendCommand(command, 1); // Display ON
}

void sendCommand(uint8_t* command, uint8_t n) {
	PORTB &= ~(1 << 2); // Sets Data/Command to low
	writeSPI(command, n, Display); //Writes command
}

void sendData(uint8_t* data, uint8_t n) {
	PORTB |= (1 << 2); // Sets Data/command to high
	writeSPI(data, n, Display);
}