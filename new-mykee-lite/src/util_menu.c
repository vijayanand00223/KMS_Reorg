/*
 * File name:util_menu.c
 *      This file contains the utility menu functions for Administrator 
 *      and installer, to check the keypad, key control, card reader, 
 * 		modem and output.
 * 
 * Version:V02.02.00 -->code restructure.
 * changes from old version: 1. code restructure and detail history 
 *                              included for files and functions
 * changes done by:   K.Sivakumar
 * 
 * Version:V02.01.00 -->GSM features added. 
 * changes from old version: 1. Added a function check_modem(), to check  
 *                              the modem is working or not. from that we 
 *                              can get a test message to identify modem is 
 * 								working. Otherwise our modem has any fault 
 * 								or not connected.  
 * changes done by:   K.Sivakumar  
 *        
 * Version:V02.00.00 -->code restructure. 
 * 
 * Copyright (c) 2004-2011 Sterna Security.
 * All rights reserved.
 * Author: R.Narayanan
 * 
 */

#include "lpc23xx.H"
#include "menu_main.h"
#include "ticker_itrp.h"
#include "wdt.h"
#include "adc.h"
#include "RFID.h"
#include "LCD.h"
#include "LED.h"
#include "AC_DC_VOLT.h"
#include "BUZZER.h"
#include "DOOR.h"


/******************************************************************************
 ** Function name:       Serial_no
 **
 ** Descriptions:        this function is used for set the serial 
 **                      number and stored in the SD card.   
 **
 ** parameters:          None
 ** Returned value:      None
 ** 
 ******************************************************************************/
void Serial_no(void)
{
	u32_t uiserial_no;	         //it is a temperavary variable to store serial number.
	                                                
#ifdef IBUTTON
	lcd_show(0U,'J',5U);
#else
	lcd_show(0U,'J',4U);	
#endif	
	lcd_show(1U, 'V', 13U);                    // Enter service code:
	if(read_code(HIDE_INPUT)==7091U)
	{
		lcd_show(0U,'e',11U);					//Enter Box serial No
		lcd_str(1U,"KA");
		
		uiserial_no =(u32_t) enter_digits((s32_t) 5,0); //enter digits for serial number
		if(uiserial_no != -1)
		{
			ks_serial=sys_t.serial =uiserial_no;
			write_system_sd();
			serial_flg=1;
			lcd_str(0,"Serial No Changed      ");
			lcd_str(1,"Writing Factory Default");
			delay(500);
			ble_serial_no();
#ifdef BIOMETRIC			
		delete_alluser();
#endif	
			writedefaults_sdcard();
		}
		else
		{
			lcd_show(0U,'e',12U);				//Illegal Value
			lcd_clr(1U);
		}						
	}
	else 
	{
#ifdef IBUTTON
	lcd_show(0U,'J',5U);
#else
	lcd_show(0U,'J',4U);	
#endif
		lcd_show(1U, 'S', 16U);                   //Invalid service code
		delay((u32_t) MESSAGE_TIMEOUT);
		lcd_show(1U,'V',27U);					//	Operation cancelled
		buzzer(300);
		delay((u32_t) MESSAGE_TIMEOUT); 
	}	
}
/******************************************************************************
 ** Function name:       Customer_ID
 **
 ** Descriptions:        this function is used for set the serial 
 **                      number and stored in the SD card.   
 **
 ** parameters:          None
 ** Returned value:      None
 ** 
 ******************************************************************************/
void Customer_ID(void)
{
	u32_t uicust_id;	         //it is a temperavary variable to store serial number.
	                                                
#ifdef IBUTTON
	lcd_show(0U,'J',6U);
#else
	lcd_show(0U,'J',5U);	
#endif
	lcd_show(1U, 'V', 13U);                    // Enter service code:
	if(read_code(HIDE_INPUT)==7091U)
	{
		lcd_str(0,"Enter Customer ID");					//Enter Box serial No
		lcd_clr(1);
		
		uicust_id =(u32_t) enter_digits((s32_t) 6,0); //enter digits for serial number
		if(uicust_id != -1)
		{
			sys_t.cus_id=uicust_id;
			write_system_sd();
			serial_flg=1;
			lcd_str(0,"Customer ID Changed    ");
			lcd_str(1,"Writing Factory Default");
			delay(500);
#ifdef BIOMETRIC			
		delete_alluser();
#endif	
			writedefaults_sdcard();
		}
		else
		{
			lcd_show(0U,'e',12U);				//Illegal Value
			lcd_clr(1U);
		}						
	}
	else 
	{
#ifdef IBUTTON
	lcd_show(0U,'J',6U);
#else
	lcd_show(0U,'J',5U);	
#endif
		lcd_show(1U, 'S', 16U);                   //Invalid service code
		delay((u32_t) MESSAGE_TIMEOUT);
		lcd_show(1U,'V',27U);					//	Operation cancelled
		buzzer(300);
		delay((u32_t) MESSAGE_TIMEOUT); 
	}	
}

