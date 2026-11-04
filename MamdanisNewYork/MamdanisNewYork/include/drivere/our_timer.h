#pragma once
#include <stdint.h>

typedef void (*timer_callback_t)(void); // vi bruker callbacks fordi det er sick as fuck

typedef struct {
	uint32_t frequency;
	uint32_t compare_value; // denne trenger du ikke bry deg om for init
	timer_callback_t callback; // Fungerer som en vanlig poll timer når denne ikke er satt
	uint8_t divider; // denne trenger du ikke bry deg om for init
} cooler_timer_t;

uint32_t timer_done(uint32_t channel); // returnerer 1 hvis counteren har telt opp til verdien gitt av valgt frekvens i init
void timer_channel_init(cooler_timer_t* timer_config, uint32_t channel); // frekvensen er i Hz, channel kan være 0, 1 eller 2, så man kan totalt bruke 3 countere
uint32_t timer_have_missed_flags(uint32_t channel);  // returnerer 1 om man har gått glipp av et flagg