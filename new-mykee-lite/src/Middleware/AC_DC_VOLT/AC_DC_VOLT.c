#include "lpc23xx.h"
#include "AC_DC_VOLT.h"
#include "ticker_itrp.h"

f32_t ac_voltage(u16_t adc_count)
{
	f32_t ac_volt;
	ac_volt = (f32_t) adc_count/31.21;
	return (ac_volt);
}

f32_t dc_voltage(u16_t adc_count)
{
	f32_t dc_volt;
	dc_volt = (f32_t)adc_count/38.61;
	return (dc_volt);	
}

