#pragma once

typedef enum {IO, Display, CAN} SPIselect;

void setupSPI();
void slectSlaveSPI(SPIselect slave);
void writeByteSPI(char data);
void writeSPI(char* data, uint16_t n);
char readSPI(SPIselect slave);