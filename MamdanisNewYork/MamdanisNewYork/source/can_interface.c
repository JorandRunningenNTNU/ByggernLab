/*
 * node_2.c
 *
 * Created: 4.11.2026 14:48:23
 *  Author: vikin
 */ 

#include <stdio.h>
#include "can.h"
#include "solenoide.h"
#include "servo.h"

#define CAN_INPUTS_ID 0x69
#define CAN_DEATH_ID 0x67

void can_interface_handle_message(CanMsg* m){
	switch(m->id){
		case CAN_INPUTS_ID: {
			uint8_t joystick_x = (uint8_t)m->byte[0];
			uint8_t joystick_y = (uint8_t)m->byte[1];
			uint8_t trigger = ((0x01) & (uint8_t)(m->byte[2]));
			
			uint16_t pos = (((uint16_t)joystick_y) << 8);
			servo_set_position(pos);

			if(trigger){
				solenoide_fire();
				}else{
				solenoide_retract();
			}
			
			//missing setting motor reference 
			break; 
			}
		default:
			printf("Unkown can message received");
		break;
	}
}