/******************************************************************************
 ** Function name:       System_Identity
 **
 ** Descriptions:        this function is used for system identification. 
 ** 					 It displays the company logo, name and the version number.
 **
 ** parameters:          None
 ** Returned value:      None
 ** 
 ******************************************************************************/
void System_Identity(void)
{	
	char szstr[32]={"FW:MYKEE"};
	u16_t si;
	
	strcat(szstr,BOX_VERSION);
	lcd_clr(0U);
	lcd_clr(1U);

	lcd_str(0, "STERNA MYKEE SYSTEM");

	lcd_str(1U, szstr);
	for(si=0U;si<1000U;si++)
	{
		delay(100U);
		if(pressed_key)
		{
			break;			
		}
    	if(((ULALARMS &(u32_t) SERVICE_DOOR_OPEN)!=0)&&((u32_t)glogin_user!=(u32_t) ADMINISTRATOR))
		{
			si=0xFFU;
			break;    	
		}			
	}
	if(si==(u16_t) 0xFF)
	{
		alarm_handler();
	}
	pressed_key=0;
	gkeyb_buf[0]=0U;
	gkeyb_buf[1]=0U;
	gkeyb_buf[2]=0U;
	gkeyb_buf[3]=0U;
	return;	
}

/******************************************************************************
 ** Function name:       utilities
 **
 ** Descriptions:       this function is used for utility operation
 **						 like system restart, write defaults in SD card.
 **
 ** parameters:          None
 ** Returned value:      None
 ** 
 ******************************************************************************/
void utilities(void)
{
	s32_t itemp_key;
  u8_t uclimit;
  s32_t imenu_i = 1;
	char *p ="";
	
	if(glogin_user ==(u8_t) ADMINISTRATOR)
	{
#ifdef IBUTTON	
		uclimit = 6U;// if the user is operator or admin,
		             //they can access only first 3 options from menu.
#else
		uclimit = 5U;// if the user is operator or admin,
		             //they can access only first 3 options from menu.
#endif								 
	}
	else
	{
#ifdef IBUTTON	
		uclimit = 6U;// if the user is operator or admin,
		             //they can access only first 3 options from menu.
#else
		uclimit = 5U;// if the user is operator or admin,
		             //they can access only first 3 options from menu.
#endif								 
	}
	while(1)
	{
		p = NULL;
		
		lcd_show(0U, 'J', 0U);                        // Administrator menu
		lcd_show_index_str(1U, 'J',(u8_t) imenu_i,p);
		
		if (get_menu_key(&itemp_key)==0)
		{
				return;
		}
		if (select_menu(&imenu_i, itemp_key,(s32_t) uclimit))
		{
			switch(imenu_i)
			{
				case 1: 
					System_Identity();
					break;
				case 2: 
					system_restart();
					break;
				case 3: 
					writedefaults_sdcard();
					break;
#ifdef IBUTTON					
				case 4: 
					register_keys();
					break;					
				case 5: 
					Serial_no();			
					break;					
				case 6:
					Customer_ID();
					break;
#else
				case 4: 
					Serial_no();			
					break;					
				case 5:
					Customer_ID();
					break;
#endif					
				default:
					break;
			
			}
		}
	}
}

/******************************************************************************
 ** Function name:       system_restart
 **
 ** Descriptions:       this function used for system restart
 **
 ** parameters:          None
 ** Returned value:      None
 ** 
 ******************************************************************************/
void system_restart(void)
{
	rstflag=1;
	wdt_2S_Init();
    VICIntEnClr |= INT_MCI | INT_TIMER0 | INT_UART0 | INT_UART1 | INT_UART2| INT_TIMER1;		//Disable Interrupts
	wdt_reset_feed();
    VICIntEnable |= INT_MCI | INT_TIMER0 | INT_UART0 | INT_UART1| INT_UART2 | INT_TIMER1;		//Enable Interrupts
	buzzer(300);
	lcd_show(0U,'V',20U);       // AKA reset Initiated
	lcd_show(1U,'V',21U);       // System Restarting
	delay(4000U);
	
	glast_event=0;
	glast_event_check=0;
}

/******************************************************************************
 ** Function name:       Check_keycontrol
 **
 ** Descriptions:       this function is used for key taken/return operation
 **
 ** parameters:          None
 ** Returned value:      None
 ** 
 ******************************************************************************/
