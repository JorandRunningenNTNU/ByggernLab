#include "timer.h"
#include <sam.h>
#include <stdint.h>
#include <stdio.h>

static uint32_t counter_compare_value[3];
static uint32_t have_missed_timer_flags[3] = {0, 0, 0};
static uint32_t waiting_for_reset[3] = {0, 0, 0};

void timer_init(uint32_t frequency, uint32_t channel){
	// Counteren tikker oppover med en frekvens på MCK/128 = 84MHz/128 = 656 250 Hz
	// Counteren får bit overflow og reseter med mellomrom på 6544s som er cirka 1 time og 50 min. 
	PMC->PMC_PCER0 = (1<<(27 + channel)); // Enabler perifiral klokke for counter 0 channel 0
	TC0->TC_CHANNEL[channel].TC_CCR = (1<<0); // enabler klokke/counter 0 channel 0. 
	TC0->TC_CHANNEL[channel].TC_CMR = (1<<15) | (3<<0);// velger klokkefrekvens MCK/128, ingen komparator trigger, og waveform-mode
	TC0->TC_CHANNEL[channel].TC_CCR = (1<<2); // resetter og starter klokken
	
	counter_compare_value[channel] = 656250 / frequency;
}


uint32_t timer_done(uint32_t channel){
	uint32_t counter = TC0->TC_CHANNEL[channel].TC_CV;
	
	// sjekker at vi ikke alt har reagert på dette flagget
	if (waiting_for_reset[channel]){
		if (counter < counter_compare_value[channel]){
			waiting_for_reset[channel] = 0;
		}
		return 0;
	}
	
	// Sjekker om vi har gått glipp av et flagg
	if(counter > 2*counter_compare_value[channel]){
		have_missed_timer_flags[channel] = counter;
	}
	
	// Returnerer at counteren er over compare verdien
	if(counter > counter_compare_value[channel]){
		TC0->TC_CHANNEL[channel].TC_CCR = (1<<2); // resetter counter og starter klokken
		waiting_for_reset[channel] = 1;
		return 1;
	}
	
	// Returnerer at counteren er under compare verdien
	return 0;
}

uint32_t timer_have_missed_flags(uint32_t channel){
	if(have_missed_timer_flags[channel]){
		have_missed_timer_flags[channel] = 0;
		return 1;
	}
	return 0;
}