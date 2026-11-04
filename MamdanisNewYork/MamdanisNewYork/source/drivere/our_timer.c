#include <sam.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "our_timer.h"

#define TC_MCK 84000000

static cooler_timer_t timers[3];


void timer_channel_init(cooler_timer_t* timer_config, uint32_t channel){
	if(timer_config->frequency){
		// Counteren tikker oppover med en frekvens på MCK/128 = 84MHz/128 = 656 250 Hz
		// Counteren får bit overflow og reseter med mellomrom på 6544s som er cirka 1 time og 50 min. 
		uint32_t frequency = timer_config->frequency;
		PMC->PMC_PCER0 = (1<<(ID_TC0 + channel)); // Enabler perifiral klokke for counter 0 channel 0
		TC0->TC_CHANNEL[channel].TC_CCR = TC_CCR_CLKDIS; // disabler klokke/counter 0 channel 0.
	
		// Pick the smallest divider where the period fits in the 32-bit counter
		static const uint32_t div[4]  = {2, 8, 32, 128};
		static const uint32_t clks[4] = {
			TC_CMR_TCCLKS_TIMER_CLOCK1,                // MCK/2
			TC_CMR_TCCLKS_TIMER_CLOCK2,                // MCK/8
			TC_CMR_TCCLKS_TIMER_CLOCK3,                // MCK/32
			TC_CMR_TCCLKS_TIMER_CLOCK4,                // MCK/128
		};
		uint8_t i = 0;
		while((i < 3) && ((uint64_t)TC_MCK / div[i] / frequency) > UINT32_MAX){
			i++;
		}
	
	
		TC0->TC_CHANNEL[channel].TC_CMR = TC_CMR_WAVSEL_UP_RC | TC_CMR_WAVE | clks[i];// velger divider og waveform-mode
	
		timers[channel].divider = (2*i + 1);
		uint32_t RC = (TC_MCK >> (2*i + 1)) / frequency;
		TC0->TC_CHANNEL[channel].TC_RC = RC;
		timers[channel].compare_value = RC;
	
		if(timer_config->callback){ // om vi har en callback funksjon så bruker vi interrupts
			timers[channel].callback = timer_config->callback;
			TC0->TC_CHANNEL[channel].TC_IER = TC_IER_CPCS;                  // interrupt on RC compare
			NVIC_EnableIRQ((IRQn_Type)(TC0_IRQn + channel));
		}else{
				TC0->TC_CHANNEL[channel].TC_IDR = 0xFFFFFFFF;
		}
	
		TC0->TC_CHANNEL[channel].TC_CCR = TC_CCR_CLKEN; // enabler klokke/counter 0 channel 0. 
		TC0->TC_CHANNEL[channel].TC_CCR = TC_CCR_SWTRG; // resetter og starter klokken
	}
}

uint32_t timer_done(uint32_t channel){
	if(!timers[channel].callback){
		return (TC0->TC_CHANNEL[channel].TC_SR & TC_SR_CPCS) != 0;
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