void Check_keycontrol(void)
{
	u16_t ustimeout;
	char szstr[32];	
	u8_t uci;
	u8_t uclk[MAX_KEYS];
	u8_t ucprev_state[MAX_KEYS];
	u8_t ucchek_status[MAX_KEYS];
	u8_t ucstate;
	u8_t uctoggle_led = 0U;
	u32_t uikeystaus=0U;
	u8_t total_key_flag=0;

	lcd_show(0U, 'W', 5U);         // Check Key Control
	lcd_show(1U, 'V', 13U);              //Enter service code:
	if (read_code(HIDE_INPUT)!= 1998U)
	{
		lcd_show(0U, 'W', 5U);    // Check Key Control
		lcd_show(1U, 'S', 16U);          // Invalid service code
		delay((u32_t) MESSAGE_TIMEOUT);
		return;
	}

#ifndef NO_DOOR_NO_SOLENOID
#ifndef TRUEKEY32
	if(!trg(2))
#else
	if(!trg(1))	
#endif	
	{
		ustimeout =(u16_t) sys_t.open_door_time * 10U;

		while (1) 								// Wait for the door to be opened
		{
			lcd_show(0U, 'D', 0U); 				// Please open the door
#ifndef TRUEKEY32			
			if (trg(2))
#else
			if (trg(1))
#endif			
			{
				break;
			}

			if (key1(100U)==ESC)
			{
				return;
			}

			sprintf(szstr, "%d", (ustimeout + 8U) / 10U);
			lcd_show_str(1U, 'D', 4U, szstr);              //Timer:__

			ustimeout--;

			if (ustimeout==0)
			{
				lcd_show(0U, 'D', 2U); 			// Open door timeout
				key1((u32_t) MESSAGE_TIMEOUT);
				buzzer(1000);
				return;
			}
		}
	}
#endif	

	for (uci = 0U; uci <(u8_t) max_keys_available; uci++)
	{
		uclk[uci] = 0U;
	}

	for (uci = 0U; uci <(u8_t) max_keys_available; uci++)
	{
		ucprev_state[uci] = lock_state[uci];
		ucchek_status[uci]= ucprev_state[uci];
	}
	for (uci = 0U; uci <(u8_t) max_keys_available; uci++)
	{
		ucstate = lock_state[uci];
		ucstate &=(u8_t) ~(LOCK_GREEN | LOCK_RED | LOCK_SOL);
		if((ucstate & 0xF0)==(ucprev_state[uci] & 0xF0))	
		{
			if (ucstate &(u8_t) LOCK_CLOSED)
			{
				ucstate |=(u8_t) LOCK_GREEN;
				uclk[uci] = 1U;
				total_key_flag=uci+1;
			}
			else if (ucstate &(u8_t) LOCK_OPEN)
			{
				ucstate |=(u8_t) LOCK_RED;
				uclk[uci] = 1U;
				total_key_flag=uci+1;			
			}
			else
			{
				uclk[uci] = 0U;
			}
		}
		lock_state[uci] = ucstate;
	}	

	ustimeout =(u16_t) sys_t.key_turn_time * 10U;

	while (1)							 // Process keys, one by one
	{
		lcd_show(0U, 'W', 9U); 			// Turn indicated keys

		sprintf(szstr, "%d", (ustimeout + 8U) / 10U);
		lcd_show_str(1U, 'D', 4U, szstr);
		

		if (ustimeout==0)
		{
			lcd_show(1U, 'K', 2U); // Key handling timeout
			delay(ONE_SEC);
			for(uci=0;uci<total_key_flag;uci++)
			{
				if((ucchek_status[uci]&0xF0)!=(ucprev_state[uci]&0xF0))
				{
					uikeystaus=0xFFU;
					ustimeout =(u16_t) sys_t.key_turn_time * 10U;					
					break;
				}
				else
				{
					uikeystaus=0;
				}
			}
			if(uikeystaus)
			{
				sprintf(szstr,"%d",uci+1);
				lcd_show_str(0U,'K',15,szstr);
				lcd_show(1U,'K',16);
				delay(MESSAGE_TIMEOUT);
			}
			else
			{
				break;
			}
		}
		else
		{
			ustimeout--;
		}

		if (key1(100U)==ESC)
		{
			for(uci=0;uci<total_key_flag;uci++)
			{
				if((ucchek_status[uci]&0xF0)!=(ucprev_state[uci]&0xF0))
				{
					uikeystaus=0xFFU;
					break;
				}
				else
				{
					uikeystaus=0;
				}
			}
			if(uikeystaus)
			{
				uikeystaus=0;	
				sprintf(szstr,"%d",uci+1);
				lcd_show_str(0U,'K',15,szstr);
				lcd_show(1U,'K',16);
				delay(MESSAGE_TIMEOUT);
			}
			else
			{
				break;
			}
		}

#ifndef NO_DOOR_NO_SOLENOID
#ifndef TRUEKEY32
		if (trg(2)==0)
#else
		if (trg(1)==0)
#endif				
		{
			for(uci=0;uci<total_key_flag;uci++)
			{
				if((ucchek_status[uci]&0xF0)!=(ucprev_state[uci]&0xF0))
				{
					uikeystaus=0xFFU;
					break;
				}
				else
				{
					uikeystaus=0;
				}
					
			}
			if(uikeystaus)
			{
				uikeystaus=0;	
				sprintf(szstr,"%d",uci+1);
				lcd_show_str(0U,'K',15,szstr);
				lcd_show(1U,'K',16);
				
				delay(MESSAGE_TIMEOUT);

				
				ustimeout =(u16_t) sys_t.open_door_time * 10U;
			
				while (1)								// Wait for the door to be opened
				{
					lcd_show(0U, 'D', 0U);				// Please open the door
#ifndef TRUEKEY32					
					if (trg(2))
#else
					if (trg(1))
#endif					
					{
						ustimeout =(u16_t) sys_t.key_turn_time * 10U;					
						break;
					}			
					if(ustimeout)
					{
						sprintf(szstr, "%d", (ustimeout + 8U) / 10U);
						lcd_show_str(1U, 'D', 4U, szstr);
				
						ustimeout--;
					}
					else
					{
						lcd_show(0U,'K',17);
						buzzer(100);
					}
					key1(100U);

				}
			}
			else
			{
				break;
			}


		}
#endif

		for (uci = 0U; uci <(u8_t) total_key_flag; uci++)
		{
			if (uclk[uci])
			{
				break;
			}
		}

		if (uci ==(u32_t) total_key_flag)
		{
			break;
		}

		for (uci = 0U; uci <(u32_t) total_key_flag; uci++)
		{
			ucstate = lock_state[uci];
			ucstate &=(u8_t) ~(LOCK_SOL);
			lock_state[uci] = ucstate;

			if ((ucstate &(u8_t) (LOCK_OPEN | LOCK_CLOSED))==0)
			{
				if (uclk[uci]) // In between state so user is turning the key
				{
					break;
				}
			}
		}

		if (uci <(u8_t) total_key_flag) // Key being turned?
		{
			ucstate = lock_state[uci];
			ucstate |=(u8_t) LOCK_SOL;
			lock_state[uci] = ucstate;

			sprintf(szstr, "%d", uci + 1U);

			if (ucprev_state[uci] &(u8_t) LOCK_CLOSED)
			{
				lcd_show_str(0U, 'Q', 1U, szstr);
			}
			else
			{
				lcd_show_str(0U, 'Q', 2U, szstr);
			}

			ustimeout =(u16_t) sys_t.key_turn_time * 10U;

			while (1)
			{
				sprintf(szstr, "%d", (ustimeout + 8U) / 10U);
				lcd_show_str(1U, 'D', 4U, szstr);
				if (ustimeout==0)
				{
					lcd_show(1U, 'K', 2U); // Key handling timeout
					key1((u32_t) MESSAGE_TIMEOUT);
					break;
				}
				else
				{
					ustimeout--;
				}

				sprintf(szstr, "%d", uci + 1U);
				ucstate = lock_state[uci];

				if (ucstate &(u8_t) LOCK_OPEN)
				{
					ucstate &=(u8_t) ~(LOCK_GREEN);
					ucstate |=(u8_t) LOCK_RED;
					lock_state[uci] = ucstate;
					ucprev_state[uci] = lock_state[uci];
					lcd_show_str(0U, 'Q', 3U, szstr);// Key x taken
					ustimeout =(u16_t) sys_t.key_turn_time * 10U;
					break;
				}
				else if (ucstate &(u8_t) LOCK_CLOSED)
				{
					ucstate &=(u8_t) ~(LOCK_RED);
					ucstate |=(u8_t) LOCK_GREEN;
					lock_state[uci] = ucstate;
					ucprev_state[uci] = lock_state[uci];
					lcd_show_str(0U, 'Q', 4U, szstr);// Key x returned
					ustimeout =(u16_t) sys_t.key_turn_time * 10U;
					break;
				}
				else
				{
				}

				uctoggle_led = !uctoggle_led; // Blinking LED
				if (ucprev_state[uci] &(u8_t) LOCK_CLOSED)
				{
					if (uctoggle_led)
					{
						ucstate |=(u8_t) LOCK_GREEN;
					}
					else
					{
						ucstate &=(u8_t) ~(LOCK_GREEN);
					}
				}
				else if (ucprev_state[uci] &(u8_t) LOCK_OPEN)
				{
					if (uctoggle_led)
					{
						ucstate |=(u8_t) LOCK_RED;
					}
					else
					{
						ucstate &=(u8_t) ~(LOCK_RED);
					}
				}
				else
				{
				}
				
				lock_state[uci] = ucstate;
				if (key1(100U))
				{
					break;
				}
			}
			ucstate &=(u8_t) ~(LOCK_SOL);
			lock_state[uci] = ucstate;
			delay(500U);
		}
	}

	for (uci = 0U; uci <(u8_t) max_keys_available; uci++) // Done, so inactivate all LEDs and solenoids
	{
		ucstate = lock_state[uci];
		ucstate &=(u8_t) ~(LOCK_RED | LOCK_SOL | LOCK_GREEN);
		lock_state[uci] = ucstate;
	}

#ifndef NO_DOOR_NO_SOLENOID
	ustimeout =(u16_t) sys_t.close_door_time * 10U;
	while (1)
	{
		lcd_show(0U, 'D', 1U); // Please close the door
		sprintf(szstr, "%d", (ustimeout + 8U) / 10U);
		lcd_show_str(1U, 'D', 4U, szstr);

#ifndef TRUEKEY32
		if (trg(2)==0)
#else
		if (trg(1)==0)
#endif		
		{
			break;
		}

		key1(100U);

		ustimeout--;

		if (ustimeout==0)
		{
			lcd_show(0U, 'D', 3U); // Close door timeout
			key1((u32_t) MESSAGE_TIMEOUT);

			ULALARMS |=(u32_t) DOOR_OPEN_TIMEOUT;

			card_id_available = 0;
			buzzer(ONE_SEC);

			prot_log_event(EVT_DOOR_CLS_TOUT,(s32_t) user_code_main ,0U, 0U); //ALARM EVT Logged in the event. MLN		



			break;
		}
	}
#endif	
	return;

}

