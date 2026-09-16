#pragma once
#include <avr/io.h>

typedef enum {Neutral, Up, Down, Left, Right} JoystickDiscrete;
typedef struct {
	int8_t X;
	int8_t Y;
} JoysticAnalog;

void joystickCalibrate();
JoysticAnalog readJoystickAnalog();
JoystickDiscrete readJoystickDiscrete();
