#pragma once

typedef enum {IO, Display, CAN} spi_select_t;

void setupSPI();
void spi_unselect_all_slaves();
void spi_select_slave(spi_select_t slave);
void spi_write_byte(uint8_t data);
void spi_write(uint8_t* data, uint16_t n);
void spi_write_INT(uint8_t* data, uint16_t n);
uint8_t spi_read_byte();
void spi_read(uint8_t* data, uint16_t n);