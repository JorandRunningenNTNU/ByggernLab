#pragma once

typedef enum {IO, Display, CAN} SPIselect;

void setupSPI();
void unSelectAllSlavesSPI();
void selectSlaveSPI();
void writeByteSPI(uint8_t data);
void writeSPI(uint8_t* data, uint16_t n);
uint8_t readByteSPI();
void readSPI(uint8_t* data, uint16_t n);