/*
 * File name: msg_menu.c
 * 	 	This file contains the LCD related functions and 
 * 		some general fuctions like enter digits, find string, read code
 * 		and read string.
 * 
 * Version:V02.02.00 -->code restructure.
 * changes from old version: 1. code restructure and detail history 
 *                              included for files and functions
 * changes done by:   K.Sivakumar
 * 
 * Version:V02.01.00 -->GSM features added. 
 * changes from old version: 1. Added a function find_sms(), 
 * 								is used to select the string for sms description
 * 								from the given array list based upon 
 * 								the occured event.
 * changes done by:  B.Sedhu sundaram  
 *
 * Version V 02.00.00 -- code restructure
 * 
 * Copyright (c) 2004-2011 Sterna Security.
 * All rights reserved.
 * Author:sterna security.
 */ 
#include "uart.h"
#include "LCD.h"
#include "menu_main.h"














 











/******************************************************************************
 ** Function name:      select_menu
 **
 ** Descriptions:       select the option from the given menu 
 **
 ** parameters:         *menu_i, temp_key, limit
 ** 				 
 ** Returned value:     return '1' or '0'.
 ** 
 ******************************************************************************/
s32_t select_menu(s32_t *menu_i, s32_t temp_key, s32_t limit)
{
    if (temp_key == ENTER)
    {
        return 1;
    }
    else if(temp_key == ARROW_D)
    {
        if (*menu_i < limit)
        {
            (*menu_i)++;
        }
    }
    else if (temp_key == ARROW_U)
    {
        if(*menu_i > 1)
        {
            (*menu_i)--;
        }
    }
    else if ((temp_key >= '1') && (temp_key <= ('0' + limit)))
    {
        *menu_i = temp_key - '0';
        return 1;
    }
    else
	{

	}
    
    return 0;
}

/******************************************************************************
 ** Function name:      read_code
 **
 ** Descriptions:       this function used to get the user code and pin value 
 **						for admin,operator, installer & enduser
 **
 ** parameters:         hide_input=>used to determine whether the enter input 
 **						value to be display or not.And also used to select the 
 **						no.of digits of code
 ** 				 
 ** Returned value:     if the entered code format is correct return that value. else return '-1'
 ** 
 ******************************************************************************/
u32_t read_code(s32_t hide_input)
{
	
		char szstr[32];
    CODE code;
    volatile s32_t i=0;
    char ctmp;
		u8_t ucount;
    code.num = 0U;
		sprintf(szstr,"hide_input=%d\r",hide_input);

	for(i=0;i<10;i++)
		code.array[i] = '\0';
	if(hide_input ==0xFF)				// FOR ADMIN PIN NUMBER
	{
		ucount=6U;
	}
	else if(hide_input==0xFD)
	{
		ucount=4U;
	}
	else if(hide_input ==0xEE)
	{
		ucount=2U;
	}
	else
	{
		ucount=4U;
	}
    for (i = 0; i <(s32_t) ucount; i++)
    {
		if(hide_input==2 || hide_input==0xFD)
		{
			if(ULALARMS)
			{
				ctmp=(char) key1((u32_t) MESSAGE_TIMEOUT);
			}
			else
			{
				ctmp=(char) key1(300U);
			}
		
			if(ctmp)
			{

				if((i==0) &&(ctmp >= '0') && (ctmp <= '9'))
				{								
					lcd_init();
					lcd_show(0U, 'S',0U);
				}
			
				hide_input=0x00;
			}
		}
		
		
		else if(hide_input==0xEE)
		{
			ctmp=(char) key1(3000U);
		}
		else
		{
			ctmp =(char) key1(10000U);
		}		
				
        if ((ctmp >= '0') && (ctmp <= '9'))
        {
            code.array[i] = ctmp;
			
            if(hide_input==0xEE)
            {
            	lcd_char(ctmp);
            }
						else if (hide_input)
            {
                lcd_char('*');
            }
            else
            {
            	lcd_char(ctmp);
            }
                
        }
        else if (ctmp == ARROW_L)
        {
            if (i > 0)
            {
	            code.array[i-1] = '\0';
                lcd_delete(1);
                i -= 2;
            }
            else
            {
                return -1;
            }
        }
				else if(ctmp==ARROW_U)				//for communication updation using TCP
				{
					return -2;
				}
				else if(ctmp==ARROW_D)				//for communication updation for FW
				{
					return -3;
				}
        else if (ctmp == ENTER)                     // ENTER expands short input with zeroes
        {                                           // Example: 4 becomes 0004
            if (i > 0)
            {

									return (u32_t) atoi(code.array);
              
            }
            else
            {
                return -1;
            }
        }
				else if (ctmp == ARROW_R)                      
        {
						arrow_r_flg=1;	
					 --i;
					 lcd_init();
					lcd_str(0U,"Enter MasterCode 1:");
				}
				else if(ctmp==ESC)
				{
					return -4;
				}
        else
        {
				
            return -1;
        }
    }
		sprintf(szstr,"kpd=%d\n ",atoi(code.array));
		pstr(szstr);
	return (u32_t) atoi(code.array);
}
// the following array is used in read string function. 
// to get the following alphabetical and numerical value.
const char alphanum_array[10][5] =
{
    { '0', '.', ',', '-', '+' },
    { '1', '(', ')', '*', '#' },
    { 'A', 'B', 'C', '2', '@' },
    { 'D', 'E', 'F', '3', '_' },
    { 'G', 'H', 'I', '4',     },
    { 'J', 'K', 'L', '5',     },
    { 'M', 'N', 'O', '6',     },
    { 'P', 'Q', 'R', 'S', '7' },
    { 'T', 'U', 'V', '8',     },
    { 'W', 'X', 'Y', 'Z', '9' }
};