/******************************************************************************
 ** Function name:      Check_cardreader
 **
 ** Descriptions:       this function is used for check card reader operation
 **
 ** parameters:          None
 ** Returned value:      None
 ** 
 ******************************************************************************/
void Check_cardreader(void)
{
	u32_t uicard=0U;
	char szstr[32];
	u8_t  uci,i;
	unsigned char read_rfideas[5]={0xfa,0x00,0x00,0x00,0x0c};
#ifndef IBUTTON
	lcd_show(0U, 'W', 4U);					// Check RFID Reader
#else
		lcd_show(0U, 'W', 5U);					// Check RFID Reader
#endif	
	lcd_show(1U, 'S', 10U); 				// Show the RFID

	card_id_available = 0;

	for (uci = 0U; uci < 100U; uci++)
	{
		if(!(FIO2PIN & 0x00000004))
		{	
			if(regular_card()==1)
			{		
				uicard= card_id;
				card_id_available =0;
				card_id=0U;
				break;
			}
		}
		for(i=0;i<5;i++)
		{
			pchar(read_rfideas[i]);
		} 

		if (key1(100U))
		{
			uicard= 0U;
			break;
		}

		if (card_id_available)
		{
			buzzer(ONE_SEC);
			uicard= card_id;
			card_id_available =0;
			card_id=0U;
			break;
		}

	}
	if (uicard)
	{
		sprintf(szstr, "%08X", uicard);
		lcd_show_str(1U, 'c', 3U, szstr);	  //RFID [%s]
		key1(10000U);

	}
	else
	{
		lcd_show(0U, 'W', 4U);				 // Check RFID Reader
#ifndef TRUEKEY32		
		lcd_show(1U, 'W', 11U); 			 // No RFID present 
#else
		lcd_show(1U, 'W', 10U); 			 // No RFID present 
#endif		
		delay((u32_t) MESSAGE_TIMEOUT);
	}
}


