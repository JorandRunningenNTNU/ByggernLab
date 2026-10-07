#include "../../include/program/cantest.h"
#include "../../include/drivere/uart.h"
#include "../../include/drivere/mcp2515.h"
#include "../../include/drivere/spi.h"
#include "../../include/drivere/ioBoard.h"

void can_test_node_1(){
	setupPrintfUART();
	setupSPI();
	setupMCP2515();
	uint8_t data[8];
	uint8_t readData[10];
	uint8_t n = 0;
	while(1){
		
		ioboard_update_data();
		if ((ioboard_data.navButton) & (1<<3)) {
			break;
		}
		
		for(uint8_t i = 0; i<8; i++){
			data[i] = n+i;
		}
		mcp2515_send_bytes(0, 1, data, 8);
		mcp2515_read_buffer(0, readData);
		
		printf("id: %d \t", readData[0]);
		printf("lengde: %d \t", readData[1]);
		printf("data: ");
		for(uint8_t i = 2; i<10; i++){
			printf("%d \t", readData[i]);
		}
		printf("\n");
		
		n++;
	}
}

menu_item_t can_test_1 = {
	.name = "can_test_node_1.lmao",
	.parent = NULL,
	.children = NULL,
	.child_count = 0,
	.action = can_test_node_1
};