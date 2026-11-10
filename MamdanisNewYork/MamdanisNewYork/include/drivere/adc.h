#pragma once

typedef enum {
	ADC_ALL_GOOD,
	ADC_INIT_ERROR,
	ADC_OVERRUN,
	ADC_BUSY_ERROR,
	ADC_INPUT_OUT_OF_BOUNDS
} adc_return_t;

adc_return_t adc_setup(const uint32_t frequency);
uint32_t adc_read(); // returnerer 0-1024
adc_return_t adc_start_read(void);