/******************************************************************************
 ** Function name:   check_board
 **
 ** Descriptions:    this function is used for check board parameters
 **                  like voltage,battery voltage,service door open/close
 **					 & buzzer checking
 **
 ** parameters:      None
 ** Returned value:  None
 ** 
 ******************************************************************************/
void Check_board(void)
{
	s32_t itemp_key;
	u16_t us_volt;
	u8_t uci;
	s32_t imenu_i = 1;
	volatile register u16_t Volt_Value;
	f32_t fvoltage;
	char szstr[20];

	while(1)
	{
		lcd_show(0U, 'P', 0U); 								// Diagnostic menu
		lcd_show_index(1U, 'P',(u8_t) imenu_i);
		if (get_menu_key(&itemp_key)==0)
		{
			return;
		}
		if (select_menu(&imenu_i,itemp_key, 5))
		{
			switch (imenu_i)
			{
				case 1:
					for (uci=0U; uci<100U; uci++)
					{
						if (AD0DR1 & (1<<31)) 				//AC
						{
							us_volt =read_ac_adc_ch();
							if (us_volt)
							{
								lcd_show(0U, 'V', 18U);
								fvoltage=ac_voltage(us_volt);
								sprintf(szstr, "%2.2f",fvoltage);
								lcd_show_str(1U, 'P', 6U, szstr);	//AC Voltage:__V
							}
							else
							{
								lcd_show(0U, 'V', 19U);           //Mains OFF
								lcd_clr(1U);
							}
						}
						if (key1(100U))
						{
							break;
						}
					}
					break;
				case 2:
					for (uci=0U; uci<100U; uci++)
					{
						if (AD0DR0 & (1<<31)) //BATTERY
						{
							us_volt = read_dc_adc_ch();
							fvoltage=dc_voltage(us_volt);
							sprintf(szstr, "%2.2f",fvoltage);
							lcd_show_str(0U, 'P', 7U, szstr);  //Battery Voltage:__V
							lcd_clr(1U);
						}
						if (key1(100U))
						{
							break;
						}
					}
					break;
				case 3:
					for (uci=0U; uci<100U; uci++)
					{
						lcd_show(0U, 'P', 3U);  //Outer Door status
#ifndef NO_DOOR_NO_SOLENOID
#ifndef TRUEKEY32
						if (trg(2))
#else
						if (trg(1))
#endif											
						{
							lcd_show(1U, 'P', 9U); //Closed
						}
						else
						{
							lcd_show(1U, 'P', 8U); //Opened
						}
#else
						lcd_show(1,'P',11);        //No outer door
#endif					
						if (key1(100U))
						{
							break;
						}
					}	
					break;
				case 4:
					for (uci=0U; uci<100U; uci++)
					{
						lcd_show(0U, 'P', 4U);     //Service Door Status
#ifndef TRUEKEY32						
						if (trg(1))
#else
						if (trg(0))
#endif						
						{
							lcd_show(1U, 'P', 9U); //Closed
						}
						else
						{
							lcd_show(1U, 'P', 8U); //Opened
						}
						ULALARMS &=(u32_t) ~(SERVICE_DOOR_OPEN);
						if (pressed_key)
						{
							break;
						}
						delay(100U);
					}
					break;
				case 5:
				{
					lcd_show(0U, 'P', 10U);   //Buzzer checking.....
					lcd_clr(1U);
					buzzer(5000);
					delay(5000U);
				}
					break;
				default:
					break;
			}
		}
	}
}

