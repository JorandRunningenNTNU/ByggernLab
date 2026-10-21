#pragma once
#include <stdlib.h>
#include <avr/io.h>

#define XRAM __attribute__((section(".xram,\"aw\",@nobits;")))

void sram_test(void);
void setupSRAM();