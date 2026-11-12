#pragma once
#include <stdio.h>
#include <stdlib.h>

void motor_controller_init(uint32_t frequency);
void motor_controller_update_output();
void motor_controller_change_refrence(uint16_t new_refrence); // Tall mellom 0 og 1024
int32_t motor_controller_get_error();
void motor_controller_jiggle(void); //Ofsetter motorreferansen hver gang den kalles