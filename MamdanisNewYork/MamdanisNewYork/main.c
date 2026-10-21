#include "sam.h"
#include "include/drivere/uart.h"
#include <stdio.h>
#include "include/test.h"
#include "servo.h"

int main(void){
    SystemInit(); // for mikrokontrolleren
	uart_init(F_CPU, 9600);
	servo_init();
	
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
