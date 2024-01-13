#include "lpc23xx.h"
#include "CLOCK_CALNDR.h"
#include "rtc.h"
#include <time.h>
#include "menu_main.h"
#include "ticker_itrp.h"
#include "LCD.h"

typedef uint32_t time_t;

/******************************************************************************
 ** Function name:      set_time
 **
 ** Descriptions:       set_time() used for set the time(hh:mm) by keypad
 **
 ** parameters:         none
 **
 ** Returned value:     none
 ** 
 ******************************************************************************/
static void set_time(void)
{
	s8_t schr, scmin;

	lcd_show(0, 'G', 1);         // enter new value
	lcd_str(1, "  :   (24H)");   // 24 hour format
	glcd_x = 0;
	glcd_y = 1U;
	schr =(s8_t) enter_digits(2,0); //get value for hh.
	if (schr == -1)
	{
		lcd_show(0, 'G', 2);       // illegal value
		lcd_show(1, 'G', 0);       // not saved
		key1(MESSAGE_TIMEOUT);
		return;
	}
	lcd_char(':');
	scmin =(s8_t) enter_digits(2,0); //get value for mm.
	if (scmin == -1)
	{
		lcd_show(0, 'G', 2);      // Illegal value
		lcd_show(1, 'G', 0);      // not saved
		key1(MESSAGE_TIMEOUT);
		return;
	}
	if ((schr > 24) || (schr < 0) || (scmin > 59) || (scmin < 0))
	{
		lcd_show(1, 'G', 2);    // Illegal value
		key1(MESSAGE_TIMEOUT);
		return;
	}
	if ((schr == 24) && (scmin != 0))
	{
		lcd_show(1, 'G', 2); // Illegal value
		key1(MESSAGE_TIMEOUT);
		return;
	}
	if (schr == 24)
	{
		schr=0;
	}
	write_hr(schr);//RTC_HOUR = schr;
	write_min(scmin);//RTC_MIN = scmin;
	write_sec((u8_t)0);//RTC_SEC = 0;
}

/******************************************************************************
 ** Function name:      set_date
 **
 ** Descriptions:       this function used for set date,month and year
 **
 ** parameters:         none
 **
 ** Returned value:     none
 ** 
 ******************************************************************************/
