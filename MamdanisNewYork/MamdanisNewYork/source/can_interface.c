/*
 * node_2.c
 *
 * Created: 4.11.2026 14:48:23
 *  Author: vikin
 */ 

// 100% menneskeskrevet

#include <stdio.h>
#include "can.h"
#include "solenoide.h"
#include "servo.h"
#include "motor_controller.h"
#include "can_interface.h"
#include "da_beep.h"

volatile bool play_da_heart_sound = false;

void can_interface_handle_message(CanMsg* m){
	switch(m->id){
		case CAN_INPUTS_ID: {
			uint8_t joystick_x = (uint8_t)m->byte[0];
			uint8_t joystick_y = (uint8_t)m->byte[1];
			uint8_t trigger = ((0x01) & (uint8_t)(m->byte[2]));
			
			uint16_t pos = (uint16_t)((uint32_t)joystick_y *1024 / 256);
			servo_set_position(pos);

			if(trigger){
				solenoide_fire();
			}else{
				solenoide_retract();
			}
			uint16_t reference = 1024 - (uint16_t)((uint32_t)joystick_x *1024 / 256);   // 0 til 1024
			motor_controller_change_refrence(reference);
			break; 
			}
		case CAN_SOUND_ID:
			{
				uint16_t sound_freq =  ((uint32_t)m->byte[1] << 8) | ((uint32_t)m->byte[0]);
				uint16_t duration_freq = (uint16_t)(m->byte[2]);
				da_beep(sound_freq,duration_freq);
			}
		break;
		case CAN_HEART_CRACK_ID:
			play_da_heart_sound = true;
		break;
		case CAN_HEART_LOSS_ID:
			da_beep(1000,3);
		break;
		default:
			//printf("Unkown can message received");
		break;
	}
}