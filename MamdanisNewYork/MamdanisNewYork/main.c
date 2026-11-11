#include "sam.h"
#include "uart.h"
#include <stdio.h>
#include "include/test.h"
#include "servo.h"
#include "pwm.h"
#include "can.h"
#include "encoder.h"
#include "adc.h"
#include "motor_controller.h"
#include "solenoide.h"
#include "da_beep.h"

int main(void){
    SystemInit(); // for mikrokontrolleren
	WDT->WDT_MR = WDT_MR_WDDIS; // <3 <3 <3 watchdawg timer DISABEL!
	
	uart_init(F_CPU, 9600);
	pwm_init();
	encoder_init();
	servo_init();
	solenoide_init();
	motor_controller_init(1000);
	can_setup();
	adc_setup();
	
	test();
}
