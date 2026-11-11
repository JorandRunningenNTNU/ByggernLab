#pragma once
#include <stdint.h>

typedef void (*timer_callback_t)(void); // vi bruker callbacks fordi det er meget kult synes jeg

typedef enum {
	TIM_ALL_GOOD,
	TIM_INIT_ERROR,
	TIM_READ_ERROR,
	TIM_BUSY_ERROR,
	TIM_INPUT_OUT_OF_BOUNDS
} tim_return_t;

uint32_t timer_done(const uint32_t channel); // returnerer 1 hvis counteren til kanalen har telt opp til verdien gitt av valgt frekvensen i init


tim_return_t timer_channel_init(const uint32_t frequency, // frekvensen er i Hz
								const timer_callback_t callback, // Fungerer som en vanlig poll timer når denne ikke er satt
								const uint32_t channel); //  channel kan være 0, 1 eller 2, så man kan totalt bruke 3 countere