/******************************************************************************
 ** Function name:   Check_output
 **
 ** Descriptions:    check red and green LED output for connnecting key holders..
 **
 ** parameters:      None
 ** Returned value:  None
 ** 
 ******************************************************************************/
void Check_output(void)
{
	u8_t ucstate;
	u8_t uci;
	u8_t ucflag=0U;
#ifndef NO_DOOR_NO_SOLENOID
	char szstr[32];
	s32_t timeout;
	lcd_show(0U, 'D', 0U);					 // Please open the door
	relay(1, ON);
	lock_on(1); 
	timeout =((s32_t) sys_t.open_door_time *(s32_t) 10);

	while (1) 								// Wait for the door to be opened
	{
		lcd_show(0U, 'D', 0U); 				// Please open the door
#ifndef TRUEKEY32		
		if (trg(2))
#else
		if (trg(1))
#endif		
		{
			break;
		}

		if (key1(100U))
		{
			lock_on(0); 
			relay(1, OFF);
			return;
		}

		sprintf(szstr, "%d", (timeout + 8) / 10);
		lcd_show_str(1U, 'D', 4U, szstr);

		timeout--;

		if (timeout==0)
		{
			lock_on(0); 
			relay(1, OFF);
			lcd_show(0U, 'D', 2U); // Open door timeout
			key1((u32_t)MESSAGE_TIMEOUT);
			buzzer(1000);
			return;
		}
	}
	relay(1, OFF);
	lock_on(0);
#endif
#ifndef TRUEKEY32
	lcd_show(0U, 'W', 9U);      // Checking LEDs...
#else
	lcd_show(0U, 'W', 8U);      // Checking LEDs...
#endif	
	lcd_clr(1U);
	while (1)
	{
		for (uci = 0U; uci <(u8_t) MAX_KEYS ; uci++)
		{
#ifndef TRUEKEY32
			lcd_show(0U, 'W', 9U);   // Checking LEDs...
#else
			lcd_show(0U, 'W', 8U);   // Checking LEDs...
#endif			
			lcd_clr(1U);
#ifndef NO_DOOR_NO_SOLENOID
#ifndef TRUEKEY32
			if (trg(2)==0)
#else
			if (trg(1)==0)
#endif			
			{
				ucflag=0x0FU;
				break;
			}
#endif		
			if (key1(100U)== ESC)
			{
				ucflag=0x0FU;
				break;
			}
#ifdef TRUEKEY32						
			ucstate = lock_state[uci];
			ucstate &=(u8_t) ~(LOCK_GREEN | LOCK_RED | LOCK_SOL);
#endif			
			if(ucflag)
			{
#ifdef IBUTTON
				green_led[uci]=1;
				red_led[uci]=0;
#else
				ucstate |=(u8_t) LOCK_RED;
#endif				
			}
			else
			{
#ifdef IBUTTON				
				red_led[uci]=1;
				green_led[uci]=0;
#else
				ucstate |=(u8_t) LOCK_GREEN;
#endif				
			}
			lock_state[uci] = ucstate;
			delay(300U);
		}
		if(ucflag ==(u8_t) 0x0F )
		{
			break;
		}
		ucflag =(u8_t) !ucflag;
	}

	for (uci = 0U; uci <(u8_t) MAX_KEYS; uci++)
	{
#ifdef IBUTTON		
		green_led[uci]=0;
		red_led[uci]=0;
#else
		ucstate = lock_state[uci];
		ucstate &=(u8_t) ~(LOCK_GREEN | LOCK_RED | LOCK_SOL);
		lock_state[uci] = ucstate;
#endif
	}
#ifndef NO_DOOR_NO_SOLENOID
	timeout =((s32_t) sys_t.close_door_time *(s32_t) 10);

	while (1)
	{
		lcd_show(0U, 'D', 1U); // Please close the door
		sprintf(szstr, "%d", (timeout + 8) / 10);
		lcd_show_str(1U, 'D', 4U, szstr);
#ifndef TRUEKEY32
		if (trg(2)==0)
#else
		if (trg(1)==0)
#endif		
		{
			break;
		}

		key1(100U);

		timeout--;

		if (timeout==0)
		{
			lcd_show(0U, 'D', 3U); 												// Close door timeout
			key1((u32_t) MESSAGE_TIMEOUT);

			ULALARMS |=(u32_t) DOOR_OPEN_TIMEOUT;
			card_id_available = 0;
			buzzer(ONE_SEC);
			prot_log_event(EVT_DOOR_CLS_TOUT,(s32_t) user_code_main,  0U,0U); //ALARM EVT Logged in the event		
			break;
		}
	}
#endif	
	return;

}

