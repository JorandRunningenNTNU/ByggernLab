#pragma once
#include <stdio.h>
#include <stdlib.h>

void motor_controller_init(float set_dt);
void motor_controller_update_output();
void motor_controller_change_refrence(uint16_t new_refrence);
int16_t motor_controller_get_error();