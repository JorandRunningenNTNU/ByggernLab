#pragma once
void adc_setup();
uint32_t adc_read(); // returnerer 0-1024
void adc_start_read(void);