#pragma once
#include <stdint.h>

uint32_t timer_done(uint32_t channel); // returnerer 1 hvis counteren har telt opp til verdien gitt av valgt frekvens i init
void timer_init(uint32_t frequenzy, uint32_t channel); // frekvensen er i Hz, channel kan være 0, 1 eller 2, så man kan totalt bruke 3 countere
uint32_t timer_have_missed_flags(uint32_t channel);  // returnerer 1 om man har gått glipp av et flagg