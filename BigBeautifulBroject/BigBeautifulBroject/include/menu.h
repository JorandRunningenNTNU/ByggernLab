#pragma once

#include <stddef.h>
#include <stdint.h>

typedef struct menu_item_t {
	const char *name;
	struct menu_item_t *parent;
	struct menu_item_t **children;
	uint8_t child_count;

	void (*action)(void);
} menu_item_t;

void render_menu();
void update_menu();

extern menu_item_t root;

extern menu_item_t *menu_current;

extern uint8_t menu_selected;

extern uint8_t menu_shift;

void setupMenu();


