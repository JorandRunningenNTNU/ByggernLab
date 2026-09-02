#pragma once
#include "avr/io.h"

void initilize();
void sendByte(unsigned char data);
unsigned char readByte();