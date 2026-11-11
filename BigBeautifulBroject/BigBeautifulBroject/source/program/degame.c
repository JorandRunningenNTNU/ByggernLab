#include <stdio.h>
#include <stdlib.h>
#include "degame.h"
#include "ioBoard.h"
#include "joystick.h"
#include "oled.h"
#include "sram.h"
#include "mcp2515.h"

static uint8_t lives;

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
					}
				}
			}
		}
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
}

menu_item_t degame = {
	.name = "degame.lmao",
	.parent = NULL,
	.children = NULL,
	.child_count = 0,
	.action = awesome_game
};