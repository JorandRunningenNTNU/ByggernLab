#include <stdio.h>
#include "degame.h"
#include "ioBoard.h"
#include "joystick.h"
#include "oled.h"
#include "sram.h"
#include "mcp2515.h"

void awesome_game() {
	setupJoystick();
	joystick_data_t joystick;
	char buffer[17];
	uint8_t right_button_1 = (1<<0);
	uint8_t right_button_5 = (1<<4);
	uint8_t right_button_6 = (1<<5);
	
	uint8_t can_msg[3];
	while (1) {
		ioboard_update_data();
		joystick = joystick_read_data();
		oled_goto_line(0);
		snprintf(buffer, sizeof(buffer), "%-16d", (signed int)joystick.X);
		oled_print(buffer);
		
		oled_goto_line(1);
		snprintf(buffer, sizeof(buffer), "%-16d", (signed int)joystick.Y);
		oled_print(buffer);
		
		if (ioboard_data.rightButtons & right_button_1) {
			oled_goto_line(2);
			oled_print("AAAAAAH!!       ");
		} else {
			oled_goto_line(2);
			oled_print("                ");
		}
		
		can_msg[0] = joystick.X;
		can_msg[1] = joystick.Y;
		can_msg[2] = ioboard_data.rightButtons;
		
		mcp2515_send_bytes(0, 67, can_msg, 3);
		
		if (ioboard_data.rightButtons & right_button_5) {
			break;
		}
		if (ioboard_data.rightButtons & right_button_6) {
			setupJoystick();
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