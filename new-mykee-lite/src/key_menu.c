/*
 * File name:key_menu.c
 * 		This file contains the key removal and return operation.
 *      And also has emergency key taken and return operation for administrator.
 * 
 * Version:V02.02.00 -->code restructure.
 * changes from old version: 1. code restructure and detail history 
 *                              included for files and functions
 * changes done by:   K.Sivakumar
 * 
 * Version:V02.01.00 -->GSM features added.  
 * changes from old version: 1. Modification in function remove_return_key(), to get the event details like
 * 								emergency key taken, return, door open time out and close time out for send the SMS. 
 *                           2. Modification in function user_key(), to get the event details like 
 * 								wrong time task, high security key taken, user key taken and removal for send the SMS. 
 * changes done by:  B.Sedhu sundaram  
 *        
 * Version:V02.00.00 -->code restructure. 
 * 
 * Copyright (c) 2004-2011 Sterna Security.
 * All rights reserved.
 * Author:sterna security.
 */


#include "lpc23xx.H"
#include "menu_main.h"
#include "timer.h"
#include "uart.h"
#include "LCD.h"
#include "LED.h"
#include "BUZZER.h"
#include "RFID.h"
#include "CLOCK_CALNDR.h"
#include "DOOR.h"

#define REMOVE_KEY 1
#define RETURN_KEY 2

u8_t user_key_id=0;
u8_t alarm_key[16];
u8_t alarm_key1[16];
u32_t unauth_key_remove_flag;

#define Taken_Process  1
#define Return_Process 2
/************************************************************************************************
 ** Function name:      emergency_keyaccess                                                    **
 **                                                                                            **
 ** Descriptions:       emergency key access function for select, remove or return operation   **
 **                                                                                            **
 ** parameters:         none                                                                   **
 **                                                                                            **
 ** Returned value:     none
 ** 
 ******************************************************************************/
void emergency_keyaccess(void)
{
	u32_t pass_code=0;
	s32_t itemp_key;
	u32_t user_code=0;
	while(1)
	{
		lcd_show(0, 'V', 7);
		lcd_show(1, 'V', 8);
		if (get_menu_key(&itemp_key) == 0)
		{
			return;
		}

		lcd_show(0, 'A', 4);
		lcd_show(1, 'V', 13);
		pass_code=myRand(Emergency_Code ,1);
		user_code=read_code(HIDE_INPUT);
		if (user_code != pass_code)
	{
		
		lcd_show(0, 'A', 4);
		lcd_show(1, 'S', 16U);		//Invalid service code
		delay(MESSAGE_TIMEOUT);
		return;
	}
		if (ULALARMS &(u32_t) DOOR_OPEN_TIMEOUT)
		{
			return;
		}
#ifndef TRUEKEY32		
		emerg_remove_return_key();
		return;
#else
		break;
#endif		
	}
#ifdef TRUEKEY32
	while(1)
	{
		lcd_show(0U, 'U',(u8_t) imenu_i); 			// Administrator menu
		lcd_show(1U, 'U',(u8_t) imenu_i+2);

		if (get_menu_key(&itemp_key)==0)
		{
			return;
		}
		if (select_menu(&imenu_i, itemp_key, 2))
		{
		 sprintf(szstr,"\rMenu:%d\r",imenu_i);
					 pstr(szstr);
			switch (imenu_i)
			{
				case 1:
					if (remove_return_key(REMOVE_KEY))
					{
				        imenu_i +=1;
					}
					break;
				case 2:
					if (remove_return_key(RETURN_KEY))
					{
						imenu_i=imenu_i-1;
					}
					break;
				default:
					break;
			}
		}
		if (ULALARMS &(u32_t) DOOR_OPEN_TIMEOUT)
		{
			return;
		}

	}
#endif	
}