/******************************************************************************
 ** Function name:   Check_output
 **
 ** Descriptions:    check red and green LED output for connnecting key holders..
 **
 ** parameters:      None
 ** Returned value:  None
 ** 
 ******************************************************************************/
void Check_solenoid(void)
{
	u8_t ucstate;
	u8_t uci;
	u8_t ucflag=0U;
#ifndef NO_DOOR_NO_SOLENOID
	char szstr[32];
	s32_t timeout;
	lcd_show(0U, 'D', 0U);					 // Please open the door
	relay(1, ON);
	lock_on(1); 
	timeout =((s32_t) sys_t.open_door_time *(s32_t) 10);

	while (1) 								// Wait for the door to be opened
	{
		lcd_show(0U, 'D', 0U); 				// Please open the door
#ifndef TRUEKEY32		
		if (trg(2))
#else
		if (trg(1))
#endif		
		{
			break;
		}

		if (key1(100U))
		{
			lock_on(0); 
			relay(1, OFF);
			return;
		}

		sprintf(szstr, "%d", (timeout + 8) / 10);
		lcd_show_str(1U, 'D', 4U, szstr);

		timeout--;

		if (timeout==0)
		{
			lock_on(0); 
			relay(1, OFF);
			lcd_show(0U, 'D', 2U); // Open door timeout
			key1((u32_t)MESSAGE_TIMEOUT);
			buzzer(1000);
			return;
		}
	}
	relay(1, OFF);
	lock_on(0); 
#endif

	lcd_show(0U, 'W', 8U);      // Checking Solenoidss...
	lcd_clr(1U);
	while (1)
	{
		for (uci = 0U; uci <(u8_t) MAX_KEYS ; uci++)
		{
			lcd_show(0U, 'W', 8U);   // Checking Solenoids...
			lcd_clr(1U);
#ifndef NO_DOOR_NO_SOLENOID
#ifndef TRUEKEY32
			if (trg(2)==0)
#else
			if (trg(1)==0)
#endif
			{
				ucflag=0x0FU;
				break;
			}
#endif		
			if (key1(100U)== ESC)
			{
				ucflag=0x0FU;
				break;
			}			
			{
				green_led[uci]=1;
				solenoid[uci]=0;
			}
			
			lock_state[uci] = ucstate;
			delay(1000U);
			{
				solenoid[uci]=1;
				green_led[uci]=0;
			}
		}
		if(ucflag ==(u8_t) 0x0F )
		{
			break;
		}
		ucflag =(u8_t) !ucflag;
	}

	for (uci = 0U; uci <(u8_t) max_keys_available; uci++)
	{
		green_led[uci]=0;
		red_led[uci]=0;
		solenoid[uci]=1;
	}
#ifndef NO_DOOR_NO_SOLENOID
	timeout =((s32_t) sys_t.close_door_time *(s32_t) 10);

	while (1)
	{
		lcd_show(0U, 'D', 1U); // Please close the door
		sprintf(szstr, "%d", (timeout + 8) / 10);
		lcd_show_str(1U, 'D', 4U, szstr);
#ifndef TRUEKEY32
		if (trg(2)==0)
#else
		if (trg(1)==0)
#endif		
		{
			break;
		}

		key1(100U);

		timeout--;

		if (timeout==0)
		{
			lcd_show(0U, 'D', 3U); 												// Close door timeout
			key1((u32_t) MESSAGE_TIMEOUT);

			ULALARMS |=(u32_t) DOOR_OPEN_TIMEOUT;
			card_id_available = 0;
			buzzer(ONE_SEC);
			prot_log_event(EVT_DOOR_CLS_TOUT,(s32_t) user_code_main, 0U, 0U); //ALARM EVT Logged in the event		
			break;
		}
	}
#endif	
	return;

}

/******************************************************************************
 ** Function name:   Check_keypad
 **
 ** Descriptions:    this function is used for check the keypad keys
 **
 ** parameters:      None
 ** Returned value:  None
 ** 
 ******************************************************************************/
