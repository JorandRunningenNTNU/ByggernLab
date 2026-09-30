
#include <string.h>
#include "../include/menu.h"
#include "../include/drivere/oled.h"
#include "../include/drivere/ioBoard.h"
#include "../include/drivere/uart.h"

void setupMenu(){
	menu_current = &root;
	menu_selected = 0;
}

void render_menu() {
	oled_goto_line(0);
	oled_goto_column(0);
	oled_print(menu_current->name);
	uint8_t len = strlen(menu_current->name);

	for (uint8_t j = len; j < 15; j++) {
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
		if (menu_selected == 0) {
			menu_selected = menu_current->child_count - (menu_current == &root);
		} else {
			menu_selected--;
		}
	}
	else if ((ioboard_data.navButton & ~prvs_btn) & (1<<2)) { //down
		menu_selected++;
		menu_selected %= menu_current->child_count + (menu_current != &root);
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
			menu_item_t *previous_menu = menu_current;

			menu_current = menu_current->parent;

			for (uint8_t i = 0; i < menu_current->child_count; i++) {
				if (menu_current->children[i] == previous_menu) {
					menu_selected = i;
					break;
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

static void say_hello(void)
{
	setupPrintfUART();
	printf("Hello world!\n");
}

menu_item_t *menu_current;
uint8_t menu_selected;
uint8_t menu_shift = 0;

static menu_item_t hello;
static menu_item_t smile;
static menu_item_t your_mom;
static menu_item_t placeholder0;
static menu_item_t placeholder1;
static menu_item_t placeholder2;
static menu_item_t placeholder3;
static menu_item_t placeholder4;

static menu_item_t bye;
static menu_item_t hello_again;
static menu_item_t say_hi;

static menu_item_t nude0;
static menu_item_t nude1;
static menu_item_t nude2;
static menu_item_t nude3;
static menu_item_t nude4;
static menu_item_t nude5;
static menu_item_t nude6;
static menu_item_t nude7;
static menu_item_t nude8;


static menu_item_t bye = {
	.name = "Bye",
	.parent = &hello,
	.children = NULL,
	.child_count = 0,
	.action = NULL
};

static menu_item_t hello_again = {
	.name = "Hello_Again",
	.parent = &hello,
	.children = NULL,
	.child_count = 0,
	.action = NULL
};

static menu_item_t *hello_children[] = {
	&bye,
	&hello_again
};


static menu_item_t say_hi = {
	.name = "say_hi.lmao",
	.parent = &smile,
	.children = NULL,
	.child_count = 0,
	.action = say_hello
};

static menu_item_t *smile_children[] = {
	&say_hi
};

static menu_item_t nude0 = {
	.name = "nude0",
	.parent = &your_mom,
	.children = NULL,
	.child_count = 0,
	.action = NULL
};

static menu_item_t nude1 = {
	.name = "nude1",
	.parent = &your_mom,
	.children = NULL,
	.child_count = 0,
	.action = NULL
};

static menu_item_t nude2 = {
	.name = "nude2",
	.parent = &your_mom,
	.children = NULL,
	.child_count = 0,
	.action = NULL
};

static menu_item_t nude3 = {
	.name = "nude3",
	.parent = &your_mom,
	.children = NULL,
	.child_count = 0,
	.action = NULL
};

static menu_item_t nude4 = {
	.name = "nude4",
	.parent = &your_mom,
	.children = NULL,
	.child_count = 0,
	.action = NULL
};

static menu_item_t nude5 = {
	.name = "nude5",
	.parent = &your_mom,
	.children = NULL,
	.child_count = 0,
	.action = NULL
};

static menu_item_t nude6 = {
	.name = "nude6",
	.parent = &your_mom,
	.children = NULL,
	.child_count = 0,
	.action = NULL
};

static menu_item_t nude7 = {
	.name = "nude7",
	.parent = &your_mom,
	.children = NULL,
	.child_count = 0,
	.action = NULL
};

static menu_item_t nude8 = {
	.name = "nude8",
	.parent = &your_mom,
	.children = NULL,
	.child_count = 0,
	.action = NULL
};

static menu_item_t *your_mom_children[] = {
	&nude0,
	&nude1,
	&nude2,
	&nude3,
	&nude4,
	&nude5,
	&nude6,
	&nude7,
	&nude8,
};


static menu_item_t hello = {
	.name = "Hello",
	.parent = &root,
	.children = hello_children,
	.child_count = 2,
	.action = NULL
};

static menu_item_t smile = {
	.name = ":3",
	.parent = &root,
	.children = smile_children,
	.child_count = 1,
	.action = NULL
};

static menu_item_t your_mom = {
	.name = "Your_Mom",
	.parent = &root,
	.children = your_mom_children,
	.child_count = 9,
	.action = NULL
};

static menu_item_t placeholder0 = {
	.name = "placeholder0",
	.parent = &root,
	.children = NULL,
	.child_count = 0,
	.action = NULL
};
static menu_item_t placeholder1 = {
	.name = "placeholder1",
	.parent = &root,
	.children = NULL,
	.child_count = 0,
	.action = NULL
};

static menu_item_t placeholder2 = {
	.name = "placeholder2",
	.parent = &root,
	.children = NULL,
	.child_count = 0,
	.action = NULL
};

static menu_item_t placeholder3 = {
	.name = "placeholder3",
	.parent = &root,
	.children = NULL,
	.child_count = 0,
	.action = NULL
};

static menu_item_t placeholder4 = {
	.name = "placeholder4",
	.parent = &root,
	.children = NULL,
	.child_count = 0,
	.action = NULL
};

static menu_item_t *root_children[] = {
	&hello,
	&smile,
	&your_mom,
	&placeholder0,
	&placeholder1,
	&placeholder2,
	&placeholder3,
	&placeholder4
};


menu_item_t root = {
	.name = "Root",
	.parent = NULL,
	.children = root_children,
	.child_count = 8,
	.action = NULL
};