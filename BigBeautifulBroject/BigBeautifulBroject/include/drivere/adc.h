#pragma once
#include <avr/io.h>

void setupADC();
void adc_read_all(uint8_t * data);