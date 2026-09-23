#include <stdlib.h>
#include <avr/io.h>
#include "../include/sleep.h"

void sleep(uint32_t ms){
	uint32_t n = 491*ms;
	while(n>1){
		n--;
	}
}

void sleepUs(uint32_t us){
	uint32_t n = us;
	while(n>1){
		n--;
	}
}