/*
 * setup.c
 *
 * Created: 7.10.2026 13:58:05
 *  Author: vikin
 */ 

#include "sram.h"
#include "adc.h"
#include "spi.h"
#include "oled.h"
#include "uart.h"
#include "menu.h"
#include "menu_builder.h"

void setup(){
	setupUART();
	setupSRAM();
	setupADC();
	setupSPI();
	setupOLED();

	setupMenu();
	buildMenu();
}