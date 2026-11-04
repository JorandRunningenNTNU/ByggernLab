#include "sam.h"
#include "include/drivere/uart.h"
#include <stdio.h>
#include "include/test.h"
#include "servo.h"
#include "pwm.h"
#include "can.h"
#include "encoder.h"
#include "adc.h"

int main(void){
    SystemInit(); // for mikrokontrolleren
	WDT->WDT_MR = WDT_MR_WDDIS; // <3 <3 <3 watchdawg timer DISABEL!
	
	adc_setup();
	uart_init(F_CPU, 9600);
		pwm_init();
	encoder_init();
	servo_init();
	can_setup();
	
	test();
}
