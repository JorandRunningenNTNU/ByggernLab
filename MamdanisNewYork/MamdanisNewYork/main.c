#include "sam.h"
#include "include/drivere/uart.h"
#include <stdio.h>
#include "include/test.h"
#include "servo.h"
#include "include/drivere/can.h"
#include "include/drivere/encoder.h"

int main(void){
    SystemInit(); // for mikrokontrolleren
	WDT->WDT_MR = WDT_MR_WDDIS; // <3 <3 <3
	
	uart_init(F_CPU, 9600);
	servo_init();
	can_setup();
	encoder_init();
	
	while(1){
		for(int i = 0; i<256; i++){
			servo_set_position(i);
			for(int j = 0; j <84000; j ++);
		}
		for(int i = 255; i>0; i--){
			servo_set_position(i);
			for(int j = 0; j <84000; j ++);
		}
	}
}
