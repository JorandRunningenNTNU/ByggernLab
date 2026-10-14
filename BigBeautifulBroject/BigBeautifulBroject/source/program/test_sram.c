/*
 * sram_test.c
 *
 * Created: 7.10.2026 15:11:42
 *  Author: vikin
 */ 

#include "sram.h"
#include "menu.h"

menu_item_t  test_sram = {
	.name = "test_sram.lmao",
	.parent = NULL,
	.children = NULL,
	.child_count = 0,
	.action = sram_test
};