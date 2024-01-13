#ifndef _RTC_H_
#define _RTC_H_

#include "lpc23xx.h"
#include "ticker_itrp.h"

void rtc_init(void);
u8_t read_sec(void);
u8_t read_min(void);
u8_t read_hr(void);
u8_t read_dayofmonth(void);
u8_t read_month(void);
u16_t read_year(void);
u32_t read_time(void);
u32_t read_date(void);
void write_sec(u8_t sec);
void write_min(u8_t min);
void write_hr(u8_t hr);
void write_dayofmonth(u8_t dayofmonth);
void write_month(u8_t month);
void write_year(u16_t year);


#endif



