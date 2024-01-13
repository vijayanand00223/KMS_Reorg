/*
 * File Name:aka_sys.c
 * 		This file contains system oriented general functions like system setup,
 * 		set time, set date, and network settings. 
 * 		from the above fuction we can set initial settings like time, date, 
 * 		IP address, port number and etc..
 * 		Only Administrator and operator can set the above setting.
 *   
 * Version:V02.02.00 -->code restructure.
 * changes from old version: 1. code restructure and detail history 
 *                              included for files and functions
 * changes done by:   K.Sivakumar
 *      
 * Version:V02.00.00 -->code restructure. 
 * 
 * Copyright (c) 2004-2011 Sterna Security.
 * All rights reserved.
 * Author:sterna security.
 */

#include <string.h>
#include <stdio.h>
#include "lpc23xx.H"
#include "menu_main.h"
#include "LCD.h"
#include "BUZZER.h"
#include "CLOCK_CALNDR.h"



 
/******************************************************************************
 ** Function name:      set_option
 **
 ** Descriptions:       set any one option or value for input *popt
 **
 ** parameters:         cattribute=>used for get the string from list of array
 **						*popt=> location to store te string
 **						ucount=> limit the no.of option
 **
 ** Returned value:     if the getting value is 0, return '0'. else return '1'.
 ** 
 ******************************************************************************/
s32_t set_option(char cattribute, u8_t *popt, u8_t ucount)
{
	s32_t itemp_key;
	s32_t imenu_i = 1;
	
	while (1)
	{
		lcd_show(0, cattribute, 0);
		lcd_show_index(1U, cattribute,(u8_t) imenu_i);

		if (get_menu_key(&itemp_key) == 0)
		{
			return 0;
		}

		if (select_menu(&imenu_i, itemp_key,(s32_t) ucount) != 0)
		{
			read_system_sd();
			*popt = imenu_i;
			write_system_sd();
			return 1;
		}
	}
}
//Keys enable disable option
u8_t sets_enable_diable(char *p ,char cattribute, u8_t *popt, u8_t ucount)
{
	s32_t itemp_key;
	s32_t imenu_i=1;
	if(*popt==1 || *popt==2)
	{
		imenu_i=*popt;
	}
	while (1)
	{
		lcd_str(0,p);
		lcd_show_index(1U, cattribute,(u8_t) imenu_i);

		if (get_menu_key(&itemp_key) == 0)
		{
			return 0;
		}

		if (select_menu(&imenu_i, itemp_key,(s32_t) ucount) != 0)
		{
			read_system_sd();
			*popt = imenu_i;
			write_system_sd();
			return 1;
		}
	}
}


/******************************************************************************
 ** Function name:      operating_parameter
 **
 ** Descriptions:       this function is used for set the time limit for door open time,
 **						door close time and key turn time.
 **
 ** parameters:         none
 **
 ** Returned value:     none
 ** 
 ******************************************************************************/
void operating_parameter(void)
{
	s32_t itemp_key;
	s32_t imenu_i = 1;
	u8_t ucval, *p=0;

	while (1)
	{
		switch (imenu_i)
		{
			case 1:
				p = &sys_t.open_door_time;
				break;
			case 2:
				p = &sys_t.close_door_time;
				break;
			case 3:
				p = &sys_t.key_turn_time;
				break;
			default: 
				break;
		}

		lcd_show(0, 'o', 0);
		lcd_show_index_int(1U, 'o',(u8_t) imenu_i, *p);

		if (get_menu_key(&itemp_key) == 0)
		{
			return;
		}

		if (select_menu(&imenu_i, itemp_key, 3) != 0) // select parameter from 3 option 
		{
			lcd_show(0, 'G', 1);
			lcd_str(1, "");
			glcd_x = 0;
			glcd_y = 1;
			ucval =(u8_t) enter_digits(3,0); // enter digits for time limit
			if ((ucval < 5) || (ucval > 30))
			{
				lcd_show(0, 'G', 2); //Illegal value
				lcd_show(1, 'e', 7); //Enter 12 to 30 sec
				key1(MESSAGE_TIMEOUT);
			}
			else
			{
				*p = ucval;
				write_system_sd();
			}
		}
	}
}

