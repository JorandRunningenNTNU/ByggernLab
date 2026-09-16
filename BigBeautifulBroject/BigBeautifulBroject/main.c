#include <stdlib.h>
#include <avr/io.h>
#include <stdio.h>
#include "include/drivere/uart.h"
#include "include/sram.h"
#include "include/drivere/adc.h"
#include "include/joystick.h"
#include "include/sleep.h" 

int main(void){
	setupPrintf();	
	setupSRAM();
	setupADC();
	joystickCalibrate();
	
	JoysticAnalog JSA;
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
	}

	
	
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

