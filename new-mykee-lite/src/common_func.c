/*
 * File Name:common_func.c
 * 		This file contains common functions calling from different file.
 * 		so these functions are general functions. 
 *   
 * Version:V02.02.00 -->code restructure.
 * changes from old version: 1. code restructure and detail history 
 *                              included for files and functions
 * changes done by:   K.Sivakumar
 * 
 * Version:V02.01.00 -->GSM features added. 
 * changes from old version: 1. Modification in function delete_location(void), 
 * 								to save the pending task event in the SD card for GSM features.                      
 * changes done by:   B.Sedhu sundaram
 *        
 * Version:V02.00.00 -->code restructure. 
 * 
 * Copyright (c) 2004-2011 Sterna Security.
 * All rights reserved.
 * Auther: R.Narayanan.
 */

#include "lpc23xx.h"
#include "menu_main.h"
#include "LCD.h"


/******************************************************************************
 ** Function name:      compare_date
 **
 ** Descriptions:       it compares the two dates, months & year, and returns the coresponding value.
 **						for the right task time.
 ** 
 ** parameters:         uifirst_date=> first date to compare
 **						uisec_date=> second date to compare
 **
 ** Returned value:     if second date>first date return '1'
 **						else second date<first date return '0'
 ** 
 ******************************************************************************/
