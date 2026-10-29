/*
 * pwm.c
 *
 * Created: 14.10.2026 13:24:27
 *  Author: vikin
 */ 
#include "sam.h"
#include "sam3x8e.h"

#define PWM_MCK 84000000
#define PWM_PID 36u
#define PERIOD 52500

uint8_t PWM_ENA_CHID[] = {
	PWM_ENA_CHID0,
	PWM_ENA_CHID1,
	PWM_ENA_CHID2,
	PWM_ENA_CHID3,
	PWM_ENA_CHID4,
	PWM_ENA_CHID5,
	PWM_ENA_CHID6,
	PWM_ENA_CHID7,
};

uint8_t ceil_log2(uint32_t x) {
	uint32_t z = 0;
	for (uint32_t y = x; y; y >>= 1) ++z;
	return z;
}

uint32_t CPRD_from_freq(uint32_t freq, uint8_t CMR){
	return (PWM_MCK >> CMR) / freq;
}

uint32_t CMR_from_freq(uint32_t freq){
	const uint8_t max_size = 16u;
	uint8_t size = ceil_log2(CPRD_from_freq(freq,0));
	uint8_t div = (size > max_size) ? size - max_size : 0;
	uint32_t CMR[] = {
		PWM_CMR_CPRE_MCK,
		PWM_CMR_CPRE_MCK_DIV_2,
		PWM_CMR_CPRE_MCK_DIV_4,
		PWM_CMR_CPRE_MCK_DIV_8,
		PWM_CMR_CPRE_MCK_DIV_16,
		PWM_CMR_CPRE_MCK_DIV_32,
		PWM_CMR_CPRE_MCK_DIV_64,
		PWM_CMR_CPRE_MCK_DIV_128,
		PWM_CMR_CPRE_MCK_DIV_256,
		PWM_CMR_CPRE_MCK_DIV_512,
		PWM_CMR_CPRE_MCK_DIV_1024,
	};
	
	return CMR[div];
}

uint32_t cdty_from_duty(uint16_t duty, uint8_t channel){
	uint32_t CPRD = PWM->PWM_CH_NUM[channel].PWM_CPRD;
	uint32_t CDTY = CPRD-((CPRD*duty) >> 16);
	return CDTY;
}

uint32_t cdty_from_us(uint32_t us){
	return (PERIOD - (2625*us)/1000);
}

void pwm_init(){
	PIOB->PIO_ABSR |=	  PIO_ABSR_P12;				// peripheral B = PWMH0
	PIOB->PIO_ABSR |=	  PIO_ABSR_P13;				// peripheral B = PWMH1
	PIOB->PIO_PDR  |=	  PIO_PDR_P12;				// hand the pin to the peripheral
	PIOB->PIO_PDR  |=	  PIO_PDR_P13;				// hand the pin to the peripheral
	
	PMC->PMC_PCER1	|=	 PMC_PCER1_PID36;		//Aktiverer periferal klokken til PWM (PID = 36)
}

void pwm_channel_init(uint32_t frequency, uint8_t channel, uint16_t start_duty){
		uint32_t CMR = CMR_from_freq(frequency);
		uint32_t CPRD = CPRD_from_freq(frequency,CMR);
	
		PWM->PWM_CH_NUM[channel].PWM_CMR   =  CMR;
		PWM->PWM_CH_NUM[channel].PWM_CPRD  =  PWM_CPRD_CPRD(CPRD);
		PWM->PWM_CH_NUM[channel].PWM_CDTY  =  cdty_from_duty(start_duty,channel);
		PWM->PWM_ENA					  |=  PWM_ENA_CHID[channel];
}

void pwm_set_duty_cycle(uint16_t duty, uint8_t channel){
	uint32_t CPRD = PWM->PWM_CH_NUM[channel].PWM_CPRD;
	uint32_t count = cdty_from_duty(duty,channel);
	count = (count > CPRD) ? CPRD : count;
	count = (count < 0) ? 0 : count;
	PWM->PWM_CH_NUM[channel].PWM_CDTYUPD = count;
}