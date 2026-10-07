#include <avr/io.h>
#include <stdlib.h>
#include "..\..\include\drivere\spi.h"
#include "..\..\include\sleep.h"
#include "..\..\include\drivere\uart.h"

volatile uint8_t spi_INT_busy = 0;
volatile uint8_t *spi_tx_data;
volatile uint8_t spi_tx_length;
volatile uint8_t spi_tx_index;


// PB1 Can
// PB3 Display
// PB4 IO 

void setupSPI(){
	DDRB |= (1 << 7) | (1 << 5) | (1 << 4) | (1 << 3) | (1 << 1); // PBn som output
	spi_unselect_all_slaves();
	
	// valgte innstillinger
	// MSB sendes først
	// klokken er lav i ideal state
	// Sampler på leading edge
	// SCK frekvens er f_osc/4, antar f_osc er den interne frekvensen
	SPSR |= (1 << SPI2X);
	SPCR |= (1 << MSTR); // setter atmega til master
	SPCR |= (1 << SPE); // enable SPI
}

void spi_unselect_all_slaves(){
	PORTB |= (1 << 4) | (1 << 3) | (1 << 1);
}

void spi_select_slave(spi_select_t slave){
	spi_unselect_all_slaves();
	
	if (slave == CAN){PORTB &= ~(1 << 1);}
	if (slave == Display){PORTB &= ~(1 << 3);}
	if (slave == IO){PORTB &= ~(1 << 4);}
}

void spi_write_INT(uint8_t* data, uint8_t n){
	while(spi_INT_busy);
	spi_INT_busy = 1;
	spi_tx_index = 0;
	spi_tx_length = n;
	spi_tx_data = data;
	SPCR |= (1 << SPIE);
	sei();
	SPDR = spi_tx_data[spi_tx_index++];
}

ISR(SPI_STC_vect){
	if(spi_tx_index < spi_tx_length){
		SPDR = spi_tx_data[spi_tx_index++];
	}
	else
	{
		SPCR &= ~(1 << SPIE);
		spi_INT_busy = 0;
	}
}

void spi_write_byte(uint8_t data){
	SPDR = data;
	while(!(SPSR & (1 << 7))){}
}

void spi_write(uint8_t* data, uint16_t n){
	for(uint16_t i = 0; i<n; i++){
		spi_write_byte(data[i]);
	}
}

uint8_t spi_read_byte(){
	spi_write_byte(0);
	return SPDR;
}

void spi_read(uint8_t* data, uint16_t n){
	for(uint16_t i = 0; i<n; i++){
		data[i] = spi_read_byte();
	}
}
