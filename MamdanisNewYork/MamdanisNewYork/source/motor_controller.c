#include "motor.h"
#include "motor_controller.h"
#include "encoder.h"
#include "pwm.h"
#include <stdlib.h>
#include <sam.h>

#include "our_timer.h"

static int32_t reference = 0;
static int32_t position;
static int64_t speed;

#define K 20 // Kp = 10
#define Ti 1 // Ki = 10
#define Td 20 // Kd = 0.5

#define MAX_THRUST 1023
#define MIN_THRUST -1023

static int32_t freq = 1;
static int32_t e_integral = 0;

int16_t int32_to_int16_saturated(int32_t value)
{
	if (value > INT16_MAX) {
		return INT16_MAX;
	}
	if (value < INT16_MIN) {
		return INT16_MIN;
	}
	return (int16_t)value;
}

static void update_state();


void motor_controller_init(uint32_t frequency){
	motor_initilize();
	freq = (int32_t)frequency;
	
	cooler_timer_t timer_config = {
		.callback = motor_controller_update_output,
		.frequency = freq
	};
	
	timer_channel_init(&timer_config,0);
}

void motor_controller_change_refrence(uint16_t new_refrence){
	reference = new_refrence;
}

void motor_controller_update_output(){
	update_state();
	int32_t e = reference - position; // e er mellom -1024 og 1024
	e_integral += e;
	int32_t integral_ledd = (Ti*e_integral)/freq;
	if(integral_ledd > MAX_THRUST){
		e_integral = MAX_THRUST*freq/Ti;
	}else if(integral_ledd < MIN_THRUST){
		e_integral = MIN_THRUST*freq/Ti;
	}	
	int32_t thrust = K*(e + (Ti*e_integral)/freq - speed/Td);
	
	if (thrust < MIN_THRUST){
		thrust = MIN_THRUST;
	}
	else if (MAX_THRUST < thrust){
		thrust = MAX_THRUST;
	}
	
	motor_set_output(int32_to_int16_saturated(thrust));
}

int32_t motor_controller_get_error(){
	return reference - position;
}

static void update_state(){
	int32_t new_position = (encoder_get_position()*1024) / 5618;
	speed = (8*speed + 2*((new_position - position)*freq))/10; // lavpass-filtrerer endringen i posisjon for � f� fart
	position = new_position;
}