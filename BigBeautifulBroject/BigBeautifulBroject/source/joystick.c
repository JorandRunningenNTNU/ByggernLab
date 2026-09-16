#include "../include/joystick.h"
#include "../include/drivere/adc.h"
#include "../include/drivere/uart.h"
#include "../include/sleep.h"
// høyre/venstre er analog channel 0
// opp/ned er analog channel 1

// venstre 68
// høyre 245
// opp 243
// ned 70
// midt 162, 161

uint8_t Xmax = 0;
uint8_t Xmin = 255;
uint8_t Xneu = 0;
uint8_t Ymax = 0;
uint8_t Ymin = 255;
uint8_t Yneu = 0;



JoysticAnalog readJoystickAnalog(){
	uint8_t p_data[4];
	readADC(p_data);
	uint8_t X = p_data[0];
	uint8_t Y = p_data[1];
	
	uint8_t Xa;
	uint8_t Ya;
	
	if (X>=Xneu){Xa = (X-Xneu)*100/(Xmax-Xneu);}
	if (X<Xneu){Xa = (X-Xneu)*100/(Xneu-Xmin);}
	if (Y>=Xneu){Ya = (Y-Yneu)*100/(Ymax-Yneu);}
	if (Y<Xneu){Ya = (Y-Yneu)*100/(Yneu-Ymin);}
		
	if ((Xa > -10) && (Xa < 10)){Xa = 0;}
	if ((Ya > -10) && (Ya < 10)){Ya = 0;}
	
	return (JoysticAnalog) {Xa, Ya};
}

JoystickDiscrete readJoystickDiscrete(){
	uint8_t p_data[4];
	readADC(p_data);
	uint8_t X = p_data[0];
	uint8_t Y = p_data[1];
	
	if (X > (Xneu + 10)){return Right;}
	if (X < (Xneu - 10)){return Left;}
	if (Y > (Yneu + 10)){return Up;}
	if (Y < (Yneu - 10)){return Down;}
	return Neutral;
}

void joystickCalibrate(){
	setupPrintf();
	uint8_t p_data[4];
	printf("Hold joysticken i midten\n");
	
	// delay
	sleep(5000);

	readADC(p_data);
	Xneu = p_data[0];
	Yneu = p_data[1];
	
	printf("Beveg joysticken rundt i sirkler\n");
	uint32_t n = 5000; // 5 sekunder
	while(n > 1){
		readADC(p_data);
		uint8_t X = p_data[0];
		uint8_t Y = p_data[1];
		
		if (X > Xmax){Xmax = X;}
		if (X < Xmin){Xmin = X;}
		if (Y > Ymax){Ymax = Y;}
		if (Y < Ymin){Ymin = Y;}
		n--;
		sleep(1);
	}
	printf("Ferdig kalibrert\n");
}