void total_register(void)
{
	u8_t i,count=0,j=0;
	char szstr[16];
	lcd_str(0,"Total Key Register");
	lcd_clr(1);
	i_button_read=1;
	pstr("Key registration\r");
	
	key_pos=0;         //all key position zero
	key_chk=0;         //all key alarm zero
	key_no=i;
	i_button_read=1;
	while(i_button_read);
	delay(10);
	for(i=0;i<MAX_KEYS;i++)
	{
		check_key[i]=0;
		if(ibutton_value[i][0] && ibutton_value[i][0]!=0xFF && ibutton_value[i][0]!=0x00 && ibutton_value[i][4]!=0x00)
		{
			count++;
			green_led[i]=1;
			prot_log_event(EVT_KEY_REGISTERED,0,0,i+1);
			sprintf(szstr,"%d",i+1);
			lcd_show_str(0,'D',8,szstr);
			lcd_show(1,'D',9);
			delay(10);
			buzzer(10);
			sprintf(szstr,"Key:%d-Fcode:%02X",i+1,org_key_data[i][0]);
			lcd_str(0,szstr);	
			sprintf(szstr,"%02X%02X%02X%02X%02X%02X",org_key_data[i][1],org_key_data[i][2],org_key_data[i][3],org_key_data[i][4],org_key_data[i][5],org_key_data[i][6]);
			lcd_str(1,szstr);	
			delay(2000);
		}
		else
		{
			prot_log_event(EVT_KEY_NOT_REGISTERED,0,0,i+1);
			sprintf(szstr,"%d",i+1);
			lcd_show_str(0,'D',8,szstr);
			lcd_show(1,'D',12);
			delay(500);
			buzzer(200);
 		}
		
		 for(j=0;j<12;j++)
		 {
			
			key1_t[i].keyvalue[j]=org_key_data[i][j]=ibutton_value[i][j];
		 }
  	
		
		for(j=0;j<7;j++)
		{
 			
			if(org_key_data[i][j]==0xFF||(org_key_data[i][0]==0x00))
      {
				key_not_reg[i]=1;
        key1_t[i].status=LOCK_OPEN ;
				key1_t[i].task_removal_time=0;
      }
			else if(org_key_data[i][j]!=0XFF && org_key_data[i][0]!=0x00)
      {       
       key_not_reg[i]=2;
       key1_t[i].status=LOCK_CLOSED;
      }
		}
		
		sprintf(szstr,"\r\nKey:%d",i+1);
		pstr(szstr);	
		sprintf(szstr,"  Fcode:%02X",org_key_data[i][0]);
		pstr(szstr);
		sprintf(szstr,"  VALUE:%02X%02X%02X%02X%02X%02X",org_key_data[i][1],org_key_data[i][2],org_key_data[i][3],org_key_data[i][4],org_key_data[i][5],org_key_data[i][6]);
		pstr(szstr);	
		
	}

	write_key_sd();
	
	lcd_show(0,'D',6);
#ifndef TRUEKEY32	
	sprintf(szstr,"Total %d Fobs Registered",count);
#else
	sprintf(szstr,"Total %d Keys Registered",count);
#endif	
	lcd_str(1,szstr);
	delay(1000);
	for(i=0;i<MAX_KEYS;i++)
	{
		green_led[i]=0;
	}
	create_keyevents();
}
/*         add new keys                */
void addnewkeys(void)
{
  u8_t i,j,flag=0,count=0;
	char szstr[16];
	u8_t a=0;     //count for reg key
	
	lcd_show(0,'e',25);
	
	i_button_read=1;
	while(i_button_read)
	{
		delay(100);
	}
	i_button_read=1;
	while(i_button_read)
	{
		delay(100);
	}
	
	
	for(i=0;i<MAX_KEYS ;i++)
	{
		if(ibutton_value[i][0] && ibutton_value[i][0]!=0xFF )
		{
			for(j=0;j<12;j++)
			{
					if((org_key_data[i][0]!=ibutton_value[i][0]&& org_key_data[i][4]!=ibutton_value[i][4]&& org_key_data[i][5]!=ibutton_value[i][5]))
					{
					 flag++;
          }
			}
			if(flag)
			{
				flag=0;
				count++;
				green_led[i]=1;
				prot_log_event(EVT_KEY_REGISTERED,0,0,i);		
				
				
				for(j=0;j<12;j++)
				{
				 org_key_data[i][j]=key1_t[i].keyvalue[j]=ibutton_value[i][j];
			 	 key_not_reg[i]=2;
				}
				key_chk&=~(1<<i);     //new key alarm clear
				key_pos&=~(1<<i);     //new key pos make clear
				check_key[i]=0;
				unauth_key_remove_flag&=~(1<<i);
			}
     a++;			
		}	
    else if(ibutton_value[i][1]==0xFF)
		{
			key_chk&=~(1<<i);         //not reg pos  key alarm clear
			key_pos&=~(1<<i);         //not reg pos  key pos alarm clear
			check_key[i]=0;
			unauth_key_remove_flag&=~(1<<i);
      key1_t[i].status=1;
			
			for(j=0;j<12;j++)
			{
				 org_key_data[i][j]=key1_t[i].keyvalue[j]=ibutton_value[i][j];
			 	 key_not_reg[i]=1;
		  }
    }
		
    for(j=1;j<7;j++)
		{
 			
			if(org_key_data[i][j]==0xFF||(org_key_data[i][0]==0x00))
      {
				key_not_reg[i]=1;
        key1_t[i].status=LOCK_OPEN;
      }
			else if(org_key_data[i][j]!=0XFF && org_key_data[i][0]!=0x00)
      {
       key_not_reg[i]=2;
       key1_t[i].status=LOCK_CLOSED;
			 
      }
		}
		
		sprintf(szstr,"\rKey:%d",i+1);
		pstr(szstr);	
		sprintf(szstr,"  Fcode:%02X",org_key_data[i][0]);
		pstr(szstr);
		sprintf(szstr,"  VALUE:%02X%02X%02X%02X%02X%02X",org_key_data[i][1],org_key_data[i][2],org_key_data[i][3],org_key_data[i][4],org_key_data[i][5],org_key_data[i][6]);
		pstr(szstr);
          
	}
	write_key_sd();
	sprintf(szstr,"%d",count);
	lcd_show_str(0,'D',10,szstr);
	lcd_show(1,'D',11);
	delay(5000);
	for(i=0;i<MAX_KEYS;i++)
	{
		green_led[i]=0;
	}
	
	
}
void register_keys(void)
{
	u8_t temp_key,i;
	for(i=0;i<24;i++)
	{
		green_led[i]=0;
	}
	while(1)
	{
		lcd_show(0, 'D', 6);   
		lcd_show(1, 'D', 7); 	
		temp_key=key1(5000);
		if(temp_key=='1')
		{
			lcd_show(0,'V',7);
			lcd_show(1,'V',8);
			temp_key=key1(5000);
			if(temp_key == ENTER)
			{
				total_register();
			}
		}
		else if(temp_key=='2')
		{
			lcd_show(0,'V',7);
			lcd_show(1,'V',8);
			temp_key=key1(5000);
			if(temp_key == ENTER)
			{
				addnewkeys();	
			}
		}
		else
			return;
		}	
	
}
#ifdef IBUTTON
void key_check_2(void)          //without userlogin
{
			u8_t chkey=0;
	    u8_t i;
	    char szstr[50];	    
			static u8_t toggle_flg[MAX_KEYS];
			
			i_button_read=1;
			while(i_button_read);	
		  delay(10);		
			for(i=0;i<MAX_KEYS;i++)
			{
				read_keyevents_sd(i);
				if(key_not_reg[i]==2&&(ibutton_value[i][1]!=org_key_data[i][1])&&(ibutton_value[i][2]!=org_key_data[i][2])&&(ibutton_value[i][3]!=org_key_data[i][3])&&(ibutton_value[i][4]!=org_key_data[i][4])&&(ibutton_value[i][5]!=org_key_data[i][5])&&(ibutton_value[i][6]!=org_key_data[i][6]))    // access not permit keys taken
					 {
						  chkey=2; 
						  green_led[i]=0;
							red_led[i]=0;
							sprintf(szstr,"\rKey:%d\r",i+1);
							pstr(szstr);
					 }
           if((key_events_t[i].status==LOCK_OPEN)&&(((ibutton_value[i][1]!=org_key_data[i][1])&&(ibutton_value[i][1]!=0xff))||((ibutton_value[i][2]!=org_key_data[i][2])&&(ibutton_value[i][2]!=0xff))||((ibutton_value[i][3]!=org_key_data[i][3])&&(ibutton_value[i][3]!=0xff))||((ibutton_value[i][4]!=org_key_data[i][4])&&(ibutton_value[i][4]!=0xff))||((ibutton_value[i][5]!=org_key_data[i][5])&&(ibutton_value[i][5]!=0xff))||((ibutton_value[i][6]!=org_key_data[i][6])&&(ibutton_value[i][6]!=0xff))))		//key position changed
					 {
						if(alarm_key1[i]!=1)
						{
							sprintf (szstr,"Fob %d in Wrong Position",i+1);
							lcd_str(0,szstr);
							buzzer(500);
							delay(1000);
							prot_log_event(EVT_ALARM_KEYRETURNED,0,0,i+1);
							alarm_key1[i]=1;
						}
								if(!toggle_flg[i])
								{
									toggle_flg[i]=1;
									red_led[i]=1;
								}
								else if(toggle_flg[i])
								{
									toggle_flg[i]=0;
									red_led[i]=0;
								} 
					 }
				    if(ibutton_value[i][0]==0xFF && ibutton_value[i][1]==0xFF && ibutton_value[i][2]==0xFF && ibutton_value[i][3]==0xFF && ibutton_value[i][4]==0xFF && alarm_key1[i]) 		 
						{
							alarm_key1[i]=0;
							pstr("\rWrong fob position removed");
						}
					
				if(((chkey==2)&&(key_not_reg[i]==2)&&(key_events_t[i].status==LOCK_CLOSED)))  //unauthorized key remove
				{
					chkey=0;
					
					i_button_read=1;
					while(i_button_read);
					delay(5);
			
					if(((key_not_reg[i]==2)&&(ibutton_value[i][4]!=org_key_data[i][4])&&(ibutton_value[i][5]!=org_key_data[i][5])))
					{
								  														
						buzzer(2000);
              check_key[i]=1;							
							
						if(!(key_pos&(1<<i)))
						{
							key_pos|=(1<<i);
						}
						
						
						prot_log_event(EVT_UNAUTH_KEYREMOVED,0,0,i+1);//event(key return),user id ,key id, act  - login and close wothout acctivity
						delay(3);
						if(alarm_key[i]!=1 &&((trg(1))&&(trg(2))))         //door open //
						{
							lcd_dis(1);
							
							sprintf(szstr,"%d",i+1);
							lcd_show_str(0,'E',3,szstr);
							lcd_show(1,'E',7);
							buzzer(2000);
							alarm_key[i]=1;	
						}
						if(!trg(1)&&!trg(2))                        //door close//
						{
							buzzer(2000);	
						}
						prot_log_event(EVT_ALARM_KEYTAKEN,0,0,i+1);
						unauth_key_remove_flag|=(1<<i);
						key_events_t[i].status=LOCK_OPEN;
						

						write_keyevents_sd();
					 
						delay(2);
					
						key_events_t[i].status=LOCK_OPEN;
						

						 write_keyevents_sd();	
						
					}
				 }

				else if(key_not_reg[i]==2&&(key_events_t[i].status==LOCK_OPEN))
				{
					pstr("\rUnauthorized key return");
					delay(5);
					
					if((ibutton_value[i][4]==org_key_data[i][4])&&(ibutton_value[i][5]==org_key_data[i][5]))
					{
						buzzer(500);		
						lcd_str(0,"Unauthorized Fob Return");
						sprintf (szstr,"Fob Position:%d",i+1);
						lcd_str(1,szstr);
						delay(1000);
				  	 
						check_key[i]=0;
	
				   key_pos&=~(1<<i);	
					delay(3);
					prot_log_event(EVT_UNAUTH_KEYRETURNED,0,0,i+1);
				
					
					key_events_t[i].status=LOCK_CLOSED;						
					
					
					write_keyevents_sd();		
			  	
					delay(3);

					
					key_events_t[i].status=LOCK_CLOSED;
					 
					write_keyevents_sd();
          delay(3);
					green_led[i]=0;
					red_led[i]=0;
				}
        }
		 }
					 
}

