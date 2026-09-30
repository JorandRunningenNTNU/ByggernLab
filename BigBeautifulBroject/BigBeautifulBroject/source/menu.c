
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
	oled_clear();
	oled_goto_line(0);
	oled_goto_column(0);
	oled_print(menu_current->name);
	for (int i = 0; i < menu_current->child_count && i < 6; i++) {
		oled_goto_line(i+1);
		oled_goto_column(0);
		if (menu_selected == i) {
			oled_print(">");
		}
		else {
			oled_print(" ");
		}
		oled_print(menu_current->children[i]->name);
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
		oled_print("Back");
	}
}

static uint8_t prvs_btn = 0;

uint8_t update_menu() {
	uint8_t n = 0;
	if ((ioboard_data.navButton & ~prvs_btn) & (1<<4)) { //up
		n = 1;
		if (menu_selected == 0) {
			menu_selected = menu_current->child_count - (menu_current == &root);
		} else {
			menu_selected--;
		}
	}
	else if ((ioboard_data.navButton & ~prvs_btn) & (1<<2)) { //down
		n = 1;
		menu_selected++;
		menu_selected %= menu_current->child_count + (menu_current != &root);
	}
	else if((ioboard_data.navButton & ~prvs_btn) & (1<<0)) { //select
		n = 1;
		if (menu_selected == menu_current->child_count) {
			menu_current = menu_current->parent;
			menu_selected = 0;
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
	prvs_btn = ioboard_data.navButton;
	return n;
}

static void say_hello(void)
{
	setupPrintfUART();
	printf("Hello world!\n");
}

menu_item_t *menu_current;
uint8_t menu_selected;

static menu_item_t hello;
static menu_item_t smile;
static menu_item_t your_mom;

static menu_item_t bye;
static menu_item_t hello_again;
static menu_item_t say_hi;


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
	.children = NULL,
	.child_count = 0,
	.action = NULL
};

static menu_item_t *root_children[] = {
	&hello,
	&smile,
	&your_mom
};


menu_item_t root = {
	.name = "Root",
	.parent = NULL,
	.children = root_children,
	.child_count = 3,
	.action = NULL
};