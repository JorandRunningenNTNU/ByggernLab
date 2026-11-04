/*
 * pwm.c
 *
 * Created: 14.10.2026 13:24:27
 *  Author: vikin
 */ 
#include "sam.h"

#define PWM_MCK 84000000

uint8_t pwm_channels_to_enable[8] = {0};

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

void pwm_init(){
	PIOB->PIO_ABSR |=	  PIO_ABSR_P12;				// peripheral B = PWMH0
	PIOB->PIO_ABSR |=	  PIO_ABSR_P13;				// peripheral B = PWMH1
	PIOB->PIO_PDR  |=	  PIO_PDR_P12;				// hand the pin to the peripheral
	PIOB->PIO_PDR  |=	  PIO_PDR_P13;				// hand the pin to the peripheral
	
	PMC->PMC_PCER1	|=	 PMC_PCER1_PID36;		//Aktiverer periferal klokken til PWM (PID = 36)
}

void pwm_start_all(){
	for(int i = 0; i < 8; i++){
		if(pwm_channels_to_enable[i]){
			PWM->PWM_ENA = PWM_ENA_CHID[i];
		}
	}
}

void pwm_start_channel(uint8_t channel){
	PWM->PWM_ENA = PWM_ENA_CHID[channel];
}

void pwm_channel_init(uint32_t frequency, uint8_t channel){
		PWM->PWM_DIS = PWM_ENA_CHID[channel];
	
		uint32_t CMR = CMR_from_freq(frequency);
		uint32_t CPRD = CPRD_from_freq(frequency,CMR);
		
		PWM->PWM_CH_NUM[channel].PWM_CMR   =  CMR;
		PWM->PWM_CH_NUM[channel].PWM_CPRD  =  PWM_CPRD_CPRD(CPRD);
		pwm_channels_to_enable[channel]	   =  1;
}

uint32_t cdty_from_us(uint32_t us, uint8_t channel){
	uint32_t CPRD = PWM->PWM_CH_NUM[channel].PWM_CPRD;
	uint32_t CMR = ((PWM->PWM_CH_NUM[channel].PWM_CMR) & 0x0F);
	return CPRD -(uint32_t)((((uint64_t)us * (uint64_t)(PWM_MCK/1000000)) >> CMR));
}

void pwm_set_pulse_width_us(uint32_t width_us, uint8_t channel){
	uint32_t CPRD = PWM->PWM_CH_NUM[channel].PWM_CPRD;
	uint32_t CDTY = cdty_from_us(width_us,channel);
	CDTY = (CDTY > CPRD) ? CPRD : CDTY;
	PWM->PWM_CH_NUM[channel].PWM_CDTYUPD = CDTY;
}

void pwm_set_duty_cycle(uint16_t duty, uint8_t channel){
	uint32_t CPRD = PWM->PWM_CH_NUM[channel].PWM_CPRD;
	uint32_t CDTY = cdty_from_duty(duty,channel);
	CDTY = (CDTY > CPRD) ? CPRD : CDTY;
	PWM->PWM_CH_NUM[channel].PWM_CDTYUPD = CDTY;
}