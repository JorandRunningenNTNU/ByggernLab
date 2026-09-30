#pragma once
typedef struct{
	uint8_t touchpadX;
	uint8_t touchpadY;
	uint8_t touchpadSize;
	
	uint8_t touchSliderX;
	uint8_t touchSliderSize;
	
	uint8_t joystickX;
	uint8_t joystickY;
	uint8_t joystickButton;
	
	uint8_t rightButtons;
	uint8_t leftButtons;
	uint8_t navButton;
} IoData;

extern IoData ioData;

void updateIoData();
void turnOnLeds(uint8_t led, uint8_t on);
void turnOnLedsPWM(uint8_t led, uint8_t brightness);