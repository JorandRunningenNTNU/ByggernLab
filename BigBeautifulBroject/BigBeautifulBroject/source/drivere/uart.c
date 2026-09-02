#include "../../include/drivere/uart.h"

void initilize(){
	// baud-rate 9600
	UBRR0H = 0;
	UBRR0L = 31;
	
	// enable reciver og transmitter
	UCSR0B = (1<<3)|(1<<4);

}

void sendByte(unsigned char data){
	// vente på at bufferen er klar til å skrives til
	while(!(UCSR0A & (1<<5))){}
	
	UDR0 =  data;
}

unsigned char readByte(){
	int n = 10000;
	while(!(UCSR0A & (1<<7)) || (n--));
	
	// returnerer 254 om ingen ting leses
	if (n == 0){
		return 254;
	}
	
	return UDR0;
}
