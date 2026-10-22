
#include <sam.h>
#include <stdint.h>
#include "../../include/drivere/encoder.h"

void encoder_init()
{
	PMC->PMC_PCER0 = (1u << ID_PIOC);
	PMC->PMC_PCER1 = (1u << (ID_TC6 - 32))
	| (1u << (ID_TC7 - 32));

	uint32_t pins = (1u << 25) | (1u << 26);

	PIOC->PIO_ABSR |= pins;
	PIOC->PIO_PDR = pins;

	TC2->TC_CHANNEL[0].TC_CMR =
	TC_CMR_TCCLKS_XC0 |
	TC_CMR_ABETRG |
	TC_CMR_ETRGEDG_RISING;

	TC2->TC_CHANNEL[1].TC_CMR =
	TC_CMR_TCCLKS_XC0 |
	TC_CMR_ABETRG |
	TC_CMR_ETRGEDG_RISING;

	TC2->TC_BMR =
	TC_BMR_QDEN  |
	TC_BMR_POSEN |
	TC_BMR_EDGPHA;

	TC2->TC_CHANNEL[1].TC_CCR =
	TC_CCR_CLKEN | TC_CCR_SWTRG;

	TC2->TC_CHANNEL[0].TC_CCR =
	TC_CCR_CLKEN | TC_CCR_SWTRG;
}

int32_t encoder_get_position()
{
	return (int32_t)TC2->TC_CHANNEL[0].TC_CV;
}
