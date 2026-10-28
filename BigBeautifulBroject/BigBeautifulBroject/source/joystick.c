#include "joystick.h"
#include "adc.h"
#include "uart.h"
#include "sleep.h"
#include "oled.h"
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



joystick_data_t joystick_read_data(){
	uint8_t p_data[4];
	adc_read_all(p_data);
	uint8_t X = p_data[0];
	uint8_t Y = p_data[1];
	
	uint8_t Xa;
	uint8_t Ya;
	
	if (X < Xneu){Xa = (X-Xmin)*128/(Xneu-Xmin);}
	if (X < Xmin){Xa = 0;}
	if (X == Xneu){Xa = 128;}
	if (X > Xneu){Xa = 128 + (X-Xneu)*128/(Xmax-Xneu);}
	if (X > Xmax){Xa = 255;}

	if (Y < Yneu){Ya = (Y-Ymin)*128/(Yneu-Ymin);}
	if (Y < Ymin){Ya = 0;}
	if (Y == Yneu){Ya = 128;}
	if (Y > Yneu){Ya = 128 + (Y-Yneu)*128/(Ymax-Yneu);}
	if (Y > Ymax){Ya = 255;}
		
	if ((Xa > 123) && (Xa < 133)){Xa = 128;}
	if ((Ya > 123) && (Ya < 133)){Ya = 128;}
	
	return (joystick_data_t) {Xa, Ya};
}

joystick_discrete_t joystick_read_discrete(){
	uint8_t p_data[4];
	adc_read_all(p_data);
	uint8_t X = p_data[0];
	uint8_t Y = p_data[1];
	
	if (X > (Xneu + 10)){return Right;}
	if (X < (Xneu - 10)){return Left;}
	if (Y > (Yneu + 10)){return Up;}
	if (Y < (Yneu - 10)){return Down;}
	return Neutral;
}

void setupJoystick(){
	setupUART();
	uint8_t p_data[4];
	printf("Hold joysticken i midten\n");
	oled_clear();
	oled_goto_line(0);
	oled_print("Hold joysticken ");
	oled_goto_line(1);
	oled_print("i midten        ");
	
	// delay
	sleep_ms(5000);

	adc_read_all(p_data);
	Xneu = p_data[0];
	Yneu = p_data[1];
	
	printf("Beveg joysticken rundt i sirkler\n");
	oled_goto_line(0);
	oled_print("Beveg joysticken");
	oled_goto_line(1);
	oled_print("rundt i sirkler ");
	uint32_t n = 5000; // 5 sekunder
	while(n > 1){
		adc_read_all(p_data);
		uint8_t X = p_data[0];
		uint8_t Y = p_data[1];
		
		if (X > Xmax){Xmax = X;}
		if (X < Xmin){Xmin = X;}
		if (Y > Ymax){Ymax = Y;}
		if (Y < Ymin){Ymin = Y;}
		n--;
		sleep_ms(1);
	}
	printf("Ferdig kalibrert\n");
	oled_goto_line(0);
	oled_print("Ferdig kalibrert");
	oled_clear_line(1);
}