
#include <stdio.h>
#include <string.h>
#include "menu_main.h"
#include "ticker_itrp.h"
#include "lpc23xx.h"
#include "timer.h"
#include "uart.h"
#include "LED.h"
#include "LCD.h"
#include "BUZZER.h"
#include "RFID.h"
#include "CLOCK_CALNDR.h"
#include "DOOR.h"

u32_t	gmul_code[5];					// Temporarily  save the user code for multiple authendication
s16_t ginstaller_code = 4321;        // Installer code              
s16_t ginstaller_pin  = 1234;		// Installer Pin	
s16_t gadmin_code = 3333;    		// Admin Code
u8_t  glogin_user; 					// Temporarily store the user type
u32_t user_code_main;
u32_t user_code_ble;


long int user_end_tickvalue=0;
u8_t key_no=0;
u32_t key_pos; 
u32_t key_chk=0;  
u8_t  check_key[MAX_KEYS];


/******************************************************************************
 ** Function name:      alarm_handler
 **
 ** Descriptions:       This function is used for alarm making while 
 **   					any event occurs.
 **
 ** parameters:         none
 ** 				 
 ** Returned value:     none
 ** 
 ******************************************************************************/
void alarm_handler(void)
{
	u8_t uci;
	char szstr[32];
	static u8_t ucmainsoff, ucbatlow,ucmech_override,ucservice_door;
	u8_t uckey_status;
		static u32_t uik1;
	static u32_t uik2;
	static s32_t j=0;
	static u8_t uccount=0;
	u32_t uikey_flag=0;
	if((fw_flag == 0) && (gpacket_in > 0))
	{
		gpacket_in = 0;
		buzzer(MESSAGE_TIMEOUT);
		lcd_show(0,'V',23);
		lcd_show(1,'V',24);		
		delay(MESSAGE_TIMEOUT);
		buzzer(100);
	}

	if(sys_t.self_user_allign && (endtick_value%86400)<10)		//86400
	{

			if(sys_t.self_user_allign & 0x01)
				self_user_allign();
			if(sys_t.self_user_allign & 0x02)			
				selftask_allign();
			sys_t.self_user_allign=0;
			write_system_sd();				
	}
	
//key overide is mainly used for real time key monitoring.
//If some one try to change the key position it automatically update in to key event log.
	if(ULALARMS & KEY_OVERRIDE)
	{
		for(uci=0;uci<max_keys_available;uci++)
		{
				uckey_status=lock_key_state[uci];
				read_keyevents_sd((s32_t) uci);
				if((uckey_status & LOCK_OPEN)&&(key_events_t[uci].status!=LOCK_OPEN))
				{
					update_key_log(ginstaller_code,0,(s32_t) uci,LOCK_OPEN);
				}
				else if((uckey_status & LOCK_CLOSED)&&(key_events_t[uci].status!=LOCK_CLOSED))
				{
					update_key_log(ginstaller_code,0,(s32_t) uci,LOCK_CLOSED);
				}
				else if(((uckey_status & (LOCK_OPEN | LOCK_CLOSED))==0)&&((key_events_t[uci].status == LOCK_OPEN)))//||(key_events_t.status == LOCK_CLOSED)((key_events1.status !=LOCK_OPEN)||((key_events1.status ==LOCK_OPEN)&&(key_events1.user==INSTALLER_code))))
				{
					update_key_log(ginstaller_code,0,(s32_t) uci,(u8_t) LOCK_NOT_FOUND);
				}
				else
				{
					
				}
		}
		ULALARMS &=~(KEY_OVERRIDE);
	}
	if(ULALARMS & SERVICE_DOOR_OPEN)        // check the service door open/close
	{
#ifndef TRUEKEY32		
		if(trg(1))
		{
			while (trg(1))
#else
		if(trg(0))
		{
			while (trg(0))
#endif				
			{
				if(glogin_user==ADMINISTRATOR)
				{
					if(ucservice_door==0)
					{
						prot_log_event(EVT_TAMPER,ginstaller_code,0,0);
						ucservice_door=1U;
					}					
				}
				else
				{
					if(ucservice_door==0)
					{
						relay(1,ON);			
						prot_log_event(EVT_BURGLARY,ginstaller_code,0,0);
					}				
					buzzer(200);
					lcd_show(0,'E',17);
					lcd_clr(1);		
					ucservice_door=1U;
				}
				buzzer(2000);
				delay(200); 
			}
			buzzer(10);
			relay(1,OFF);
			prot_log_event(EVT_BACKDOOR_CLOSED,ginstaller_code,0,0);
			ULALARMS &=~(SERVICE_DOOR_OPEN);
			if(ucservice_door==1)		//One time flag for service door.
			{
				if((ULALARMS&SERVICE_DOOR_OPEN)==0)
				{
					ucservice_door=0;
				}
			}
		}
		ULALARMS &=~(SERVICE_DOOR_OPEN);
		return;
	}
		
		
#ifndef NO_DOOR_NO_SOLENOID
	if ((ULALARMS & DOOR_OPEN_TIMEOUT) || (ULALARMS & MECH_OVERRIDE))
	{
#ifndef TRUEKEY32		
		if(trg(2))
		{
			while (trg(2))
#else
		if(trg(1))
		{
			while (trg(1))
#endif			
			{
				if(ULALARMS & DOOR_OPEN_TIMEOUT)
				{
 					if(ULALARMS &ENDUSERALARM)
						glogin_user=END_USER;
					lcd_show(0,'D',3);
					lcd_show(1,'D', 1);
				}
				else
				{
					lcd_show(0,'E',3);							//MECHANICAL OVERRIDE
					lcd_show(1,'D', 1);						// Please close the door
					if(ucmech_override==0)
					{
						prot_log_event(EVT_MECH_OVERRIDE,ginstaller_code,0,0);
					}
					ucmech_override=1U;
				}
				relay(1,ON);
				buzzer(2000);
				delay(200); 
			}
			relay(1,OFF);
			ULALARMS &= ~(ENDUSERALARM) ;
			glogin_user=0;
			buzzer(10);
			if(ULALARMS & DOOR_OPEN_TIMEOUT)
			{
				prot_log_event(EVT_OUTDOOR_CLSD,ginstaller_code,0,0);
				ULALARMS &= ~(DOOR_OPEN_TIMEOUT); 		
			}
			else
			{
				prot_log_event(EVT_OUTDOOR_CLSD,ginstaller_code,0,0);
				ULALARMS &= ~(MECH_OVERRIDE); 		
			}
		}
		if(ULALARMS & DOOR_OPEN_TIMEOUT)	//Door closed before DOOR_OPEN_TIMEOUT Alarm handled
		{
			ULALARMS &= ~(DOOR_OPEN_TIMEOUT); 
		}
		if(ULALARMS & MECH_OVERRIDE)	//Door closed before DOOR_OPEN_TIMEOUT Alarm handled
		{
			ULALARMS &= ~(MECH_OVERRIDE); 
		}
	}
	/*mech_override one time flag*/
	if(ucmech_override==1)
	{
		if((ULALARMS & MECH_OVERRIDE)==0)
		{
			ucmech_override=0;
		}
	}
	
#endif

/*
Check for AC Power & Battery:
If AC Power fails, check if battery low is SET. If SET, then send event and continuously Buzz.
If AC Power is present and Battery low is set, then it is "Battery Fault!", that is Battery is either not
Disconnected or is low, needs to be charged.
*/
	if (ULALARMS & AC_POWER_OFF)
	{
		lcd_show(0,'E',0);
		lcd_show(1,'E',2);
		if(ucmainsoff == 0)
		{
			buzzer(ONE_SEC);
			prot_log_event(EVT_POWER_FAULT, ginstaller_code, 0, 0);	
		}
		delay(ONE_SEC+50);
		ucmainsoff = 1U;

		if (ULALARMS & BATTERY_LOW)
		{
			lcd_show(0,'E',0);
			lcd_show(1,'V',16);
			if(ucbatlow == 0)
			{
				prot_log_event(EVT_BATTERY_FAULT,ginstaller_code,0,0);
			}
			ucbatlow = 1U;
			if(ULALARMS & AC_POWER_OFF)
			{
				buzzer(ONE_SEC);
				delay(ONE_SEC+50);
			}
		}
	}
	else
	{
		if (ULALARMS & BATTERY_LOW)
		{
		  lcd_show(0,'E',0);
			lcd_show(1,'V',16);
			if(ucbatlow == 0)
			{
				prot_log_event(EVT_BATTERY_FAULT,ginstaller_code,0,0);
			}
			ucbatlow = 1U;
		}
	}
	
/*
... Check for AC Power & Battery
*/

/*
MainsOff and BatLow are one time flags. They are to log events only once.

*/
	
	if(ucmainsoff == 1)
	{
		if ((ULALARMS & AC_POWER_OFF)==0)
		{
			ucmainsoff = 0;
			lcd_show(1,'V',17);
			prot_log_event(EVT_POWER_RESTORED, ginstaller_code,0, 0);
			buzzer(ONE_SEC);
			delay(ONE_SEC+50);
		}
	}
	if(ucbatlow == 1)
	{
		if((ULALARMS & BATTERY_LOW)==0)
		{
			ucbatlow = 0;
		}
	}
	
		if (ULALARMS & TASK_OVERDUE)
	{
		if(j==0)
		{
			uccount=0;		//For keeping track of number of keys which have exceeded TASK OVERDUE
		}
		for(; j<MAX_KEYS; j++)
		{
			if(KeyTask_TimeLimit[j] == 0xFFFFFFFF)
			{
				uccount++;
				uikey_flag =0;

				lcd_show(0,'E',0);		
				buzzer(800);
				sprintf(szstr,"%d",j+1);
				lcd_show_str(1,'E',5,szstr);
				if (j < 32)
				{
					if((uik1 & (1 << j))==0)
					{
						read_keyevents_sd(j);	
						read_key_sd(j);
						if((key_events_t[j].exist == EXIST) && (key1_t[j].flag))
						{
							
								prot_log_event((s32_t) EVT_TASK_TIMEOUT,(s32_t) key_events_t[j].user,(s32_t) key_events_t[j].task, j+1);	

							
							key1_t[j].flag=0x00;
							write_key_sd();
						}
					}
				}
					

					
				if (j < 32)
				{
					uik1 |= 1 << j;
				}
				
				delay(ONE_SEC);
				j++;
				break;
			}

			
		}

		if((uccount == 0) && (j== MAX_KEYS))
		{
			ULALARMS &= ~TASK_OVERDUE;
		}
		if(j==MAX_KEYS)
		{
			j=0;
		}
	}
	else
	{
		uik1 = 0;
		uik2 = 0;
	}
}
/******************************************************************************
 ** Function name:      Find_card
 **
 ** Descriptions:       This function used to find the correct card  
 **						 from the user 
 ** 
 ** parameters:         ulcard=>card value for the shown RFID.
 **
 ** Returned value:     if the card value matches with the existing shown
 **                     value, return that card value.
 **                     otherwise return '-1'.
 ** 
 ******************************************************************************/
s32_t Find_card(u32_t ulcard)
{
	s32_t i;
	for (i = 0; i < MAX_USERS_LOCATION; i++)
	{

		read_user_sd(i);
		if(user_t.card==ulcard)
		{
			
			greenled_onoff(ON);
			delay(100);
			greenled_onoff(OFF);
		return i;
		}
		if(user_t.user_id ==0xFFFFFFFF)
		{
			red_led_onoff(ON);
			 delay(100);
			 red_led_onoff(OFF);
			return -1;
		}
	}
	return -1;									// Not found or at end of list
}

/******************************************************************************
 ** Function name:      main_menu_administrator
 **
 ** Descriptions:       this function used to explain administrator menu....
 **						the administrator menu contains system setup, diagnostics, utilities
 **						and emergency_keyaccess operation
 ** 
 ** parameters:         none
 **
 ** Returned value:     none
 ** 
 ******************************************************************************/
void main_menu_administrator(void) 
{
	s32_t itemp_key;
    s32_t imenu_i = 1;

	while(1)
    {
    	glogin_user =ADMINISTRATOR;
        lcd_show(0, 'A', 0);                        // Administrator menu
        lcd_show_index(1U, 'A',(u8_t) imenu_i);

        if (get_menu_key(&itemp_key)==0)
        {
        	glogin_user =0;
            return;
        }		
		if (select_menu(&imenu_i, itemp_key,6))		      
        {
        	switch(imenu_i)					
            {
	            case 1: system_setup(); 	    	break;
	            case 2: diagnostics();    			break;
	            case 3: utilities();        		break;
	            case 4: emergency_keyaccess();	break;
							case 5: if(fw_upload_func())
													return;
													break;
						case 6:
										fingerprint=1;
										glcd_refresh_time=0;							
										finger_module_menu();
										break;
	            default:
	            	break;
			}
        }
		if (ULALARMS & DOOR_OPEN_TIMEOUT)
		{
			glogin_user =0;
			return;		
		}				
    }
}


#ifndef TASK_MANAGEMENT 
void main_menu_user(void)
{
	s32_t  itemp_key;
    s32_t  menu_i = 1;
    u8_t  ucfound = 0x00;
	u8_t ucflag_menu=(u8_t) 0;
	u8_t  ucstate;
	char szstr[32];
	u16_t ustimeout;
	u16_t ustest_task;
	u8_t uci,uckey_no;
	s16_t sdlocation;
	u8_t removeflag=(u8_t) 0;
	u8_t returnflag=(u8_t) 0;

	sdlocation=find_user_sd1(user_code_main);
	if(sdlocation!=-1)
	{
		read_user_sd((s32_t) sdlocation);
		
		for(uci=0;uci<MAX_KEYS;uci++)
		{
			if((user_t.user_key>>uci)&0x01)
			{
				ucflag_menu++;
			}
		}
		sprintf(szstr,"\rUser Key cnt:%d-%d",user_t.user_key,ucflag_menu);
		pstr(szstr);
		if(ucflag_menu==0)
		{
			lcd_show(0,'e',3);			//No keys to remove
			lcd_show(1,'e',4);			//or return
			delay(MESSAGE_TIMEOUT);
			return;
		}
		if(ble_user_flg)
		{
			prot_log_event((s32_t) EVT_LOGIN,(s32_t) user_code_main ,0, 0);
			ble_login_success();
		}
	}
	else
	{
		if(ble_user_flg)
		{
			ble_invalid_login();
			ble_user_flg=0;
			lcd_clr(1);
				lcd_show(0, 'S', 2);					// Invalid Login
				Wrong_entry();
				return;
		}
	}
	while(1)
	{
		if(ucflag_menu==1)
		{
			ucflag_menu=0;
			read_user_sd((s32_t) sdlocation);
			for(uci=0;uci<MAX_KEYS;uci++)
			{
				if((user_t.user_key>>uci)&0x01)
				{
					ucflag_menu +=1;
					uckey_no=uci;
				}
			}
			if(ucflag_menu==0)
			{
				lcd_show(0,'e',3);			//No keys to remove
				lcd_show(1,'e',4);			//or return
				delay(MESSAGE_TIMEOUT);
				return;
			}
			if(ucflag_menu==1)
			{
				read_key_sd((s32_t) uckey_no);
				read_keyevents_sd((s32_t) uckey_no);
			    
				sprintf(szstr,"%X-%X-%X-%X",ucstate,key_events_t[uckey_no].status,key1_t[uckey_no].enabled,key1_t[uckey_no].exist);
				pstr(szstr);
				if((key_events_t[uckey_no].status == LOCK_CLOSED) && (key1_t[uckey_no].enabled!=0) )//&& (key1_t[uckey_no].exist == EXIST))
				{
					ucfound=(u8_t) user_mykee(2,user_code_main,uckey_no);
				}
				else if((key_events_t[uckey_no].status == LOCK_OPEN)&& (key1_t[uckey_no].enabled!=0))//&&(key_events_t[uckey_no].exist==EXIST))
				{
					ucfound=(u8_t) user_mykee(0,user_code_main,uckey_no);
				}
				else
				{
					lcd_show(0,'e',5);  // Key disabled/Not sensed 
					lcd_show(1,'e',6);  // Please check with Admin 
					delay(MESSAGE_TIMEOUT);
					return;
				}
	
			}
	
		}
		if(ucflag_menu!=1)
		{
			for (uci = 0; uci < MAX_KEYS; uci++)
			{
				read_key_sd((s32_t) uci);
				read_keyevents_sd((s32_t) uci);
				if (((user_t.user_key>>uci)&0x01) && (key_events_t[uci].status == LOCK_OPEN) && (key1_t[uci].enabled))   // changed for task only for key removal
				{
					returnflag +=1;
				}
				if (((user_t.user_key>>uci)&0x01) && (key_events_t[uci].status == LOCK_CLOSED) && (key1_t[uci].enabled))   // changed for task only for key removal
				{
					removeflag +=1;
				}
			}
			sprintf(szstr,"\rRem:%d,Ret:%d",removeflag,returnflag);
			pstr(szstr);
			if(returnflag && !removeflag)
			{
				ucfound=(u8_t) user_mykee(0,user_code_main,0);
			}
			else
			{
				ucfound=(u8_t) user_mykee(1,user_code_main,0);
			}
		}
		if(ble_user_flg)
		{
			ble_user_flg=0;
			ble_invalid_login();
			}
		if((ucfound == 0)||(ucflag_menu==1))
		{
		
#ifndef NO_DOOR_NO_SOLENOID
			if (trg(2))
			{
				buzzer(ONE_SEC);
				ustimeout = sys_t.close_door_time * 10;
				while(1)
				{
					lcd_show(0, 'D', 1);						// Please close the door
					sprintf(szstr, "%d", (ustimeout + 8) / 10);
					lcd_show_str(1, 'D', 4,szstr);
			
					if (trg(2) == 0)
					{
						return;
					}
					if(key1(100)== ESC)
					{
						ucfound =3U;
						if(ucflag_menu!=1)
						{
							ucflag_menu=0;
							break;
						}
						else
						{
							ULALARMS |= DOOR_OPEN_TIMEOUT;
							ULALARMS |=(u32_t) ENDUSERALARM;
							return;
						}
					}
					ustimeout--;
			
					if (ustimeout == 0)
					{
						lcd_show(0, 'D', 3);					// Close door timeout
						lcd_show(1,'D',1);
						key1(MESSAGE_TIMEOUT);
						ULALARMS |= DOOR_OPEN_TIMEOUT;
						ULALARMS |=(u32_t) ENDUSERALARM;						
						card_id_available = 0;
						buzzer(ONE_SEC);		
						return;
					}
				}
			}
			else
			{
				return;
			}
#endif	//NO_DOOR_NO_SOLENOID
		}
		else if(ucfound ==2)
		{
			return;
		}
		else
		{
			
		}
	}
}

#endif
/******************************************************************************
 ** Function name:      menu_loginuser
 **
 ** Descriptions:       this function helps to seperate  
 **						whether the login user is admin,operator or end user.
 ** 
 ** parameters:         susr_bkp=>login code
 **
 ** Returned value:     none
 ** 
 ******************************************************************************/
void menu_loginuser()
{
	u8_t flg=0;
	prot_log_event((s32_t) EVT_LOGIN,(s32_t) user_code_main ,0, 0);

	
#ifdef TASK_MANAGEMENT 
		main_menu_user_task();
#else		
		main_menu_user();
#endif		
	
	if(!flg)
		prot_log_event((s32_t) EVT_LOGOUT,(s32_t) user_code_main , 0,0);
	else
		flg=0;
	glogin_user=0;
}
s32_t find_user_sd(s32_t user_code)
{
	u16_t i;
	for(i=0;i<MAX_USERS_LOCATION;i++)
	{
		read_user_sd(i);
		if(user_t.card==user_code)
		{			
			return i;
		}
		if(user_t.card==-1)
			return -1;
	}
	return -1;
}
s32_t find_user_sd1(s32_t user_code)
{
	u16_t i;
	for(i=0;i<MAX_USERS_LOCATION;i++)
	{
		read_user_sd(i);
		if(user_t.user_id==user_code)
		{			
			return i;
		}
		if(user_t.user_id ==-1)
			return -1;
	}
	return -1;
}



/******************************************************************************
 ** Function name:      menu_login
 **
 ** Descriptions:       This function used to login for the users for multiple authendication tasks
 **						here the users may be admin, installer, and end user.
 ** 
 ** parameters:         source_keyb=> login code
 **
 ** Returned value:     none
 ** 
 ******************************************************************************/
void menu_login(s32_t source_keyb)
{
		s16_t sdlocation;
    s32_t suser_pin=0;
    char  szstr[32];
	u32_t user_pin,user_pin1;

	read_system_sd();
	get_timestamp(szstr,1);		
	
	if(source_keyb)				// user login by only user code and PIN
	{
		card_id_available = 0;	
        lcd_clr(1);
		if(source_keyb!=0xFFFFFFFF)
			user_code_main=(s16_t) source_keyb;
			if(user_code_main==myRand(Master_Code ,1))
			{
				lcd_str(0,"Master Menu         ");
				lcd_str(1,"Enter Master Pin:");
				user_pin1=myRand(Master_Code,2);
				user_pin=read_code(0xFF);
				if(user_pin!=-1U)
				{
					if( user_pin==user_pin1)
					{
						prot_log_event(EVT_MASTER_MENU_LOGIN,0,0,0);
						main_menu_administrator();
						prot_log_event(EVT_MASTER_MENU_LOGOUT,0,0,0);
					}
					else if(user_pin==243768)
					{
						Serial_no();	
					}
					else
					{
						prot_log_event(EVT_MASTER_MENU_ACC_FAIL,0,0,0);
						lcd_clr(1);
						lcd_show(0, 'S', 2);					// Invalid Login
						Wrong_entry();
						return;
					}
				}
			}
		else 											// for administrator, operator and users
		{
			sdlocation=find_user_sd(user_code_main);
			if(sdlocation!=-1)
			{
				read_user_sd((s32_t) sdlocation);
				if(user_t.finger_enabled==1)
				{
					lcd_show(0,'S',17);
					lcd_show(1, 'S', 8);						 // Not allowed					 
					key1(MESSAGE_TIMEOUT);
					return;
				}
			if(user_t.card==user_code_main)
			{
				glogin_user=END_USER;
				buzzer(500);	
				read_user_sd((s32_t) sdlocation);
				sprintf(szstr,"User Code      : %d",user_code_main);
				lcd_str(0,szstr);
				if(source_keyb==0xFFFFFFFF)
				{
					suser_pin=user_code_pin;
				}
				else
				{
					lcd_show(1, 'S', 1);						//	user pin					
					suser_pin = read_code(HIDE_INPUT);
					if (suser_pin==-1)							// if not entered
					{
						return;
					}
        }           
				if(suser_pin != user_t.user_pin  || suser_pin==0)
				{
					lcd_show(1, 'S', 3); 
					prot_log_event((s32_t) EVT_INVALID_USR_PIN,(s32_t) user_code_main,0,0);
					Wrong_entry();
					ble_invalid_login();
					return;
				}
				if((suser_pin == user_t.user_pin  && user_t.user_pin ))
				{
					if (user_t.enabled == 0)
				    {
				    	lcd_show(0,'S',13);                 //User Disabled
							lcd_show(1,'S', 8);                 // Not allowed
							delay(MESSAGE_TIMEOUT);
							return;
				    }
						user_code_main=user_t.user_id;
						
					menu_loginuser();
					return;
				}
			}
			else
			{
				lcd_clr(1);
				lcd_show(0, 'S', 2);					// Invalid Login
				Wrong_entry();
				
				return;
			}
			}
			else
			{
			pstr("\rInvalid Login");
				lcd_clr(1);
				lcd_show(0, 'S', 2);					// Invalid Login
				Wrong_entry();
				return;
			}
  	}				
	}
	else									// card users  operatiors and users
	{
		sdlocation=Find_card(card_id);
		if (sdlocation != -1)
		 {
   		 card_id=0x00;
		 	 card_id_available =0;
			 lcd_clr(1);
			 lcd_clr(0);
			 delay(100);
			 read_user_sd(sdlocation);
			 lcd_clr(0);
			 user_code_main = user_t.user_id ;			
 			 if (user_t.enabled == 0)
			 {
				 lcd_show(0,'S',13);
				 lcd_show(1, 'S', 8);						 // Not allowed					 
				 key1(MESSAGE_TIMEOUT);
				 return;
			 }	
				if(user_t.finger_enabled==1)
				{
					lcd_show(0,'S',17);
					lcd_show(1, 'S', 8);						 // Not allowed					 
					key1(MESSAGE_TIMEOUT);
					return;
				}
			 	 buzzer(500);
  				glogin_user=END_USER;
				  if(user_t.login_type==0x01) 			// card only users
				 {
						menu_loginuser();
						return;
				 }
				 else						// card and PIN users.
				 {
						lcd_clr(0);
						lcd_show(0, 'S', 1);
						suser_pin = read_code(HIDE_INPUT);
						if (suser_pin==-1)
						{
							return;
						}
						if((suser_pin != user_t.user_pin  ) || suser_pin==0)
						{
							lcd_show(1, 'S', 3); 
							prot_log_event((s32_t) EVT_INVALID_USR_PIN,(s32_t) user_code_main,0, 0);
							Wrong_entry();
							lcd_show(1, 'S', 1);						
							return;
						}
						if ((suser_pin == user_t.user_pin  && user_t.user_pin ))
						{
							menu_loginuser();
							return;
						}
}					 	

		 }			
		 else
		 {
			 lcd_show(0, 'S', 2);					 // "Invalid card" to be shown and re-login.
			 lcd_clr(1);
			 Wrong_entry();
			 if(source_keyb==0xFFFFFFFF)
				ble_invalid_login();
			 return;
		 }
	}
}


/******************************************************************************
 ** Function name:      Wrong_entry
 **
 ** Descriptions:       this occurs when invalid login or wrong time task access
 ** 
 ** parameters:         none
 **
 ** Returned value:     none
 ** 
 ******************************************************************************/
void Wrong_entry(void)
{
	buzzer(500);
	delay(ONE_SEC);
	
	buzzer(ONE_SEC);
	delay(ONE_SEC);
}

void service_menu(void)
{
	lcd_clr(1);
	lcd_str(0,"Service Menu:");
	if (read_code(HIDE_INPUT)!= 1300)
	{
		lcd_str(0,"Service Menu    ");
		lcd_show(1, 'S', 16U);		//Invalid service code
		delay(MESSAGE_TIMEOUT);
		return;
	}
	main_menu_administrator();
}
void alarm_display(u8_t loc)
{
			char szstr[10]="";
			sprintf(szstr,"KEY %d-INSTALLER",loc+1);
			lcd_str(1,szstr);	
			delay(1000);
}
void arrow_right_pressed(u32_t user_pin1)
{
	u32_t user_pin;
	user_pin=myRand(Master_Code ,1);
	if(user_pin1==user_pin)
	{
		pstr("\rFirst Pin Verified");
		lcd_init();
		lcd_str(0U,"Enter MasterCode 2:");
		user_pin1=myRand(Master_Code,2);
		user_pin=read_code(0xFF);
		if(user_pin!=-1U)
		{
			if( user_pin==user_pin1)
			{
				prot_log_event(EVT_MASTER_MENU_LOGIN,0,0,0);
				main_menu_administrator();
				prot_log_event(EVT_MASTER_MENU_LOGOUT,0,0,0);
			}
			else if(user_pin==243768)
			{
				Serial_no();	
			}
			else
			{
				prot_log_event(EVT_MASTER_MENU_ACC_FAIL,0,0,0);
				lcd_clr(1);
				lcd_show(0, 'S', 2);					// Invalid Login
				Wrong_entry();
				return;
			}
		}
	}
	else
	{
				lcd_clr(1);
				lcd_show(0, 'S', 2);					// Invalid Login
				Wrong_entry();
	}
}
u32_t myRand (u32_t next,u8_t func)  // Generate a 4 digit pseudo-random integer
{
	
	endtick_value=get_ticks_value();
	

	if(endtick_value!=-1)
	{
		if(sys_t.serial!=-1)
			next=( (endtick_value/3600) + next + sys_t.serial );
	}
	if(func==1)
		next = ((next*60 ) / 100 ) % 10000 ;
	else if(func==2)
		next = ((next*666 ) / 10000 ) % 1000000 ;

  return next ;
}
 u32_t strbin_to_dec(const char * str) {
    u32_t  val = 0;
	
	
	while (*str != '\0')
		val = 2 * val + (*str++ - '0');
    
    return val;
	}
void ble_user_key_status(void)
{
	u8_t uci;
	u8_t key_status[MAX_KEYS];
		p1char('*');
		send_bledata("+STATUS");
		for(uci=0;uci<MAX_KEYS;uci++)
		{
			read_key_sd((s32_t) uci);
			read_keyevents_sd((s32_t) uci);
			 if((task_t.key[uci])==1)
			 {
				if((key_events_t[uci].status == LOCK_CLOSED) && (key1_t[uci].enabled!=0) )//&& (key1_t[uckey_no].exist == EXIST))
				{
					key_status[uci]=0x01;
				}
				else if((key_events_t[uci].status == LOCK_OPEN ) && (key1_t[uci].enabled!=0) )//&& (key1_t[uckey_no].exist == EXIST))
				{
					key_status[uci]=0x02;
				}
				else
					key_status[uci]=0x00;
			 }
			 else
				key_status[uci]=0x00;
				
			p1char(key_status[uci]);
		}
		p1char('#');
	}
u8_t ble_user_key_no(void)
{
	char szstr[32];
	if(lock_flg2)
		{
			lock_flg2=0;
			buzzer(500);
			ble_key_no 	=ble_protocol_packet_in[6];
			lcd_str(0,szstr);
			delay(1000);
			return ble_key_no;
		}
		return 0;
}

/*---------------------------------------------THE END ---------------------------------------------*/

