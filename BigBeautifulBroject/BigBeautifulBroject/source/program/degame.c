#include <stdio.h>
#include <stdlib.h>
#include "degame.h"
#include "ioBoard.h"
#include "joystick.h"
#include "oled.h"
#include "sram.h"
#include "mcp2515.h"
#include "timer.h"

static uint8_t lives;

int16_t parabola(int16_t t, int16_t a) {
	return a*t - (t*t)/2;
}

void check_can_messages(void) {
	uint8_t rx[10];

	uint8_t flags = mcp2515_which_read_buffer_has_data();

	for (uint8_t buffer = 0; buffer < 2; buffer++) {
		if (flags & (1 << buffer)) {
			mcp2515_read_buffer(buffer, rx);

			if (rx[0] == 67 && rx[1] >= 1) {
				if (rx[2] == 1) {
					ioboard_write_led(lives, 0);
					if (lives != 0) {
						lives -= 1;
						if (lives != 0) {
							mcp2515_send_bytes(1, 64, 0, 1);
						}
					}
				}
			}
		}
	}
}

void death_animation() {
	oled_clear();
	uint8_t symbol[8];
	timer_set_frequency(10);
	timer_flag = 0;
	while(timer_flag < 20) {
		oled_goto_line(4);
		oled_goto_column(59);
		for (uint8_t i = 0; i < 8; i++) {
			symbol[i] = pgm_read_byte(&heart_frames[0][i]);
		}
		oled_send_data(symbol, 8);
	}
		
	mcp2515_send_bytes(0, 66, 0, 1);
	while(timer_flag < 40) {
		oled_goto_line(4);
		oled_goto_column(55);
		for (uint8_t i = 0; i < 8; i++) {
			symbol[i] = pgm_read_byte(&heart_frames[1][i]);
		}
		oled_send_data(symbol, 8);
		for (uint8_t i = 0; i < 8; i++) {
			symbol[i] = pgm_read_byte(&heart_frames[2][i]);
		}
		oled_send_data(symbol, 8);
	}
	timer_flag = 0;
	for (uint8_t i = 0; i < 8; i++) {
		symbol[i] = pgm_read_byte(&heart_frames[3][i]);
	}
	const int8_t vx[8] = {-5, -4, -3, -2, 2, 3, 4, 5};

	const uint8_t vy[8] = {6, 7, 8, 7, 8, 7, 6, 5};

	timer_flag = 0;
	uint8_t last_tick = 255;
		
		
		
	while (timer_flag < 24) {
		// Denne koden er delvis rettet på av ChatGPT
		uint8_t t = timer_flag;

		if (t == last_tick) {
			continue;
		}
		last_tick = t;

		oled_clear();

		for (uint8_t i = 0; i < 8; i++) {int16_t x = 60 + (int16_t)vx[i] * t;int16_t y = 32 - parabola(t, vy[i]);

			if (x >= 0 && x <= 120 &&y >= 0 && y <= 56) {
				for (uint8_t j = 0; j < 8; j++) {
					symbol[j] = pgm_read_byte(&heart_frames[3 + (((i/2)%2) + timer_flag/3)%2 ][j]);
				}

				oled_draw_8x8((uint8_t)x, (uint8_t)y, symbol);
			}
		}
		// Her er den ikke lenger rettet på av ChatGPT
	}
}

void awesome_game() {
	setupJoystick();
	joystick_data_t joystick;
	
	uint8_t can_msg[3];
	
	lives = 6;
	
	for (uint8_t i = 1; i <= 6; i++ ) {
		ioboard_write_led(i, 1);
	}
	
	while (1) {
		ioboard_update_data();
		joystick = joystick_read_data();
		oled_goto_line(0);
		
		oled_print_number(joystick.X);
		
		oled_goto_line(1);
		oled_print_number(joystick.Y);
		
		oled_goto_line(2);
		oled_print_number(lives);
		
		if (ioboard_data.rightButtons & (1<<0)) {
			oled_goto_line(3);
			oled_print("AAAAAAH!!       ");
		} else {
			oled_goto_line(3);
			oled_print("                ");
		}
		
		can_msg[0] = joystick.X;
		can_msg[1] = joystick.Y;
		can_msg[2] = ioboard_data.rightButtons;
		
		mcp2515_send_bytes(0, 69, can_msg, 3);
		
		check_can_messages();
		
		if (ioboard_data.rightButtons & (1<<4)) {
			
			for (uint8_t i = 1; i <= 6; i++ ) {
				ioboard_write_led(i, 0);
			}
			break;
			
		}
		if (ioboard_data.rightButtons & (1<<5)) {
			setupJoystick();
		}
		
		if (lives == 0) {
			break;
		}
	}
	
	for (uint8_t i = 1; i <= 6; i++ ) {
		ioboard_write_led(i, 0);
	}
	
	death_animation();

	oled_clear();
}

menu_item_t degame = {
	.name = "degame.lmao",
	.parent = NULL,
	.children = NULL,
	.child_count = 0,
	.action = awesome_game
};