#include <sam.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "our_timer.h"

#define TC_MCK 84000000

static timer_callback_t timer_callbacks[3];
//0 brukes til motorkontroller
//1 brukes til motorkontroller start sekvens
//2 brukes til motor lyd driver


tim_return_t timer_channel_init(const uint32_t frequency, const timer_callback_t callback, const uint32_t channel){
	if((channel > 3) || (frequency > TC_MCK/2)){
		return TIM_INPUT_OUT_OF_BOUNDS;
	}
	
	if(TC0->TC_CHANNEL[channel].TC_CMR){
		return TIM_BUSY_ERROR;
	}
	
	PMC->PMC_PCER0 = (1<<(ID_TC0 + channel)); // Enabler perifiral klokke for timer counter 0 for riktig kanal
	TC0->TC_CHANNEL[channel].TC_CCR = TC_CCR_CLKDIS; // disabler klokke/counter 0.
	
	// Velg den raskeste klokken hvor compare verdien fortsatt passer in i uint32_t
	static const uint8_t div_lookup[4]  = {2, 8, 32, 128};
	static const uint32_t clks_lookup[4] = {
		TC_CMR_TCCLKS_TIMER_CLOCK1,                // MCK/2
		TC_CMR_TCCLKS_TIMER_CLOCK2,                // MCK/8
		TC_CMR_TCCLKS_TIMER_CLOCK3,                // MCK/32
		TC_CMR_TCCLKS_TIMER_CLOCK4,                // MCK/128
	};
	uint8_t i = 0;
	while((i < 3) && ((TC_MCK / (uint64_t)div_lookup[i] / (uint64_t)frequency) > UINT32_MAX)) i++;
	
	
	TC0->TC_CHANNEL[channel].TC_CMR = TC_CMR_WAVSEL_UP_RC | TC_CMR_WAVE | clks_lookup[i];// velger divider og waveform-mode
		
	uint8_t divider_exponent = (2*i + 1);
	
	uint32_t RC = (TC_MCK >> divider_exponent) / frequency;
	TC0->TC_CHANNEL[channel].TC_RC = RC;
	
	if(callback){ // om vi har en callback funksjon så bruker vi interrupts
		timer_callbacks[channel] = callback;
		TC0->TC_CHANNEL[channel].TC_IER = TC_IER_CPCS;                  // interrupt på RC compare
		NVIC_EnableIRQ((IRQn_Type)(TC0_IRQn + channel));				// enable interrupten i NVIC
		NVIC_SetPriority((IRQn_Type)(TC0_IRQn + channel), 1);
	}else{
		TC0->TC_CHANNEL[channel].TC_IDR = 0xFFFFFFFF; // disabler alle interrupts for den kanalen ellers
	}
	
	TC0->TC_CHANNEL[channel].TC_CCR = TC_CCR_CLKEN; // enabler klokke/counter 0 for kanalen 
	TC0->TC_CHANNEL[channel].TC_CCR = TC_CCR_SWTRG; // resetter og starter klokken på den kanalen
	
	return TIM_ALL_GOOD;
}

void timer_channel_stop(const uint32_t channel){
	TC0->TC_CHANNEL[channel].TC_CCR = TC_CCR_CLKDIS;// disabler klokka
}

void timer_channel_start(const uint32_t channel){
	TC0->TC_CHANNEL[channel].TC_CCR = TC_CCR_CLKEN; // enabler klokke/counter 0 for kanalen
	TC0->TC_CHANNEL[channel].TC_CCR = TC_CCR_SWTRG; // resetter og starter klokken på den kanalen
}

void timer_channel_deinit(const uint32_t channel){
	timer_channel_stop(channel);
	TC0->TC_CHANNEL[channel].TC_IDR = 0xFFFFFFFF; // disabler alle interrupts for denne
	TC0->TC_CHANNEL[channel].TC_CMR = 0;// setter ting til null
}
	

uint32_t timer_done(const uint32_t channel){
	if(!timer_callbacks[channel]){
		volatile uint32_t flag = (TC0->TC_CHANNEL[channel].TC_SR & TC_SR_CPCS);
		return (flag != 0);
	}
	return 0;
}

static void timer_handle(const uint32_t channel){
	uint32_t sr = TC0->TC_CHANNEL[channel].TC_SR;  // lesing av status registeret resetter alle flaggene
	if((sr & TC_SR_CPCS) && timer_callbacks[channel]){
		timer_callbacks[channel]();	
	}
}

void TC0_Handler(void){ timer_handle(0); }
void TC1_Handler(void){ timer_handle(1); }
void TC2_Handler(void){ timer_handle(2); }