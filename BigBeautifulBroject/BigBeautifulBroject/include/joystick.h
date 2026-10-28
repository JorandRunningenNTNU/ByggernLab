#pragma once
#include <avr/io.h>

typedef enum {Neutral, Up, Down, Left, Right} joystick_discrete_t;
typedef struct {
	uint8_t X;
	uint8_t Y;
} joystick_data_t;

void setupJoystick();
joystick_data_t joystick_read_data();
joystick_discrete_t joystick_read_discrete();
