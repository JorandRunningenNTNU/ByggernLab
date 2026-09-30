#pragma once

typedef enum {IO, Display, CAN} SPIselect;

void setupSPI();
void spi_unselect_all_slaves();
void spi_select_slave(SPIselect slave);
void spi_write_byte(uint8_t data);
void spi_write(uint8_t* data, uint16_t n);
uint8_t spi_read_byte();
void spi_read(uint8_t* data, uint16_t n);