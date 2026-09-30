#include <avr/io.h>
#include <stdlib.h>
#include "..\..\include\drivere\ioBoard.h"
#include "..\..\include\drivere\spi.h"
#include "..\..\include\sleep.h"

static void readData(uint8_t adress, uint8_t* data, uint8_t nBytes);

IoData ioData;


void updateIoData(){
	uint8_t data[3];
	
	// lese touchpad
	readData(0x01, data, 3);
	ioData.touchpadX = data[0];
	ioData.touchpadY = data[1];
	ioData.touchpadSize = data[2];
	
	// lese touchslider
	readData(0x02, data, 2); 
	ioData.touchSliderX = data[0];
	ioData.touchSliderSize = data[1];
	
	// lese joystick
	readData(0x03, data, 3);
	ioData.joystickX = data[0];
	ioData.joystickY = data[1];
	ioData.joystickButton = data[2];
	
	// lese buttons
	readData(0x04, data, 3);
	ioData.rightButtons = data[0];
	ioData.leftButtons = data[1];
	ioData. navButton = data[2];
	
}

void readData(uint8_t adress, uint8_t* data, uint8_t nBytes){
	selectSlaveSPI(IO);
	writeByteSPI(adress);
	sleepUs(40);
	readSPI(data, nBytes);
	unSelectAllSlavesSPI();
}

void turnOnLeds(uint8_t led, uint8_t on){
	led -=1;
	selectSlaveSPI(IO);
	writeByteSPI(0x05);
	sleepUs(40);
	writeByteSPI(led);
	writeByteSPI(on);
	unSelectAllSlavesSPI();
}

void turnOnLedsPWM(uint8_t led, uint8_t brightness){
	led -=1;
	selectSlaveSPI(IO);
	writeByteSPI(0x06);
	sleepUs(40);
	writeByteSPI(led);
	writeByteSPI(brightness);
	unSelectAllSlavesSPI();
}



