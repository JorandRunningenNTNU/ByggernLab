#pragma once

typedef enum {IO, Display, CAN} SPIselect;

void setupSPI();
void selectSlaveSPI(SPIselect slave);
void writeByteSPI(char data, SPIselect slave);
void writeSPI(char* data, uint16_t n, SPIselect slave);
char readSPI(SPIselect slave);