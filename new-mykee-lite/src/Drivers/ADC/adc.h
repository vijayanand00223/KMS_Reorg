#ifndef _ADC_H_
#define _ADC_H_

#include "ticker_itrp.h"

void adc_init(void);
u16_t read_ac_adc_ch(void);
u16_t read_dc_adc_ch(void);


#endif

