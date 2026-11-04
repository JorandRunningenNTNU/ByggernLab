#include <avr/io.h>
#include <avr/interrupt.h>
#include "../../include/drivere/timer.h"

void setupTimer(void) {
	TCNT1 = 0;
	OCR1A = 50000;
	OCR1B = 0;
	TCCR1A = 0;
	TCCR1B = (1 << WGM12) | (1 << CS11) | (1<<CS10);
	TIMSK |= (1 << OCIE1A);
}

uint8_t timer_flag = 0;

ISR(TIMER1_COMPA_vect) {
	timer_flag += 1;
}
