#include "lpc23xx.h"
#include "rtc.h"
#include "menu_main.h"
#include "ticker_itrp.h"


void rtc_init(void)
{
	 RTC_CCR   = 0x11;                               /* Use 32kHz clock source for RTC */
}

u8_t read_sec(void)
{
	return(RTC_SEC);
}
u8_t read_min(void)
{
	return(RTC_MIN);
}
u8_t read_hr(void)
{
	return(RTC_HOUR);
}
u8_t read_dayofmonth(void)
{
	return(RTC_DOM);
}
u8_t read_month(void)
{
	return(RTC_MONTH);
}
u16_t read_year(void)
{
	return(RTC_YEAR);
}
u32_t read_time(void)
{
	return(RTC_CTIME0);
}
u32_t read_date(void)
{
	return(RTC_CTIME1);
}
void write_sec(u8_t sec)
{
	RTC_SEC = sec;
}
void write_min(u8_t min)
{
	RTC_MIN = min;
}
void write_hr(u8_t hr)
{
	RTC_HOUR = hr;
}
void write_dayofmonth(u8_t dayofmonth)
{
	RTC_DOM = dayofmonth;
}
void write_month(u8_t month)
{
	RTC_MONTH = month;
}
void write_year(u16_t year)
{
	RTC_YEAR = year;
}


/*-----------------------------------------end of file-------------------------------------------*/


