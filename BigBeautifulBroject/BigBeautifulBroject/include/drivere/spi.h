#pragma once

typedef enum {IO, Display, CAN} SPIselect;

void setupSPI();
void writeByteSPI(uint8_t data, SPIselect slave);
void writeSPI(uint8_t* data, uint16_t n, SPIselect slave);
uint8_t readByteSPI(SPIselect slave);
void readSPI(uint8_t* data, uint16_t n, SPIselect slave);