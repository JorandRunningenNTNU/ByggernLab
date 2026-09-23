#pragma once
typedef struct{
	uint8_t joystickX;
	uint8_t joystickY;
	uint8_t joystickButton;
	uint8_t rightButtons;
	uint8_t leftButtons;
	uint8_t navButton;
} IoData;

extern IoData ioData;

void updateIoData();