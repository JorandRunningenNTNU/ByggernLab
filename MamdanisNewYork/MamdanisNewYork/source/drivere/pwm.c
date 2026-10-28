/*
 * pwm.c
 *
 * Created: 14.10.2026 13:24:27
 *  Author: vikin
 */ 
#include "sam.h"
#include "sam3x8e.h"
#define PWM_PID 36u
#define PERIOD 52500


uint32_t cdty_from_us(uint32_t us){
	return (PERIOD - (2625*us)/1000);
}

void pwm_init(){
	PIOB->PIO_ABSR |=	  PIO_ABSR_P13;				// peripheral B = PWMH1
	PIOB->PIO_PDR   =	  PIO_PDR_P13;				// hand the pin to the peripheral
	
	PMC->PMC_PCER1	|=	 PMC_PCER1_PID36;		//Aktiverer periferal klokken til PWM (PID = 36)
						
	PWM->PWM_CH_NUM[1].PWM_CMR	  =	PWM_CMR_CPRE_MCK_DIV_32;
	PWM->PWM_CH_NUM[1].PWM_CPRD |=	PWM_CPRD_CPRD(PERIOD); //Setter til 50 Hz
	PWM->PWM_CH_NUM[1].PWM_CDTY |=  cdty_from_us(1500);
	PWM->PWM_ENA				|=  PWM_ENA_CHID1;
}

void pwm_set_duty_cycle_us(uint32_t us){
	uint32_t count = cdty_from_us(us);
	count = (count > PERIOD) ? PERIOD : count;
	count = (count < 0) ? 0 : count;
	PWM->PWM_CH_NUM[1].PWM_CDTYUPD = count;
}