/******************************************************************************
 ** Function name:      set_ip
 **
 ** Descriptions:       set the IP address for communication
 **
 ** parameters:         *ip=>used for a location to store IP address
 **
 ** Returned value:     if the entered digits are correct, return '1'. else return '0'.
 ** 
 ******************************************************************************/
s32_t set_ip(u32_t *ip)
{
	u32_t uip_tmp =(u8_t) 0;
	s32_t ib;
	u8_t uci;

	lcd_show(0, 'G', 1);
	
	lcd_str(1, "   .   .   .   ");
	glcd_x = 0;
	glcd_y = 1;

	for (uci = 0; uci < 4; uci++) //enter the 3 digits value for 4 times.
	{
		ib = enter_digits(3,0);

		if (ib == -1)
		{
			return 0;
		}

		if ((ib < 0) || (ib > 255))
		{
			uci--;
			lcd_delete(3);
			continue;
		}

		uip_tmp <<= 8;
		uip_tmp |= ib;

		lcd_forward(1);
	}

	*ip = uip_tmp;
	write_system_sd();
	
	return 1;
}
s32_t set_ip1(u32_t *ip)
{
	u32_t uip_tmp =(u8_t) 0;
	s32_t ib;
	u8_t uci;

	lcd_show(0, 'G', 1);
	
	lcd_str(1, "   .   .   .   ");
	glcd_x = 0;
	glcd_y = 1;

	for (uci = 0; uci < 4; uci++) //enter the 3 digits value for 4 times.
	{
		ib = enter_digits(3,0);

		if (ib == -1)
		{
			return 0;
		}

		if ((ib < 0) || (ib > 255))
		{
			uci--;
			lcd_delete(3);
			continue;
		}

		uip_tmp <<= 8;
		uip_tmp |= ib;

		lcd_forward(1);
	}

	*ip = uip_tmp;
	write_system_sd();
	write_comm_sd();
	
	return 1;
}
u8_t fw_upload_func(void)
{
		s32_t itemp_key;
	u32_t pass_code=0;
	lcd_show(0, 'V', 7);
	lcd_show(1, 'V', 8);
	if (get_menu_key(&itemp_key) == 0)
	{
		return 0;
	}

	lcd_show(0, 'I', 4);
	lcd_show(1, 'V', 13);
	pass_code=myRand(Firmware_Code,1);
	if (read_code(HIDE_INPUT)!= pass_code )
	{
		lcd_show(0, 'I', 8);
		lcd_show(1, 'S', 16U);		//Invalid service code
		delay(MESSAGE_TIMEOUT);
		return 0;
	}
	lcd_clr(0);
	lcd_show(0U,'e',13U);
	delay(1000);
	gcommunication_time=60;
	fw_flag=1;
	ghb_time=2;
	return 1;
}
/******************************************************************************
 ** Function name:      network1
 **
 ** Descriptions:       set the network parameters like IP address, port address, 
 **						netmask and gateway for communication.
 **
 ** parameters:         none
 **
 ** Returned value:     none
 ** 
 ******************************************************************************/
