
#include <string.h>
#include <stdlib.h>
#include "../include/menu.h"
#include "../include/drivere/oled.h"
#include "../include/drivere/ioBoard.h"
#include "../include/drivere/uart.h"

#include "../include/program/wave.h"

menu_item_t root = {
	.name = "Root",
	.parent = NULL,
	.children = NULL,
	.child_count = 0,
	.action = NULL
};

menu_item_t scripts = {
	.name = "scripts",
	.parent = NULL,
	.children = NULL,
	.child_count = 0,
	.action = NULL
};

menu_item_t test_scripts = {
	.name = "test_scripts",
	.parent = NULL,
	.children = NULL,
	.child_count = 0,
	.action = NULL
};

static void say_hello(void)
{
	setupUART();
	printf("Hello world!\n");
}

static menu_item_t say_hi = {
	.name = "say_hi.lmao",
	.parent = NULL,
	.children = NULL,
	.child_count = 0,
	.action = say_hello
};


void setupMenu(){
	menu_current = &root;
	menu_selected = 0;
	
	menu_insert(&scripts, &root);
	
	menu_insert(&say_hi, &scripts);
	menu_insert(&wave, &scripts);
}

void render_menu() {
	oled_goto_line(0);
	oled_goto_column(0);
	oled_print(menu_current->name);
	uint8_t len = strlen(menu_current->name);

	for (uint8_t j = len; j < 16; j++) {
		oled_print(" ");
	}
	for (int i = 0; i < 6; i++) {
		if (i + menu_shift < menu_current->child_count) {
			oled_goto_line(i+1);
			oled_goto_column(0);
			if (menu_selected == i + menu_shift) {
				oled_print(">");
			}
			else {
				oled_print(" ");
			}
			oled_print(menu_current->children[i + menu_shift]->name);
			len = strlen(menu_current->children[i + menu_shift]->name);

			for (uint8_t j = len; j < 15; j++) {
				oled_print(" ");
			}
		}
		else {
			oled_clear_line(i+1);
		}

	}
	if (menu_current != &root) {
		oled_goto_line(7);
		oled_goto_column(0);
		if (menu_selected == menu_current->child_count) {
			oled_print(">");
		}
		else {
			oled_print(" ");
		}
		oled_print("Back           ");
	} 
	else {
		oled_clear_line(7);
	}
}

static uint8_t prvs_btn = 0;

void update_menu() {
	if ((ioboard_data.navButton & ~prvs_btn) & (1<<4)) { //up
		if (!(menu_current == &root && menu_current->child_count == 0)) {
			if (menu_selected == 0) {
				menu_selected = menu_current->child_count - (menu_current == &root);
				} else {
				menu_selected--;
			}
		}
	}
	else if ((ioboard_data.navButton & ~prvs_btn) & (1<<2)) { //down
		if (!(menu_current == &root && menu_current->child_count == 0)) {
			menu_selected++;
			menu_selected %= menu_current->child_count + (menu_current != &root);
		}
	}
	else if ((ioboard_data.navButton & ~prvs_btn) & (1<<3)) { // back
		if (menu_current != &root) {
			menu_item_t *previous_menu = menu_current;

			menu_current = menu_current->parent;

			for (uint8_t i = 0; i < menu_current->child_count; i++) {
				if (menu_current->children[i] == previous_menu) {
					menu_selected = i;
					break;
				}
			}
		}
	}
	else if((ioboard_data.navButton & ~prvs_btn) & (1<<0)) { //select
		if (menu_selected == menu_current->child_count) {
			if (menu_current != &root) {
				menu_item_t *previous_menu = menu_current;

				menu_current = menu_current->parent;

				for (uint8_t i = 0; i < menu_current->child_count; i++) {
					if (menu_current->children[i] == previous_menu) {
						menu_selected = i;
						break;
					}
				}
			}
		}
		else {
			if (strlen(menu_current->children[menu_selected]->name) >= 5 && strcmp(menu_current->children[menu_selected]->name + strlen(menu_current->children[menu_selected]->name) - 5, ".lmao") == 0) {
				menu_current->children[menu_selected]->action();
			} else {
				menu_current = menu_current->children[menu_selected];
				menu_selected = 0;
			}
		}
		
	}
	if ((menu_selected - menu_shift > 5) & ~(menu_selected == menu_current->child_count)) {
		menu_shift = menu_selected - 5;
	}
	else if (menu_selected < menu_shift) {
		menu_shift = menu_selected;
	}
	prvs_btn = ioboard_data.navButton;
}

uint8_t menu_insert(menu_item_t *new_item, menu_item_t *parent)
{
	menu_item_t **new_children;

	new_children = malloc(
	(parent->child_count + 1) * sizeof(menu_item_t *)
	);

	if (new_children == NULL) {
		return 0;
	}

	for (uint8_t i = 0; i < parent->child_count; i++) {
		new_children[i] = parent->children[i];
	}

	new_children[parent->child_count] = new_item;

	free(parent->children);

	parent->children = new_children;
	parent->child_count++;

	new_item->parent = parent;

	return 1;
}

menu_item_t *menu_current;
uint8_t menu_selected;
uint8_t menu_shift = 0;