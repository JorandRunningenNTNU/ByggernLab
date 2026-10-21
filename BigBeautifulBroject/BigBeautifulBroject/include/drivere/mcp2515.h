#pragma once

void setupMCP2515();
void mcp2515_send_bytes(uint8_t buffer, uint8_t can_id, uint8_t* data, uint8_t nBytes);
uint8_t mcp2515_which_read_buffer_has_data(); // 0-bit er buffer 0, 1-bit er buffer 1
void mcp2515_read_buffer(uint8_t buffer, uint8_t* data); // data må ha 10-byte lengde
