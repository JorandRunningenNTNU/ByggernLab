#pragma once
#include <avr/io.h>

void setupDisplay();
void oled_reset();
void oled_home();
void oled_goto_line(uint8_t line);
void oled_goto_column(uint8_t column);
void oled_clear_line(uint8_t line);
void oled_pos(uint8_t row,uint8_t column);
void oled_print(char* data);
void sendCommand(uint8_t* command, uint8_t n);
void sendData(uint8_t* data, uint8_t n);