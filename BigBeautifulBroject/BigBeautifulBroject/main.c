//#define F_CPU 4915200

#include <stdio.h>
#include "ioboard.h"
#include "menu.h"
#include "setup.h"

int main(void){
	setup();
	
	while(1){
		ioboard_update_data();
		update_menu();
		render_menu();
		continue;
	}
}

