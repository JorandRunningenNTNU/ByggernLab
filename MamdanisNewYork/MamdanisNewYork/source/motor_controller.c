#include "motor.h"
#include "motor_controller.h"
#include "encoder.h"
#include "pwm.h"
#include <stdlib.h>
#include <sam.h>

static uint16_t reference = 0;
static int16_t position;
static double speed = 0;

static float Kp = 10; // 10
static float Kd = 0.5; // 0.5
static float Ki = 10; // 5
static float dt = 0;
static float e_integral = 0;

static void update_state();


void motor_controller_init(float set_dt){
	motor_initilize();
	dt = set_dt;
}

void motor_controller_change_refrence(uint16_t new_refrence){
	reference = new_refrence;
}

void motor_controller_update_output(){
	update_state();
	int16_t e = reference - position; // e er mellom -1024 og 1024
	e_integral += e*dt;
	int16_t thrust = (int16_t)(Kp*e + Ki*e_integral + Kd*(-speed));
	
	if (thrust < -1023){
		thrust = -1023;
	}
	else if (1023 < thrust){
		thrust = 1023;
	}
	
	motor_set_output(thrust);
}

int16_t motor_controller_get_error(){
	return reference - position;
}

static void update_state(){
	int16_t new_position = (encoder_get_position()*1024) / 5618;
	speed = 0.8*speed + 0.20*((new_position - position)/dt); // lavpass-filtrerer endringen i posisjon for å få fart
	position = new_position;
}