static void set_date(void)
{
	s8_t scday, scmonth, scyear;
	u8_t ucmonths[13]=
	{ 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	lcd_show(0, 'G', 1);      // enter new value for date 
	lcd_show(1, 'd', 4);      // format-DMY
	glcd_x = 0;
	glcd_y = 1;

	scday =(s8_t) enter_digits(2,0); // enter new value for date 

	if (scday == -1)
	{
		lcd_show(0, 'G', 2);     // Illegal value
		lcd_show(1, 'G', 0);     // not saved
		key1(MESSAGE_TIMEOUT);
		return;
	}

	lcd_char('-');

	scmonth =(s8_t) enter_digits(2,0); // enter new value for month

	if (scmonth == -1)
	{
		lcd_show(0, 'G', 2);     // illegal value
		lcd_show(1, 'G', 0);     // not saved
		key1(MESSAGE_TIMEOUT);
		return;
	}

	if ((scday > 31) || (scday < 1) || (scmonth > 12) || (scmonth < 1))
	{
		lcd_show(1, 'G', 2);          // illegal value
		key1(MESSAGE_TIMEOUT);
		return;
	}

	lcd_char('-');

	scyear =(s8_t) enter_digits(2,0);  // enter new value for year

	if (scyear == -1)
	{
		lcd_show(0, 'G', 2);
		lcd_show(1, 'G', 0);
		key1(MESSAGE_TIMEOUT);
		return;
	}
	if ((scyear%4) == 0)   // identify leap year days of feb month
	{
		ucmonths[2]=29;    // and days of feb month
	}
	if (scday > ucmonths[scmonth])
	{
		lcd_show(1, 'G', 2);
		key1(MESSAGE_TIMEOUT);
		return;
	}
	write_dayofmonth(scday);
	write_month(scmonth);
	write_year(scyear+2000);
}

/******************************************************************************
 ** Function name:      date_time
 **
 ** Descriptions:       this function used to get the time and date with various formats.
 **
 ** parameters:         none
 **
 ** Returned value:     none
 ** 
 ******************************************************************************/
void date_time1(void)
{
	s32_t itemp_key;
	s32_t imenu_i = 1;
	char *p ="";
	char szstr[24];

	while (1)
	{
		p = NULL;

		lcd_show(0, 't', 0);
		p = szstr;

		if ((sys_t.date_display < 1) || (sys_t.date_display > 3))
		{
			sys_t.date_display = 1;
		}

		switch (imenu_i)
		{
			case 1:
				p = find_string('f', (sys_t.time_display == 1) ? 1 : 2);
				break;
			case 2:
				get_timestamp(szstr, 0);
				break;
			case 3:
				p = find_string('d',(s32_t) sys_t.date_display);
				break;
			case 4:
				get_timestamp(szstr, 1);
				break;
			default: 
				break;
		}

		lcd_show_index_str(1U, 't',(u8_t) imenu_i, p);

		if (get_menu_key(&itemp_key) == 0)
		{
			return;
		}

		if (select_menu(&imenu_i, itemp_key, 4) != 0)
		{
			switch (imenu_i)
			{
				case 1:
					set_option('f', &sys_t.time_display, 2);// set the option from 12h or 24h
					break;
				case 2:
					set_time();
					break;
				case 3:
					set_option('d', &sys_t.date_display, 3);// set the option from DMY, MDY, YMD
					break;
				case 4:
					set_date();
					break;
				default: 
					break;
			}
		}
	}
}

u16_t get_doy_dow(u8_t flag,char *str1)
{
	time_t  tick_value;
	struct tm *final_datetime;
	tick_value=get_ticks_value();
	final_datetime=localtime(&tick_value);
	if(flag==0x02 )
		return final_datetime->tm_wday;
	else if(flag==0x01)
		return ((final_datetime->tm_yday)+1);
	return 0;
}
u32_t get_sec_time_value(u32_t tick_value)
{
	u32_t secs;
	time_t convert_back_example2;
	struct tm *final_datetime;
	convert_back_example2=tick_value;
	final_datetime =localtime(&convert_back_example2);	  
	final_datetime->tm_mon+=1;
	final_datetime->tm_year+=1900;	
	secs= (final_datetime->tm_hour*3600)+(final_datetime->tm_min*60)+final_datetime->tm_sec;
	return secs;	
}

u32_t get_ticks_value(void)
{
	time_t end_time;
	struct tm end_str_time;
	/* Fill our starting structure*/
	end_str_time.tm_sec = read_sec();
	end_str_time.tm_min = read_min();
	end_str_time.tm_hour = read_hr();
	end_str_time.tm_mday = read_dayofmonth();
	end_str_time.tm_mon = (read_month())-1;
	end_str_time.tm_year = (read_year())-1900;
	end_str_time.tm_isdst = 0;
	end_time = mktime(&end_str_time);
	return end_time;
}
void set_date_time(u32_t diff_time)
{
	time_t convert_back_example2;
	struct tm *final_datetime;
	convert_back_example2=diff_time;
	final_datetime =localtime(&convert_back_example2);	  
	final_datetime->tm_mon+=1;
	final_datetime->tm_year+=1900;
	write_hr(final_datetime->tm_hour);
	write_min(final_datetime->tm_min);
	write_sec(final_datetime->tm_sec);
	write_dayofmonth(final_datetime->tm_mday);
	write_month(final_datetime->tm_mon);
	write_year(final_datetime->tm_year);
	return;
}

 void verify_date_time(u32_t diff_time)
 {
	 time_t convert_back_example2;
	 struct tm *final_datetime;
	 convert_back_example2=diff_time;
	 final_datetime =localtime(&convert_back_example2);    
	 final_datetime->tm_mon+=1;
	 final_datetime->tm_year+=1900;	
 }


