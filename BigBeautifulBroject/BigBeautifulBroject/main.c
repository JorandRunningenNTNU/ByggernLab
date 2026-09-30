#include <stdlib.h>
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "include/drivere/uart.h"
#include "include/sram.h"
#include "include/drivere/adc.h"
#include "include/joystick.h"
#include "include/sleep.h" 
#include "include/drivere/spi.h"
#include "include/drivere/ioBoard.h"
#include "include/drivere/display.h"
#include "include/test.h"

int main(void){
	setupPrintf();	
	setupSRAM();
	setupADC();
	setupSPI();
	setupDisplay();
	//joystickCalibrate();

	setupTest();
	while(1){

		
		whileTest();
		continue;
	}
}

