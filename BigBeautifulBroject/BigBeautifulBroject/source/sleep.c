#include <stdlib.h>
#include <avr/io.h>
#include "../include/sleep.h"

void sleep(uint32_t ms){
	uint32_t n = 491*ms;
	while(n>1){
		n--;
	}
}