#endif
#ifndef TASK_MANAGEMENT 
s32_t user_mykee(u8_t ucremove_mode,u16_t ustask,u16_t key_no)
{
	s32_t   imenu_i;
    s32_t   itemp_key=0;
    s32_t   var=0;
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
		u32_t  uctoggle_led1 = 0;
	u8_t  ucfound =0;
	u32_t local_ticks=0;
	u8_t  user_task[32];
    u8_t  ucstate;
	u8_t  ucstatus;	
    u8_t  uclk[MAX_KEYS];
		u8_t task_count=0;
		char szstr1[32];
		u8_t user_key=0;
		u8_t user_key_option[16];
		u8_t key_entered=0;
		u8_t ui=0;
		char array[3];

	s16_t sdlocation;
  	pstr("\rUser MyKee Operation");
		local_ticks =get_ticks_value();
		sprintf(szstr,"\rUsercode:%d",user_code_main);
		pstr(szstr);
		sdlocation=find_user_sd1(user_code_main);
			if(sdlocation!=-1)
			{
				read_user_sd((s32_t) sdlocation);
			}
	//variable  removemode==1-->key removal and multi task, remove_mode ==2 --> one task and one key ,remove_mode == 0 -->key Return
    if(ucremove_mode)				
		{
			uitimeout=0;
					i_button_read=1;
					while(i_button_read);
					delay(10);
				for (uci = 0; uci < MAX_KEYS; uci++)
				{
					read_key_sd((s32_t) uci);
					read_keyevents_sd((s32_t) uci);
					if (((user_t.user_key>>uci)&0x01))   // changed for task only for key removal
					{
						sprintf(szstr,"\ruser key:%d Key:%d enabled Sta:%d",user_t.user_key,uci,key1_t[uci].status);
						pstr(szstr);
						if(ibutton_value[uci][0]!=0xFF && ibutton_value[uci][0]!=0xFF && ibutton_value[uci][0]!=0xFF && ibutton_value[uci][0]!=0xFF)
						{
							sprintf(szstr,"\rKey:%d dow:%d-%d st:%d-%d et:%d-%d",uci,user_t.dow,gcurrent_dow,user_t.start_date,local_ticks,user_t.end_date,local_ticks);
							pstr(szstr);
							
							if ((user_t.dow & (1 << get_doy_dow(0x02,szstr))) &&(user_t.start_date < local_ticks) && (user_t.end_date > local_ticks))
							{
							
								sprintf(szstr,"\rKey:%d ",uci);
								pstr(szstr);
							
							
								uitimeout++;
							}
							if(user_t.access_type==0xFE)
							{
								u8_t hr,min,sec;
								hr=RTC_HOUR;
								min=RTC_MIN;
								sec=RTC_SEC;
								ucfound=(hr*3600)+(min*60)+sec;
								if((ucfound<user_t.start_time)||(ucfound>user_t.end_time))
								{
									uitimeout=0;
								}
							}
						}
					}			
				}
				
				get_timestamp(szstr, 1U);
				ustask=user_t.user_id ;
				{
					ucfound = 1U;
				}
				if(ucfound==(u8_t) 0)
				{
	
					if((u32_t) TRG_WRONG_TASTTIME) 	//RLY2 TRIGGER WHEN task overdue
					{
						relay(2,ON);
						Cnt_TriggerTimeout = Set_Trigger_Timeout((u32_t) ONE_SEC);	// Delay value for 1 sec
					}
					if(gdashboard_trg &(u32_t) TRG_WRONG_TASTTIME) 	
					{
						prot_log_event((s32_t) EVT_WRONG_SHEDULED_TIME,(s32_t) ustask,0,0 );
					}
					else
					{
						prot_log_event((s32_t) EVT_WRONG_SHEDULED_TIME,(s32_t) ustask,0,0);
					}

					lcd_show(0U, 'S', 11U);                       // Wrong Task entry
					lcd_show(1U,'S',12U);
					key1((u32_t) MESSAGE_TIMEOUT);
					return 1;//0xAA;
				}
				else if((uitimeout==(u32_t) 0) && (ucremove_mode==(u8_t)1))
				{
					lcd_show(0U, 'K', 7U);					// No keys to remove
					lcd_clr(1U); 	
					key1((u32_t) MESSAGE_TIMEOUT);
					return 0;//0xFF;

				}
				else if(user_t.start_date)
				{

					if(compare_date(user_t.start_date,gcurrent_date)==0)			
					{
						
					}
					else if(((compare_date(gcurrent_date,user_t.end_date))==0)&& (user_t.end_date))
					{
						lcd_show(0U,'V',3U);                 //Task Expired
						lcd_show(1U,'e',6U);                 //Please check with Admin
						delay((u32_t) MESSAGE_TIMEOUT);
						
					}
					else
					{
						return 1;
					}
			
				}
				else
				{
	            	return 1;
				}
	     
		var=0;

	}
	else
	{
	    scount = 0;
	    for (uci = 0; uci < MAX_KEYS; uci++)                // Determine number of keys to process
	    {
	     key_no=uci;
					i_button_read=1;
					while(i_button_read);
					delay(10);
			
			read_key_sd((s32_t) uci);
			read_keyevents_sd((s32_t) uci);
		
			get_timestamp(szstr, 1U);

			if((ibutton_value[uci][0]==0xFF && ibutton_value[uci][1]==0xFF && ibutton_value[uci][2]==0xFF && ibutton_value[uci][3]==0xFF)&&(key_events_t[uci].user == user_t.user_id))
			{
        scount++;
				break;
			}
			else if((ibutton_value[uci][0]!=org_key_data[uci][0] && ibutton_value[uci][3]!=org_key_data[uci][3] && ibutton_value[uci][4]!=org_key_data[uci][4] && ibutton_value[uci][5]!=org_key_data[uci][5]) && (key_events_t[uci].user == user_t.user_id))
			{
				scount++;
				break;
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
					clear_keyposition();
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
					lcd_clr(0);
					delay(10);
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
						prot_log_event((s32_t) EVT_DOOR_OPEN_TOUT,(s32_t) ustask,0, 0);
					}
					else
					{
						prot_log_event((s32_t) EVT_DOOR_OPEN_TOUT,(s32_t) ustask, 0,0);
					}
					clear_keyposition();
          return 0;
        }
    }

#endif	

    for (uci = 0; uci < MAX_KEYS; uci++)                  // Determine which keys to process
    {
        uclk[uci] = 0U;
				user_key_option[uci]=0xFF;
    }
		ucj=0;
				i_button_read=1;
					while(i_button_read);
					delay(10);
    for (uci = 0; uci < MAX_KEYS; uci++)
    {
			read_key_sd((s32_t) uci);
			read_keyevents_sd((s32_t) uci);
			get_timestamp(szstr,(u8_t) 1);

		if (((user_t.user_key>>uci)&0x01) && (ucremove_mode))  // changed for task only for key removal
    {
				sprintf(szstr,"\rKey:%02X-%02X %02X-%02X %02X-%02X %02X-%02X K-Sta:%d",ibutton_value[uci][1],org_key_data[uci][1],ibutton_value[uci][2],org_key_data[uci][2],ibutton_value[uci][3],org_key_data[uci][3],ibutton_value[uci][4],org_key_data[uci][4],key_events_t[uci].status);
				pstr(szstr);
			if(key_events_t[uci].status ==(u8_t) LOCK_CLOSED && (ibutton_value[uci][1]==org_key_data[uci][1] && ibutton_value[uci][2]==org_key_data[uci][2] && ibutton_value[uci][3]==org_key_data[uci][3] && ibutton_value[uci][4]==org_key_data[uci][4] && ibutton_value[uci][5]==org_key_data[uci][5] && ibutton_value[uci][6]==org_key_data[uci][6]&& (key_not_reg[uci]==2)))
			{
				green_led[uci]=1;
				uclk[uci] = 1U;	
				user_key_option[ucj]=uci;
				sprintf(szstr,"\rSelecting key:%d",user_key_option[ucj]);
				pstr(szstr);
				ucj++;
			}
			else if ((ibutton_value[uci][0]==0xFF && ibutton_value[uci][0]==0xFF && ibutton_value[uci][0]==0xFF && ibutton_value[uci][0]==0xFF)&&(key_events_t[uci].user == user_t.user_id ))
			{
				red_led[uci]=1;
				uclk[uci] = 2U;
			}
		}
		else if(ucremove_mode==(u8_t) 0)	// implemented  for return the multi user task key by all muti users.
		{
			if((ucstate &(u8_t) LOCK_OPEN)&&(key_events_t[uci].user ==(u16_t) ginstaller_code))
			{
			}				
			else if ((ibutton_value[uci][0]==0xFF && ibutton_value[uci][0]==0xFF && ibutton_value[uci][0]==0xFF && ibutton_value[uci][0]==0xFF)&&(key_events_t[uci].user == user_t.user_id ))
			{
				red_led[uci]=1;
				uclk[uci] = 2U;
			}
			else if((ibutton_value[uci][0]!=org_key_data[uci][0] && ibutton_value[uci][3]!=org_key_data[uci][3] && ibutton_value[uci][4]!=org_key_data[uci][4] && ibutton_value[uci][5]!=org_key_data[uci][5]) && (key_events_t[uci].user == user_t.user_id))
			{
				red_led[uci]=1;
				uclk[uci] = 2U;
			}
		}
		else
		{
			
		}
    }

    uitimeout =(u32_t)((u32_t) sys_t.key_turn_time * 10U);
    scount = 0;
		uik1=0U;
		uik2=0U;
		ucfound=0U;					// used for highsecurity removed and normal key -key taken with no key number
		ucstatus=0U;
		strcpy(szstr2,"");
		var=4;
		lcd_clr(1);
		pressed_key=0;
		while(1)                                        // Process keys, one by one
    {
    	if(ULALARMS &(u32_t) SERVICE_DOOR_OPEN)//to handle burglary/srv door open  alarm immediatly
			{
				alarm_handler();
			}
		
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
        if (trg(2)==0)
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

          uitimeout =(u32_t)((u32_t) sys_t.key_turn_time * 10U);	
					pstr("\rEntering into keycheck");
            while (1)
            {
								red_led[0]=0;
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

										sprintf(szstr,"\rUser:%d-%d Key %d-%d:%02X-%02X %02X-%02X %02X-%02X %02X-%02X %02X-%02X %02X-%02X",key_events_t[uck].user,user_t.user_id,uck,key_events_t[uck].status,ibutton_value[uck][1],org_key_data[uck][1],ibutton_value[uck][2],org_key_data[uck][2],ibutton_value[uck][3],org_key_data[uck][3],ibutton_value[uck][4],org_key_data[uck][4],ibutton_value[uck][5],org_key_data[uck][5],ibutton_value[uck][6],org_key_data[uck][6]);
										pstr(szstr);
			
										if(((key_events_t[uck].status==LOCK_OPEN)&& (((ibutton_value[uck][1]!=org_key_data[uck][1])&&(ibutton_value[uck][1]!=0xff))||((ibutton_value[uck][2]!=org_key_data[uck][2])&&(ibutton_value[uck][2]!=0xff))||((ibutton_value[uck][3]!=org_key_data[uck][3])&&(ibutton_value[uck][3]!=0xff))||((ibutton_value[uck][4]!=org_key_data[uck][4])&&(ibutton_value[uck][4]!=0xff))||((ibutton_value[uck][5]!=org_key_data[uck][5])&&(ibutton_value[uck][5]!=0xff))||((ibutton_value[uck][6]!=org_key_data[uck][6])&&(ibutton_value[uck][6]!=0xff)))) ||		//key position changed
										(key_events_t[uck].status==LOCK_OPEN && (key_events_t[uck].user != user_t.user_id) && ibutton_value[uck][1]==org_key_data[uck][1] && ibutton_value[uck][2]==org_key_data[uck][2] && ibutton_value[uck][3]==org_key_data[uck][3] && ibutton_value[uck][4]==org_key_data[uck][4] && ibutton_value[uck][5]==org_key_data[uck][5] && ibutton_value[uck][6]==org_key_data[uck][6]))
										{
											sprintf(szstr,"\rKey %d-%d:%02X-%02X %02X-%02X %02X-%02X %02X-%02X",uci,key_not_reg[uck],ibutton_value[uck][1],org_key_data[uck][1],ibutton_value[uck][2],org_key_data[uck][2],ibutton_value[uck][3],org_key_data[uck][3],ibutton_value[uck][4],org_key_data[uck][4]);
											pstr(szstr);
											sprintf(szstr,"\r Key:%d",uck);
											pstr(szstr);
											
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
											if(key_entered)
												key_entered=0;
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
										
												 if(uclk[uck]==2)
													red_led[uck]=1;
												 else
													red_led[uck]=0;	
										}

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
												prot_log_event((s32_t) EVT_KEY_RETURNED,(s32_t) ustask,0,uck+1);												
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
												buzzer(500);
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
												
													key_entered=ble_key_no;
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
													for (uci = 0; uci < MAX_KEYS; uci++)					// Done, so inactivate all LEDs and solenoids
													{
														solenoid[uci]=1;
													}
													// Done, so inactivate all LEDs and solenoids
													if((user_key-1)<=MAX_KEYS)
													{
														read_keyevents_sd(user_key-1);
														read_key_sd(user_key-1);
														sprintf(szstr,"\rUserKey:%d-%d",user_key,user_key_option[user_key-1]);
														pstr(szstr);
														if(((user_t.user_key>>(user_key-1))&0x01) && key_events_t[user_key-1].status==LOCK_CLOSED)// && key_events_t[user_key-1].user == user_t.user_id)
														{
															uci=user_key-1;
															green_led[user_key-1]=1;
															key_entered=user_key;
															break;
														}
														else if((((user_t.user_key>>(user_key-1))&0x01) && key_events_t[user_key-1].user != user_t.user_id && key_events_t[user_key-1].status==LOCK_OPEN))
														{
															lcd_clr(0);
															lcd_str(0,"Fob Already Taken");
															delay(1000);
															buzzer(1000);
															break;
														}
														else
														{
															lcd_clr(0);
															lcd_str(0,"Invalid Fob Entered");
															delay(1000);
															buzzer(1000);
														}
													}
													else
													{
													lcd_clr(0);
														lcd_str(0,"Invalid Fob Entered");
														delay(1000);
														buzzer(1000);
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
												
                        	update_key_log((s32_t) ustask,(s32_t) ustask,(s32_t) uci,(u8_t) LOCK_OPEN);
													prot_log_event((s32_t) EVT_KEY_TAKEN,(s32_t) ustask,0,uci+1);
												uclk[uci] = 2U;
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
												prot_log_event((s32_t) EVT_ERGY_KEY_TAKE ,(s32_t) ustask,0,(s32_t) uci);	
												uik1 ^=( 1U << (u32_t)uci);
											}
											else
											{
												prot_log_event((s32_t) EVT_ERGY_KEY_RETURN ,(s32_t) ustask,0,(s32_t) uci);
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
										sprintf(szstr, "%d", ((uitimeout + 8U) / 10U));
										lcd_show_str(1U, 'D', 4U, szstr);
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
											
											if(uclk[uci]==2)
											{
												user_key++;
											}
											sprintf(szstr,"\rUserkeytoreturn:%d",user_key );
											pstr(szstr);
             
			
										if(((key_events_t[uci].status==LOCK_OPEN)&& (((ibutton_value[uci][1]!=org_key_data[uci][1])&&(ibutton_value[uci][1]!=0xff))||((ibutton_value[uci][2]!=org_key_data[uci][2])&&(ibutton_value[uci][2]!=0xff))||((ibutton_value[uci][3]!=org_key_data[uci][3])&&(ibutton_value[uci][3]!=0xff))||((ibutton_value[uci][4]!=org_key_data[uci][4])&&(ibutton_value[uci][4]!=0xff))||((ibutton_value[uci][5]!=org_key_data[uci][5])&&(ibutton_value[uci][5]!=0xff))||((ibutton_value[uci][6]!=org_key_data[uci][6])&&(ibutton_value[uci][6]!=0xff)))) ||		//key position changed
										(key_events_t[uci].status==LOCK_OPEN && (key_events_t[uci].user != user_t.user_id) && ibutton_value[uci][1]==org_key_data[uci][1] && ibutton_value[uci][2]==org_key_data[uci][2] && ibutton_value[uci][3]==org_key_data[uci][3] && ibutton_value[uci][4]==org_key_data[uci][4] && ibutton_value[uci][5]==org_key_data[uci][5] && ibutton_value[uci][6]==org_key_data[uci][6]))
										{
											sprintf(szstr,"\rKey %d-%d:%02X-%02X %02X-%02X %02X-%02X %02X-%02X",uci,key_not_reg[uck],ibutton_value[uci][1],org_key_data[uci][1],ibutton_value[uci][2],org_key_data[uci][2],ibutton_value[uci][3],org_key_data[uci][3],ibutton_value[uci][4],org_key_data[uci][4]);
											pstr(szstr);
											sprintf(szstr,"\r Key:%d",uci);
											pstr(szstr);
											
											if((key_events_t[uci].status==LOCK_OPEN && (key_events_t[uci].user != user_t.user_id)))// && ibutton_value[uci][1]==org_key_data[uci][1] && ibutton_value[uci][2]==org_key_data[uci][2] && ibutton_value[uci][3]==org_key_data[uci][3] && ibutton_value[uci][4]==org_key_data[uci][4]))
											{
												lcd_str(0,"Unauthorized Fob Position");
											}
											else
											{
												lcd_str(0,"Fob in Wrong Position!");
											}
											sprintf(szstr1,"Please Remove Fob %d",uci+1);
											lcd_str(1,szstr1);
											buzzer(1000);
											delay(1000);

											solenoid[uci]=0;
											red_led[uci]=1;
											if(key_entered)
												key_entered=0;
												uctoggle_led1=0;
											while(1)
											{
											uctoggle_led1++;
											sprintf(szstr,"L:%d",uctoggle_led1);
											pstr(szstr);
												if(uctoggle_led1 <2)
													red_led[uci]=1;
												else if(uctoggle_led1 <4)
													red_led[uci]=0;
												else
													uctoggle_led1=0;
											i_button_read=1;
											while(i_button_read);
											delay(10);
											
												if(ibutton_value[uci][1]==0xFF && ibutton_value[uci][2]==0xFF && ibutton_value[uci][3]==0xFF && ibutton_value[uci][4]==0xFF)
												{
													solenoid[uci]=1;
													sprintf(szstr1,"Wrong Fob %d removed",uci+1);
													lcd_str(0,szstr1);
													lcd_clr(1);
													delay(1000);
													break;
												}
												if(trg(2)==0)
												{
													solenoid[uci]=1;
													break;
												}
												if(lock_flg1)
												{
													lock_flg1=0;
													solenoid[uci]=1;
													break;
												}
												delay(1);
											}
										lcd_str(0U,"Please Return Fob");
											if(uclk[uci]==2)
												red_led[uci]=1;
											else
													red_led[uci]=0;
										
										}
										if(uclk[uci] && (key_events_t[uci].user == user_t.user_id) && ibutton_value[uci][1]==org_key_data[uci][1] && ibutton_value[uci][2]==org_key_data[uci][2] && ibutton_value[uci][3]==org_key_data[uci][3] && ibutton_value[uci][4]==org_key_data[uci][4] && ibutton_value[uci][5]==org_key_data[uci][5] && ibutton_value[uci][6]==org_key_data[uci][6])
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
												prot_log_event((s32_t) EVT_KEY_RETURNED,(s32_t) ustask,0,uci+1);												
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
												if(user_key!=1)
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
			prot_log_event((s32_t) EVT_NOKEY_TAKEN,(s32_t) ustask,0,0);	
		}
		else
		{
			prot_log_event((s32_t) EVT_NOKEY_TAKEN,(s32_t) ustask,0,0);
		}
	}
    clear_keyposition();
    if (trg(2)==0)
    {
    	var =2;      		
    }
    return var;
}


#endif
#ifdef IBUTTON
/******************************************************************************
 ** Function name:      emergency_remove_return_key
 **
 ** Descriptions:       this function explains the process while key remove/return
 **						if any keys & tasks assigned for the user, they may take/return key
 **						if doesn't assigned, our system will display no keys to remove/return
 ** 
 ** parameters:         option=> 1=>emergency access
 **															 0=>user access
 **
 ******************************************************************************/
s32_t emerg_remove_return_key()
{
	u32_t uik1 =(u32_t) 0;
	u32_t uitimeout;
	char  szstr[32];
	char  szstr2[50]="";
	char  szstr3[5]="";

	u8_t  uci,uck;
	u8_t  uclk[MAX_KEYS],key_flg[MAX_KEYS];
	u8_t 	ucprev_state[MAX_KEYS];
	u8_t  ucount;
	u8_t  ucstate;
	u8_t  uctoggle_led = 0;
	u8_t 	total_key_flag=0;
	u8_t key_entered=0;
	u8_t user_key=0;
	
	ucount = 0U;
	for (uci = 0U; uci <(u8_t) MAX_KEYS ; uci++) // Determine number of keys to process
	{
		read_key_sd((s32_t) uci);
		read_keyevents_sd((s32_t) uci);
	}

#ifndef NO_DOOR_NO_SOLENOID

	uitimeout =(u32_t) ((u32_t)sys_t.open_door_time * 10U);
		lock_on(1);
	while (1) // Wait for the door to be opened
	{
		lcd_show(0U, 'D', 0U);
		if (trg(2))
		{
			break;
		}
		if (key1(100U) ==ESC)
		{
				lock_on(0);
			return 0;
		}
		sprintf(szstr, "%d", (uitimeout + 8U) / 10U);
		lcd_show_str(1U, 'D', 4U, szstr);
		uitimeout--;
		if (uitimeout==0U)
		{
				lock_on(0);
			lcd_show(0U, 'D', 2U); // Open door timeout
			key1((u32_t) MESSAGE_TIMEOUT);
			buzzer(1000);
			prot_log_event((s32_t) EVT_DOOR_OPEN_TOUT,(s32_t) user_code_main,0,0);
			return 0;
		}
	}
		lock_on(0);
	
#endif
	for (uci = 0U; uci <(u8_t) max_keys_available; uci++)		// Determine which keys to process
	{
		uclk[uci] = 0U;
		key_flg[uci]=0;
	}
	for (uci = 0U; uci <(u8_t) max_keys_available; uci++)
	{
		ucprev_state[uci] = lock_state[uci];
	}
		for (uci = 0U; uci <(u8_t) max_keys_available; uci++)
		{
			read_key_sd((s32_t) uci);
			read_keyevents_sd((s32_t) uci);

			if (key_events_t[uci].status == LOCK_CLOSED)//&&(key_events_t[uci].status ==(u8_t) LOCK_CLOSED))
			{
				green_led[uci]=1;
				uclk[uci] = 1U;
				total_key_flag=uci+1;
			}
			if (key_events_t[uci].status==LOCK_OPEN)//&&(key_events_t[uci].status ==(u8_t) LOCK_OPEN))
			{
				red_led[uci]=1;
				uclk[uci] = 1U;
				total_key_flag=uci+1;				
			}
			
		}


	uitimeout =(u32_t) ((u32_t)sys_t.key_turn_time * 10U);
	ucount = 0U;
	while (1) // Process keys, one by one
	{
		if (ULALARMS &(u32_t) SERVICE_DOOR_OPEN)//to handle burglary/srv door open  alarm immediatly
		{
			alarm_handler();
		}
		if (uitimeout<=(u32_t)1)
		{
			uitimeout=0;
			lcd_show(1U, 'K', 2U); // Key handling timeout
			clear_keyposition();
			break;
		}
		else
		{
			uitimeout--;
		}
		if (pressed_key)
		{
			pressed_key =(char) 0;
			
			break;
		}
#ifndef NO_DOOR_NO_SOLENOID
		if (trg(2)==0)
		{
			clear_keyposition();
			break;
		}
#endif
		for (uci = 0U; uci <(u8_t) MAX_KEYS; uci++)
		{
			if (uclk[uci])
			{
				break;
			}
		}

		if(uci == total_key_flag)
		{
			break;
		}
		
		lcd_show(0U, 'K', 5U); // Take indicated keys
		lcd_clr(1);

			while (1)
			{
				if ((ULALARMS &(u32_t) SERVICE_DOOR_OPEN) && uitimeout)//to handle burglary/srv door open  alarm immediatly
				{
					alarm_handler();
				}
				
#ifndef NO_DOOR_NO_SOLENOID
				
				if (trg(2)==0)
				{
					break;
				}
#endif				
				if (uitimeout<=1U)
				{
					uitimeout=0;
					lcd_show(1U, 'K', 2U); // Key handling timeout

					clear_keyposition();
					break;
				}
				else
				{
					uitimeout=uitimeout-1;
				}
				
					user_key=read_code(0xEE);
				
				
				if(user_key==252)
				{
					clear_keyposition();
					break;
				}	
				else if(user_key<=MAX_KEYS && user_key && user_key!=255 && key_events_t[user_key-1].status==LOCK_OPEN)
				{
					lcd_str(0,"Fob not valid");
					delay(500);
					buzzer(500);
				}
				else if(user_key<=MAX_KEYS && user_key && user_key!=255)
					key_entered=user_key;
				for(uci=0;uci<MAX_KEYS;uci++)
				{
				sprintf(szstr, "%d", uci+1);
				i_button_read=1;
				while(i_button_read)
				{
					delay(10);
				}
				if(key_entered)
				{
					sprintf(szstr, "%d", key_entered);
					uck=key_entered-1;
					sprintf(szstr2,"Take %d Fob",key_entered);
					lcd_str(0,szstr2);
					lcd_clr(1);
					solenoid[uck]=0;
					if(ibutton_value[uck][0]==0xFF && ibutton_value[uck][1]==0xFF && ibutton_value[uck][2]==0xFF && ibutton_value[uck][3]==0xFF && key_events_t[uck].status==LOCK_CLOSED)
					{
						solenoid[uck]=1;
						key_entered=0;
						green_led[uck]=0;
						red_led[uck]=1;
						
						lcd_show_str(0U, 'K', 3U, szstr);		// Key x taken
						update_key_log((s32_t) user_t.user_id , 0,(s32_t) uck,(u8_t) LOCK_OPEN);
						KeyTask_TimeLimit[uck] = 0U;
						buzzer(500);
						if ((uck < 8)&&(uci<total_key_flag))
						{
							uik1 |= 1 << uck;
						}
						sprintf(szstr3, "%d,", (uck+1U));
						strcat(szstr2, szstr3);
						ucount++;
					
						prot_log_event((s32_t) EVT_ERGY_KEY_TAKE,(s32_t) 0,0,uck + 1U);
						uitimeout =(u32_t) ((u32_t)sys_t.key_turn_time * 10U);
						lcd_show(0U, 'K', 5U); // Take indicated keys
						break;
					}
				}
				if(key_events_t[uci].status==LOCK_OPEN  && ibutton_value[uci][1]==org_key_data[uci][1] && ibutton_value[uci][2]==org_key_data[uci][2] && ibutton_value[uci][3]==org_key_data[uci][3] && ibutton_value[uci][4]==org_key_data[uci][4])
				{
					red_led[uci]=0;
					green_led[uci]=1;
					lcd_show_str(0U, 'K', 4U, szstr);				// Key x returned
			
					update_key_log((s32_t) user_t.user_id , 0,(s32_t) uci,(u8_t) LOCK_CLOSED);
					KeyTask_TimeLimit[uci] = 0U;
					read_key_sd((s32_t) uci);
					buzzer(500);
					if (key1_t[uci].task_removal_time)
					{
						key1_t[uci].task_removal_time=0U;
						key1_t[uci].flag=0U;
						write_key_sd();
					}
					if ((uci < 8)&&(uci<total_key_flag))//changed from 32 
					{
						uik1 |= 1 << uci;
					}
					sprintf(szstr3, "%d,", (uci+1U));
					strcat(szstr2, szstr3);
					ucount++;
					prot_log_event((s32_t) EVT_ERGY_KEY_RETURN,(s32_t) 0, 0,uci + 1U);
					uitimeout =(u32_t) ((u32_t)sys_t.key_turn_time * 10U);
					lcd_show(0U, 'K', 5U); // Take indicated keys
					break;
				}
				}
				if ((uitimeout%10U) ==0U)
				{
					uctoggle_led = !uctoggle_led; // Blinking LED
				}
				if (key_entered)
				{
					if (uctoggle_led)
					{
						green_led[key_entered-1]=1;
					}
					else
					{
						green_led[key_entered-1]=0;
					}
				}
				lock_state[uci] = ucstate;
				lock_key_state[uci]=lock_state[uci];
			}		
	}
	delay((u32_t) ONE_SEC);
	if (ucount==0U)
	{
		buzzer(1000);
		prot_log_event((s32_t) EVT_NOKEY_TAKEN,(s32_t) user_code_main,0,0);
	}
	for (uci = 0U; uci <(u8_t) max_keys_available; uci++) // Done, so inactivate all LEDs and solenoids
	{
		ucstate = lock_state[uci];
		ucstate &= (u8_t) ~(LOCK_RED | LOCK_SOL | LOCK_GREEN);
		lock_state[uci] = ucstate;
		lock_key_state[uci]=lock_state[uci];
	}
#ifndef NO_DOOR_NO_SOLENOID
	uitimeout =(u32_t) ((u32_t) sys_t.close_door_time * 10U);
	clear_keyposition();
	while (1)
	{
		lcd_show(0U, 'D', 1U); // Please close the door
		sprintf(szstr, "%d", (uitimeout + 8U) / 10U);
		lcd_show_str(1U, 'D', 4U, szstr);//Timer: %s
		if (trg(2) == 0)
		{
			break;
		}
		key1(100U);
		uitimeout--;
		if (uitimeout == 0U)
		{
		
			lcd_show(0U, 'D', 3U); // Close door timeout
			key1((u32_t) MESSAGE_TIMEOUT);
			ULALARMS |=(u32_t) DOOR_OPEN_TIMEOUT;
			card_id_available = 0;
			buzzer(ONE_SEC);
			prot_log_event((s32_t) EVT_DOOR_CLS_TOUT,(s32_t) user_code_main ,0,0U); //ALARM EVT Logged in the event. MLN
			break;
		}
	}
#endif
	return 0;
}



#else
/******************************************************************************
 ** Function name:      remove_return_key
 **
 ** Descriptions:       this function explains the process while key remove/return
 **						if any keys & tasks assigned for the user, they may take/return key
 **						if doesn't assigned, our system will display no keys to remove/return
 ** 
 ** parameters:         option=> to select key remove or return
 **
 ******************************************************************************/
s32_t remove_return_key(s32_t option)
{
	u32_t uik1 =(u32_t) 0;
	u32_t uik2 =(u32_t) 0;
	u32_t uitimeout;
	char  szstr[32];
	char  szstr1[32];
	char  szstr2[50]="";
	char  szstr3[5]="";
	char sms_description[150];
	u8_t  uci,ucj;
	u8_t  uclk[MAX_KEYS];
	u8_t  ucount;
	u8_t  ucstate;
	u8_t  uctoggle_led = 0;
	u8_t login_flag;
	u8_t total_key_flag=0;
	
	ucount = 0U;
	for (uci = 0U; uci <(u8_t) MAX_KEYS; uci++) // Determine number of keys to process
	{
		read_keyevents_sd((s32_t) uci);
		ucstate = lock_state[uci];
		if (option == REMOVE_KEY)
		{
			if ((ucstate !=(u8_t) LOCK_OPEN)&&(key_events_t[uci].status ==(u8_t) LOCK_CLOSED))
			{
				ucount++;
			}
		}
		else 		//IF "option" was to return the key
		{
			if (ucstate &(u8_t) LOCK_OPEN)
			{
				ucount++;
			}
		}
	}
	if (ucount==0U) // No keys, so quit here
	{
		lcd_clr(1U);
		if (option == REMOVE_KEY)
		{
			lcd_show(0U, 'K', 7U); // No keys to remove
		}
		else
		{
			lcd_show(0U, 'K', 8U); // No keys to return
		}
		key1((u32_t) MESSAGE_TIMEOUT);
		return 1;
	}

#ifndef NO_DOOR_NO_SOLENOID
	relay(1, ON);
lock_on(1);
	uitimeout =(u32_t) ((u32_t)sys_t.open_door_time * 10U);

	while (1) // Wait for the door to be opened
	{
		lcd_show(0U, 'D', 0U);
		if (trg(1))
		{
		lock_on(0);
			relay(1, OFF);
			break;
		}
		if (key1(100U) ==ESC)
		{
		lock_on(0);
			relay(1, OFF);
			return 0;
		}
		sprintf(szstr, "%d", (uitimeout + 8U) / 10U);
		lcd_show_str(1U, 'D', 4U, szstr);
		uitimeout--;
		if (uitimeout==0U)
		{
		lock_on(0);
			relay(1, OFF);
			lcd_show(0U, 'D', 2U); // Open door timeout
			key1((u32_t) MESSAGE_TIMEOUT);
			buzzer(1000);
			if (((u32_t) TRG_DOOR_OPENTOUT))//RELAY2 login timed out
			{
				relay(2, ON);
				Cnt_TriggerTimeout = Set_Trigger_Timeout((u32_t) ONE_SEC); // Delay value for 1 sec
			}
			if ((gdashboard_trg &(u32_t) TRG_DOOR_OPENTOUT))//RELAY2 login timed out
			{
				prot_log_event((s32_t) EVT_DOOR_OPEN_TOUT,(s32_t) user_code_main, 0, uik1);

			}
			else
			{
				prot_log_event((s32_t) EVT_DOOR_OPEN_TOUT,(s32_t) user_code_main, 0, uik1);
			}
			
			return 0;
		}
	}
	lock_on(0);

#endif
	for (uci = 0U; uci <(u8_t) MAX_KEYS; uci++)		// Determine which keys to process
	{
		uclk[uci] = 0U;
	}
	for (uci = 0U; uci <(u8_t) MAX_KEYS; uci++)
	{

		read_keyevents_sd((s32_t) uci);
		ucstate = lock_state[uci];
		ucstate &=(u8_t) ~(LOCK_GREEN | LOCK_RED | LOCK_SOL);
		if (option == REMOVE_KEY)
		{
			if ((ucstate !=(u8_t) LOCK_OPEN)&&(key_events_t[uci].status ==(u8_t) LOCK_CLOSED))
			{
				ucstate |=(u8_t) LOCK_GREEN;
				uclk[uci] = 1U;
				total_key_flag=uci+1;
			}
		}
		else
		{
			if ((ucstate &(u8_t) LOCK_OPEN)&&(key_events_t[uci].status ==(u8_t) LOCK_OPEN))
			{
				ucstate |=(u32_t) LOCK_RED;
				uclk[uci] = 1U;
				total_key_flag=uci+1;				
			}
		}
		lock_state[uci] = ucstate;
	}

	uitimeout =(u32_t) ((u32_t)sys_t.key_turn_time * 10U);

	ucount = 0U;
	lock_on(0);
	while (1) // Process keys, one by one
	{

		if (uitimeout<=(u32_t)1)
		{
			uitimeout=0;
			lcd_show(1U, 'K', 2U); // Key handling timeout
			break;
		}
		else
		{
			uitimeout--;
		}

		if (pressed_key)
		{
			pressed_key =(char) 0;
			break;
		}
#ifndef NO_DOOR_NO_SOLENOID
		if (trg(1)==0)
		{
			break;
		}
#endif
		for (uci = 0U; uci <(u8_t) 16; uci++)
		{
			if (uclk[uci])
			{
				break;
			}
		}
		if(uci == total_key_flag)
		{
			break;
		}
		for (uci = 0U; uci <(u8_t) total_key_flag; uci++)			
		{
			ucstate = lock_state[uci];
			ucstate &=(u8_t) ~(LOCK_SOL);
			lock_state[uci] = ucstate;
			lock_key_state[uci]=lock_state[uci];
			if ((ucstate &(u8_t) (LOCK_OPEN | LOCK_CLOSED))==(u8_t) 0)
			{
				if (uclk[uci]) // In between state so user is turning the key
				{
					break;
				}
			}
		}
		if (option == REMOVE_KEY)		
		{
			lcd_show(0U, 'K', 5U); // Take indicated keys
		}
		else
		{
			lcd_show(0U, 'K', 6U); // Return indicated keys
		}
		sprintf(szstr, "%d", (uitimeout + 8U) / 10U);
		lcd_show_str(1U, 'D', 4U, szstr); //Timer: %s

		if (uci <(u8_t) total_key_flag) // Key being turned?	//changed from max keys
		{
			ucstate = lock_state[uci];
			ucstate |=(u8_t) LOCK_SOL;
			lock_state[uci] = ucstate;
			lock_key_state[uci]=lock_state[uci];
			sprintf(szstr, "%d", uci + 1U);
			uitimeout =(u32_t) ((u32_t)sys_t.key_turn_time * 3160U);
			while (1)
			{
				
				if (option == REMOVE_KEY)
				{
					lcd_show_str(0U, 'K', 0U, szstr);//Take key %s
				}
				else
				{
					lcd_show_str(0U, 'K', 1U, szstr);//Return key %s
				}

				lcd_clr(1);
#ifndef NO_DOOR_NO_SOLENOID
				
				if (trg(1)==0)
				{
					break;
				}
#endif				
				if (uitimeout<=1U)
				{
					uitimeout=0;
					lcd_show(1U, 'K', 2U); // Key handling timeout

					break;
				}
				else
				{
					uitimeout--;
				}
				key1(10U);
				sprintf(szstr, "%d", uci + 1U);
				ucstate = lock_state[uci];
				if (option == REMOVE_KEY)
				{
					if (ucstate &(u8_t) LOCK_CLOSED)
					{
						ucstate &=(u8_t) ~(LOCK_RED);
						ucstate |=(u8_t) LOCK_GREEN;
						lock_state[uci] = ucstate;
						lock_key_state[uci]=lock_state[uci];
						uitimeout =(u32_t) ((u32_t)sys_t.key_turn_time * 10U);
						uclk[uci] = 1U;
						break;
					}
					if (ucstate &(u8_t) LOCK_OPEN)
					{
						ucstate &=(u8_t) ~(LOCK_GREEN);
						ucstate |=(u8_t) LOCK_RED;
						lock_state[uci] = ucstate;
						lock_key_state[uci]=lock_state[uci];
						lcd_show_str(0U, 'K', 3U, szstr);		// Key x taken

						update_key_log((s32_t) user_t.user_id , 0,(s32_t) uci,(u8_t) LOCK_OPEN);
						KeyTask_TimeLimit[uci] = 0U;

						if ((uci < 32U)&&(uci<total_key_flag))
						{
							uik1 |= 1 << uci;
						}
						else if(uci>=32)
						{
							uik2 |= (1 << ((u32_t)uci - 32U));
						}
						prot_log_event((s32_t) EVT_ERGY_KEY_TAKE,(s32_t) user_code_main,0,uci+1);
						sprintf(szstr3, "%d,", (uci+1U));
						strcat(szstr2, szstr3);
						ucount++;
						if (ucount==1U)
						{
							if (((u32_t) TRG_EMER_KEYTAKEN))//RELAY2 login timed out
							{
								relay(2, ON);
								Cnt_TriggerTimeout
										= Set_Trigger_Timeout((u32_t) ONE_SEC); // Delay value for 1 sec
							}
						}
					uitimeout =(u32_t) ((u32_t)sys_t.key_turn_time * 10U);
						uclk[uci] = 0U;
						break;
					}
				}
				else
				{
					if (ucstate &(u8_t) LOCK_CLOSED)
					{
						ucstate &=(u8_t) ~(LOCK_RED);
						ucstate |=(u8_t) LOCK_GREEN;
						lock_state[uci] = ucstate;
						lock_key_state[uci]=lock_state[uci];
						uclk[uci] = 0U;
						lcd_show_str(0U, 'K', 4U, szstr);				// Key x returned
						update_key_log((s32_t) user_t.user_id, 0,(s32_t) uci,(u8_t) LOCK_CLOSED);
						KeyTask_TimeLimit[uci] = 0U;
						for(ucj=0;ucj<MAX_KEYS;ucj++)
							read_key_sd((s32_t) ucj);
						if (key1_t[uci].task_removal_time)
						{
							key1_t[uci].task_removal_time=0U;
							key1_t[uci].flag=0U;
							write_key_sd();
						}
						if ((uci < 32)&&(uci<total_key_flag))//chhanged from 32 
						{
							uik1 |= 1 << uci;
						}
						else if(uci>=32)
						{
							uik2 |= (1U << ((u32_t)uci - 32U));
						}
						prot_log_event((s32_t) EVT_ERGY_KEY_RETURN,(s32_t) user_code_main,0,uci+1);
						sprintf(szstr3, "%d,", (uci+1U));
						strcat(szstr2, szstr3);
						ucount++;
						if (ucount==1U)
						{
							if ((u32_t) TRG_EMER_KEYRETURN) //RELAY2 WHEN EMERGENCY KEY ACCESS
							{
								relay(2, ON);
								Cnt_TriggerTimeout
										= Set_Trigger_Timeout((u32_t) ONE_SEC); // Delay value for 1 sec										
							}
						}
					uitimeout =(u32_t) ((u32_t)sys_t.key_turn_time * 10U);
						break;
					}
				}
				if ((uitimeout%100U) ==0U)
				{
					uctoggle_led = !uctoggle_led; // Blinking LED
				}
				if (option == REMOVE_KEY)
				{
					if (uctoggle_led)
					{
						ucstate |= (u8_t)LOCK_GREEN;
					}
					else
					{
						ucstate &=(u8_t) ~(LOCK_GREEN);
					}
				}
				else
				{
					if (uctoggle_led)
					{
						ucstate |= (u8_t)LOCK_RED;
					}
					else
					{
						ucstate &=(u8_t)~(LOCK_RED);
					}
				}
				lock_state[uci] = ucstate;
				lock_key_state[uci]=lock_state[uci];
				if (pressed_key)
				{
					break;
				}
			}
			ucstate &= (u8_t)~(LOCK_SOL);
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
	delay((u32_t) ONE_SEC);
	clear_keyposition();
#ifndef NO_DOOR_NO_SOLENOID
	uitimeout =(u32_t) ((u32_t) sys_t.close_door_time * 10U);
	while (1)
	{
		lcd_show(0U, 'D', 1U); // Please close the door
		sprintf(szstr, "%d", (uitimeout + 8U) / 10U);
		lcd_show_str(1U, 'D', 4U, szstr);//Timer: %s
		if (trg(1) == 0)
		{
			break;
		}
		key1(100U);
		uitimeout--;
		if (uitimeout == 0U)
		{
			lcd_show(0U, 'D', 3U); // Close door timeout
			key1((u32_t) MESSAGE_TIMEOUT);
			ULALARMS |=(u32_t) DOOR_OPEN_TIMEOUT;
			card_id_available = 0;
			buzzer(ONE_SEC);
			if ((u32_t) TRG_DOOR_CLOSETOUT) //RELAY2 TRIGGER WHEN DOOR  OPENED LONG TIME
			{
				relay(2, ON);
				Cnt_TriggerTimeout = Set_Trigger_Timeout((u32_t) ONE_SEC); // Delay value for 1 sec									
			}
			if (gdashboard_trg &(u32_t) TRG_DOOR_CLOSETOUT)
			{
				prot_log_event((s32_t) EVT_DOOR_CLS_TOUT,(s32_t) user_code_main,0, 0U);//ALARM EVT Logged in the event. MLN			
			}
			else 
			{
				prot_log_event((s32_t) EVT_DOOR_CLS_TOUT,(s32_t) user_code_main,0, 0U); //ALARM EVT Logged in the event. MLN
			}
			break;
		}
	}
#endif	
	return 0;

}



#endif
/*-----------------------------------------end of file-------------------------------------------*/

