#pragma once

typedef enum {
	ADC_ALL_GOOD,
	ADC_INIT_ERROR,
	ADC_OVERRUN,
	ADC_BUSY_ERROR,
	ADC_INPUT_OUT_OF_BOUNDS
} adc_return_t;

adc_return_t adc_setup(void);
uint32_t adc_read_newest_result(); // returnerer 12 bit tall