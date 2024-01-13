#ifndef _CLOCK_CALNDR_H_
#define _CLOCK_CALNDR_H_

#include "ticker_itrp.h"


void date_time1(void);
u16_t get_doy_dow(u8_t flag,char *str1);
u32_t get_sec_time_value(u32_t tick_value);
u32_t get_ticks_value(void);
void set_date_time(u32_t diff_time);
void verify_date_time(u32_t diff_time);

#endif

