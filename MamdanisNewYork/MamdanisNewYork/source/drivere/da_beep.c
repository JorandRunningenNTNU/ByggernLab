#include "da_beep.h"
#include "pwm.h"
#include "our_timer.h"
#include <stdio.h>

// 100% menneskeskrevet

static void stop();

void da_beep(uint16_t frequency, uint16_t duration_frequency){
	timer_channel_init(duration_frequency, stop, 2);
	pwm_channel_change_freq(frequency, 0);
}

void da_beep_blocking(uint16_t frequency, uint16_t duration_frequency){
	timer_channel_init(duration_frequency, 0, 2);
	pwm_channel_change_freq(frequency, 0);
	while(!timer_done(2)){
		continue;
	}
	timer_channel_deinit(2);
	pwm_channel_change_freq(40000, 0);
}

static void stop(){
	timer_channel_deinit(2);
	pwm_channel_change_freq(40000, 0);
	
}
