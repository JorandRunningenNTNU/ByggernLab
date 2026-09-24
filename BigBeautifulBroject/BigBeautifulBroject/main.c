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
#include "include/fonts.h"

int main(void){
	setupPrintf();	
	setupSRAM();
	setupADC();
	setupSPI();
	setupDisplay();
	//joystickCalibrate();
	
	uint8_t n = 0;
	while(1){
		uint8_t test[8];
		for(uint8_t i = 0; i<8; i++){
			test[i] = pgm_read_byte(&font8[34][i]);
		}
		sendData(test, 8);
		//updateIoData();
		//printf("X: %d, Y %d, Btn %d \n", ioData.joystickX, ioData.joystickY, ioData.joystickButton);
		sleep(100);
		n++;
		if(n == 100){n = 0;}
	}
	
	
	/*JoysticAnalog JSA;
	JoystickDiscrete JSD;
	
	while(1){
		JSA = readJoystickAnalog();
		JSD = readJoystickDiscrete();
		
		if (JSD == Up){printf("X: %d	Y: %d  opp\n", JSA.X, JSA.Y);}
		if (JSD == Down){printf("X: %d	Y: %d  ned\n", JSA.X, JSA.Y);}
		if (JSD == Left){printf("X: %d	Y: %d  venstre\n", JSA.X, JSA.Y);}
		if (JSD == Right){printf("X: %d	Y: %d  hoyre\n", JSA.X, JSA.Y);}
		if (JSD == Neutral){printf("X: %d	Y: %d  midt\n", JSA.X, JSA.Y);}
		
		sleep(100);
	}*/

	
	
	/*
	char n = 0;
	while(1){
		n++;
		if (n == 255){
			n = 0;
		}
		sendByte(n);
	}
	*/
	
}

