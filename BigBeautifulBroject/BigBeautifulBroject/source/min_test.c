/*
 * min_test.c
 *
 * Created: 7.10.2026 13:56:37
 *  Author: vikin
 */ 

#include "menu.h"

static menu_item_t min_test_greie = {
	.name = "min_test.lmao",
	.parent = NULL,
	.children = NULL,
	.child_count = 0,
	.action = min_test
};


void setupMin(){
	//setup greier
	//...
	
	
	menu_insert(&min_test_greie,&root)
}

void min_test(){
	while(1){
		//flere greier
	}
}