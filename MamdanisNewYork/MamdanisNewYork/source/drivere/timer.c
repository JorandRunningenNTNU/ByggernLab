#include "timer.h"
#include <sam.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#define TC_MCK 84000000

typedef void (*timer_callback_t)(void);

typedef struct {
	uint32_t frequency;
	uint32_t compare_value;
	timer_callback_t callback;
	uint8_t has_missed_timer_flag;
	uint8_t divider;
	uint8_t waiting_for_reset;
	uint8_t waveform;
} cooler_timer_t;



static cooler_timer_t timers[3];
	
static uint8_t ceil_log2(uint32_t x) {
	uint32_t z = 0;
	for (uint32_t y = x; y; y >>= 1) ++z;
	return z;
}

uint8_t TCCLKS_from_freq(uint32_t freq){
	uint8_t exp = ceil_log2(TC_MCK/freq)/2;
	uint8_t lookup[] = {
		TC_CMR_TCCLKS_TIMER_CLOCK5,
		TC_CMR_TCCLKS_TIMER_CLOCK1,
		TC_CMR_TCCLKS_TIMER_CLOCK2,
		TC_CMR_TCCLKS_TIMER_CLOCK3,
		TC_CMR_TCCLKS_TIMER_CLOCK4
	};
	return lookup[exp];
}


void timer_channel_init(cooler_timer_t* timer_config, uint32_t channel, timer_callback_t callback){
	// Counteren tikker oppover med en frekvens på MCK/128 = 84MHz/128 = 656 250 Hz
	// Counteren får bit overflow og reseter med mellomrom på 6544s som er cirka 1 time og 50 min. 
	uint32_t frequency = timer_config->frequency;
	PMC->PMC_PCER0 = (1<<(ID_TC0 + channel)); // Enabler perifiral klokke for counter 0 channel 0
	TC0->TC_CHANNEL[channel].TC_CCR = TC_CCR_CLKDIS; // disabler klokke/counter 0 channel 0. 
	
	uint8_t TCCLKS = TCCLKS_from_freq(frequency);
	
	TC0->TC_CHANNEL[channel].TC_CMR = TC_CMR_WAVE | TCCLKS;// velger divider og waveform-mode
	if(timer_config->waveform){
		TC0->TC_CHANNEL[channel].TC_CMR |= TC_CMR_WAVSEL_UP_RC;
	}
	
	uint8_t exp = ceil_log2(TC_MCK/frequency)/2;
	timers[channel].divider = exp;
	uint32_t RC = (TC_MCK >> exp) / frequency;
	TC0->TC_CHANNEL[channel].TC_RC = RC;
	
	if(timer_config->callback){
		timers[channel].callback = timer_config->callback;
		TC0->TC_CHANNEL[channel].TC_IER = TC_IER_CPCS;                  // interrupt on RC compare
		NVIC_EnableIRQ((IRQn_Type)(TC0_IRQn + channel));
	}
	
	timers[channel].compare_value = (TC_MCK >> exp) / frequency;
	TC0->TC_CHANNEL[channel].TC_CCR = TC_CCR_CLKEN; // enabler klokke/counter 0 channel 0. 
	TC0->TC_CHANNEL[channel].TC_CCR = TC_CCR_SWTRG; // resetter og starter klokken
}

uint32_t timer_done(uint32_t channel){
	uint32_t counter = TC0->TC_CHANNEL[channel].TC_CV;
	
	// sjekker at vi ikke alt har reagert på dette flagget
	if (timers[channel].waiting_for_reset){
		if (counter < timers[channel].compare_value){
			timers[channel].waiting_for_reset = 0;
		}
		return 0;
	}
	
	// Sjekker om vi har gått glipp av et flagg
	if(counter > 2*timers[channel].compare_value){
		timers[channel].has_missed_timer_flag = counter;
	}
	
	// Returnerer at counteren er over compare verdien
	if(counter > timers[channel].compare_value){
		TC0->TC_CHANNEL[channel].TC_CCR = (1<<2); // resetter counter og starter klokken
		timers[channel].waiting_for_reset = 1;
		return 1;
	}
	
	// Returnerer at counteren er under compare verdien
	return 0;
}

uint32_t timer_have_missed_flags(uint32_t channel){
	if(timers[channel].has_missed_timer_flag){
		timers[channel].has_missed_timer_flag = 0;
		return 1;
	}
	return 0;
}

static void timer_handle(uint32_t channel){
	uint32_t sr = TC0->TC_CHANNEL[channel].TC_SR;  // reading SR clears the flag
	if((sr & TC_SR_CPCS) && timers[channel].callback){
		timers[channel].callback();
	}
}

void TC0_Handler(void){ timer_handle(0); }
void TC1_Handler(void){ timer_handle(1); }
void TC2_Handler(void){ timer_handle(2); }