#include "sam.h"
#include "include/drivere/uart.h"
#include <stdio.h>
#include "include/test.h"
#include "include/drivere/can.h"
#include "include/drivere/encoder.h"

int main(void){
    SystemInit(); // for mikrokontrolleren
	WDT->WDT_MR = WDT_MR_WDDIS; // <3 <3 <3
	
	uart_init(F_CPU, 9600);
	can_setup();
	encoder_init();
	
	test();
	while(1);
}
