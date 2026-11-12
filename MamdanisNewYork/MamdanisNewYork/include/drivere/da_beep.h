#pragma once
#include <stdio.h>
// Spiller en lyd med frequency som varer i 1/duration_frequency
// Den tar opp timer kanal 2, ikke blokkerende
void da_beep(uint16_t frequency, uint16_t duration_frequency);

// Spiller en lyd med frequency som varer i 1/duration_frequency
// Den tar opp timer kanal 1, blokkerende
void da_beep_blocking(uint16_t frequency, uint16_t duration_frequency);
