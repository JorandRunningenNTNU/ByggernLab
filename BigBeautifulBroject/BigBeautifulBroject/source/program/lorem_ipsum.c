/*
 * lorem_ipsum.c
 *
 * Created: 7.10.2026 15:08:15
 *  Author: vikin
 */ 
#include <stdio.h>
#include "menu.h"
#include "sram.h"

//lorem ipsum is a lie

int16_t lorem[1024] XRAM;

void print_lorem_ipsum(){
	for(int i = 0; i < 1024; i++){
		lorem[i] = i;
	}
	for(int i = 0; i < 1024; i++){
		printf("Tall: %d\n",lorem[i]);
	}
}

menu_item_t  lorem_ipsum = {
	.name = "lorem_ipsum.lmao",
	.parent = NULL,
	.children = NULL,
	.child_count = 0,
	.action = print_lorem_ipsum
};