void network1(void)
{
	s32_t itemp_key;
	s32_t imenu_i = 1;
	u8_t ucount =(u8_t) 0;
	u8_t uclen=15U;
	char *p="";
	char szstr[32];
	u8_t urlcnt=0;
	u8_t urlcnt1=0;
	lcd_show(0, 'V', 7);
	lcd_show(1, 'V', 8);
	if (get_menu_key(&itemp_key) == 0)
	{
		return;
	}

	lcd_show(0, 's', 4);
	lcd_show(1, 'V', 13);
	if (read_code(SHOW_INPUT)!= 8051)
	{
		lcd_show(0, 's', 4);
		lcd_show(1, 'S', 16U);		//Invalid service code
		delay(MESSAGE_TIMEOUT);
		return;
	}
	read_comm_sd();
	read_system_sd();
	while (1)
	{
		p = NULL;
		lcd_show(0, 'n', 0);
		p = szstr;

		switch (imenu_i)
		{
			case 1:
				format_ip(szstr, sys_t.ip);
				break;
			case 2:
				format_ip(szstr, sys_t.netmask);
				break;
			case 3:
				format_ip(szstr, sys_t.gateway);
				break;
			case 4:

				read_comm_sd();
				sprintf(szstr,"%s",comm_t.url);

				break;
			case 5:

				read_comm_sd();
				sprintf(szstr,"%s",comm_t.url1);

				break;
			
			case 6:
				format_ip(szstr, comm_t.remote_ip1);
				break;
			case 7:
				format_ip(szstr, sys_t.dns_ip1);
				break;
			case 8:
				format_ip(szstr, sys_t.dns_ip2);
				break;
			case 14:
				p = find_string('w', (sys_t.ip_type == 1) ? 3 : 4);
				break;
			case 15:
				p = find_string('l', (sys_t.conn_type == 1) ? 3 : 4);
				break;
			default:
				break;
		}
		lcd_show_index_str(1U, 'n',(u8_t) imenu_i, p);
		if (get_menu_key(&itemp_key) == 0)
		{
			if ((sys_t.network == 1) && (ucount != 0))
			{
				lcd_show(0, 'V', 14);
				lcd_show(1, 'V', 15);
				buzzer(MESSAGE_TIMEOUT);
				key1(MESSAGE_TIMEOUT);
				system_restart();
			}
			return;
		}

		if (select_menu(&imenu_i, itemp_key,(s32_t) uclen) != 0)
		{
			switch (imenu_i)
			{
				case 1:
					if (set_ip(&sys_t.ip) != 0)
					{
						ucount =1U;
					}
					break;
				case 2:
					if (set_ip(&sys_t.netmask) != 0)
					{
						ucount =1U;
					}
					break;
				case 3:
					if (set_ip(&sys_t.gateway) != 0)
					{
						ucount =1U;
					}
					break;
				case 4:
							lcd_show(0, 'G', 1);
							lcd_show(1, 'V', 7);
						if(key1(5000)==ENTER)
						{
							lcd_show(1, 'V', 13);
							if(read_code(4)!=2580)
							{
								lcd_str(1,"Invalid Code!!!");
								buzzer(1000);
								lcd_clr(0);
								delay(10);
								break;
							}
							lcd_str(0,"Edit Host Server Name ");
							lcd_clr(1);
							memset(url,'\0',sizeof(url));
							urlcnt=read_string(url,33);
							if(urlcnt)
							{
								strcpy(comm_t.url,url);
								comm_t.url_cnt=urlcnt;
								write_comm_sd();
								delay(10);
								read_comm_sd();
								ucount=1U;
								urlcnt=0;
							}
							else
							{
								lcd_clr(0);
								lcd_str(1,"Invalid URL!!!");
								buzzer(1000);
							}
						}
				break;
				case 5:
							lcd_show(0, 'G', 1);
							lcd_show(1, 'V', 7);
						if(key1(5000)==ENTER)
						{
							lcd_show(1, 'V', 13);
							if(read_code(4)!=2580)
							{
								lcd_str(1,"Invalid Code!!!");
								buzzer(1000);
								lcd_clr(0);
								delay(10);
								break;
							}
							lcd_str(0,"Edit Host Server Name ");
							lcd_clr(1);
							memset(url1,'\0',sizeof(url1));
							urlcnt1=read_string(url1,33);
							if(urlcnt1)
							{
								strcpy(comm_t.url1,url1);
								comm_t.url_cnt1=urlcnt1;
								write_comm_sd();
								delay(10);
								read_comm_sd();
								ucount=1U;
								urlcnt1=0;
							}
							else
							{
								lcd_clr(0);
								lcd_str(1,"Invalid URL!!!");
								buzzer(1000);
							}
						}
				break;						
				case 6:read_comm_sd();
					if (set_ip1(&comm_t.remote_ip1 ) != 0)
					{
						ucount =1U;
					}
					break;
					case 7:
					if (set_ip(&sys_t.dns_ip1 ) != 0)
					{
						ucount =1U;
					}
					break;
					case 8:
					if (set_ip(&sys_t.dns_ip2 ) != 0)
					{
						ucount =1U;
					}
					break;
				case 9:
					view_mac_address();
					break;
				case 10:
					if (set_port() != 0)
					{
						ucount =1U;
					}
					break;
			case 11:
					if (set_port_hb() != 0)
					{
						ucount =1U;
					}
					break;
			case 12:
					if (set_port_tcp() != 0)
					{
						ucount =1U;
					}
					break;
			case 13:
					if (set_port_cliq() != 0)
					{
						ucount =1U;
					}
					break;					
			case 14:
				lcd_str(0," IP Type Setting    ");
				if(set_option('w', &sys_t.ip_type , 2)!=0)
				{
					ucount=1U;
				}
				break;
				case 15:
				lcd_str(0," Conn Type Setting    ");
				if(set_option('l', &sys_t.conn_type  , 2)!=0)
				{
					ucount=1U;
				}
				break;
				default:
					break;
			}
		}
	}
}
/******************************************************************************
 ** Function name:      system_setup
 **
 ** Descriptions:       this function has the function calls for set date& time, set network,
 **						operating_parameters, alarm settings, language settings and card settings
 **
 ** parameters:         none
 **
 ** Returned value:     none
 ** 
 ******************************************************************************/
