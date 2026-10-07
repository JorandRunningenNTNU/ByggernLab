#define F_CPU 4915200

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
#include "include/drivere/ioboard.h"
#include "include/drivere/oled.h"
#include "include/menu.h"
#include "include/test.h"

int main(void){

	//setupJoystick();
	
	while(1){
		ioboard_update_data();
		update_menu();
		render_menu();
		continue;
	}
}

