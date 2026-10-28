#include "sam.h"
#include "include/drivere/uart.h"
#include <stdio.h>
#include "include/test.h"
#include "servo.h"
#include "include/drivere/can.h"
#include "include/drivere/encoder.h"
#include "include/drivere/adc.h"

int main(void){
    SystemInit(); // for mikrokontrolleren
	WDT->WDT_MR = WDT_MR_WDDIS; // <3 <3 <3 watchdawg timer DISABEL!
	
	adc_setup();
	uart_init(F_CPU, 9600);
	servo_init();
	encoder_init();
	can_setup();
	
	
	while(1){
		test();
	}
}
