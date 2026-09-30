#include <stdlib.h>
#include <avr/io.h>
#include <stdio.h>
#include "include/drivere/uart.h"
#include "include/sram.h"
#include "include/drivere/adc.h"
#include "include/joystick.h"
#include "include/sleep.h" 
#include "include/drivere/spi.h"
#include "include/drivere/ioBoard.h"
#include "include/drivere/display.h"

int main(void){
	setupPrintf();	
	setupSRAM();
	setupADC();
	setupSPI();
	setupDisplay();
	//joystickCalibrate();

	uint8_t n = 0;
	while(1){
		continue;
	}
}

