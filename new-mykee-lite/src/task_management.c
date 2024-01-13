/*
 * File name:task_management.c
 * 		This file contains the user vs task key removal and return operation.
 *      And also has emergency key taken and return operation for administrator.
 * 
 * 
 * Copyright (c) 2004-2011 Sterna Security.
 * All rights reserved.
 * Author:sterna security.
 */
#include "lpc23xx.H"
#include "menu_main.h"
#include "timer.h"
#include "uart.h"
#include "RFID.h"
#include "LCD.h"
#include "LED.h"
#include "BUZZER.h"
#include "CLOCK_CALNDR.h"
#include "DOOR.h"
/******************************************************************************
 ** Function name:      main_menu_user
 **
 ** Descriptions:       This function for general user login
 **						here the user has rights to remove/return key 
 **						for particular task
 ** 
 ** parameters:         none
 **
 ** Returned value:     none
 ** 
 ******************************************************************************/
void main_menu_user_task(void)
{
    u8_t  ucfound = 0x00;
	u8_t ucflag_menu=(u8_t) 0;
	char szstr[32];
	u16_t ustimeout;
	u16_t ustest_task;
	u8_t uci,uckey_no;

	u32_t sdlocation=0;
	if(ble_user_flg)
	{
	sdlocation=find_user_sd1(user_code_main);
	if(sdlocation!=-1)
	{
		read_user_sd((s32_t) sdlocation);
		

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
}
	for(uci=0;uci<MAX_USER_TASKS;uci++)
	{
			if(user_t.tasks[uci] && user_t.tasks[uci]!=0xFFFF)
			{
				ucflag_menu +=1;
				ustest_task=user_t.tasks[uci];
			}	
	}	
	sprintf(szstr,"\rTask cnt:%d",ucflag_menu);
	pstr(szstr);
	if(ucflag_menu==0)
	{
		lcd_show(0,'e',3);			//no keys to remove
		lcd_show(1,'e',4);			// or return
		delay(MESSAGE_TIMEOUT);
		return;
	}
	while(1)
	{
		if(ucflag_menu==1)
		{
			ucflag_menu=0;
			read_task_status((s32_t) ustest_task);
			for(uci=0;uci<MAX_KEYS;uci++)
			{
				if(task_t.key[uci])
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
			    
				if((key_events_t[uckey_no].status == LOCK_CLOSED) && (key1_t[uckey_no].enabled!=0) && (key1_t[uckey_no].exist == EXIST))
				{
					ucfound=(u8_t) user_mykee_task(2,user_code_main);
				}
				else if((key_events_t[uckey_no].status == LOCK_OPEN)&&(key_events_t[uckey_no].exist==EXIST))
				{
					ucfound=(u8_t) user_mykee_task(0,user_code_main);
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
		if(ucflag_menu !=1)
		{
			if(ucfound!=3)
			{
				ucfound =0xFFU;
			}
#ifdef TRUEKEY32
	for(uci=0;uci<MAX_KEYS;uci++)
	{
		read_keyevents_sd(uci);
		ucstate = lock_state[uci];
		if ((ucstate &(u8_t) LOCK_OPEN)&&(key_events_t[uci].user == user_t.user_id ))
		{
			pstr("\rreturn key");
			return_key=1;
			break;
		}
	}
	if(return_key)
		ucfound=(u8_t) user_mykee_task(0,0);
	else
		ucfound=(u8_t) user_mykee_task(1,0);
#else
		ucfound=(u8_t) user_mykee_task(1,0);		
#endif
		}
		if(ble_user_flg)
		{
			ble_user_flg=0;
			ble_invalid_login();
			}
		if((ucfound == 0)||(ucflag_menu==1))
		{
#ifndef NO_DOOR_NO_SOLENOID
#ifndef TRUEKEY32
			if (trg(2))
#else
			if (trg(1))
#endif			
			{
				buzzer(ONE_SEC);
				ustimeout = sys_t.close_door_time * 10;
				while(1)
				{
					lcd_show(0, 'D', 1);						// Please close the door
					sprintf(szstr, "%d", (ustimeout + 8) / 10);
					lcd_show_str(1, 'D', 4,szstr);
#ifndef TRUEKEY32			
					if (trg(2) == 0)
#else
					if (trg(1) == 0)
#endif					
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
				
						if(TRG_DOOR_CLOSETOUT)
						{
							relay(2,ON);
							Cnt_TriggerTimeout = Set_Trigger_Timeout(ONE_SEC);								
						}
						if(gdashboard_trg & TRG_DOOR_CLOSETOUT)
						{
							prot_log_event((s32_t) EVT_DOOR_CLS_TOUT,(s32_t) user_code_main, 0U,0); //ALARM EVT Logged in the event. MLN
						}
						else
						{
							prot_log_event((s32_t) EVT_DOOR_CLS_TOUT,(s32_t) user_code_main,0U, 0); //ALARM EVT Logged in the event. MLN
						}
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
void clear_keyposition(void)
{
#ifdef IBUTTON
	u8_t uci=0;
	for (uci = 0; uci < MAX_KEYS; uci++)					// Done, so inactivate all LEDs and solenoids
	{
		green_led[uci]=0;
		red_led[uci]=0;
		solenoid[uci]=1;
	}
#else
u8_t uci=0;
	u8_t  ucstate;
	for (uci = 0; uci < MAX_KEYS; uci++)                  // Done, so inactivate all LEDs and solenoids
  {
		ucstate = lock_state[uci];
    ucstate &=(u8_t) ~(LOCK_RED | LOCK_SOL | LOCK_GREEN);
    lock_state[uci] = ucstate;
    lock_key_state[uci]= lock_state[uci];
  }
#endif	
}
/******************************************************************************
 ** Function name:      user_key
 **
 ** Descriptions:       function user_key used for key removal and return by general User.
 **						if the variable  ucremovemode==1-->key removal and multi task,
 **						ucremove_mode ==2 --> one task and one key ,
 **						ucremove_mode == 0 -->key Return
f ** 
 ******************************************************************************/
#ifdef IBUTTON 
s32_t user_mykee_task(u8_t ucremove_mode,u16_t ustask)
{
	s32_t   imenu_i;
	s32_t   itemp_key=0;
	u8_t   var=0;
	u32_t uitimeout;
	u32_t uitimeout1;
	u32_t uik1 =(u32_t) 0;
	u32_t uik2 =(u32_t) 0;
	u16_t user_code;        
	s16_t uci,ucj,uck,uctmp=0;
	s16_t scount;
	char  szstr[50];   
	char *str1 ="";
	char  szstr2[32];
	char  szstr3[32];
	char  szstr4[50]="";
	char  szstr5[5]="";
	u8_t  uctoggle_led = 0;
	u8_t  ucfound =0;
	u16_t  user_task[MAX_USER_TASKS];
	u8_t  ucstate;
	u8_t  ucstatus;	
	u8_t  uclk[MAX_KEYS];
	u8_t task_count=0;
	char szstr1[50];
	s16_t sdlocation;
	u8_t user_key=0;
	u8_t user_key_option[MAX_KEYS];
	u8_t key_entered=0;
	u8_t ui=0;
	char array[3];
	u32_t local_ticks=0;
	u8_t ucm=0;
	u8_t remove_flg=0;
	s32_t iuser_code=0;
#ifndef NO_DOOR_NO_SOLENOID
       relay(1, ON);
		lock_on(1);
    uitimeout =(u32_t)((u32_t) sys_t.open_door_time * 10U);

    while(1)                                        // Wait for the door to be opened
    {
        if (trg(2))
        {
					lock_on(0); 
					if(ble_user_flg)
					ble_door_open();          
					break;
        }

        if (key1(100U)== ESC)
        {
						lock_on(0); 
            relay(1, OFF);
            return 2;
        }
	    	lcd_show(0U, 'D',0U);                            // Please open the door
        sprintf(szstr, "%d", (uitimeout + 8U) / 10U);
        lcd_show_str(1U, 'D', 4U, szstr);

        uitimeout--;

        if (uitimeout==(u32_t)0)
        {
						lock_on(0); 
            relay(1, OFF);
            lcd_show(0U, 'D', 2U);                    // Open door timeout
            key1((u32_t) MESSAGE_TIMEOUT);
						buzzer(1000);
		
			if(((u32_t) TRG_DOOR_OPENTOUT))//RELAY2 login timed out
			{
				relay(2,ON);
				Cnt_TriggerTimeout = Set_Trigger_Timeout((u32_t) ONE_SEC);	// Delay value for 1 sec
			}
			if((gdashboard_trg &(u32_t) TRG_DOOR_OPENTOUT))
			{
			
			}
			else
			{
		
			}

            return 0;
        }
    }
#endif			
	//variable  removemode==1-->key removal and multi task, remove_mode ==2 --> one task and one key ,remove_mode == 0 -->key Return
		if(ble_user_flg)
			ble_user_key_status();
	 if(ucremove_mode)				
	{
		if(ucremove_mode == 1U)
		{
			for(uci=0;uci<MAX_USER_TASKS;uci++)
			{
				user_task[uci]=0U;
			}
			task_count=0;
			/*Here the 800 task is checked by shifting user1.task[] because of stack overflow*/
			for (uci= 0; uci< MAX_USER_TASKS; uci++) // find first usr to display
			{
					if(user_t.tasks[uci] && user_t.tasks[uci]!=0xFFFF)
					{
					  sprintf(szstr1,"%d-Taskid:%d\r",uci,user_t.tasks[uci]);
						pstr(szstr1);
						ustask=user_t.tasks[uci];
						sdlocation=read_task_status((s32_t) ustask);
						if(sdlocation!=-1)
						{
							user_task[task_count]=ustask;
							task_count++;
							read_task_sd(sdlocation);
						  pstr("\r Taskname:");
							pstr(task_t.name);
						}
					}
		
			}	
			sprintf(szstr,"Task count%d\r",task_count);
			pstr(szstr);
			if (!task_count)				// No task for this user
			{
				lcd_show(0U,'V',2U);
				lcd_clr(1U);
				key1((u32_t) MESSAGE_TIMEOUT);
				return 2;
			}
			if(!ble_user_flg)
			{
			read_task_status((s32_t) user_task[0]);
			
			str1 =find_string('V',6);					//Choose task 
			strcpy(szstr3,str1);
			strcpy(szstr2,task_t.name);
			szstr2[16] = '\0';
			if(task_count==1)
			{
				lcd_clr(0);
				lcd_clr(1);
				sprintf(szstr, "%s",szstr2);
				lcd_str(0U, szstr);
			}
			else
			{
				sprintf(szstr, "%-20s* %c",szstr2,0x19);						
			}
			}
			else
			{
				lcd_show(0,'V',6);
				lcd_clr(1);
			}
			imenu_i = 0;
		}												// menu_i for selecting each two digit and j for bit shift 
		uitimeout1=(u32_t)30U;
		ucm=0;
		ble_task_flg=0;
	   while(1)
	    {
				i_button_read=1;
				while(i_button_read);
				delay(10);
				for(uck=0;uck<MAX_KEYS;uck++)
				{
					read_key_sd(uck);
					read_keyevents_sd(uck);
					wrong_unauth_key_check(uck);
					if(key_events_t[uck].status==LOCK_OPEN && (key_events_t[uck].user == user_t.user_id) && ibutton_value[uck][1]==org_key_data[uck][1] && ibutton_value[uck][2]==org_key_data[uck][2] && ibutton_value[uck][3]==org_key_data[uck][3] && ibutton_value[uck][4]==org_key_data[uck][4]&& ibutton_value[uck][5]==org_key_data[uck][5] && ibutton_value[uck][6]==org_key_data[uck][6])
					{
						sprintf(szstr1, "%d", uck + 1);
						green_led[uck]=1;
						red_led[uck]=0;
						uclk[uck] = 1U;
						lcd_clr(0);
						lcd_show_str(0U, 'K', 4U, szstr1);
						delay(500);
						pstr("\rFob x returned");
						update_key_log((s32_t) user_t.user_id, 0x00,(s32_t) uck,(u8_t) LOCK_CLOSED);			
						prot_log_event((s32_t) EVT_KEY_RETURNED,user_t.user_id ,0,uck+1);												
						KeyTask_TimeLimit[uck] = 0U;
						read_key_sd((s32_t) uck);
						if(key1_t[uck].task_removal_time)
						{
							key1_t[uck].task_removal_time=0U;
							key1_t[uck].flag=0U;
							write_key_sd();
						}
						uclk[uck]=1;
						if (uci < 32)
						{
								uik1 |=( 1 << (u32_t)uck);
						}
						else
						{
								uik2 |=( 1 << ((u32_t)uck - 32));
						}
						sprintf(szstr5,"%d,",(uck+1));
						strcat(szstr4,szstr5);	
						scount++;
						pstr("\rReturned");
						buzzer(200);
						ble_user_key_status();
						if(scount==1)
						{
							if((u32_t) TRG_KEY_RETURN)	// trigger for key access
							{
								relay(2,ON);
								Cnt_TriggerTimeout = Set_Trigger_Timeout((u32_t) ONE_SEC);	// Delay value for 1 sec									
							}
						}		                        
						uitimeout =(u32_t) ((u32_t)sys_t.key_turn_time * 10U);
						lcd_clr(0);
						lcd_str(0U,"Please Return Fob");
						
						break;
					}
				}
				if(ucremove_mode==(u8_t)1)
	    	{
					if(!ble_user_flg)
					{
					if(itemp_key == ARROW_D && (imenu_i+1) < task_count)
					{
						read_task_status(user_task[imenu_i]);		
						strcpy(szstr2,task_t.name);
						szstr2[16] = '\0';							
						sprintf(szstr3, "%-20s  %c",szstr2,0x18);						
						imenu_i++;
						ustask=user_task[imenu_i];
						read_task_status((s32_t) ustask);		
						if(imenu_i+1==task_count)
						{
							strcpy(szstr2,task_t.name);
							szstr2[16] = '\0';								
							sprintf(szstr, "%-20s*",szstr2);
						}
						else
						{
							strcpy(szstr2,task_t.name);
							szstr2[16] = '\0';								
							sprintf(szstr, "%-20s* %c",szstr2,0x19);						
						}														
					
				 }
				else if (itemp_key == ARROW_U && imenu_i>0)
				{
						imenu_i--;
						if(imenu_i==0)
						{
							str1 =find_string('V',6);
							strcpy(szstr3,str1);	
						}
						else
						{
								ustask=user_task[imenu_i-1];
								read_task_status((s32_t) ustask);
								strncpy(szstr2, task_t.name, 16U);
								szstr2[16] = '\0';
								sprintf(szstr3, "%-21s %c", szstr2,0x18);					
						}
						ustask=user_task[imenu_i];
						read_task_status((s32_t) ustask);		
						if(imenu_i+1==task_count)
						{
							strcpy(szstr2,task_t.name);
							szstr2[16] = '\0';								
							sprintf(szstr, "%-20s*",szstr2);
						}
						else
						{
							strcpy(szstr2,task_t.name);
							szstr2[16] = '\0';										
							sprintf(szstr, "%-20s* %c",szstr2,0x19);						
						}	
						

				}
			
			}
			uitimeout=0;
			sprintf(szstr1,"keypressed:%d",itemp_key);
			pstr(szstr1);
				for (uci = 0; uci < MAX_KEYS; uci++)
				{
					read_key_sd((s32_t) uci);
					read_keyevents_sd((s32_t) uci);
					if ((task_t.key[uci])==1 && (key1_t[uci].enabled && key1_t[uci].enabled!=0xFF))   // changed for task only for key removal
					{
						if(key_events_t[uci].status ==(u8_t) LOCK_CLOSED)
						{
							green_led[uci]=1;
							uitimeout++;
						}
						else if((key_events_t[uci].status ==(u8_t) LOCK_OPEN)&&(key_events_t[uci].exist==(u32_t) EXIST))
						{
							red_led[uci]=1;
						}
						else
						{
							green_led[uci]=0;
							red_led[uci]=0;
						}
					}
					else
					{
						green_led[uci]=0;
						red_led[uci]=0;
					}					
				}				
				if(!ble_user_flg && task_count>1)
				{
					lcd_str(0U,szstr3);						// it shows previous task list			
					lcd_str(1U, szstr);					// displaycurrent selected task list				
				}
	    }
				
			 if ((itemp_key ==(s32_t) ENTER)||(ucremove_mode==(u8_t)2) || ble_task_flg || (task_count==1 && !ble_user_flg))
	     {
				if(task_count==1)
				{
					delay(500);
					for(uci=0;uci<MAX_KEYS;uci++)
					{
						read_keyevents_sd(uci);
						read_key_sd(uci);
						if (task_t.key[uci]==1 && (key_events_t[uci].status == LOCK_CLOSED) && (key1_t[uci].enabled))   // changed for task only for key removal
						{
							remove_flg +=1;
						}
					}
					if(!remove_flg)
					{
						ucremove_mode=0;
					}
				}
				if(ble_task_flg)
				{
					ustask	=((ble_protocol_packet_in[5]<<8)&0xFF00)|(ble_protocol_packet_in[6]&0xFF);
					read_task_status(ustask);
				}
					ucfound =0U;
					clear_keyposition();
				
				get_timestamp(szstr, 1U);
				local_ticks =get_ticks_value();
				ustask=task_t.task_id;
				ucm=(task_t.dow & (1 << get_doy_dow(0x02,szstr)));
				if (ucm && (task_t.start_date  <=local_ticks) && (task_t.end_date >= local_ticks))
				{
					pstr("\rDOW & Date ok");
					ucfound = 1U;
				}
				if((task_t.access_type & (1 << 7))==0)
				{
					u8_t hr,min,sec;
					u32_t ucfound_1=0;
					hr=RTC_HOUR;
					min=RTC_MIN;
					sec=RTC_SEC;
					ucfound_1=(hr*3600)+(min*60)+sec;
					ucfound_1=get_sec_time_value(local_ticks);
					sprintf(szstr1,"\rcont:%d-%d-%d",ucfound_1,task_t.start_time,task_t.end_time);
					pstr(szstr1);
					if((ucfound_1<task_t.start_time)||(ucfound_1>task_t.end_time))
					{	
						ucfound = 0U;
					}
				}
				if(ucfound==(u8_t) 0)
				{
					pstr("\rTask rejected");
					if((u32_t) TRG_WRONG_TASTTIME) 	//RLY2 TRIGGER WHEN task overdue
					{  		
						relay(2,ON);
						Cnt_TriggerTimeout = Set_Trigger_Timeout((u32_t) ONE_SEC);	// Delay value for 1 sec
					}
					if(gdashboard_trg &(u32_t) TRG_WRONG_TASTTIME) 	
					{
						prot_log_event((s32_t) EVT_TSK_WRNG_TZ,(s32_t) user_code_main,(s32_t) ustask, 0U);
					}
					else
					{
						prot_log_event((s32_t) EVT_TSK_WRNG_TZ,(s32_t) user_code_main,(s32_t) ustask, 0U);
					}

					lcd_show(0U, 'S', 11U);                       // Wrong Task entry
					lcd_clr(1);
					key1((u32_t) MESSAGE_TIMEOUT);
					if(ble_task_flg)
					{
						ble_task_flg=0;
						ble_task_select(0,5);
					}
					return 0;
				}
				else if((uitimeout==(u32_t) 0) && (ucremove_mode==(u8_t)1))
				{
					lcd_show(0U, 'K', 7U);					// No keys to remove
					lcd_clr(1U); 	
					key1((u32_t) MESSAGE_TIMEOUT);
					if(ble_task_flg)
					{
						ble_task_flg=0;
						ble_task_select(0,6);
					}
					return 0xFF;

				}
				else if(task_t.start_date)
				{

					if(compare_date(task_t.start_date,gcurrent_date)==0)			
					{
						if(ble_task_flg)
						{
							buzzer(200);
							ble_task_flg=0;
							ble_task_select(task_t.task_id,0xFF);
						}
						break;
						if(ucremove_mode==2U)
						{
							return 0xAA;
						}
					}
					else if(((compare_date(gcurrent_date,task_t.end_date))==0)&& (task_t.end_date))
					{
						pstr("\rTask Expired");
						lcd_show(0U,'V',3U);                 //Task Expired
						lcd_show(1U,'e',6U);                 //Please check with Admin
						delay((u32_t) MESSAGE_TIMEOUT);
						if(ucremove_mode==2U)
						{
							return 0xAA;
						}
					}
					else
					{
						break;
					}
			
				}
				else
				{
	            	break;
				}
	     }				
				ucm=get_menu_key1(&itemp_key);
				sprintf(szstr1, "\rKey_val:%d Time:%d\r",ucm,uitimeout1);
        pstr(szstr1);
				if (ucm==0)
				{
				pstr("\rdec");
					uitimeout1=uitimeout1-1;
				}
				
				
				
				if(uitimeout1<1 || ucm==0xff)
				{
					clear_keyposition();
						
					 return 0;
				}
#ifndef NO_DOOR_NO_SOLENOID
        if (trg(2)==0)
        {
					clear_keyposition();
						
					 return 0;

        }
#endif
		}
		uitimeout1=0;
		var=0;
		sprintf(szstr,"\rTaskMulti:%d",task_t.multiple);
		pstr(szstr);
		if(task_t.multiple)
		{
			
			for(uci=0;uci<0x05;uci++)
			{
				if((user_code_main!=task_t.user_code[uci])&& (task_t.user_code[uci]))
				{
					gmul_code[uci]=task_t.user_code[uci];
					sprintf(szstr,"\rMul_Task:%d",gmul_code[uci]);
					pstr(szstr);
				}
				else
				{
					gmul_code[uci]=0U;
				}
			}
			
			user_code =	user_t.user_id;
			card_id_available =0;
			
			scount =1;
			uctmp=0;
			uitimeout=30U;			
			card_id_available =0;
			lock_flg=0;
			for(;;)
			{
				u8_t i;
#ifndef NO_DOOR_NO_SOLENOID
        if (trg(2)==0)
        {
					clear_keyposition();
						
					 return 0;

        }
#endif
				uitimeout--;
				if((task_t.multiple-(u8_t) scount)>(u8_t) 1)
				{
					sprintf(szstr,"%d",(task_t.multiple-(u8_t) scount));
					lcd_show_str(0U,'K',10U,szstr);			//Users Remaining
				}
				else
				{
					sprintf(szstr,"%d",(task_t.multiple-(u8_t) scount));
				 	lcd_show_str(0U,'K',11U,szstr);			// User Remaining
				}

				lcd_show(1U,'K',12U);				//Press Enter/ Show RFID
				
				itemp_key =key1((u32_t) ONE_SEC);
				if(!card_id_available)
				{
					if(!(FIO2PIN & 0x00000004))
					{	
						pstr("card reading");
						if(regular_card()==1)
						{				
						}
						
					}
				}

		if(fingerprint==1)
		{
			p1str("verify finger");
			if(verify_identity()==1)
			{
				fingerprint=2;
			}
		}
		if(fingerprint==2)
		{
			iuser_code=getresult_identy();
			if(iuser_code==-3)//Access Denied
			{
				pstr("pin -3\r");
				lcd_str(0,"Access Denied");
				lcd_clr(1);
				buzzer(MESSAGE_TIMEOUT);
				delay(MESSAGE_TIMEOUT);
				fingerprint=1;
			}
			else if(iuser_code==-4)
			{
				pstr("waitfor finger\r");
				fingerprint=2;
			}
			else if(iuser_code!=0)
			{
				fingerprint=1;
				finger_user=1;
				if(check_user_bio(iuser_code))
				{
						//pass finger print flag
					if(multiple_task_access(3)==0)
					{
						uctmp++;
					}
					else
					{
						scount ++;
						uctmp=0;
					}
				}
			}
			else
				fingerprint=1;
		}
				if(itemp_key==ENTER)
				{
					uitimeout=30U;
					if(multiple_task_access(1)==0)
					{
						uctmp++;
					}
					else
					{
						scount++;
						uctmp=0;
					}
					card_id_available=0;
				}
				else if(itemp_key == ESC)
				{
					read_user_status(user_code_main);
					if(task_count==1)
						return 0;
					return 1;
				}
				else if (card_id_available) 					// RFID -> UART
				{
					uitimeout=30U;
					if(multiple_task_access(0)==0)
					{
						uctmp ++;
					}
					else
					{
						scount ++;
						uctmp=0;
					}
					card_id_available =0;
				}
				else if(lock_flg)
				{
					lock_flg=0;
					uitimeout=30U;
					if(multiple_task_access(2)==0)
					{
						uctmp++;
					}
					else
					{
						scount++;
						uctmp=0;
					}
					card_id_available=0;
				}
				else
				{
					
				}
				
				if(uctmp>=5)
				{
					lcd_show(0U,'K',13U);  //maximum entries reached
					lcd_show(1U,'K',14U);  //process cancelled
					delay((u32_t) MESSAGE_TIMEOUT);
					read_user_status(user_code_main);
					return 1;
				}
				if(task_t.multiple==(u8_t) scount)
				{
					break;
				}
				if(uitimeout==(u32_t) 0) 
				{
					read_user_status(user_code_main);
					if(task_count==1)
						return 0;
					return 1 ;
				}
			}
		}
			pstr("\rRead user status");
			read_user_status(user_code_main);
			pstr("\rRead completed");
	}
	else
	{
	    scount = 0;
	    for (uci = 0; uci < MAX_KEYS; uci++)                // Determine number of keys to process
	    {
				ucstate = lock_state[uci];
			
				read_key_sd((s32_t) uci);
				read_keyevents_sd((s32_t) uci);
			
				get_timestamp(szstr, 1U);
				sprintf(szstr1,"\rUser to return key:%d-%d",user_t.user_id,key_events_t[uci].user);
				pstr(szstr1);
        	if((key_events_t[uci].status == LOCK_OPEN)&&((u8_t)(key_events_t[uci].user ==(u16_t) ginstaller_code)))
        	{

/*This function is used for after key event creation only autherised key can be return */				
						for(ucj=0;ucj<MAX_USER_TASKS;ucj++)					//initially find the first task of user
						{
							if(user_t.tasks[ucj])
							{
								ustask=user_t.tasks[ucj];
								read_task_status((s32_t) ustask);
								if((task_t.exist==(u32_t)EXIST) &&((u32_t) (task_t.key[uci])))
								{
									scount++;
								}
							}
							if(scount)
							{
								break;
							}	
						}
					}
					else if ((key_events_t[uci].status ==LOCK_OPEN)&&(key_events_t[uci].user == user_t.user_id ))
					{
						scount++;
						break;
					}
					else
					{
						
					}            
	    }
	    if (scount==0)                                     // No keys, so quit here
	    {
            lcd_show(0U, 'K', 8U);                    // No keys to return
            lcd_clr(1U);
	        key1((u32_t) MESSAGE_TIMEOUT);
	        return 0xFF;
	    }
	}


    for (uci = 0; uci < MAX_KEYS; uci++)                  // Determine which keys to process
    {
        uclk[uci] = 0U;
    }
		user_key=0;
    for (uci = 0; uci < MAX_KEYS; uci++)
    {
			read_key_sd((s32_t) uci);
			read_keyevents_sd((s32_t) uci);
		
		if ((task_t.key[uci])==1 && (ucremove_mode) && (key1_t[uci].enabled && key1_t[uci].enabled!=0xFF))   // changed for task only for key removal
    {
			if(key_events_t[uci].status ==(u8_t) LOCK_CLOSED)
			{
				green_led[uci]=1;
				uclk[uci] = 1U;	
				user_key+=1;
				user_key_option[0]=uci;
			}
			else if ((ibutton_value[uci][0]==0xFF && ibutton_value[uci][0]==0xFF && ibutton_value[uci][0]==0xFF && ibutton_value[uci][0]==0xFF)&&(key_events_t[uci].user == user_t.user_id ))
			{
				red_led[uci]=1;
				uclk[uci] = 2U;
			}
		}
		else if( ucremove_mode==(u8_t) 0 && (key_events_t[uci].user == user_t.user_id ) && key_events_t[uci].status ==(u8_t) LOCK_OPEN)	// implemented  for return the multi user task key by all muti users.
		{
			pstr("\rreturn process");

			if ((key_events_t[uci].status ==(u8_t) LOCK_OPEN)&&(key_events_t[uci].user == user_t.user_id ))
			{
			pstr("\rreturn key");
				red_led[uci]=1;
				uclk[uci] = 2U;
			}
			else
			{
			
			}
		}
		else
		{
		
		}
		pstr("\rloop");
    }
		pstr("\rnext");
		if(user_key==1)
		{
			ucremove_mode=2;
			user_key=0;
		}
    uitimeout =(u32_t)((u32_t) sys_t.key_turn_time * 10U);
    scount = 0;
	uik1=0U;
	uik2=0U;
	ucfound=0U;					// used for highsecurity removed and normal key -key taken with no key number
	ucstatus=0U;
	strcpy(szstr2,"");
	var=4;
			pressed_key=0;
			pstr("\rProcess keys, one by one");
	while(1)                                        // Process keys, one by one
    {
    	if(ULALARMS &(u32_t) SERVICE_DOOR_OPEN)//to handle burglary/srv door open  alarm immediatly
			{
				pstr("service door opened");
				alarm_handler();
			}
		
			if (uitimeout<=(u32_t) 1)
			{
				pstr("key timed out");
				uitimeout=0;
					lcd_show(1U, 'K', 2U);                    // Key handling timeout
					var =0;
					break;
			}
			else
			{
					uitimeout--;
			}
		if(pressed_key)
		{
			pstr("key pressed");
			pressed_key= (char) 0;
			var =3;
			break;
		}
#ifndef NO_DOOR_NO_SOLENOID
        if (trg(2)==0)
        {
					pstr("trg opened");
        	var =2;
            break;		
        }
#endif
		if(var!=4)
		{
			pstr("var!=4");
			break;
		}

        for (uci = 0; uci < MAX_KEYS; uci++)
        {
            if (uclk[uci])
            {
            	break;
            }
        }

        if (uci == MAX_KEYS)
        {
					pstr("var!=0");
        	var=0;
          break;
        }

		if (ucremove_mode==1)
		{
			user_key=0;
			for(uci=0;uci<MAX_KEYS;uci++)
			{
				sprintf(szstr,"\rKey Sta:%d",uclk[uci]);
				pstr(szstr);
				if(uclk[uci]==1)
				{
					green_led[uci]=1;
					user_key +=1;
				}
				else if(uclk[uci]==2)
				{
					red_led[uci]=1;
					user_key +=1;
				}
				else
				{
					green_led[uci]=0;
					red_led[uci]=0;
					}
			}
			if(!user_key)
				return 0;
			else
			{

				}
		 }
		 else if(ucremove_mode==2)
		 {
				uci=user_key_option[0];
				sprintf(szstr,"Single Key no:%d ",uci);
				pstr(szstr);
				key_entered=uci+1;
		 }
		 else
		 {
			 lcd_show(0U, 'K', 6U);					 // Return indicated keys
			 lcd_clr(1);
		 }
					lcd_clr(1);
          uitimeout =(u32_t)((u32_t) sys_t.key_turn_time * 10U);
					pstr("\rEntering into keycheck");
					var=4;
            while (1)
            {
								
								i_button_read=1;
								while(i_button_read);
								delay(10);
							 if(lock_flg1==1)
							 {
									lock_flg1=0;
									clear_keyposition();
									return 0;
							 }
							 
								for(uck=0;uck<MAX_KEYS;uck++)
								{
										read_key_sd(uck);
										read_keyevents_sd(uck);
										if(uclk[uck])
										{
											user_key++;
										}
											
                    
										wrong_unauth_key_check(uck);
										if(key_events_t[uck].status==LOCK_OPEN && (key_events_t[uck].user == user_t.user_id) && ibutton_value[uck][1]==org_key_data[uck][1] && ibutton_value[uck][2]==org_key_data[uck][2] && ibutton_value[uck][3]==org_key_data[uck][3] && ibutton_value[uck][4]==org_key_data[uck][4]&& ibutton_value[uck][5]==org_key_data[uck][5] && ibutton_value[uck][6]==org_key_data[uck][6])
                    {
												sprintf(szstr, "%d", uck + 1);
                        green_led[uck]=1;
												red_led[uck]=0;
                        uclk[uck] = 1U;
												lcd_clr(0);
                        lcd_show_str(0U, 'K', 4U, szstr);
												delay(500);
												pstr("\rFob x returned");
                        update_key_log((s32_t) user_t.user_id, 0x00,(s32_t) uck,(u8_t) LOCK_CLOSED);			
												prot_log_event((s32_t) EVT_KEY_RETURNED,user_t.user_id ,0,uck+1);												
												KeyTask_TimeLimit[uck] = 0U;
												read_key_sd((s32_t) uck);
												if(key1_t[uck].task_removal_time)
												{
													key1_t[uck].task_removal_time=0U;
													key1_t[uck].flag=0U;
													write_key_sd();
												}
												uclk[uck]=1;
                        if (uci < 32)
                        {
                            uik1 |=( 1 << (u32_t)uck);
                        }
                        else
                        {
                            uik2 |=( 1 << ((u32_t)uck - 32));
                        }
                        sprintf(szstr5,"%d,",(uck+1));
												strcat(szstr4,szstr5);	
                        scount++;
												pstr("\rReturned");
												buzzer(200);
												ble_user_key_status();
												if(scount==1)
												{
													if((u32_t) TRG_KEY_RETURN)	// trigger for key access
													{
														relay(2,ON);
														Cnt_TriggerTimeout = Set_Trigger_Timeout((u32_t) ONE_SEC);	// Delay value for 1 sec									
													}
												}		                        
                        uitimeout =(u32_t) ((u32_t)sys_t.key_turn_time * 10U);
												lcd_clr(0);
												lcd_str(0U,"Please Return Fob");
												
                        break;
                    }
								}

                if (uitimeout <=(u32_t) 1)
                {
                	uitimeout=0;
									lcd_clr(0);
                  lcd_show(1U, 'K', 2U);            // Key handling timeout
									clear_keyposition();
									return 0;
                }
                  uitimeout--;
									uitimeout1++;
								
								if(ucremove_mode && !key_entered)
								{
										user_key=0;
										for(uci=0;uci<MAX_KEYS;uci++)
										{
											if(uclk[uci]==1)
											{
												green_led[uci]=1;
												user_key +=1;	
											}
											else if(uclk[uci]==2)
											{
												if(uci)
													red_led[uci]=1;
											}
											else
											{
												green_led[uci]=0;
												red_led[uci]=0;
											}
										}
										if(!user_key)
										{
											clear_keyposition();
											return 0;
										}
										else
										{
											if(ble_user_flg)
											{
											lcd_clr(0);
												lcd_str(0,"Fob Remove/Return");
												if(ble_user_key_no())
												{
													read_keyevents_sd(ble_key_no-1);
													read_key_sd(ble_key_no-1);
													if((task_t.key[ble_key_no-1]) && key_events_t[ble_key_no-1].status==LOCK_CLOSED && (key1_t[ble_key_no-1].enabled && key1_t[ble_key_no-1].enabled!=0xFF))
														key_entered=ble_key_no;
													else
													{
														buzzer(1000);
															lcd_clr(0);
															lcd_str(0,"Invalid Fob Entered");
															delay(1000);
													}
													ble_key_no=0;
													break;
												}

											}
											else
											{
												if(ui==0)
												{
													lcd_clr(0);
													lcd_str(0,"Enter Fob Position:");
												}
												if(uitimeout1>50)
												{
													uitimeout1=0;
													if(ui)
													{
													for(ui=0;ui<3;ui++)
															array[ui]='\0';
														ui=0;
														}
												}
												user_key=0;
												if(pressed_key)
												{
													if(pressed_key==ENTER)
													{
														user_key=atoi(array);
														pressed_key= (char) 0;
														
														for(ui=0;ui<3;ui++)
															array[ui]='\0';
														ui=0;
													}
													else if(pressed_key==ESC)
													{
													clear_keyposition();
													return 0;
													}
													else if (pressed_key == ARROW_L)
													{
															if (ui > 0)
															{
																array[ui-1] = '\0';
																	lcd_delete(1);
																	ui--;
															}
															else
															{
																	//return -1;
															}
															pressed_key= (char) 0;
													}
													else if(pressed_key ==ARROW_U)				//for communication updation using TCP
													{
														//return -2;
														pressed_key= (char) 0;
													}
													else if(pressed_key==ARROW_D)				//for communication updation for FW
													{
														//return -3;
														pressed_key= (char) 0;
													}
													else if(pressed_key==ARROW_R)
														pressed_key= (char) 0;
													else if ((pressed_key >= '0') && (pressed_key <= '9'))
													{
														glcd_x=19+ui;
														lcd_char(pressed_key);
														array[ui]=pressed_key;
														ui++;
														pressed_key= (char) 0;
													}
													sprintf(szstr,"Press:%c",pressed_key);
													pstr(szstr);
													pressed_key= (char) 0;
												}
												sprintf(szstr,"\rUserKey:%d",user_key);
												pstr(szstr);

												if(user_key!=0xFF && user_key!=0 && user_key)
												{
													Enter_flag=0;
													for (uci = 0; uci < MAX_KEYS; uci++)					// Done, so inactivate all LEDs and solenoids
													{
														solenoid[uci]=1;
													}
													
													if((user_key-1)<=MAX_KEYS)
													{
														read_keyevents_sd(user_key-1);
														read_key_sd(user_key-1);
														sprintf(szstr,"\rUserKey:%d-%d",user_key,user_key_option[user_key-1]);
														pstr(szstr);
														if((task_t.key[user_key-1]) && key_events_t[user_key-1].status==LOCK_CLOSED && (key1_t[user_key-1].enabled && key1_t[user_key-1].enabled!=0xFF))// && key_events_t[user_key-1].user == user_t.user_id)
														{
															uci=user_key-1;
															green_led[user_key-1]=1;
															key_entered=user_key;
															break;
														}
														else if(((task_t.key[user_key-1]) && key_events_t[user_key-1].user != user_t.user_id && key_events_t[user_key-1].status==LOCK_OPEN))
														{
															buzzer(1000);
															lcd_clr(0);
															lcd_str(0,"Fob Already Taken");
															delay(1000);
															
															break;
														}
														else
														{
															buzzer(1000);
															lcd_clr(0);
															lcd_str(0,"Invalid Fob Entered");
															delay(1000);
															
														}
													}
													else
													{
														buzzer(1000);
														lcd_clr(0);
														lcd_str(0,"Invalid Fob Entered");
														delay(1000);
														
													}
												}
												if(user_key==253)
												{
													clear_keyposition();
													return 0;
												}
											
											}
											}
								

								}
								
								if (ucremove_mode && key_entered)
								{
										sprintf(szstr, "%d", key_entered);
										lcd_show_str(0U, 'K', 0U, szstr);
								}
								else if(!ucremove_mode)
								{
										lcd_str(0U,"Please Return Fob");
								}
								if(ULALARMS &(u32_t) SERVICE_DOOR_OPEN)//to handle burglary/srv door open  alarm immediatly
								{
									alarm_handler();
								}
								sprintf(szstr,"\rVar:%d",var);
								pstr(szstr);
								if(var!=4)
									break;

                if (ucremove_mode && key_entered)
                {
									uci=key_entered-1;
									solenoid[uci]=0;
									green_led[uci]=1;
										
										if(ibutton_value[key_entered-1][0]==0xFF && ibutton_value[key_entered-1][1]==0xFF && ibutton_value[key_entered-1][2]==0xFF && ibutton_value[key_entered-1][3]==0xFF && key_events_t[key_entered-1].status==LOCK_CLOSED)
                    {
												uci=key_entered-1;
												key_entered=0;
												solenoid[uci]=1;
												green_led[uci]=0;
												red_led[uci]=1;
												sprintf(szstr, "%d", uci + 1);
                        lcd_show_str(0U, 'K', 3U, szstr);
												delay(1000);
												pstr("\rKey taken");
												
                        	update_key_log((s32_t) user_t.user_id ,(s32_t) ustask,(s32_t) uci,(u8_t) LOCK_OPEN);
													read_key_sd((s32_t) uci);
													if(key1_t[uci].enabled==2U)
														prot_log_event((s32_t) EVT_HIGHSEC_REMOVAL,user_t.user_id,(s32_t) ustask,uci+1);
													else
														prot_log_event((s32_t) EVT_KEY_TAKEN,user_t.user_id,(s32_t) ustask,uci+1);
												uclk[uci] = 2U;
												
												if(task_t.time_limit)
												{
													get_timestamp(szstr,1U);
													
													KeyTask_TimeLimit[uci] = endtick_value+(task_t.time_limit*60);	//Convert to seconds
													read_key_sd((s32_t) uci);
													key1_t[uci].task_removal_time=KeyTask_TimeLimit[uci];
													key1_t[uci].flag=1U;
													write_key_sd();
												}
												else
												{
													KeyTask_TimeLimit[uci] = 0U;	//Convert to seconds
												}
                        if (uci < 32)
                        {
                            uik1 |= (1U << (u32_t)uci);
                        }
                        else
                        {
                            uik2 |= (1U << ((u32_t)uci - 32U));
                        }
										sprintf(szstr5,"%d,",(uci+1));
										strcat(szstr4,szstr5);
										scount++;
										ucfound++;
										read_key_sd((s32_t) uci);
										if(key1_t[uci].enabled==2U)
										{
					
											if((u32_t) TRG_HIGHSEC_REMOVE)	//RELAY2 WHEN EMERGENCY KEY ACCESS
											{
												relay(2,ON);
												Cnt_TriggerTimeout = Set_Trigger_Timeout((u32_t) ONE_SEC);	// Delay value for 1 sec
											}

															if(gdashboard_trg &(u32_t) TRG_HIGHSEC_REMOVE)	
											{
												uik1 ^=( 1U << (u32_t)uci);
											}
											else
											{
												uik1 ^=( 1U << (u32_t)uci);
											}
											ucfound--;
										}
	                	if(scount==1)
	                	{
	                		if((u32_t) TRG_KEY_TAKEN)	// trigger for key access
											{
												relay(2,ON);
												Cnt_TriggerTimeout = Set_Trigger_Timeout((u32_t) ONE_SEC);	// Delay value for 1 sec									
											}
										}
										ble_user_key_status();


										uitimeout =(u32_t)((u32_t) sys_t.key_turn_time * 10U);
                        if(ucremove_mode==2)
												{
													clear_keyposition();
													return 0;
												}
                        break;
                    }
                }
                else if(!ucremove_mode)
                {
									user_key=0;
									uitimeout =(u32_t)((u32_t) sys_t.key_turn_time * 10U);
									while(1)
									{
										if (uitimeout <=(u32_t) 1)
										{
											uitimeout=0;
											lcd_show(1U, 'K', 2U);            // Key handling timeout
											break;
										}
										else
										{
											uitimeout--;
										}
										if (!trg(2))
										{
											lock_on(0); 
												break;
										}
										if(ucremove_mode)
											break;
										if(lock_flg1==1)
										 {
										 lock_flg1=0;
											clear_keyposition();
												return 0;
										 }
										if (key1(100U)== ESC)
										{
											clear_keyposition();
											return 0;
										}
										user_key=0;
										i_button_read=1;
											while(i_button_read);
										for(uci=0;uci<MAX_KEYS;uci++)
										{
											if(uclk[uci])
											{
												user_key++;
											}
											sprintf(szstr,"\rUserkeytoreturn:%d",user_key );
											pstr(szstr);
											wrong_unauth_key_check(uci);
									
										if(uclk[uci]==2 && (key_events_t[uci].user == user_t.user_id) && ibutton_value[uci][1]==org_key_data[uci][1] && ibutton_value[uci][2]==org_key_data[uci][2] && ibutton_value[uci][3]==org_key_data[uci][3] && ibutton_value[uci][4]==org_key_data[uci][4] && ibutton_value[uci][5]==org_key_data[uci][5] && ibutton_value[uci][6]==org_key_data[uci][6])
                    {
												sprintf(szstr, "%d", uci + 1);
                        green_led[uci]=1;
												red_led[uci]=0;
                        uclk[uci] = 1U;
                        lcd_show_str(0U, 'K', 4U, szstr);
												delay(1000);
												buzzer(500);
												pstr("\rKey x returned");
												ucremove_mode=1;
                        update_key_log((s32_t) user_t.user_id, 0x00,(s32_t) uci,(u8_t) LOCK_CLOSED);			
												prot_log_event((s32_t) EVT_KEY_RETURNED,user_t.user_id,0,uci+1);												
												KeyTask_TimeLimit[uci] = 0U;
												read_key_sd((s32_t) uci);
												if(key1_t[uci].task_removal_time)
												{
													key1_t[uci].task_removal_time=0U;
													key1_t[uci].flag=0U;
													write_key_sd();
												}
												uclk[uci]=0;
                        if (uci < 32)
                        {
                            uik1 |=( 1 << (u32_t)uci);
                        }
                        else
                        {
                            uik2 |=( 1 << ((u32_t)uci - 32));
                        }
                        sprintf(szstr5,"%d,",(uci+1));
												strcat(szstr4,szstr5);	
                        scount++;
												pstr("\rReturned");
												ble_user_key_status();
												if(scount==1)
												{
													if((u32_t) TRG_KEY_RETURN)	// trigger for key access
													{
														relay(2,ON);
														Cnt_TriggerTimeout = Set_Trigger_Timeout((u32_t) ONE_SEC);	// Delay value for 1 sec									
													}
												}		                        
                        uitimeout =(u32_t) ((u32_t)sys_t.key_turn_time * 10U);
												if(user_key)
												{
													ucremove_mode=1;
													uclk[uci]=1;
												}
                        break;
                    }
										}
										if(ucremove_mode)
										break;
										if(!user_key)
										{
											clear_keyposition();
											return 0;
										}
									}
                }
								
								
								if((uitimeout%1U)==(u32_t) 0)
								{
									uctoggle_led = !(uctoggle_led);           // Blinking LED
								}

                if (ucremove_mode && key_entered)
                {
										if (uctoggle_led)
										{
											green_led[uci]=1;
										}
                    else
                    {
                      green_led[uci]=0;
                    }
                }
                else if(!ucremove_mode)
                {
                    if (uctoggle_led)
                    {
                        red_led[uci]=1;
                    }
                    else
                    {
                        red_led[uci]=0;
                    }
                }

							
							if(trg(2)==0)
							{
								var =2;
								break;	
							}
							if(var!=4)
								break;
							if (key1(10U)== ESC)
							{
								clear_keyposition();
								return 0;
							}
            }
						pstr("\rEnd of while");
			if (key1(100U)== ESC)
			{
				clear_keyposition();
				return 0;
			}
    }
		lock_on(0); 
	relay(1,OFF);
	delay((u32_t) ONE_SEC);
	if(scount==0)
	{

		buzzer(1000);
		if((u32_t) TRG_NOKEY_TAKEN) 	//RLY2 TRIGGER WHEN NO KEY TAKEN
		{	
			relay(2,ON);
			Cnt_TriggerTimeout = Set_Trigger_Timeout((u32_t) ONE_SEC);	// Delay value for 1 sec
		}
		if(gdashboard_trg &(u32_t) TRG_NOKEY_TAKEN) 	
		{
			
		}
		else
		{

		}

			
	}
    if (scount)
    {
        if (ucremove_mode)
        {
        	if(ucfound)
        	{
	        	
		        if(gdashboard_trg &(u32_t) TRG_KEY_TAKEN)
				{	        		
		        }
				else
				{

				}
        	}
        }
        else
        {
           if(gdashboard_trg &(u32_t) TRG_KEY_RETURN)
					{	        		

            }
			else
			{
				
			}
				
        }
    }

    clear_keyposition();
    if (trg(1)==0)
    {
    	var =2;      		
    }
    return var;
}

#else
s32_t user_mykee_task(u8_t ucremove_mode,u16_t ustask)
{
	s32_t   imenu_i;
    s32_t   itemp_key=0;
    u8_t   var=0;
    u32_t uitimeout;
		u32_t uitimeout1;
    u32_t uik1 =(u32_t) 0;
    u32_t uik2 =(u32_t) 0;
	u16_t user_code;      
    s16_t uci,ucj,uck,uctmp=0;
    s16_t scount;
    char  szstr[50];   
	char *str1 ="";
	char  szstr2[32];
	char  szstr3[32];
	char  szstr4[50]="";
	char  szstr5[5]="";
    u8_t  uctoggle_led = 0;
		u8_t  uctoggle_led1 = 0;
	u8_t  ucfound =0;
    u8_t  ucflag_menu=1;
	u16_t  user_task[MAX_USER_TASKS];
    u8_t  ucstate;
	u8_t  ucstatus;	
    u8_t  uclk[MAX_KEYS];
		u8_t task_count=0;
		char szstr1[50];
	char  sms_description[150];
	s16_t sdlocation;
	u8_t user_key=0;
		u8_t user_key_option[MAX_KEYS];
		u8_t key_entered=0;
		u8_t ui=0;
		char array[3];
			u32_t local_ticks=0;
			u8_t ucm=0;
  	u8_t remove_flg=0;
		u8_t remove_key=0;
	u8_t return_key=0;
	s32_t   imenu_i1;
	u8_t remove_return=0;
		
#ifndef NO_DOOR_NO_SOLENOID
       relay(1, ON);
		lock_on(1);
    uitimeout =(u32_t)((u32_t) sys_t.open_door_time * 10U);

    while(1)                                        // Wait for the door to be opened
    {
        if (trg(1))
        {
				lock_on(0); 
				if(ble_user_flg)
						ble_door_open();
          
            break;
        }

        if (key1(100U)== ESC)
        {
						lock_on(0); 
            relay(1, OFF);
            return 2;
        }
		lcd_show(0U, 'D',0U);                            // Please open the door
        sprintf(szstr, "%d", (uitimeout + 8U) / 10U);
        lcd_show_str(1U, 'D', 4U, szstr);

        uitimeout--;

        if (uitimeout==(u32_t)0)
        {
						lock_on(0); 
            relay(1, OFF);
            lcd_show(0U, 'D', 2U);                    // Open door timeout
            key1((u32_t) MESSAGE_TIMEOUT);
			buzzer(1000);
		
			if(((u32_t) TRG_DOOR_OPENTOUT))//RELAY2 login timed out
			{
				relay(2,ON);
				Cnt_TriggerTimeout = Set_Trigger_Timeout((u32_t) ONE_SEC);	// Delay value for 1 sec
			}
			if((gdashboard_trg &(u32_t) TRG_DOOR_OPENTOUT))
			{	
				
			}
			else
			{

			}

            return 0;
        }
    }

#endif			
	//variable  removemode==1-->key removal and multi task, remove_mode ==2 --> one task and one key ,remove_mode == 0 -->key Return
		if(ble_user_flg)
			ble_user_key_status();
			sprintf(szstr,"\rucremove_mode-%d",ucremove_mode);
			pstr(szstr);
	
	if(!ucremove_mode)
	{
	    scount = 0;
	    for (uci = 0; uci < MAX_KEYS; uci++)                // Determine number of keys to process
	    {
			
				read_key_sd((s32_t) uci);
				read_keyevents_sd((s32_t) uci);
			
				get_timestamp(szstr, 1U);
				sprintf(szstr1,"\rUser to return key:%d-%d",user_t.user_id,key_events_t[uci].user);
				pstr(szstr1);
        	if((key_events_t[uci].status == LOCK_OPEN)&&((u8_t)(key_events_t[uci].user ==(u16_t) ginstaller_code)))
        	{

/*This function is used for after key event creation only autherised key can be return */				
						for(ucj=0;ucj<MAX_USER_TASKS;ucj++)					//initially find the first task of user
						{
							if(user_t.tasks[ucj])
							{
								ustask=user_t.tasks[ucj];
								read_task_status((s32_t) ustask);
								if((task_t.exist==(u32_t)EXIST) &&((u32_t) (task_t.key[uci])))
								{
									scount++;
								}
							}
							if(scount)
							{
								break;
							}	
						}
					}
					else if ((key_events_t[uci].status ==LOCK_OPEN)&&(key_events_t[uci].user == user_t.user_id ))
					{
						pstr("\rUser Key");
						scount++;
					}
					else
					{
						//
					}            
	    }
	    if (scount==0)                                     // No keys, so quit here
	    {
            lcd_show(0U, 'K', 8U);                    // No keys to return
            lcd_clr(1U);
	        key1((u32_t) MESSAGE_TIMEOUT);
	        return 0xFF;
	    }
			
		if(scount)
		{
		imenu_i1 = 1;
		while(1)
	{
		lcd_show(0U, 'U',(u8_t) imenu_i1); 			// Administrator menu
		lcd_show(1U, 'U',(u8_t) imenu_i1+2);

		if (get_menu_key(&itemp_key)==0)
		{
			return 0;
		}
		if (select_menu(&imenu_i1, itemp_key, 2))
		{
		 sprintf(szstr,"\rMenu:%d\r",imenu_i1);
					 pstr(szstr);
			switch (imenu_i1)
			{
				case 1:
					ucremove_mode=1;
					remove_return=1;
					break;
				case 2:
					ucremove_mode=0;
					break;
				default:
					break;
			}
			if(imenu_i1)
				break;
		}
		if (ULALARMS &(u32_t) DOOR_OPEN_TIMEOUT)
		{
			return 0;
		}

	}
	
		}
	}
	if(ucremove_mode)				
	{
		if(ucremove_mode == 1U)
		{
			for(uci=0;uci<MAX_USER_TASKS;uci++)
			{
				user_task[uci]=0U;
			}
			task_count=0;
			/*Here the 800 task is checked by shifting user1.task[] because of stack overflow*/
			for (uci= 0; uci< MAX_USER_TASKS; uci++) // find first usr to display
			{
					if(user_t.tasks[uci] && user_t.tasks[uci]!=0xFFFF)
					{
					  sprintf(szstr1,"%d-Taskid:%d\r",uci,user_t.tasks[uci]);
						pstr(szstr1);
						ustask=user_t.tasks[uci];
						sdlocation=read_task_status((s32_t) ustask);
						if(sdlocation!=-1)
						{
							user_task[task_count]=ustask;
							task_count++;
							read_task_sd(sdlocation);
						  pstr("\r Taskname:");
							pstr(task_t.name);
						}
					}
		
			}	
			sprintf(szstr,"Task count%d BLE USerflg %d \r",task_count,ble_user_flg);
			pstr(szstr);
			if (!task_count)				// No task for this user
			{
				lcd_show(0U,'V',2U);
				lcd_clr(1U);
				key1((u32_t) MESSAGE_TIMEOUT);
				return 2;
			}
			if(!ble_user_flg)
			{
			read_task_status((s32_t) user_task[0]);
			
			str1 =find_string('V',6);					//Choose task 
			strcpy(szstr3,str1);
			strcpy(szstr2,task_t.name);
			szstr2[16] = '\0';
			if(task_count==1)
			{
				lcd_clr(0);
				lcd_clr(1);
				sprintf(szstr, "%s",szstr2);
				lcd_str(0U, szstr);
			}
			else
			{
				sprintf(szstr, "%-20s* %c",szstr2,0x19);						
			}
			}
			else
			{
				lcd_show(0,'V',6);
				lcd_clr(1);
			}
			imenu_i = 0;
		}												// menu_i for selecting each two digit and j for bit shift 
		uitimeout1=(u32_t)30U;
		ucm=0;
		ble_task_flg=0;
		itemp_key=0;
	   while(1)
	    {

				if(ucremove_mode==(u8_t)1)
	    	{
					if(!ble_user_flg)
					{
					if(itemp_key == ARROW_D && (imenu_i+1) < task_count)
					{
						read_task_status(user_task[imenu_i]);		
						strcpy(szstr2,task_t.name);
						szstr2[16] = '\0';							
						sprintf(szstr3, "%-20s  %c",szstr2,0x18);						
						imenu_i++;
						ustask=user_task[imenu_i];
						read_task_status((s32_t) ustask);		
						if(imenu_i+1==task_count)
						{
							strcpy(szstr2,task_t.name);
							szstr2[16] = '\0';								
							sprintf(szstr, "%-20s*",szstr2);
						}
						else
						{
							strcpy(szstr2,task_t.name);
							szstr2[16] = '\0';								
							sprintf(szstr, "%-20s* %c",szstr2,0x19);						
						}														
					
				 }
				else if (itemp_key == ARROW_U && imenu_i>0)
				{
						imenu_i--;
						if(imenu_i==0)
						{
							str1 =find_string('V',6);
							strcpy(szstr3,str1);	
						}
						else
						{
								ustask=user_task[imenu_i-1];
								read_task_status((s32_t) ustask);
								strncpy(szstr2, task_t.name, 16U);
								szstr2[16] = '\0';
								sprintf(szstr3, "%-21s %c", szstr2,0x18);					
						}
						ustask=user_task[imenu_i];
						read_task_status((s32_t) ustask);		
						if(imenu_i+1==task_count)
						{
							strcpy(szstr2,task_t.name);
							szstr2[16] = '\0';								
							sprintf(szstr, "%-20s*",szstr2);
						}
						else
						{
							strcpy(szstr2,task_t.name);
							szstr2[16] = '\0';										
							sprintf(szstr, "%-20s* %c",szstr2,0x19);						
						}	
						

				}
			
			}
			uitimeout=0;
			sprintf(szstr1,"keypressed:%d",itemp_key);
			pstr(szstr1);
				for (uci = 0; uci < MAX_KEYS; uci++)
				{
					ucstate =(u8_t) lock_state[uci];
					read_key_sd((s32_t) uci);
					read_keyevents_sd((s32_t) uci);
					sprintf(szstr1,"\rTask key:%d Key Sta%d-%d Ex:%d",task_t.key[uci],key1_t[uci].enabled,key_events_t[uci].status,key_events_t[uci].exist);
					pstr(szstr1);
					if ((task_t.key[uci])==1 && (key1_t[uci].enabled && key1_t[uci].enabled!=0xFF))   // changed for task only for key removal
					{
						if(key_events_t[uci].status ==(u8_t) LOCK_CLOSED)
						{
							{
								ucstate |=(u8_t) LOCK_GREEN;
								uitimeout++;
							}
							
						}
						else if((key_events_t[uci].status ==(u8_t) LOCK_OPEN)&&(key_events_t[uci].exist==(u32_t) EXIST))
						{
							ucstate |=(u8_t) LOCK_RED;
						}
						else
						{
							ucstate &=(u8_t) ~(LOCK_GREEN);
							ucstate &=(u8_t) ~(LOCK_RED);
						}
					}
					else
					{
						ucstate &=(u8_t) ~(LOCK_GREEN);
						ucstate &=(u8_t) ~(LOCK_RED);
					}
					lock_state[uci]=ucstate;
					lock_key_state[uci]= lock_state[uci];
				}
				
				
				if(!ble_user_flg && task_count>1)
				{
					lcd_str(0U,szstr3);						// it shows previous task list			
					lcd_str(1U, szstr);					// displaycurrent selected task list				
				}
	    }
				
			 if ((itemp_key ==(s32_t) ENTER)||(ucremove_mode==(u8_t)2) || ble_task_flg || (task_count==1 && !ble_user_flg))
	     {
				if(task_count==1)
				{
					delay(500);
					for(uci=0;uci<MAX_KEYS;uci++)
					{
						read_keyevents_sd(uci);
						read_key_sd(uci);
						if (task_t.key[uci]==1 && (key_events_t[uci].status == LOCK_CLOSED) && (key1_t[uci].enabled))   // changed for task only for key removal
						{
							remove_flg +=1;
						}
					}
					if(!remove_flg)
					{
						ucremove_mode=0;
					}
				}
				if(ble_task_flg)
				{
					ustask	=((ble_protocol_packet_in[5]<<8)&0xFF00)|(ble_protocol_packet_in[6]&0xFF);
					read_task_status(ustask);
				}
					ucfound =0U;
					clear_keyposition();
				
				get_timestamp(szstr, 1U);
				local_ticks =get_ticks_value();
				ustask=task_t.task_id;
				ucm=(task_t.dow & (1 << get_doy_dow(0x02,szstr)));	
				if (ucm && (task_t.start_date  <=local_ticks) && (task_t.end_date >= local_ticks))
				{
					pstr("\rDOW & Date ok");
					ucfound = 1U;
				}
				if((task_t.access_type & (1 << 7))==0)
				{
					u8_t hr,min,sec;
					u32_t ucfound_1=0;
								hr=RTC_HOUR;
								min=RTC_MIN;
								sec=RTC_SEC;
								ucfound_1=(hr*3600)+(min*60)+sec;
								ucfound_1=get_sec_time_value(local_ticks);
								sprintf(szstr1,"\rcont:%d-%d-%d",ucfound_1,task_t.start_time,task_t.end_time);
								pstr(szstr1);
					if((ucfound_1<task_t.start_time)||(ucfound_1>task_t.end_time))
					{	
						ucfound = 0U;
					}
				}
				if(ucfound==(u8_t) 0)
				{
					pstr("\rTask rejected");
					if((u32_t) TRG_WRONG_TASTTIME) 	//RLY2 TRIGGER WHEN task overdue
					{  		
						relay(2,ON);
						Cnt_TriggerTimeout = Set_Trigger_Timeout((u32_t) ONE_SEC);	// Delay value for 1 sec
					}
					if(gdashboard_trg &(u32_t) TRG_WRONG_TASTTIME) 	
					{
						prot_log_event((s32_t) EVT_TSK_WRNG_TZ,(s32_t) user_code_main,(s32_t) ustask, 0U);
					}
					else
					{
						prot_log_event((s32_t) EVT_TSK_WRNG_TZ,(s32_t) user_code_main,(s32_t) ustask, 0U);
					}

					lcd_show(0U, 'S', 11U);                       // Wrong Task entry
					lcd_clr(1);
					key1((u32_t) MESSAGE_TIMEOUT);
					if(ble_task_flg)
					{
						ble_task_flg=0;
						ble_task_select(0,5);
					}
					return 0;
				}
				else if((uitimeout==(u32_t) 0) && (ucremove_mode==(u8_t)1))
				{
					lcd_show(0U, 'K', 7U);					// No keys to remove
					lcd_clr(1U); 	
					key1((u32_t) MESSAGE_TIMEOUT);
					if(ble_task_flg)
					{
						ble_task_flg=0;
						ble_task_select(0,6);
					}
					return 0xFF;

				}
				else if(task_t.start_date)
				{

					if(compare_date(task_t.start_date,gcurrent_date)==0)			
					{
						if(ble_task_flg)
						{
							buzzer(200);
							ble_task_flg=0;
							ble_task_select(task_t.task_id,0xFF);
						}
						break;
						if(ucremove_mode==2U)
						{
							return 0xAA;
						}
					}
					else if(((compare_date(gcurrent_date,task_t.end_date))==0)&& (task_t.end_date))
					{
						pstr("\rTask Expired");
						lcd_show(0U,'V',3U);                 //Task Expired
						lcd_show(1U,'e',6U);                 //Please check with Admin
						delay((u32_t) MESSAGE_TIMEOUT);
						if(ucremove_mode==2U)
						{
							return 0xAA;
						}
					}
					else
					{
						break;
					}
			
				}
				else
				{
	            	break;
				}
	     }
			 	
				
	    	
				
				ucm=get_menu_key1(&itemp_key);
				sprintf(szstr1, "\rKey_val:%d Time:%d\r",ucm,uitimeout1);
        pstr(szstr1);
				if (ucm==0)
				{
				pstr("\rdec");
					uitimeout1=uitimeout1-1;
				}
				
				
				
				if(uitimeout1<1 || ucm==0xff)
				{
					clear_keyposition();
						
					 return 0;
				}
#ifndef NO_DOOR_NO_SOLENOID
        if (trg(1)==0)
        {
					clear_keyposition();
						
					 return 0;

        }
#endif
		}
		uitimeout1=0;
		var=0;
		sprintf(szstr,"\rTaskMulti:%d",task_t.multiple);
		pstr(szstr);
		if(task_t.multiple)
		{
			
			for(uci=0;uci<0x05;uci++)
			{
				if((user_code_main!=task_t.user_code[uci])&& (task_t.user_code[uci]))
				{
					gmul_code[uci]=task_t.user_code[uci];
					sprintf(szstr,"\rMul_Task:%d",gmul_code[uci]);
					pstr(szstr);
				}
				else
				{
					gmul_code[uci]=0U;
				}
			}
			
			user_code =	user_t.user_id;
			card_id_available =0;
			
			scount =1;
			uctmp=0;
			uitimeout=30U;			
			card_id_available =0;
			lock_flg=0;
			for(;;)
			{
				u8_t i;		
#ifndef NO_DOOR_NO_SOLENOID
        if (trg(1)==0)
        {
					clear_keyposition();
						
					 return 0;

        }
#endif
				uitimeout--;
				if((task_t.multiple-(u8_t) scount)>(u8_t) 1)
				{
					sprintf(szstr,"%d",(task_t.multiple-(u8_t) scount));
					lcd_show_str(0U,'K',10U,szstr);			//Users Remaining
				}
				else
				{
					sprintf(szstr,"%d",(task_t.multiple-(u8_t) scount));
				 	lcd_show_str(0U,'K',11U,szstr);			// User Remaining
				}

				lcd_show(1U,'K',12U);				//Press Enter/ Show RFID
				
				itemp_key =key1((u32_t) ONE_SEC);
				if(!card_id_available)
				{
					if(!(FIO2PIN & 0x00000004))
					{	
						pstr("card reading");
						if(regular_card()==1)
						{				
						}
						
					}
				}

				
				if(itemp_key==ENTER)
				{
					uitimeout=30U;
					if(multiple_task_access(1)==0)
					{
						uctmp++;
					}
					else
					{
						scount++;
						uctmp=0;
					}
					card_id_available=0;
				}
				else if(itemp_key == ESC)
				{
					read_user_status(user_code_main);
					if(task_count==1)
						return 0;
					return 1;
				}
				else if (card_id_available) 					// RFID -> UART
				{
					uitimeout=30U;
						if(multiple_task_access(0)==0)
						{
							uctmp ++;
						}
						else
						{
							scount ++;
							uctmp=0;
						}
					card_id_available =0;
				}
				else if(lock_flg)
				{
					lock_flg=0;
					uitimeout=30U;
					if(multiple_task_access(2)==0)
					{
						uctmp++;
					}
					else
					{
						scount++;
						uctmp=0;
					}
					card_id_available=0;
				}
				else
				{

				}
				
				if(uctmp>=5)
				{
					lcd_show(0U,'K',13U);  //maximum entries reached
					lcd_show(1U,'K',14U);  //process cancelled
					delay((u32_t) MESSAGE_TIMEOUT);
					read_user_status(user_code_main);
					return 1;
				}
				if(task_t.multiple==(u8_t) scount)
				{
					break;
				}
				if(uitimeout==(u32_t) 0) 
				{
					read_user_status(user_code_main);
					if(task_count==1)
						return 0;
					return 1 ;
				}
			}
		}
			pstr("\rRead user status");
			read_user_status(user_code_main);
			pstr("\rRead completed");
	}
	

    for (uci = 0; uci < MAX_KEYS; uci++)                  // Determine which keys to process
    {
        uclk[uci] = 0U;
    }
		user_key=0;

    for (uci = 0; uci < MAX_KEYS; uci++)
    {
#ifndef IBUTTON
			ucstate = lock_state[uci];
			ucstate &=(u8_t) ~(LOCK_GREEN | LOCK_RED | LOCK_SOL);
#endif		
			read_key_sd((s32_t) uci);
			read_keyevents_sd((s32_t) uci);

		sprintf(szstr1,"\rTask key:%d, %d-%d-%02X",uci,task_t.key[uci],key1_t[uci].enabled,key_events_t[uci].status);
		pstr(szstr1);
		if ((task_t.key[uci])==1 && (ucremove_mode) && (key1_t[uci].enabled && key1_t[uci].enabled!=0xFF))   // changed for task only for key removal
    {
			if((ucstate != (u8_t)LOCK_OPEN)&&(key_events_t[uci].status ==(u8_t) LOCK_CLOSED))
			{
				ucstate |=(u8_t) LOCK_GREEN;
				uclk[uci] = 1U;	
				user_key_option[0]=uci;
				remove_key++;
			}
			else if((ucstate &(u8_t) LOCK_OPEN)&&(key_events_t[uci].exist==(u32_t) EXIST)&&(key_events_t[uci].user == user_t.user_id ))
			{

			}
		}
		else if( ucremove_mode==(u8_t) 0 && (key_events_t[uci].user == user_t.user_id ) && key_events_t[uci].status ==(u8_t) LOCK_OPEN)	// implemented  for return the multi user task key by all muti users.
		{
			pstr("\rreturn process");
			
			if ((ucstate &(u8_t) LOCK_OPEN)&&(key_events_t[uci].user == user_t.user_id ))
			{
			ucstate |=(u8_t) LOCK_RED;
				uclk[uci] = 2U;
				return_key++;
			}
			else
			{
		
			}
		}
		else
		{
	
		}
		lock_state[uci] = ucstate;
    lock_key_state[uci]= lock_state[uci];	
		pstr("\rloop");
    }
		pstr("\rnext");
		if(user_key==1)
		{
			ucremove_mode=2;
			user_key=0;
		}
    uitimeout =(u32_t)((u32_t) sys_t.key_turn_time * 10U);
    scount = 0;
	uik1=0U;
	uik2=0U;
	ucfound=0U;					// used for highsecurity removed and normal key -key taken with no key number
	ucstatus=0U;
	strcpy(szstr2,"");
	var=4;
			pressed_key=0;
			pstr("\rProcess keys, one by one");
	while(1)                                        // Process keys, one by one
    {
		
			if (uitimeout<=(u32_t) 1)
			{
				uitimeout=0;
					lcd_show(1U, 'K', 2U);                    // Key handling timeout
					var =0;
					break;
			}
			else
			{
					uitimeout--;
			}
		if(pressed_key)
		{
			pressed_key= (char) 0;
			var =3;
			break;
		}
#ifndef NO_DOOR_NO_SOLENOID
        if (trg(1)==0)
        {
        	var =2;
            break;		
        }
#endif
		if(var!=4)
			break;

        for (uci = 0; uci < MAX_KEYS; uci++)
        {
            if (uclk[uci])
            {
            	break;
            }
        }

        if (uci == MAX_KEYS)
        {
        	var=0;
          break;
        }

				if(ucremove_mode)
					lcd_str(0,"Please Remove Keys ");	
				else if(!ucremove_mode)
					lcd_str(0,"Please Return Keys ");	
				else
					lcd_show(0U, 'K', 5U);
				
				sprintf(szstr, "%d", (uitimeout + 8U) / 10U);
				lcd_show_str(1U, 'D', 4U, szstr);
				
				for (uci =(s16_t) 0; uci <(s16_t) MAX_KEYS; uci++)
       {
					ucstate =(u8_t) lock_state[uci];
					ucstate &=(u8_t) ~(LOCK_SOL);
					lock_state[uci] = ucstate;
					lock_key_state[uci]= lock_state[uci];	 
					if ((ucstate &(u8_t) (LOCK_OPEN | LOCK_CLOSED))==(u8_t)0)
					{
							if (uclk[uci])                          // In between state so user is turning the key
							{
									break;
							}
							else
							{
							}
					}
        }
        if (uci <(s16_t) MAX_KEYS)                           // Key being turned?
        {
					
					pstr("\rKey Turned");
        	ucstate =(u8_t) lock_state[uci];
        	ucstate |=(u8_t) LOCK_SOL;
        	lock_state[uci] = ucstate;
        	lock_key_state[uci]= lock_state[uci];	        	
					uitimeout =(u32_t) ((u32_t)sys_t.key_turn_time * 3160U);
					sprintf(szstr, "%d", uci + 1);	

					pstr("\rEntering into keycheck");
					var=4;
            while (1)
            {
								if (ucremove_mode)
								{
									lcd_show_str(0U, 'K', 0U, szstr);//Take key %s
								}
								else
								{
									lcd_show_str(0U, 'K', 1U, szstr);//Return key %s
								}
							
									lcd_clr(1);
								ucstate = lock_state[uci];
							 if(lock_flg1==1)
							 {
									lock_flg1=0;
									clear_keyposition();
									return 0;
							 }
							 

								if (uitimeout <=(u32_t) 1)
                {
                	uitimeout=0;
									lcd_clr(0);
                  lcd_show(1U, 'K', 2U);            // Key handling timeout
									clear_keyposition();
                  break;
                }
                else
                {
									pstr("\rdec");
                  uitimeout--;	
                }
								if (ucremove_mode)//uclk[uci]==1)
                {
									pstr("\rucremove_mode=1");
									if(ucstate &(u8_t) LOCK_CLOSED && uclk[uci])	
									{
                        ucstate &=(u8_t) ~(LOCK_RED);
                        ucstate |=(u8_t) LOCK_GREEN;
                        lock_state[uci] = ucstate;
                        lock_key_state[uci]= lock_state[uci];
                        uitimeout =(u32_t)((u32_t) sys_t.key_turn_time * 10U);
                        uclk[uci] = 1U;
                        break;
									}
                    if (ucstate &(u8_t) LOCK_OPEN)
                    {
                        ucstate &=(u8_t) ~(LOCK_GREEN);
                        ucstate |=(u8_t) LOCK_RED;
												ucstate &=(u8_t) ~(LOCK_SOL);
                        lock_state[uci] = ucstate;
                        lock_key_state[uci]= lock_state[uci];
												sprintf(szstr, "%d", uci + 1U);
                        lcd_show_str(0U, 'K', 3U, szstr);// Key x taken
												if(remove_key)
													remove_key--;
						{
													update_key_log((s32_t) user_t.user_id ,(s32_t) ustask,(s32_t) uci,(u8_t) LOCK_OPEN);
                        	read_key_sd((s32_t) uci);
													if(key1_t[uci].enabled==2U)
														prot_log_event((s32_t) EVT_HIGHSEC_REMOVAL,user_t.user_id,(s32_t) ustask,uci+1);
													else
														prot_log_event((s32_t) EVT_KEY_TAKEN,user_t.user_id,(s32_t) ustask,uci+1);
												uclk[uci] = 0;
						}
						if(task_t.time_limit)
						{
							get_timestamp(szstr,1U);
							
							KeyTask_TimeLimit[uci] = endtick_value+(task_t.time_limit*60);	//Convert to seconds
							read_key_sd((s32_t) uci);
							key1_t[uci].task_removal_time=KeyTask_TimeLimit[uci];
							key1_t[uci].flag=1U;
							write_key_sd();
						}
						else
						{
							KeyTask_TimeLimit[uci] = 0U;	//Convert to seconds
						}

                        if (uci < 32)
                        {
                            uik1 |= (1U << (u32_t)uci);
                        }
                        else
                        {
                            uik2 |= (1U << ((u32_t)uci - 32U));
                        }
						sprintf(szstr5,"%d,",(uci+1));
						strcat(szstr4,szstr5);
                        scount++;
						ucfound++;
						
						read_key_sd((s32_t) uci);
						if(key1_t[uci].enabled==2U)
						{
							if((u32_t) TRG_HIGHSEC_REMOVE)	//RELAY2 WHEN EMERGENCY KEY ACCESS
							{
								relay(2,ON);
								Cnt_TriggerTimeout = Set_Trigger_Timeout((u32_t) ONE_SEC);	// Delay value for 1 sec
							}

	           	if(gdashboard_trg &(u32_t) TRG_HIGHSEC_REMOVE)	
							{
								prot_log_event((s32_t) EVT_HIGHSEC_REMOVAL,(s32_t) user_code_main,(s32_t) ustask,(uik1 &(1 <<(u32_t) uci)));	
								uik1 ^=( 1U << (u32_t)uci);
							}
							else
							{
								prot_log_event((s32_t) EVT_HIGHSEC_REMOVAL,(s32_t) user_code_main,(s32_t) ustask,(uik1 &(1 <<(u32_t) uci)));																	
								uik1 ^=( 1U << (u32_t)uci);
							}
							ucfound--;
						}
						if(scount==1)
						{
							if((u32_t) TRG_KEY_TAKEN)	// trigger for key access
							{
								relay(2,ON);
								Cnt_TriggerTimeout = Set_Trigger_Timeout((u32_t) ONE_SEC);	// Delay value for 1 sec
							}
						}
                        uitimeout =(u32_t)((u32_t) sys_t.key_turn_time * 10U);
												sprintf(szstr1,"\rremove_key:%d,return_key:%d",remove_key,return_key);
												pstr(szstr1);
												if(!remove_key)
												{
													clear_keyposition();
													return 0;
												}
												return_key++;
                        break;
                    }
                }
                else
                {
										pstr("\r ucremove_mode=0");
										if(ucstate &(u8_t) LOCK_OPEN && uclk[uci]==2)
										{
                        ucstate &=(u8_t) ~(LOCK_GREEN);
                        ucstate |=(u8_t) LOCK_RED;
                        lock_state[uci] = ucstate;
                        lock_key_state[uci]= lock_state[uci];
                        uitimeout =(u32_t)((u32_t)sys_t.key_turn_time * 10U);
                        break;							

										}	                
                    if (ucstate &(u8_t) LOCK_CLOSED)
                    {
                        ucstate &=(u8_t) ~(LOCK_RED);
                        ucstate |=(u8_t) LOCK_GREEN;
												ucstate &=(u8_t) ~(LOCK_SOL);
                        lock_state[uci] = ucstate;
                        lock_key_state[uci]= lock_state[uci];
                        uclk[uci] = 0U;
												sprintf(szstr, "%d", uci + 1U);
                        lcd_show_str(0U, 'K', 4U, szstr);// Key x returned
												if(return_key)
													return_key--;
                        update_key_log((s32_t) user_t.user_id, 0x00,(s32_t) uci,(u8_t) LOCK_CLOSED);		
													prot_log_event((s32_t) EVT_KEY_RETURNED,user_t.user_id,0,uci+1);		
						
						KeyTask_TimeLimit[uci] = 0U;
						read_key_sd((s32_t) uci);
						if(key1_t[uci].task_removal_time)
						{
							key1_t[uci].task_removal_time=0U;
							key1_t[uci].flag=0U;
							write_key_sd();
						}
           
                        if (uci < 32)
                        {
                            uik1 |=( 1 << (u32_t)uci);
                        }
                        else
                        {
                            uik2 |=( 1 << ((u32_t)uci - 32));
                        }
                        sprintf(szstr5,"%d,",(uci+1));
						strcat(szstr4,szstr5);	
                        scount++;
	                	if(scount==1)
	                	{
	                		if((u32_t) TRG_KEY_RETURN)	// trigger for key access
							{
								relay(2,ON);
								Cnt_TriggerTimeout = Set_Trigger_Timeout((u32_t) ONE_SEC);	// Delay value for 1 sec									
							}

						}		                        
						sprintf(szstr1,"\rremove_key:%d,return_key:%d",remove_key,return_key);
						pstr(szstr1);
									if(!return_key)
												{
												clear_keyposition();
												return 0;
												}
                        uitimeout =(u32_t) ((u32_t)sys_t.key_turn_time * 10U);
                        break;
                    }
                }
								if(var!=4)
									break;
                	
								if((uitimeout%100U)==(u32_t) 0)
								{
									uctoggle_led = !(uctoggle_led);           // Blinking LED
								}
								if (uclk[uci]==1)
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
                else if(uclk[uci]==2)
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
                lock_state[uci] = ucstate;
								lock_key_state[uci]= lock_state[uci];

								if(pressed_key)
								{
									pressed_key= (char) 0;
									var =3;
									break;
								}

							if(trg(1)==0)
							{
								var =2;
								break;	
							}
							if(var!=4)
								break;
							if (key1(10U)== ESC)
							{
								clear_keyposition();
								return 0;
							}
            }
						pstr("\rEnd of while");
						ucstate &=(u8_t) ~(LOCK_SOL);
					lock_state[uci] = ucstate;
          lock_key_state[uci]= lock_state[uci];
				}
			if (key1(100U)== ESC)
			{
				clear_keyposition();
				return 0;
			}
    }
		lock_on(0); 
	relay(1,OFF);
	delay((u32_t) ONE_SEC);
	if(scount==0)
	{

		buzzer(1000);
		if((u32_t) TRG_NOKEY_TAKEN) 	//RLY2 TRIGGER WHEN NO KEY TAKEN
		{	
			relay(2,ON);
			Cnt_TriggerTimeout = Set_Trigger_Timeout((u32_t) ONE_SEC);	// Delay value for 1 sec
		}
		if(gdashboard_trg &(u32_t) TRG_NOKEY_TAKEN) 	
		{

		}
		else
		{

		}

			
	}
    if (scount)
    {
        if (ucremove_mode)
        {
        	if(ucfound)
        	{
	        	
		        if(gdashboard_trg &(u32_t) TRG_KEY_TAKEN)
				{	        		
		        }
				else
				{

				}
        	}
        }
        else
        {
           if(gdashboard_trg &(u32_t) TRG_KEY_RETURN)
					{	        		
            }
			else
			{

			}
				
        }
    }

    clear_keyposition();
    if (trg(0)==0)
    {
    	var =2;      		
    }
    return var;
}


#endif
/******************************************************************************
 ** Function name:      multiple_task_access
 **
 ** Descriptions:       this function is used for check the multiple tasks for user
 ** 
 ** parameters:         ucsource_keyb => user code for checking the multiple tasks
 **
 ** Returned value:     if valid login, return '1'. else return '0'.
 ** 
 ******************************************************************************/
s32_t multiple_task_access(u8_t ucsource_keyb)
{
    s16_t suser_code=0;	
    s16_t suser_pin=0;
    char  szstr[10];
	u32_t Show_Card_Timeout =(u32_t) 0;
	u8_t uccounter,uci;	
	char szstr1[32];

	
	s16_t sdlocation,location_temp;
#ifdef BIOMETRIC	
	if(ucsource_keyb==3)//finger print
	{
		sprintf(szstr,"Fusercode:%d\r",user_t.user_id);
		p1str(szstr);
		suser_code = user_t.user_id ;
		for(uci=0;uci<0x05;uci++)
		{
			if(suser_code == gmul_code[uci])
			{
				uccounter=1U;
				gmul_code[uci]=0x0000;
				break;
			}
			else
			{
				uccounter =0;
			}
				
		}
		if(uccounter)
		{
			prot_log_event((s32_t) EVT_MULTIPLE_TASK,(s32_t) user_t.user_id , 0, 0);
			return 1;
		}
		else
		{
			lcd_show(0,'S',2);										//Invalid login code
			delay(MESSAGE_TIMEOUT);
			return 0;
		}
	}
	else 
#endif		
	if(ucsource_keyb==1)					// user code and pin user	
  {
		card_id_available = 0;	
		lcd_clr(1);		
		lcd_show(0, 'S', 0);			// Enter User code		
		suser_code =(s16_t) read_code(SHOW_INPUT);
		if (suser_code ==-1 || suser_code==0)
		{
			lcd_show(0,'S',2); 			//Invalid login code
			delay(MESSAGE_TIMEOUT);
			return 0;
		}
		sdlocation=find_user_sd(suser_code);
		if(sdlocation!=-1)
		{
			read_user_sd((s32_t) sdlocation);
			suser_code=user_t.user_id ;
			if(user_t.finger_enabled==1)
			{
				lcd_show(0,'S',17);
				lcd_show(1, 'S', 8);						 // Not allowed					 
				key1(MESSAGE_TIMEOUT);
				return 0;
			}
		}
// cheking the read user code with temp usercode array		
		for(uci=0;uci<0x05;uci++)
		{
		sprintf(szstr1,"\rMul:%d-%d",suser_code,gmul_code[uci]);
			pstr(szstr1);
			if(suser_code == gmul_code[uci])
			{
				uccounter =1U;			// if user found break from for loop
				break;
			}
			else
			{
				uccounter =0;
			}				
		}
		
		if(uccounter == 0)					// if not correct code return to prev menu
		{
			lcd_show(0,'S',2);			//Invalid login code
			lcd_clr(1);
			delay(MESSAGE_TIMEOUT);
			return 0;
		}		
   }
	 else if(ucsource_keyb==2)
	 {
			lock_flg=0;
			buzzer(500);
			suser_code	=((ble_protocol_packet_in[6]<<24)&0xFF000000)|((ble_protocol_packet_in[7]<<16)&0xFF0000)|((ble_protocol_packet_in[8]<<8)&0xFF00)|(ble_protocol_packet_in[9]&0xFF);
			suser_pin 	=((ble_protocol_packet_in[10]<<24)&0xFF000000)|((ble_protocol_packet_in[11]<<16)&0xFF0000)|((ble_protocol_packet_in[12]<<8)&0xFF00)|(ble_protocol_packet_in[13]&0xFF);
			sprintf(szstr1,"BU:%d-%d",suser_code,suser_pin );
			pstr(szstr1);
		  if (suser_code ==-1 || suser_code==0)
      {
				lcd_show(0,'S',2); 			//Invalid login code
				delay(MESSAGE_TIMEOUT);
				return 0;
      }
       sdlocation=find_user_sd1(suser_code);
			if(sdlocation!=-1)
			{
				read_user_sd((s32_t) sdlocation);
				if(user_t.finger_enabled==1)
				{
					lcd_show(0,'S',17);
					lcd_show(1, 'S', 8);						 // Not allowed					 
					key1(MESSAGE_TIMEOUT);
					return 0;
				}
			}
// cheking the read user code with temp usercode array		
		for(uci=0;uci<0x05;uci++)
		{
			sprintf(szstr1,"\rMul:%d-%d",suser_code,gmul_code[uci]);
			pstr(szstr1);
			if(suser_code == gmul_code[uci])
			{
				uccounter =1U;			// if user found break from for loop
				break;
			}
			else
			{
				uccounter =0;
			}				
		}		
		if(uccounter == 0)					// if not correct code return to prev menu
		{
			lcd_show(0,'S',2);			//Invalid login code
			lcd_clr(1);
			delay(MESSAGE_TIMEOUT);
			return 0;
		}	
	 }
	else 								// login by card and pin user
	{
		if (Find_card(card_id) != -1)		
		{
			card_id=0x00;
			lcd_clr(1);
			suser_code = user_t.user_id ;
			if(user_t.login_type)				// if card only user
			{
				for(uci=0;uci<0x05;uci++)
				{
					if(suser_code == gmul_code[uci])
					{
						uccounter=1U;
						gmul_code[uci]=0x0000;
						break;
					}
					else
					{
						uccounter =0;
					}
						
				}
				if(uccounter)
				{
					if(( TRG_LOGIN_MULTIPLE))						//RELAY2 login multiple user
					{							
						relay(2,ON);
						Cnt_TriggerTimeout = Set_Trigger_Timeout(ONE_SEC);	// Delay value for 1 sec
					}
					if((gdashboard_trg & TRG_LOGIN_MULTIPLE))				// Dashboard event
					{
						prot_log_event((s32_t) EVT_MULTIPLE_TASK,(s32_t) user_t.user_id , 0, 0);	
							
					}
					else
					{
						prot_log_event((s32_t) EVT_MULTIPLE_TASK,(s32_t) user_t.user_id , 0, 0);
					}
				
					return 1;
				}
				else
				{
					lcd_show(0,'S',2);										//Invalid login code
					delay(MESSAGE_TIMEOUT);
					return 0;
				}
			}
		}	
		else
		{
			card_id=0x00;        
			lcd_show(0, 'S', 2);                    // Invalid Login
			lcd_clr(1);
			delay(MESSAGE_TIMEOUT);
			return 0;
		}
	}
	sdlocation=read_user_status(suser_code);
	if((user_t.login_type!=4 && user_t.login_type!=2) && (user_t.user_id  !=gadmin_code))	
	{
		lcd_show(0,'V',2);						//Login By Card Only
		lcd_clr(1);
		delay(ONE_SEC);
		return 0;
	}	
		if(user_t.card && ucsource_keyb && user_t.login_type==0x05)                         // if card assigned , user must show the card first
		{
				glogin_user=END_USER;      // this line added to avoid,the coming LCD sentence should display
																	 //enduser language when the enduser language changed other from english
																		
				lcd_show(0,'V',4);         //User have RFID
				lcd_show(1,'V',5);         //Please Show it first
				Wrong_entry();
				return 0;
		}
		if(user_t.login_type!=2)
		{
			sprintf(szstr,"User Code      : %d",user_t.card);
			lcd_str(0,szstr);		//User code      :
			lcd_show(1, 'S', 1);					//Enter user PIN :
			buzzer(500);	
			lcd_show(1, 'S', 1);						//	user pin					
		}
		if(suser_code ==gadmin_code)
		{
			suser_pin =(s16_t) read_code(0xFF);			//for 6 digit pin numbers
		}
		else if(suser_pin)
		{
			ble_invalid_login();
		}
		else
		{
			suser_pin =(s16_t) read_code(HIDE_INPUT);
		}
		if (suser_pin==-1)							// if not entered
		{
			return 0;
		}		
		if(suser_pin != user_t.user_pin )
		{
				lcd_show(1, 'S', 3); 
				Wrong_entry();
				return 0;
		}
		if ((suser_pin != user_t.user_pin )||(suser_pin==-1) || (suser_pin==0))			// If pin not matched
		{
			lcd_show(1, 'S', 3); 				//Invalid PIN			
			delay(ONE_SEC);
			return 0;			
		}		
		if ((user_t.card) && (ucsource_keyb) && user_t.login_type==0x05)		//Login By Card and PIN
		{
			lcd_show(0, 'S', 10);				// Show the card
			lcd_clr(1);
			Show_Card_Timeout = set_timeout(5000);
			while((card_id_available == 0) ||(chk_timeout(Show_Card_Timeout)==0))
			{
                
			}
			if((chk_timeout(Show_Card_Timeout) != 0))
			{
				if(card_id_available == 0)
				{
					lcd_show(0, 'S', 2);					// Invalid Login
					lcd_clr(1);
					key1(MESSAGE_TIMEOUT);
					buzzer(100);
					return 0;
				}
			}  
				card_id_available = 0;
				if ((Find_card(card_id) != -1) &&(user_t.user_id == suser_code) )
				{
					card_id=0x00;
					sprintf(szstr, "%04d",user_t.user_id );
					lcd_show_str(0, 'S', 9, szstr);
					lcd_clr(1);					
				}
				else
				{
					card_id=0x00;
					lcd_show(0, 'S', 2);					// "Invalid card" to be shown and re-login.
					lcd_clr(1);
					delay(MESSAGE_TIMEOUT);
					buzzer(100);
					return 0;
				}
		}
		if (suser_pin == user_t.user_pin )
		{
			for(uci=0;uci<0x05;uci++)
			{
				if(suser_code == gmul_code[uci])
				{
					uccounter=1U;
					gmul_code[uci]=0x0000;
					break;					
				}
				else
				{
					uccounter =0;
				}
			}

			if(uccounter)
			{
	
				if((TRG_LOGIN_MULTIPLE))						//RELAY2 login for multiple access
				{							
					relay(2,ON);
					Cnt_TriggerTimeout = Set_Trigger_Timeout(ONE_SEC);	// Delay value for 1 sec
				}			
	
				if((gdashboard_trg & TRG_LOGIN_MULTIPLE))
				{
					prot_log_event((s32_t) EVT_MULTIPLE_TASK,(s32_t) user_t.user_id , 0, 0);	
					
				}						
				else
				{
					prot_log_event((s32_t) EVT_MULTIPLE_TASK,(s32_t) user_t.user_id , 0, 0);
				}
				
				return 1;
			}
			else
			{
				lcd_show(0,'S',2);				//Invalid login code
				lcd_clr(1);
				delay(MESSAGE_TIMEOUT);
				return 0;
			}		
		}		
	return 0;
}
void wrong_unauth_key_check(u8_t uck)
{
char szstr[50];
char szstr1[50];
u8_t uctoggle_led1=0;
if(((ibutton_value[uck][0]!=0x00)&&(key_events_t[uck].status==LOCK_OPEN)&& (((ibutton_value[uck][1]!=org_key_data[uck][1])&&(ibutton_value[uck][1]!=0xff))||((ibutton_value[uck][2]!=org_key_data[uck][2])&&(ibutton_value[uck][2]!=0xff))||((ibutton_value[uck][3]!=org_key_data[uck][3])&&(ibutton_value[uck][3]!=0xff))||((ibutton_value[uck][4]!=org_key_data[uck][4])&&(ibutton_value[uck][4]!=0xff))||((ibutton_value[uck][5]!=org_key_data[uck][5])&&(ibutton_value[uck][5]!=0xff))||((ibutton_value[uck][6]!=org_key_data[uck][6])&&(ibutton_value[uck][6]!=0xff)))) ||		//key position changed
										(key_events_t[uck].status==LOCK_OPEN && (key_events_t[uck].user != user_t.user_id) && (((ibutton_value[uck][1]==org_key_data[uck][1])&&(ibutton_value[uck][1]!=0xff))||((ibutton_value[uck][2]==org_key_data[uck][2])&&(ibutton_value[uck][2]!=0xff))||((ibutton_value[uck][3]==org_key_data[uck][3])&&(ibutton_value[uck][3]!=0xff))||((ibutton_value[uck][4]==org_key_data[uck][4])&&(ibutton_value[uck][4]!=0xff))||((ibutton_value[uck][5]==org_key_data[uck][5])&&(ibutton_value[uck][5]!=0xff))||((ibutton_value[uck][6]==org_key_data[uck][6])&&(ibutton_value[uck][6]!=0xff)))))
										{		
											if((key_events_t[uck].status==LOCK_OPEN && (key_events_t[uck].user != user_t.user_id) ))//&& ibutton_value[uck][1]==org_key_data[uck][1] && ibutton_value[uck][2]==org_key_data[uck][2] && ibutton_value[uck][3]==org_key_data[uck][3] && ibutton_value[uck][4]==org_key_data[uck][4]))
											{
												lcd_str(0,"Unauthorized FobPosition");
											}
											else
											{
												lcd_str(0,"Fob in Wrong Position!");
											}
											sprintf(szstr1,"Please Remove Fob %d",uck+1);
											lcd_str(1,szstr1);
											buzzer(1000);
											delay(1000);
											solenoid[uck]=0;
											red_led[uck]=1;
											
												uctoggle_led1=0;
											while(1)
											{
												uctoggle_led1++;
												sprintf(szstr,"L:%d",uctoggle_led1);
												pstr(szstr);
												if(uctoggle_led1 <2)
													red_led[uck]=1;
												else if(uctoggle_led1 <4)
													red_led[uck]=0;
												else
													uctoggle_led1=0;
												i_button_read=1;
												while(i_button_read);
												delay(10);
											
												if(ibutton_value[uck][1]==0xFF && ibutton_value[uck][2]==0xFF && ibutton_value[uck][3]==0xFF && ibutton_value[uck][4]==0xFF)
												{
													solenoid[uck]=1;
													sprintf(szstr1,"Wrong Fob %d removed",uck+1);
													lcd_str(0,szstr1);
													lcd_clr(1);
													delay(1000);
													break;
												}
												if(trg(2)==0)
												{
													solenoid[uck]=1;
													break;
												}
												if(lock_flg1==1)
												{
													lock_flg1=0;
													solenoid[uck]=1;
													break;
												}
												delay(1);
											}
											lcd_str(0,"Enter Fob Position:");
										}

}


