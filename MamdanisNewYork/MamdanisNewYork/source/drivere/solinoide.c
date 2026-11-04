#include <sam.h>

void solenoide_init(){
	PIOA->PIO_PER = (1<<16);
	PIOA->PIO_OER = (1<<16);
	PIOA->PIO_OWER = (1<<16);
}

void solenoide_fire(){
	PIOA->PIO_SODR = (1<<16);
}

void solenoide_retract(){
	PIOA->PIO_CODR = (1<<16);
}