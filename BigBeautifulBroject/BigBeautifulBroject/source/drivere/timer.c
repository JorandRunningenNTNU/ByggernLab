#include <avr/io.h>
#include <avr/interrupt.h>
#include "../../include/drivere/timer.h"

void setupTimer() {
	TCNT1 = 0;
	OCR1A = 50000;
	OCR1B = 0;
	TCCR1A = 0;
	TCCR1B = (1 << WGM12) | (1 << CS12) | (1<<CS10);
	TIMSK |= (1 << OCIE1A);
}

void timer_set_frequency(uint16_t freq) {
	uint16_t clk_freq = (49152/1024)*100;
	OCR1A = clk_freq/freq;
	TCNT1 = 0;
}

uint8_t timer_flag = 0;

ISR(TIMER1_COMPA_vect) {
	timer_flag += 1;
}
