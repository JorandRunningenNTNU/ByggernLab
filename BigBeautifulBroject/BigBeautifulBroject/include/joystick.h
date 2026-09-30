#pragma once
#include <avr/io.h>

typedef enum {Neutral, Up, Down, Left, Right} joystick_discrete_t;
typedef struct {
	int8_t X;
	int8_t Y;
} joystick_data_t;

void setupJoystick();
joystick_data_t joystick_read_data();
joystick_discrete_t joystick_read_discrete();