void system_setup(void)
{
	s32_t itemp_key;
	s32_t imenu_i = 1;
	char *p="";
	while(1)
	{
		p = NULL;

		switch (imenu_i)
		{
			case 3:
				p = find_string('a', (sys_t.alarm_enable       == 1) ? 1 : 2);
				break;
			default: 
				break;
		}
		lcd_show(0, 's', 0);
		lcd_show_index_str(1U, 's',(u8_t) imenu_i, p);
		if (get_menu_key(&itemp_key) == 0)
		{
			return;
		}
		if (select_menu(&imenu_i, itemp_key, 4) != 0)
		{
			switch (imenu_i)
			{
				case 1:
					date_time1();
					endtick_value=get_ticks_value();
					break;
				case 2:
					operating_parameter();
					break;
				case 3:
					set_option('a', &sys_t.alarm_enable, 2);
					break;
				case 4:
					network1();
					break;
				default: 
					break;

			}
		}
	}
}

/******************************************************************************
 ** Function name:      set_port
 **
 ** Descriptions:       set system telnet port
 **  
 ** parameters:         none
 **
 ** Returned value:     if the value is valid return '1'. else return '0'.
 ** 
 ******************************************************************************/
s32_t  set_port(void)
{
	s32_t itemp_key;
	u16_t *p=0;
	u32_t uival;
	char szstr[6];
	while(1)
	{
		p=&sys_t.telnet_port;		
		sprintf(szstr,"%d",sys_t.telnet_port);
		lcd_show(0,'n',10);		
		lcd_str(1,szstr);
		if (get_menu_key(&itemp_key) == 0)
		{
			return 0;
		}
		
		lcd_show(0, 'G', 1);
		lcd_str(1, "");
		glcd_x = 0;
		glcd_y = 1;
		uival = enter_digits(5,0);		// enter the values for port

		if ((uival < 23) || (uival > 65000)) //it should in range 23-65000
		{
			lcd_show(1, 'G', 2);     //illegal value
			key1(MESSAGE_TIMEOUT);
			return 0;
		}
		else
		{
			*p = uival;
			write_system_sd();
			return 1;
		}
	}
}
/******************************************************************************
 ** Function name:      set_port_tcp
 **
 ** Descriptions:       set system tcp server port
 **  
 ** parameters:         none
 **
 ** Returned value:     if the value is valid return '1'. else return '0'.
 ** 
 ******************************************************************************/
