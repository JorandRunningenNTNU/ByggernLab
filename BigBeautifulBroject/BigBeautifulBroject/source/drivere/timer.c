#include <avr/io.h>
#include "../../include/drivere/timer.h"

void setupTimer(void) {
	TCNT1 = 0;
	TCCR1A = 0;
	TCCR1B = (1 << CS11);
}