/******************************************************************************
 ** Function name:      read_string
 **
 ** Descriptions:       Read string is used to get the alpha-numeric value 
 **						from the keypad. And this function used to read and 
 **						display the existing string with edit mode. 
 **						In this edit mode, we can change the current string.
 **
 ** parameters:         *in_str=>location to get the entered value.
 **                     max_len=> limit value of no.of charectors to be enter  
 ** 				 
 ** Returned value:     if the entered code format is correct return that value.
 ** 
 ******************************************************************************/
u32_t read_string(char *in_str, s32_t max_len)
{
		s32_t  i, ij,len;
    u8_t  ucount;
    char ctmp, cprev_key, chr;
    s32_t  iupper_case = 1;
    char szstr[32];
    s32_t itest;
		s32_t k;

   ucount = 0U;
    cprev_key = 0;

   	for (len = 0; len < max_len; len++) 
    {
        if (!in_str[len])
        {
            break;
        }
        lcd_char(in_str[len]);
        szstr[len] = in_str[len];
        szstr[len + 1] = '\0';
    }

    for (i = len; i < max_len; i++)
    {
        for (ij = 0; ij < (KEY_TIMEOUT / 1000); ij++)
        {
            ctmp =(char) key1(1000U);

            if (!ctmp)
            {
                cprev_key = 0;
            }
            else
            {
            	itest=i-24;
				if(itest>=1)
				{
					glcd_x=0;
					for(k=itest;k<i;k++)
					{
						lcd_char(szstr[k]);
					}
					glcd_x=24;
				}
                break;
            }
        }

        if ((ctmp >= '0') && (ctmp <= '9'))
        {
            if (cprev_key == ctmp)
            {
                lcd_back(1);
                i--;

                if (ucount < 4U)
                {
                    ucount++;
                }
                else
                {
                    ucount = 0U;
                }
            }
            else
            {
                ucount = 0U;
            }

            chr = alphanum_array[ctmp - '0'][ucount];

            if ((!iupper_case) && (chr >= 'A') && (chr <= 'Z'))
            {
                chr += 0x20;
            }

            if (!chr)
            {
                ucount = 0U;
                chr = alphanum_array[ctmp - '0'][ucount];
            }

            szstr[i] = chr;
            lcd_char(chr);
        }
        else if (ctmp == ARROW_L)                    // Delete
        {
            if (i > 0)
            {
                lcd_delete(1);
                i -= 2;
            }
            else
            {
                return 0U;
            }
        }
        else if (ctmp == ARROW_R)                    // Space
        {
            chr = ' ';
            lcd_char(chr);
            szstr[i] = chr;
        }
        else if (ctmp == ARROW_U)                    // Uppercase
        {
            iupper_case = 1;
            i--;
        }
        else if (ctmp == ARROW_D)                    // Lowercase
        {
            iupper_case = 0;
            i--;
        }
        else if (ctmp == ENTER)                      // ENTER ends input
        {
            for (ij = 0; ij < i; ij++)
            {
                in_str[ij] = szstr[ij];
            }

            in_str[i] = '\0';
            return (u32_t) i;
        }
        else
        {
            return 0U;
        }
        cprev_key = ctmp;
    }
	
    return (u32_t) i;
}
u32_t read_hexavalue(char*in_str,int max_len)
{
    int  i, j;
    int  count, len;
    char tmp, prev_key, chr;
    char str[32];
		glcd_y=1;
		glcd_x=0;
    count = 0;
    prev_key = 0;

    for (len = 0; len < max_len; len++)
    {
        if (!in_str[len])
            break;
        lcd_char(in_str[len]);
        str[len] = in_str[len];
        str[len + 1] = '\0';
    }

    for (i = len; i <= max_len; i++)
    {
        for (j = 0; j < (KEY_TIMEOUT / 1000); j++)
        {
            tmp = key1(1000);

            if (!tmp)
                prev_key = 0;
            else
                break;
        }
		
        if (tmp >= '0' && tmp <= '9')
        {
        	if(tmp=='2'||tmp=='3')
        	{
	            if (prev_key == tmp)
	            {
	                lcd_back(1);
	                i--;

	                if (count < 4)
	                    count++;
	                else
	                    count = 0;
	            }
	            else
	                count = 0;
        	}
			else 
			{
				if(tmp=='0'||tmp=='1')
					count=0;
				else if((tmp>='4'&&tmp<='6')||tmp=='8')
					count=3;
				else if(tmp=='7'||tmp=='9')
					count=4;
			}

            chr = alphanum_array[tmp - '0'][count];

            if (!chr)
            {
                count = 0;
                chr = alphanum_array[tmp - '0'][count];
            }
			if(i<max_len)
			{
          		str[i] = chr;
            	lcd_char(chr);
			}
        }
        else if (tmp == ARROW_L)                    // Delete
        {
            if (i > 0)
            {
                lcd_delete(1);
                i -= 2;
            }
            else
                return 0;
        }
        else if (tmp == ENTER)                      // ENTER ends input
        {
            for (j = 0; j < i; j++)
                in_str[j] = str[j];

            in_str[i] = '\0';
            return i;
        }
        else
            return 0;

        prev_key = tmp;
    }
	for (j = 0; j < i; j++)
		in_str[j] = str[j];
  in_str[i] = '\0';	

	return i;
}
/******************************************************************************
 ** Function name:      enter_digits
 **
 ** Descriptions:       this function is used to enter integer value for 
 **						time, date, IP address & serial number.				
 **	
 ** parameters:         len=> limit value of no.of charectors to be enter  
 ** 				 
 ** Returned value:     if the entered code format is correct return that value. else return '-1'.
 ** 
 ******************************************************************************/
