/*
 * can_interface.h
 *
 * Created: 4.11.2026 15:56:31
 *  Author: vikin
 */ 


#ifndef CAN_INTERFACE_H_
#define CAN_INTERFACE_H_


void can_interface_handle_message(CanMsg* m);

#define CAN_INPUTS_ID 69
#define CAN_DEATH_ID 67


#endif /* CAN_INTERFACE_H_ */