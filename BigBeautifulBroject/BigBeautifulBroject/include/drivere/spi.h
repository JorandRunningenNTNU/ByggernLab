#pragma once

typedef enum {IO, Display, CAN} SPIselect;

void setupSPI();
void slectSlaveSPI(SPIselect slave);
void writeSPI(char data);
char readSPI(SPIselect slave);