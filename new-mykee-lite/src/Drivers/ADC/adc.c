#include "lpc23xx.h"
#include "adc.h"
#include "ticker_itrp.h"

void adc_init(void)
{
	PCONP    |= 1 << 12;                          /* Enable clock to the AD block */
    AD0CR     = 0x00210C07;                       /* Enable AD0.0-2 in burst mode */
}

u16_t read_ac_adc_ch(void)
{
	volatile register u16_t ac_adc_read;
	u16_t ac_adc_vaule;
	
	ac_adc_read = AD0DR1;
	ac_adc_vaule = ac_adc_read;
	ac_adc_vaule =((ac_adc_vaule >> 6U) &(u16_t) 0x3FF);
	return(ac_adc_vaule);
}

u16_t read_dc_adc_ch(void)
{
	volatile register u16_t dc_adc_read;
	u16_t dc_adc_vaule;
	
	dc_adc_read = AD0DR0;
	dc_adc_vaule = dc_adc_read;
	dc_adc_vaule =((dc_adc_vaule >> 6U) &(u16_t) 0x3FF);
	return(dc_adc_vaule);
}