s32_t compare_date(u32_t uifirst_date,u32_t uisec_date)
{
	u8_t ucday[2], ucmonth[2], ucyear[2];
	ucday[0]= (u8_t) uifirst_date& 0xFF;
	ucday[1]= (u8_t)uisec_date& (u8_t)0xFF;
	ucmonth[0]=(uifirst_date>>8) & 0xFF;
	ucmonth[1]=(uisec_date>>8) & 0xFF;
	ucyear[0] =(uifirst_date>>16) & 0xFF; 
	ucyear[1] =(uisec_date>>16) & 0xFF;

	if(ucyear[0] < ucyear[1])
	{
		return 1;
	}
	else if(ucyear[0] == ucyear[1])
	{
		if(ucmonth[0] < ucmonth[1])
		{
			return 1;
		}
		else if(ucmonth[0] == ucmonth[1])
		{
			if(ucday[0]< ucday[1])
			{
				return 1;
			}
			else if(ucday[0]== ucday[1])
			{
				return 2;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

/******************************************************************************
 ** Function name:      dow1
 **
 ** Descriptions:       it helps to find the day of the week.
 **						it may be sunday, monday... to.. saturday.
 ** 
 ** parameters:         iday, imonth, iyear=> to find the correct day of that date
 **
 ** Returned value:     return number '0' to '6' to get day sunday to saturday
 ** 
 ******************************************************************************/
s32_t dow1(s32_t iday, s32_t imonth, s32_t iyear)
{
	s32_t iresult;

    if (iyear < 100)
    {
        iyear += 2000;
    }

    if (imonth < 3) {
        imonth += 12;
        --iyear;
    }

    iresult = (iday) + (((13 * imonth) - 27)/5) + (iyear) + (iyear/4) - (iyear/100) + (iyear/400);

    iresult += 6;

    return (iresult % 7);
}

/******************************************************************************
 ** Function name:      get_timestamp
 **
 ** Descriptions:       used to get the time and date from the RTC, 
 **						and display with selected format. 
 ** 
 ** parameters:         *str  => location to get the date and time
 ** 					ucformat=> indicate time and date format(DMY,MDY,YMD)
 **
 ** Returned value:     none
 ** 
 ******************************************************************************/
void get_timestamp(char *str, u8_t ucformat)
{
    u32_t uih, uimin, uis, uid, uim, uiy;
    char  *p ="";
    char  szstr1[24], szstr2[24];
	static u32_t time=0U, date=0U;
  
	  
	while ((time !=RTC_CTIME0) || (date !=RTC_CTIME1))
    {
        time =RTC_CTIME0;
        date =RTC_CTIME1;
    }

    uis   = (time)       & 0x003F;
    uimin = (time >> 8)  & 0x003F;
    uih   = (time >> 16) & 0x001F;
    uid   = (date)       & 0x001F;
    uim   = (date >> 8)  & 0x000F;
    uiy   = (date >> 16) & 0x0FFF;
    uiy   = uiy % 100U;

    gcurrent_dow  = dow1((s32_t)uid, (s32_t)uim, (s32_t)uiy);
    gcurrent_time = (((s32_t) uih * 60) +(s32_t) uimin);
		gcurrent_date = uid | (uim<<8)|(uiy<<16);	

		
    if (sys_t.time_display == 1U)
    {
        if (uih < 12U)                                 // Determine AM/PM
        {
            p = "AM";
        }
        else
        {
            p = "PM";
        }

        if (uih > 12U)                                 // Adjust to 12hour clock, there is no 00:00
        {
            uih -= 12U;
        }
        else if (uih ==(u32_t) 0)
        {
            uih = 12U;
        }
        else
        {
      
        }

        sprintf(szstr1, "%02d:%02d %s", uih, uimin, p);
    }
    else
    {
        sprintf(szstr1, "%02d:%02d",uih, uimin);
    }

    p = find_string('x', dow1((s32_t)uid, (s32_t)uim, (s32_t)uiy));

    switch (sys_t.date_display)
    {
	    case 2U: 
	    	sprintf(szstr2, "%02d-%02d-%02d", uim, uid, uiy); 		
	    	break;
	    case 3U: 
	    	sprintf(szstr2, "20%02d-%02d-%02d", uiy, uim, uid);	
	    	break;
	    default: 
	    	sprintf(szstr2, "%02d-%02d-%02d", uid, uim, uiy); 		
	    	break;
    }

    switch (ucformat)
    {
	    case 0U: 
	    	sprintf(str, "%s", szstr1);                	
	    	break;			// Time only
	    case 1U: 
	    	sprintf(str, "%s", szstr2);               		
	    	break;			// Date only
	    case 2U: 
	    	sprintf(str, "%s", p);                   		
	    	break;			// DOW only
	    case 3U: 
	    	sprintf(str, "%s %s", p, szstr2);          	
	    	break;			// DOW and date 
	    case 4:
	    default:
				if(sys_t.time_display==2)
					sprintf(str, "%s %s %s", p, szstr2, szstr1); 	
				else
	    	sprintf(str, "%s %s",  szstr2, szstr1); 	
	    	break;			// DOW date time
    }
}

/******************************************************************************
 ** Function name:      get_menu_key
 **
 ** Descriptions:       this function used to show the diff types of options in the menu.
 **						we can select the option using the keypad
 ** 
 ** parameters:         *itemp_key=> location to get the value from keypad.
 **
 ** Returned value:     if the pressed value is 0 or ESC return '0'. else return '1'.
 ** 
 ******************************************************************************/
s32_t get_menu_key(s32_t *itemp_key)
{
    *itemp_key = key1((s32_t) KEY_TIMEOUT);

    if ((*itemp_key == 0) || (*itemp_key == ESC))
    {
        return 0;
    }

    return 1;
}

/******************************************************************************
 ** Function name:      get_menu_key
 **
 ** Descriptions:       this function used to show the diff types of options in the menu.
 **						we can select the option using the keypad
 ** 
 ** parameters:         *itemp_key=> location to get the value from keypad.
 **
 ** Returned value:     if the pressed value is 0 or ESC return '0'. else return '1'.
 ** 
 ******************************************************************************/
s32_t get_menu_key1(s32_t *itemp_key)
{
    *itemp_key = key1((s32_t) MESSAGE_TIMEOUT);

    if ((*itemp_key == 0))
    {
        return 0;
    }
	if(*itemp_key == ESC)
		return 0xff;
    return 1;
}

void date_time_display(char *szstr)
{
    u32_t uih, uimin, uis, uid, uim, uiy;
		uis   = RTC_SEC;
    uimin = RTC_MIN;
    uih   = RTC_HOUR;
    uid   = RTC_DOM;
    uim   = RTC_MONTH;
    uiy   = RTC_YEAR;
		
		uis&=0xFF;
		uimin&=0xFF;
		uih&=0xFF;
		uid&=0xFF;
		uim&=0xFF;
		uiy&=0xFF;
    sprintf(szstr, "%02d:%02d %02d-%02d-%02d", uih, uimin, uid, uim, uiy);
		gcurrent_time =uis|(uimin<<8)|(uih <<16);
		gcurrent_date = uid | (uim<<8)|(uiy<<16);	

}
/*-----------------------------------------end of file-------------------------------------------*/
