/*
 * menu_builder.c
 *
 * Created: 7.10.2026 15:02:52
 *  Author: vikin
 */ 

#include "menu.h"

extern menu_item_t test;
extern menu_item_t wave;
extern menu_item_t test_sram;
extern menu_item_t lorem_ipsum;
extern menu_item_t can_test_1;

static menu_item_t *programs[] = {
	&can_test_1,
	&lorem_ipsum,
	&test_sram,
	&wave,
	&test
};

#define PROGRAM_COUNT \
(sizeof(programs) / sizeof(programs[0]))

void buildMenu(void)
{
	for (uint8_t i = 0; i < PROGRAM_COUNT; i++)
	{
		menu_insert(programs[i], &scripts);
	}
}