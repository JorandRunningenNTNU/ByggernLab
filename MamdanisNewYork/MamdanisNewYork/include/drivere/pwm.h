/*
 * pwm.h
 *
 * Created: 14.10.2026 13:24:41
 *  Author: vikin
 */ 


#ifndef PWM_H_
#define PWM_H_

void pwm_init();
void pwm_channel_init(uint32_t frequency, uint8_t channel, uint16_t start_duty);
void pwm_set_duty_cycle(uint16_t duty, uint8_t channel);

#endif /* PWM_H_ */