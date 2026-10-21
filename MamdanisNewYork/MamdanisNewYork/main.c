#include "sam.h"
#include "include/drivere/uart.h"
#include <stdio.h>
#include "include/test.h"
#include "include/drivere/can.h"

int main(void){
    SystemInit(); // for mikrokontrolleren
	uart_init(F_CPU, 9600);
	can_setup();
	
	test();
	while(1);
}
