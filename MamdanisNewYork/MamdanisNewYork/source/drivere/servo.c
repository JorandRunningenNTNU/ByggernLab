/*
 * servo.c
 *
 * Created: 21.10.2026 11:33:26
 *  Author: vikin
 */ 

// 100% menneskeskrevet
#include <stdio.h>
#include "pwm.h"
#include "servo.h"
 
#define FREQUENCY 50
#define PULSE_STARTPOINT 988
#define MAX_DUTY_us 2100
#define MIN_DUTY_us 900
#define SERVO_PWM_CHANNEL 1


void servo_init(){
	pwm_channel_init(FREQUENCY,SERVO_PWM_CHANNEL);
	servo_set_position(512);
	pwm_start_channel(SERVO_PWM_CHANNEL);
}



//pos er et tall mellom 0 og 1023
void servo_set_position(uint16_t pos){
	uint32_t pulse_width = (uint32_t)pos + PULSE_STARTPOINT;
	pulse_width = (pulse_width > MAX_DUTY_us) ? MAX_DUTY_us : pulse_width;
	pulse_width = (pulse_width < MIN_DUTY_us) ? MIN_DUTY_us : pulse_width;
	
	pwm_set_pulse_width_us(pulse_width, 1);
}