void Check_keypad(void)
{
	u32_t ui;
	u8_t ucount=0U;
	s32_t itemp_key;
	u16_t uslimit = 9U;
	char szstr[32];
	
	lcd_show(0U,'Z',0U);      //Check Keypad
	lcd_show(1U, 'Z', 15U);	  //Press Any Key
	for (;;)	
	{

		itemp_key=key1((u32_t) MESSAGE_TIMEOUT);  // getting the value
		if(itemp_key)
		{
			ucount=0U;
		}
		if (itemp_key == ENTER)
		{
			lcd_show(1U, 'Z', 11U);				
		}
		else if(itemp_key == ARROW_D)
		{
			sprintf(szstr,"%s ","DOWN");
			strcat(szstr,find_string('Z',12));  
			lcd_str(1U, szstr);				
		}
		else if (itemp_key == ARROW_U)
		{
			sprintf(szstr,"%s ","UP");
			strcat(szstr,find_string('Z',12));
			lcd_str(1U, szstr);				
		}
		else if (itemp_key == ARROW_L)
		{		
			sprintf(szstr,"%s ","LEFT");
			strcat(szstr,find_string('Z',13));   // * or DEL
			lcd_str(1U, szstr);				
		}
		else if (itemp_key == ARROW_R)
		{
		
			sprintf(szstr,"%s ","RIGHT");
			strcat(szstr,find_string('Z',14));   // # or Space
			lcd_str(1U, szstr);				
		}	
		else if ((itemp_key >= '0') && (itemp_key <= ('0' + uslimit)))
		{
			ui =(u32_t) itemp_key - '0';
			lcd_show(1U, 'Z', ui+1U);          // show the pressed key value.
		}
		else if((itemp_key ==(s32_t) ESC)||((ucount==15) &&(itemp_key==0)))
		{
			break;
		}
		else
		{
			lcd_show(1U, 'Z', 15U);	     //Press Any Key
			ucount++;
		}
			
	}
}

/******************************************************************************
 ** Function name:   Getkey_user
 **
 ** Descriptions:    this function is used for check key status, 
 ** 				 whether is it available, remove or not sensing
 **
 ** parameters:      None
 ** Returned value:  None
 ** 
 ******************************************************************************/
void Getkey_user(void)
{
	u8_t uci;
	char szstr[24] ={ "" };
	char szstr1[24]={ "" };
	char szstr3[24]={ "" };
	u8_t ucstate;
	char *p ="";
	for (uci=0U; uci<(u8_t)max_keys_available; uci++)
	{
		if (key1(100U) == ESC)
		{
			break;
		}
		ucstate =lock_state[uci];
		read_key_sd((s32_t)uci);
		read_keyevents_sd(uci);
		if ((key_events_t[uci].exist ==(u32_t) EXIST) &&((u32_t)key_events_t[uci].status ==(u32_t)LOCK_OPEN))//if key removed
		{
			sprintf(szstr, "%04d", key_events_t[uci].user);
			p=find_string('B', 0);
			sprintf(szstr3, p, (uci+1U));
			p=find_string('B', 1);
			sprintf(szstr1, p, szstr, key_events_t[uci].task);
			lcd_str(0U, szstr3);
			lcd_str(1U, szstr1);
			delay((u32_t) MESSAGE_TIMEOUT);
		}
		else if ((ucstate!=(u8_t)LOCK_OPEN) && (ucstate!=(u8_t)LOCK_CLOSED)) //Not Sensed
		{
			p=find_string('B', 2);
			sprintf(szstr3, p, uci+1U);
			lcd_str(0U, szstr3);
			lcd_show(1U, 'V', 22U);
			delay((u32_t) MESSAGE_TIMEOUT);
		}

		else //available
		{
			p=find_string('B', 3);
			sprintf(szstr3, p, uci+1U);
			lcd_str(0U, szstr3);
			lcd_clr(1U);
			delay((u32_t) MESSAGE_TIMEOUT);
		}
	}
}

/******************************************************************************
 ** Function name:   diagnostics
 **
 ** Descriptions:    this function contains the menu for checking operations
 **					 here we are checking board,led,key open/close,keypad and card reader
 **
 ** parameters:      None
 ** Returned value:  None
 ** 
 ******************************************************************************/
void diagnostics(void)
{
	s32_t itemp_key;
	s32_t imenu_i = 1;
	
	while(1)
	{
		lcd_show(0U, 'W', 0U); // Diagnostic menu
		lcd_show_index(1U, 'W',(u8_t) imenu_i);
		if (get_menu_key(&itemp_key) == 0)
		{
			return;
		}
		
		if (select_menu(&imenu_i, itemp_key,5))
		{
			switch (imenu_i)
			{
				case 1:	
					Check_board();					
					break;
				case 2:	
					Check_output();					
					break;
#ifndef IBUTTON
//				case 3:
//					Check_solenoid();
//					break;
//				case 2:
//					Check_keycontrol();
//					break;
				case 3:	
					Check_keypad();					
					break;
				case 4:	
					Check_cardreader();				
					break;
				case 5:	
					Check_keycontrol();
					break;
#else					
				case 3:
					Check_solenoid();
					break;
				case 4:	
					Check_keypad();					
					break;
				case 5:	
					Check_cardreader();				
					break;
#endif
				default:
					break;
			}
		}
		if (ULALARMS &(u32_t) DOOR_OPEN_TIMEOUT)
		{
			return;
		}

	}
}
/*-----------------------------------------end of file-------------------------------------------*/
