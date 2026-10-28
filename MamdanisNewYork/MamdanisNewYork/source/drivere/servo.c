/*
 * servo.c
 *
 * Created: 21.10.2026 11:33:26
 *  Author: vikin
 */ 
#include <stdio.h>
#include "pwm.h"

void servo_init(){
	pwm_channel_init(50,1,10);
}

#define MAX_DUTY 2100
#define MIN_DUTY 900

void servo_set_position(uint16_t pos){
	uint32_t duty_cycle = (uint32_t)pos + 990;
	duty_cycle = (duty_cycle > MAX_DUTY) ? MAX_DUTY : duty_cycle;
	duty_cycle = (duty_cycle < MIN_DUTY) ? MIN_DUTY : duty_cycle;
	pwm_set_duty_cycle(duty_cycle, 1);
}