s32_t  set_port_hb(void)
{
	s32_t itemp_key;
	u16_t *p=0;
	u32_t uival;
	char szstr[6];
	read_comm_sd();
	while(1)
	{
		p=&comm_t.hb_port;		
		sprintf(szstr,"%d",comm_t.hb_port);
		lcd_show(0,'n',11);		
		lcd_str(1,szstr);
		if (get_menu_key(&itemp_key) == 0)
		{
			return 0;
		}
		
		lcd_show(0, 'G', 1);
		lcd_str(1, "");
		glcd_x = 0;
		glcd_y = 1;
		uival = enter_digits(5,0);		// enter the values for port

		if ((uival < 23) || (uival > 65000)) //it should in range 23-65000
		{
			lcd_show(1, 'G', 2);     //illegal value
			key1(MESSAGE_TIMEOUT);
			return 0;
		}
		else
		{
			*p = uival;
			write_comm_sd();
			return 1;
		}
	}
}

/******************************************************************************
 ** Function name:      set_port_tcp
 **
 ** Descriptions:       set system tcp server port
 **  
 ** parameters:         none
 **
 ** Returned value:     if the value is valid return '1'. else return '0'.
 ** 
 ******************************************************************************/
s32_t  set_port_tcp(void)
{
	s32_t itemp_key;
	u16_t *p=0;
	u32_t uival;
	char szstr[6];
	read_comm_sd();
	while(1)
	{
		p=&comm_t.event_port;		
		sprintf(szstr,"%d",comm_t.event_port);
		lcd_show(0,'n',12);		
		lcd_str(1,szstr);
		if (get_menu_key(&itemp_key) == 0)
		{
			return 0;
		}
		
		lcd_show(0, 'G', 1);
		lcd_str(1, "");
		glcd_x = 0;
		glcd_y = 1;
		uival = enter_digits(5,0);		// enter the values for port

		if ((uival < 23) || (uival > 65000)) //it should in range 23-65000
		{
			lcd_show(1, 'G', 2);     //illegal value
			key1(MESSAGE_TIMEOUT);
			return 0;
		}
		else
		{
			*p = uival;
			write_comm_sd();
			return 1;
		}
	}
}

/******************************************************************************
 ** Function name:      set_port_cliq
 **
 ** Descriptions:       set system cliq server port
 **  
 ** parameters:         none
 **
 ** Returned value:     if the value is valid return '1'. else return '0'.
 ** 
 ******************************************************************************/
s32_t  set_port_cliq(void)
{
	s32_t itemp_key;
	u16_t *p=0;
	u32_t uival;
	char szstr[6];
	while(1)
	{
		p=&comm_t.firmware_port ;		
		sprintf(szstr,"%d",comm_t.firmware_port );
		lcd_show(0,'n',13);		
		lcd_str(1,szstr);
		if (get_menu_key(&itemp_key) == 0)
		{
			return 0;
		}
		
		lcd_show(0, 'G', 1);
		lcd_str(1, "");
		glcd_x = 0;
		glcd_y = 1;
		uival = enter_digits(5,0);		// enter the values for port

		if ((uival < 23) || (uival > 65000)) //it should in range 23-65000
		{
			lcd_show(1, 'G', 2);     //illegal value
			key1(MESSAGE_TIMEOUT);
			return 0;
		}
		else
		{
			*p = uival;
			write_comm_sd();
			return 1;
		}
	}
}

