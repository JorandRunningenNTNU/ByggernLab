/*
 * servo.h
 *
 * Created: 21.10.2026 11:46:44
 *  Author: vikin
 */ 


#ifndef SERVO_H_
#define SERVO_H_

void servo_init();

void servo_set_position(uint16_t pos); //Bruker fulle 16 bit tall range

#endif /* SERVO_H_ */