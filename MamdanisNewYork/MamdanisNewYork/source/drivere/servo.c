/*
 * servo.c
 *
 * Created: 21.10.2026 11:33:26
 *  Author: vikin
 */ 
#include <stdio.h>
#include "pwm.h"

void servo_init(){
	pwm_init();
}

#define MAX_DUTY 2100
#define MIN_DUTY 900

void servo_set_position(uint8_t pos){
	uint32_t duty_cycle = ((uint32_t)pos)*4 + 990;
	duty_cycle = (duty_cycle > 2100) ? 2100 : duty_cycle;
	duty_cycle = (duty_cycle < 900) ? 900 : duty_cycle;
	pwm_set_duty_cycle_us(duty_cycle);
}