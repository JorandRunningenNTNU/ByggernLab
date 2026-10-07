#define F_CPU 4915200

#include <stdlib.h>
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "uart.h"
#include "sram.h"
#include "adc.h"
#include "joystick.h"
#include "sleep.h" 
#include "spi.h"
#include "ioboard.h"
#include "oled.h"
#include "menu.h"
#include "test.h"

int main(void){

	//setupJoystick();
	
	while(1){
		ioboard_update_data();
		update_menu();
		render_menu();
		continue;
	}
}

