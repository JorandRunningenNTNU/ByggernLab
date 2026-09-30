#pragma once
#include <avr/io.h>

void setupDisplay();
void oled_clear();
void oled_home();
void oled_goto_line(uint8_t line);
void oled_goto_column(uint8_t column);
void oled_clear_line(uint8_t line);
void oled_pos(uint8_t row,uint8_t column);
void oled_print(const char* data);
void oled_send_command(uint8_t* command, uint8_t n);
void oled_send_data(uint8_t* data, uint8_t n);