s32_t enter_digits(s32_t len,u8_t a)//a-choose function
{
    char str[32]="";
    volatile s32_t i=0;
    s32_t  ij,icnt=0;
    char ctmp;

    if (len > 20)
    {
        len = 20;
    }
	while(1)
    {
    	icnt=0;
    	while(1)
    	{
	    	lcd_char('_');
			glcd_x--;
			ctmp=(char) key1((u32_t) ONE_SEC);	
			icnt++;
			if(ctmp)
			{
				break;
			}
			if(icnt>30)
			{
				return -1;
			}
			
			lcd_char(' ');
			glcd_x--;
			ctmp=(char) key1((u32_t)ONE_SEC);
			icnt++;
			if(ctmp)
			{
				break;
			}
			if(icnt>30)
			{
				return -1;
			}
    	}

		if(ctmp == 'F')
		{
			return -1;
		}

if(a==0)
{
        if ((ctmp >= '0') && (ctmp <= '9')&& i<len)
        {
        	
							 str[i] = ctmp;
							str[i + 1] = '\0';
							lcd_char(ctmp);
						
        }
        else if (ctmp == ARROW_L)
        {
            if (i > 0)
            {
            	lcd_char(' ');
				glcd_x--;
                lcd_delete(1);
                i -= 2;
            }
            else
            {
                i -= 1;
            }
        }
        else if (ctmp == ESC)
        {
            return -1;
        }
        else if (ctmp == ENTER)                      // ENTER ends input
        {
            lcd_delete((s8_t) i);

            for (ij = 0; ij < len - i; ij++)
            {
                lcd_char(' ');
            }

            for(ij = 0; ij < i; ij++)
            {
                lcd_char(str[ij]);
            }
            return atoi(str);
        }
        else
        {
            i -= 1;
        }
			}
else
{
	   if ((ctmp > '0') && (ctmp < '9')&& i<len)
        {
        	
							 str[i] = ctmp;
							str[i + 1] = '\0';
							lcd_char(ctmp);
						
        }
        else if (ctmp == ARROW_L)
        {
					if (i > 0)
					{
						lcd_char(' ');
						glcd_x--;
						lcd_delete(1);
						i -= 2;
					}
					else
					{
						i -= 1;
					}
        }
        else if (ctmp == ESC)
        {
          return -1;
        }
        else if (ctmp == ENTER)                      // ENTER ends input
        {
					lcd_delete((s8_t) i);

					for (ij = 0; ij < len - i; ij++)
					{
							lcd_char(' ');
					}

					for(ij = 0; ij < i; ij++)
					{
							lcd_char(str[ij]);
					}
					return atoi(str);
        }
        else
        {
          i -= 1;
        }
}
		i++;
		if((i<=3)&&(i==len))
		{
			return atoi(str);			
		}
    }
	return -1;
//    return atoi(str);
}
//this function helps to allign four sets of integers value
//into the IP format.
void format_ip(char *str, u32_t ip1)
{
    sprintf(str, "%u.%u.%u.%u", (ip1 >> 24) & 0xff, (ip1 >> 16) & 0xff, (ip1 >>  8) & 0xff,(ip1) & 0xff);
	
}
/*-----------------------------------------end of file-------------------------------------------*/
