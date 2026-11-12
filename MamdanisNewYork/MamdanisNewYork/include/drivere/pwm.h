/*
 * pwm.h
 *
 * Created: 14.10.2026 13:24:41
 *  Author: vikin
 */ 


#ifndef PWM_H_
#define PWM_H_

#include <stdint.h>

void pwm_init();
void pwm_channel_init(uint32_t frequency, uint8_t channel); //Starter ikke kanalen
void pwm_set_duty_cycle(uint16_t duty, uint8_t channel);
void pwm_set_pulse_width_us(uint32_t width_us, uint8_t channel);
void pwm_start_all();
void pwm_start_channel(uint8_t channel);
void pwm_channel_change_freq(uint32_t frequency, uint8_t channel);

#endif /* PWM_H_ */