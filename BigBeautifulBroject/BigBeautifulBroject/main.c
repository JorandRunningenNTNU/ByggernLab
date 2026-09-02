/*
 * BigBeautifulBroject.c
 *
 * Created: 02.09.2026 15:57:30
 * Author : joran
 */ 

#include <avr/io.h>
#include "stdio.h"
#include "include/drivere/uart.h"


static int uart_putchar(char c, FILE *stream){
	if (c == '\n') {
		sendByte('\r');
	}

	sendByte((unsigned char) c);
	return 0;
}

static int uart_getchar(FILE *stream){
	return readByte();
}


int main(void){
     FILE *uart = fdevopen(uart_putchar, uart_getchar);
     stdout = uart;
     stdin = uart;
	
	initilize();
	while (1){
	    unsigned char data = readByte();
	    if (data != 254){
		    sendByte(data);
			printf("hei %d \n", 72);
	    }
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

