#include <stdio.h>
#include <avr/io.h>
#include "../../include/drivere/mcp2515.h"
#include "../../include/drivere/spi.h"


static void mcp2515_write(uint8_t adress, uint8_t* data, uint8_t nData);
static void mcp2515_write_byte(uint8_t adress, uint8_t data);
static void mcp2515_write_bit(uint8_t adress, uint8_t mask, uint8_t data);
static void mcp2515_transmitt_uploaded_bytes(uint8_t buffer);
static void mcp2515_upload_bytes_for_transmision(uint8_t buffer, uint8_t can_id, uint8_t* data, uint8_t nBytes);
static uint8_t mcp2515_read_byte(uint8_t adress);

void setupMCP2515(){
	// reset innstillinger, dette git automatisk Configuration mode
	spi_select_slave(CAN);
	spi_write_byte((1<<7) | (1<<6));
	spi_unselect_all_slaves();
	
	// recive innstillinger
	mcp2515_write_byte(0x60, 0x64); // aktivere rollover fra buffer 0 til 1, og skur av filtere for buffer 0
	mcp2515_write_byte(0x70, 0x60); // skru av filtere for buffer 0
	
	// Transmitt innstillinger
	// skrur av extended-ID og setter de tre laveste ID-bitene til 0
	mcp2515_write_byte(0x32, 0); // buffer 0
	mcp2515_write_byte(0x42, 0); // buffer 1
	mcp2515_write_byte(0x52, 0); // buffer 2
	
	// Interrupt
	//mcp2515_write_byte(0x2d, (1<<1)|(1<<0));  // skrur på interrupt for recive buffer 0 og 1
	mcp2515_write_byte(0x2B, 0x03);
	
	// Timing
	// TQ = 2(BRP+1)/F_CPU = 2(19 + 1) / 16 MHz = 2,5 ms
	// SJW = 1 TQ
	// Sync = 1 TQ
	// Prop = 2 TQ
	// PS1 = 3 TQ
	// PS2 = 2 TQ
	/*mcp2515_write_byte(0x2a, (3<<0)); // SJW = 1 og BRP = 3
	mcp2515_write_bit(0x29, 0x3f, (3<<3)|(2<<0)); // PS1 = 3, prop = 2
	mcp2515_write_bit(0x28, (7<<0), (1<<0)); // PS2 = 2*/
	mcp2515_write_byte(0x2A, (19<<0));  // SJW=1 | BRP=19,  
	mcp2515_write_byte(0x29, (1<<7) | (2<<3) | (1<<0));  // BTLMODE=1 | PS1=3 | Prop=2
	mcp2515_write_byte(0x28, (1<<0));  // PS2=2
	
	// Bytte til normal opperasjonsmodus
	mcp2515_write_bit(0x0f, (7<<5), 0);
}


void mcp2515_send_bytes(uint8_t buffer, uint8_t can_id, uint8_t* data, uint8_t nBytes){
	mcp2515_upload_bytes_for_transmision(buffer, can_id, data, nBytes);
	mcp2515_transmitt_uploaded_bytes(buffer);
}

uint8_t mcp2515_which_read_buffer_has_data(){
	uint8_t data = mcp2515_read_byte(0b00101100);
	return (data & ((1<<1)|(1<<0)));
}

void mcp2515_read_buffer(uint8_t buffer, uint8_t* data){
	uint8_t trash[3];
	
	spi_select_slave(CAN);
	switch(buffer){
		case 0:
		spi_write_byte(0b10010000); // bruker READ-RX-BUFFER, dette fjerner interuptfalg automatisk
		break;
		
		case 1:
		spi_write_byte(0b10010100); // bruker READ-RX-BUFFER
		break;
	}
	
	data[0] = spi_read_byte(); // ID
	spi_read(trash, 3);
	data[1] = spi_read_byte() & (0x0f); // antall byte med data
	for (uint8_t i=0; i<data[1]; i++){
		data[i+2] = spi_read_byte();
	}
	spi_unselect_all_slaves();
}

void mcp2515_change_mode(uint8_t mode){
	switch(mode){
	case 0:
		mcp2515_write_bit(0x0f, (7<<5), 0); // normal
		break;
	
	case 1:
		mcp2515_write_bit(0x0f, (7<<5), (1<<6)); // loopback
		break;
	}
}


static void mcp2515_write(uint8_t adress, uint8_t* data, uint8_t nData){
	spi_select_slave(CAN);
	spi_write_byte((1<<1)); // bruker WRITE
	spi_write_byte(adress);
	for (uint8_t i = 0; i<nData; i++){
		spi_write_byte(data[i]);
	}
	spi_unselect_all_slaves();
}

static void mcp2515_write_byte(uint8_t adress, uint8_t data){
	spi_select_slave(CAN);
	spi_write_byte((1<<1)); // bruker WRITE
	spi_write_byte(adress);
	spi_write_byte(data);
	spi_unselect_all_slaves();
}

static void mcp2515_write_bit(uint8_t adress, uint8_t mask, uint8_t data){
	spi_select_slave(CAN);
	spi_write_byte((1<<2) | (1<<0)); // bruker BIT MODIFY
	spi_write_byte(adress);
	spi_write_byte(mask);
	spi_write_byte(data);
	spi_unselect_all_slaves();
}

static void mcp2515_transmitt_uploaded_bytes(uint8_t buffer){
	spi_select_slave(CAN);
	spi_write_byte((1 << 7) | (1 << buffer)); // bruker REQUEST-TO-SEND
	spi_unselect_all_slaves();
}

static void mcp2515_upload_bytes_for_transmision(uint8_t buffer, uint8_t can_id, uint8_t* data, uint8_t nBytes){
	uint8_t command_data = (1<<6); // bruker LOAD TX BUFFER
	uint8_t command_id = (1<<6); // bruker LOAD TX BUFFER
	uint8_t adress_nByte = (1<<2) | (1<<0);
	
	switch(buffer){
		case 0:
			command_data |= (1<<0);
			adress_nByte |= (1<<5) | (1<<4);
			break;
			
		
		case 1:
			command_id |= (1<<1);
			command_data |= (1<<1) | (1<<0);
			adress_nByte |= (1<<6);
			break;
		
		case 2:
			command_id |= (1<<2);
			command_data |= (1<<2) | (1<<0);
			adress_nByte |= (1<<6) | (1<<4);
			break;
	}
	
	// Laster opp id
	spi_select_slave(CAN);
	spi_write_byte(command_id);
	spi_write_byte(can_id);
	spi_unselect_all_slaves();
	
	// laster opp data
	spi_select_slave(CAN);
	spi_write_byte(command_data);
	for (uint8_t i = 0; i<nBytes; i++){
		spi_write_byte(data[i]);
	}
	spi_unselect_all_slaves();
	
	// laster opp lengden til dataen
	mcp2515_write_byte(adress_nByte, nBytes);
}

static uint8_t mcp2515_read_byte(uint8_t adress){
	spi_select_slave(CAN);
	spi_write_byte((1<<1)|(1<<0)); // bruker READ
	spi_write_byte(adress);
	uint8_t data = spi_read_byte();
	spi_unselect_all_slaves();
	return data;
}

