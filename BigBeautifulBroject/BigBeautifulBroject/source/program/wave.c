#include <stdint.h>
#include <avr/pgmspace.h>

#include "../../include/drivere/oled.h"
#include "../../include/program/wave.h"
#include "../../include/menu.h"
#include "../../include/drivere/ioBoard.h"

static uint8_t phase = 0;

/*
 * One complete sine period mapped directly to shades 0 to 15.
 * Stored in flash rather than SRAM.
 */
static const uint8_t sine_table[64] PROGMEM = {
     8,  8,  9, 10, 10, 11, 12, 12,
    13, 13, 14, 14, 14, 15, 15, 15,
    15, 15, 15, 15, 14, 14, 14, 13,
    13, 12, 12, 11, 10, 10,  9,  8,
     7,  7,  6,  5,  5,  4,  3,  3,
     2,  2,  1,  1,  1,  0,  0,  0,
     0,  0,  0,  0,  1,  1,  1,  2,
     2,  3,  3,  4,  5,  5,  6,  7
};


static uint8_t shade_bit(uint8_t x, uint8_t y, uint8_t shade)
{	
	if (shade < 7) {
		for (uint8_t i = 0; i < shade; i++) {
			if ((x + 5*y) % 14 == 2*i) {
				return 1;
			}
		}
		return 0;
	}
	else if (shade == 7) {
		if ((x + y) % 2 == 0) {
			return 1;
		}
		return 0;
	} else {
		for (uint8_t i = 14; i >= shade; i--) {
			if ((x + 5*y + 1) % 14 == 2*i - 15) {
				return 0;
			}
		}
		return 1;
	}
}


static uint8_t approximate_distance(uint8_t x, uint8_t y)
{
    uint8_t max;
    uint8_t min;

    if (x > y) {
        max = x;
        min = y;
    }
    else {
        max = y;
        min = x;
    }

    /*
     * Cheap approximation of sqrt(x*x + y*y).
     */
    return max + (min >> 1);
}


void wave_animation(void)
{
    while (1) {
		
		ioboard_update_data();
		if ((ioboard_data.navButton) & (1<<3)) {
			break;
		}
		
        for (uint8_t line = 0; line < 8; line++) {

            uint8_t bytes[128];

            for (uint8_t x = 0; x < 128; x++) {

                uint8_t byte = 0;

                for (uint8_t i = 0; i < 8; i++) {

                    uint8_t y = 8 * line + i;

                    uint8_t distance =
                        approximate_distance(x, y);

                    /*
                     * Multiplying distance by approximately 1.5
                     * keeps the wavelength close to your original:
                     *
                     * sin(distance * 0.15 - phase)
                     */
                    uint8_t sine_index =
                        (distance +
                         (distance >> 1) +
                         phase) & 63;

                    uint8_t shade =
                        pgm_read_byte(
                            &sine_table[sine_index]
                        );

                    byte |=
                        shade_bit(x, y, shade) << i;
                }

                bytes[x] = byte;
            }

            oled_goto_line(line);
            oled_goto_column(0);
            oled_send_data(bytes, 128);
        }

        phase++;
    }
}


menu_item_t wave = {
    .name = "wave.lmao",
    .parent = NULL,
    .children = NULL,
    .child_count = 0,
    .action = wave_animation
};