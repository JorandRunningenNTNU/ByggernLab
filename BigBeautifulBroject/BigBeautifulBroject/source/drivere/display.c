#include <avr/io.h>
#include <stdlib.h>
#include "..\..\include\drivere\display.h"
#include "..\..\include\drivere\spi.h"
#include "..\..\include/fonts.h"

void setupDisplay(){
	DDRB |= (1 << 2);
	
	uint8_t command[3];
	
	command[0] = 0xAE;
	oled_send_command(command, 1); // Turns display off (resets) for configuration
	
	command[0] = 0xD5; // Set display clock divide ratio/oscillator frequency
	command[1] = 0xF0; // 15 oscillator value 1 divide ratio.
	oled_send_command(command, 2);
	
	command[0] = 0xA8; // Set command
	command[1] = 0x3F ;// 1/64 duty (64 rows)
	oled_send_command(command, 2);

	command[0] = 0xD3; // Set display offset
	command[1] = 0x00; // Offset 0
	oled_send_command(command, 2);
	
	command[0] = 0x40;
	oled_send_command(command, 1); // Start line 0
	
	command[0] = 0x21;
	command[1] = 0x00;
	command[2] = 0x7F;
	oled_send_command(command, 3);
	
	command[0] = 0xA1;
	oled_send_command(command, 1); // Segment remap
	
	command[0] = 0xC8;
	oled_send_command(command, 1); // COM scan direction
	
	command[0] = 0xDA; // Set COM pins hardware configuration
	command[1] = 0x12;
	oled_send_command(command, 2);
	
	command[0] = 0x81; // Set contrast
	command[1] = 0xFF; // Contrast value
	oled_send_command(command, 2);
	
	command[0] = 0xA6;
	oled_send_command(command, 1); // Normal display
	command[0] = 0xA4;
	oled_send_command(command, 1); // Display follows ram
	command[0] = 0xAF;
	oled_send_command(command, 1); // Display ON
	
	oled_clear();
}

void oled_send_command(uint8_t* command, uint8_t n) {
	PORTB &= ~(1 << 2); // Sets Data/Command to low
	spi_select_slave(Display);
	spi_write(command, n); //Writes command
	spi_unselect_all_slaves();
}

void oled_send_data(uint8_t* data, uint8_t n) {
	PORTB |= (1 << 2); // Sets Data/command to high
	spi_select_slave(Display);
	spi_write(data, n);
	spi_unselect_all_slaves();
}

void oled_clear()
{
	uint8_t zero = 0x00;

	for (uint8_t page = 0; page < 8; page++) {
		oled_goto_line(page);

		// Set column back to 0
		uint8_t commands[2] = {0x00, 0x10};
		oled_send_command(commands, 2);

		for (uint8_t col = 0; col < 128; col++) {
			oled_send_data(&zero, 1);
		}
	}
}

void oled_clear_line(uint8_t line){
		uint8_t zero = 0x00;
		oled_goto_line(line);
		for (uint8_t col = 0; col < 128; col++) {
			oled_send_data(&zero, 1);
		}
}

void oled_goto_line(uint8_t line) {
	uint8_t command[1];
	command[0] = 0xB0 + (line % 8);
	oled_send_command(command, 1);
}

void oled_goto_column(uint8_t column) {
	uint8_t command[2];

	command[0] = 0x00 | (column & 0x0F);         // Lower 4 bits
	command[1] = 0x10 | ((column >> 4) & 0x0F);  // Upper 4 bits

	oled_send_command(command, 2);
}

void oled_pos(uint8_t row,uint8_t column){
	oled_goto_line(row);
	oled_goto_column(column);
}

void oled_print(char* data) {
	uint8_t character[8];

	while (*data != '\0') {
		for (uint8_t i = 0; i < 8; i++) {
			character[i] = pgm_read_byte(&font8[*data - ' '][i]);
		}

		oled_send_data(character, 8);

		data++;
	}
}
