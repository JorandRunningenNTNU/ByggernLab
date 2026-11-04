#include "motor.h"
#include "encoder.h"
#include "pwm.h"
#include <stdlib.h>
#include <sam.h>

void motor_initilize(){
	PIOC->PIO_PER = (1<<23);
	PIOC->PIO_OER = (1<<23);
	PIOC->PIO_OWER = (1<<23);
	
	pwm_channel_init(40000, 0);
	pwm_set_duty_cycle(0, 0);
	pwm_start_channel(0);
}

void motor_set_output(int16_t output){ // tar inn tall mellom -1023 og 1023
	// sett motorretning
	if (output>=0){
		PIOC->PIO_CODR = (1<<23); // clear
	}
	else{
		PIOC->PIO_SODR = (1<<23); // set
	}
	
	// sette motorhastighet
	if (output<0){
		output*=-1;
	} 
	uint16_t abs_output = (uint16_t)output;

	pwm_set_duty_cycle((abs_output<<6), 0); // bitskifter 6 ganger for å tilpasse seg pwm sitt 0-65535 format
}