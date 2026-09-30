#include <avr/io.h>
#include <stdlib.h>
#include "..\..\include\drivere\ioBoard.h"
#include "..\..\include\drivere\spi.h"
#include "..\..\include\sleep.h"

static void readData(uint8_t adress, uint8_t* data, uint8_t nBytes);

ioboard_data_t ioboard_data;


void ioboard_update_data(){
	uint8_t data[3];
	
	// lese touchpad
	readData(0x01, data, 3);
	ioboard_data.touchpadX = data[0];
	ioboard_data.touchpadY = data[1];
	ioboard_data.touchpadSize = data[2];
	
	// lese touchslider
	readData(0x02, data, 2); 
	ioboard_data.touchSliderX = data[0];
	ioboard_data.touchSliderSize = data[1];
	
	// lese joystick
	readData(0x03, data, 3);
	ioboard_data.joystickX = data[0];
	ioboard_data.joystickY = data[1];
	ioboard_data.joystickButton = data[2];
	
	// lese buttons
	readData(0x04, data, 3);
	ioboard_data.rightButtons = data[0];
	ioboard_data.leftButtons = data[1];
	ioboard_data. navButton = data[2];
	
}

void readData(uint8_t adress, uint8_t* data, uint8_t nBytes){
	spi_select_slave(IO);
	spi_write_byte(adress);
	sleepUs(40);
	spi_read(data, nBytes);
	spi_unselect_all_slaves();
}

void ioboard_write_led(uint8_t led, uint8_t on){
	led -=1;
	spi_select_slave(IO);
	spi_write_byte(0x05);
	sleepUs(40);
	spi_write_byte(led);
	spi_write_byte(on);
	spi_unselect_all_slaves();
}

void ioboard_write_led_pwm(uint8_t led, uint8_t brightness){
	led -=1;
	spi_select_slave(IO);
	spi_write_byte(0x06);
	sleepUs(40);
	spi_write_byte(led);
	spi_write_byte(brightness);
	spi_unselect_all_slaves();
}



