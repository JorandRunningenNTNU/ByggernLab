#include <avr/io.h>
#include <stdlib.h>
#include "..\..\include\drivere\ioBoard.h"
#include "..\..\include\drivere\spi.h"
#include "..\..\include\sleep.h"

IoData ioData;

void updateIoData(){
	selectSlaveSPI(IO);
	writeByteSPI(0x03);
	sleepUs(40);
	uint8_t data[3];
	readSPI(data, 3);
	ioData.joystickX = data[0];
	ioData.joystickY = data[1];
	ioData.joystickButton = data[2];
	unSelectAllSlavesSPI();
}