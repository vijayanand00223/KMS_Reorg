/*
 * File name:mci_menu
 * 		This file contains all related read and write sd card function.
 *
 * Version:V02.00.00 -->code restructure. 
 * 
 * Copyright (c) 2004-2011 Sterna Security.
 * All rights reserved.
 * Author: R.Narayanan
 */
#include "lpc23xx.H"
#include "menu_main.h"
#include "Sbl_config.h"
#include "Sbl_iap.h"
#include "uart.h"
#include "EEPROM.h"
#include "LCD.h"
#include "LED.h"

u8_t writevalue[256];


/******************************************************************************
 ** Function name:      writedefaults_sdcard
 **
 ** Descriptions:       the default values for user, task, keys, system
 **						 & time values are write in the SD card
 **
 ** parameters:         none
 ** 				 
 ** Returned value:     none
 ** 
 ******************************************************************************/
void writedefaults_sdcard(void)
{
	s32_t itemp_key;
  u32_t usi;
	u32_t uiserial_temp;

	char szstr1[50];
	if(!serial_flg)
	{
		lcd_show(0, 'V', 7);
		lcd_show(1, 'V', 8);
		if (!get_menu_key(&itemp_key))
		{
			lcd_show(1,'V',27);
			delay(MESSAGE_TIMEOUT);
			return;
		}
		lcd_show(0, 'J', 3);
		lcd_show(1, 'V', 13);
		if(read_code(HIDE_INPUT)!=1980)
		{
			lcd_show(0, 'J', 3);	
			lcd_show(1, 'S', 16);	// invalid service code 
			delay(MESSAGE_TIMEOUT);
			lcd_show(1,'V',27);
			delay(MESSAGE_TIMEOUT);
			return;
		}
	}
  lcd_show(0, 'V', 11);
	lcd_show(1,'H',0);



	memset(&user_t, 0, sizeof(USERS));
   
  for (usi = 0xC; usi < 0x1F; usi++)
  {
		sprintf(szstr1,"sector:%X\r",usi*0x010000);
		pstr(szstr1);
    SectorErase(usi*0x010000);
  }
	read_comm_sd();	
	sprintf(comm_t.url,"mtkm.sternaemlock.com");
	sprintf(comm_t.url1,"mtkm.sternaemlock.com");
	write_comm_sd();
	read_system_sd();
	uiserial_temp = sys_t.serial;
  sys_t.date_display = 1;
  sys_t.time_display = 2;
  sys_t.dst = 2;
  sys_t.key_turn_time = 30;
  sys_t.open_door_time = 30;
  sys_t.close_door_time = 30;
  sys_t.alarm_enable = 1;
  sys_t.hooter_enable = 1;
  sys_t.network = 1;
  sys_t.dhcp = 2;
	sys_t.self_user_allign=0;
  sys_t.trigger = 0x00000000; //Lower 16bits are for Event Trigger and Upper word is for Alarm Trigger
  sys_t.trigger =TRG_BURGLARY_ALM;
  sys_t.dashbrd_trigger = 0x00000000; 
	sys_t.dashbrd_trigger=(TRG_BURGLARY_ALM|TRG_HIGHSEC_REMOVE|TRG_TASK_OVERDUE);
  
	sys_t.telnet_port =23;
	sys_t.serial =uiserial_temp;
	sys_t.standalone =0x00;
	sys_t.factory =1;
	sys_t.e_count=0;
  write_system_sd();
	lcd_show(0, 'e', 15); //Key events created
	lcd_show(1, 'e', 16); //All user can return keys
#ifdef IBUTTON	
	total_register();
#endif	
	create_keyevents();		
	glast_event = 0;                             //next event starting in 1
	glast_event_check=0;     
	delay(5);
	lcd_clr(0);
	lcd_clr(1);
	delay(5);
	system_restart();
}

/******************************************************************************
 ** Function name:      eraseboot_sdcard
 **
 ** Descriptions:       this function used to erase the boot codings of SD card
 **
 ** parameters:         none
 ** 				 
 ** Returned value:     none
 ** 
 ******************************************************************************/
void eraseboot_sdcard(void)
{
	u8_t usi;	
	for(usi=0;usi<(BOOT_BLOCK_SIZE);usi++)
	{
		 SectorErase(BOOT_BLOCK+usi);
	}
}



/******************************************************************************
 ** Function name:      create_keyevents
 **
 ** Descriptions:       this function used to find the key positions
 **						& write that events to SD card
 **
 ** parameters:         none
 ** 				 
 ** Returned value:     none
 ** 
 ******************************************************************************/
#ifdef IBUTTON 
void create_keyevents(void)
{
	u8_t uci;
	u8_t  ucstate; //Temporary Copy of the Key Status 
	char szstr[32];
	SectorErase(KEYS_EVNT_BLOCK);

		i_button_read=1;
		while(i_button_read);

	for(uci=0; uci<max_keys_available; uci++)
	{
		key_events_t[uci].exist = EXIST;
		key_events_t[uci].code = KEY_EVENT_CODE | (uci+1);
		key_events_t[uci].user =(u16_t)  ginstaller_code;
		key_events_t[uci].task = 0;

	  key_events_t[uci].ticks=endtick_value;
		if (ibutton_value[uci][0]!=0xFF && ibutton_value[uci][1]!=0xFF && ibutton_value[uci][2]!=0xFF)
		{
			key_events_t[uci].status = LOCK_CLOSED;
			pstr("Key Available");
			green_led[uci]=1;
		}
		else if (ibutton_value[uci][0]==0xFF && ibutton_value[uci][1]==0xFF && ibutton_value[uci][2]==0xFF)
		{
			key_events_t[uci].status = LOCK_OPEN;
			pstr("Key Removed");
			red_led[uci]=1;
		}
		else if (ibutton_value[uci][0]==0x00 && ibutton_value[uci][1]==0x00 && ibutton_value[uci][2]==0x00 && ibutton_value[uci][3]==0x00 && ibutton_value[uci][4]==0x00)
		{
			key_events_t[uci].status = LOCK_NOT_FOUND;
			pstr("Key Not Sensed");
		}
		else
		{
			sprintf(szstr,"%d-%d-%d-%d-%d-%d\r",uci,ibutton_value[uci][0],ibutton_value[uci][1],ibutton_value[uci][2],ibutton_value[uci][3],ibutton_value[uci][4]);
			pstr(szstr);
		}
		sprintf(szstr,"%d-",uci);
		pstr(szstr);
		
	}
	write_keyevents_sd();
	for(uci=0;uci<MAX_KEYS;uci++)
	{
		green_led[uci]=0;
		red_led[uci]=0;
		solenoid[uci]=1;
	}
}

#else
void create_keyevents(void)
{
	u8_t uci;
	u8_t  ucstate; //Temporary Copy of the Key Status 
	char szstr[32];
	SectorErase(KEYS_EVNT_BLOCK);

	for(uci=0; uci<MAX_KEYS; uci++)
	{
		ucstate = lock_state[uci];

		lock_key_state[uci]= (lock_state[uci]&(LOCK_CLOSED|LOCK_OPEN));
		key_events_t[uci].exist = EXIST;
		key_events_t[uci].code = KEY_EVENT_CODE | (uci+1);
		key_events_t[uci].user =(u16_t)  ginstaller_code;
		key_events_t[uci].task = 0;
	  key_events_t[uci].ticks=endtick_value;
		if (ucstate & LOCK_CLOSED)
		{
			key_events_t[uci].status = LOCK_CLOSED;
			sprintf(szstr ,"\rKey %d Key Available",uci);
			pstr(szstr);
		}
		else if (ucstate & LOCK_OPEN)
		{
			key_events_t[uci].status = LOCK_OPEN;
			sprintf(szstr ,"\rKey %d Key Removed",uci);
			pstr(szstr);
		}
		else 
		{
			key_events_t[uci].status = LOCK_NOT_FOUND;
			sprintf(szstr ,"\rKey %d Key Not Sensed",uci);
			pstr(szstr);
		}
	
	}
	write_keyevents_sd();
}

#endif
/******************************************************************************
 ** Function name:      read_user_sd
 **
 ** Descriptions:       read the user data from SD card
 **
 ** parameters:         block=>identify the location to read from the SD card
 ** 				 
 ** Returned value:     none
 ** 
 ******************************************************************************/
void read_user_sd(s32_t block)
{
		ReadBytes(USER_BLOCK+(sizeof(USERS)*block), writevalue, sizeof(USERS));		
    memcpy((void *)&user_t, &writevalue, sizeof(USERS));
}

/******************************************************************************
 ** Function name:      write_user_sd
 **
 ** Descriptions:       write the user data to SD card
 **
 ** parameters:         block=>identify the location to save in the SD card
 ** 				 
 ** Returned value:     none
 ** 
 ******************************************************************************/
void write_user_sd(s32_t block)
{
		memcpy((void *)writevalue,&user_t,sizeof(USERS));
		WrBytes(USER_BLOCK+(sizeof(USERS)*block), writevalue, sizeof(USERS));	
}

void read_userbkp_sd(s32_t block)
{
		ReadBytes(BACKUP_BLOCK+(sizeof(USERS)*block), writevalue, sizeof(USERS));		
    memcpy((void *)&user_t, &writevalue, sizeof(USERS));
}

void write_userbkp_sd(s32_t block)
{
		memcpy((void *)writevalue,&user_t,sizeof(USERS));
		WrBytes(BACKUP_BLOCK+(sizeof(USERS)*block), writevalue, sizeof(USERS));	
}
/******************************************************************************
 ** Function name:      read_task_sd
 **
 ** Descriptions:       read the task data from SD card
 **
 ** parameters:         block=>identify the location to read from the SD card
 ** 				 
 ** Returned value:     none
 ** 
 ******************************************************************************/
void read_task_sd(s32_t block)
{
		ReadBytes(TASK_BLOCK+(sizeof(TASKS)*block), writevalue, sizeof(TASKS));		
    memcpy((void *)&task_t, &writevalue, sizeof(TASKS));	

}

/******************************************************************************
 ** Function name:      write_task_sd
 **
 ** Descriptions:       write the task data to SD card
 **
 ** parameters:         block=>identify the location to save in the SD card
 ** 				 
 ** Returned value:     none
 ** 
 ******************************************************************************/
void write_task_sd(s32_t block)
{
		memcpy((void *) writevalue,(void *) &task_t,sizeof(TASKS));
		WrBytes(TASK_BLOCK+(sizeof(TASKS)*block), writevalue, sizeof(TASKS));
}
void read_taskbkp_sd(s32_t block)
{
		ReadBytes(BACKUP_BLOCK+(sizeof(TASKS)*block), writevalue, sizeof(TASKS));		
    memcpy((void *)&task_t, &writevalue, sizeof(TASKS));
	
}

void write_taskbkp_sd(s32_t block)
{
		memcpy((void *)writevalue,&task_t,sizeof(TASKS));
		WrBytes(BACKUP_BLOCK+(sizeof(TASKS)*block), writevalue, sizeof(TASKS));	
}
void read_userbkpbio1_sd(s32_t block)
{
		ReadBytes(BACKUP_BLOCK+(sizeof(USERS)*block), writevalue, sizeof(USERS));		
    memcpy(&user_t, &writevalue, sizeof(USERS));
}
void write_userbkpbio1_sd(s32_t block)
{
		memcpy(writevalue,&user_t,sizeof(USERS));
		WrBytes(BACKUP_BLOCK+(sizeof(USERS)*block), writevalue, sizeof(USERS));
}
/******************************************************************************
 ** Function name:      read_keyevents_sd
 **
 ** Descriptions:       read the key operation details from SD card
 **
 ** parameters:         block=>identify the location to read from the SD card
 ** 				 
 ** Returned value:     none
 ** 
 ******************************************************************************/
void read_keyevents_sd(s32_t block)
{
		ReadBytes(KEYS_EVNT_BLOCK+(sizeof(KEY_EVENTS)*block), writevalue, sizeof(KEY_EVENTS));		
    memcpy((void *) &key_events_t[block], (void *) writevalue, sizeof(KEY_EVENTS));
}

/******************************************************************************
 ** Function name:      write_keyevents_sd
 **
 ** Descriptions:       write the key operation details to SD card
 **
 ** parameters:         block=>identify the location to save in the SD card
 ** 				 
 ** Returned value:     none
 ** 
 ******************************************************************************/
void write_keyevents_sd(void)
{
	  u8_t uci;
		SectorErase(KEYS_EVNT_BLOCK);
		for(uci=0;uci<max_keys_available;uci++)
		{
			memcpy((void *) writevalue, (void *) &key_events_t[uci], sizeof(KEY_EVENTS));
			WrBytes(KEYS_EVNT_BLOCK+(sizeof(KEY_EVENTS)*uci), writevalue, sizeof(KEY_EVENTS));	
		}
}

/******************************************************************************
 ** Function name:      read_event_sd
 **
 ** Descriptions:       read the events details from SD card
 **
 ** parameters:         block=>identify the location to read from the SD card
 ** 				 
 ** Returned value:     none
 ** 
 ******************************************************************************/
void read_event_sd(s32_t block)
{
		ReadBytes(EVENT_BLOCK+(sizeof(EVENTS)*block), writevalue, sizeof(EVENTS));		
    memcpy((void *) &event_t, (void *) writevalue, sizeof(EVENTS));
}
/******************************************************************************
 ** Function name:      write_event_sd
 **
 ** Descriptions:       write the events details to SD card
 **
 ** parameters:         block=>identify the location to save in the SD card
 ** 				 
 ** Returned value:     none
 ** 
 ******************************************************************************/
void write_event_sd(s32_t block)
{
u32_t evt_block=EVENT_BLOCK+(sizeof(EVENTS)*block);	

		memcpy(writevalue,&event_t,sizeof(EVENTS));
		WrBytes(evt_block, writevalue, sizeof(EVENTS));
}
/******************************************************************************
 ** Function name:      read_key_sd
 **
 ** Descriptions:       read the key details from SD card
 **
 ** parameters:         block=>identify the location to read from the SD card
 ** 				 
 ** Returned value:     none
 ** 
 ******************************************************************************/
void read_key_sd(s32_t block)
{
		ReadBytes(KEY_BLOCK+(sizeof(KEYS)*block), writevalue, sizeof(KEYS));		
    memcpy(&key1_t[block],writevalue, sizeof(KEYS));
}

/******************************************************************************
 ** Function name:      write_key_sd
 **
 ** Descriptions:       write the key details to SD card
 **
 ** parameters:         block=>identify the location to write in the SD card
 ** 				 
 ** Returned value:     none
 ** 
 ******************************************************************************/
void write_key_sd(void)
{
	u8_t uci;
	SectorErase(KEY_BLOCK);
	for(uci=0;uci<max_keys_available;uci++)
	{
    memcpy(writevalue, &key1_t[uci], sizeof(KEYS));
		WrBytes(KEY_BLOCK+(sizeof(KEYS)*uci), writevalue, sizeof(KEYS));	
	}
}
/******************************************************************************
 ** Function name:      read_boot_sd
 **
 ** Descriptions:       read the boot codings from SD card
 **
 ** parameters:         block=>identify the location to read from the SD card
 ** 				 
 ** Returned value:     none
 ** 
 ******************************************************************************/
void read_boot_sd(s32_t block)
{
		ReadBytes(BOOT_BLOCK+(sizeof(BOOTSECTOR)*block), writevalue, sizeof(BOOTSECTOR));	
    memcpy(&Boot_memBlock_t, &writevalue, sizeof(BOOTSECTOR));
}

/******************************************************************************
 ** Function name:      write_boot_sd
 **
 ** Descriptions:       write the boot codings to SD card
 **
 ** parameters:         block=>identify the location to write in the SD card
 ** 				 
 ** Returned value:     none
 ** 
 ******************************************************************************/
void write_boot_sd(s32_t block)
{
		memcpy(writevalue,&Boot_memBlock_t,sizeof(BOOTSECTOR));
		WrBytes(BOOT_BLOCK+(sizeof(BOOTSECTOR)*block), writevalue, sizeof(BOOTSECTOR));
}

/******************************************************************************
 ** Function name:      read_system_sd
 **
 ** Descriptions:       read the system details from SD card
 **
 ** parameters:         none
 ** 				 
 ** Returned value:     none
 ** 
 ******************************************************************************/
void read_system_sd(void)
{
	ReadBytes(SYS_BLOCK, writevalue, sizeof(SYSTEM));		
	memcpy((void *) &sys_t, (void *) writevalue, sizeof(SYSTEM));
	
}

/******************************************************************************
 ** Function name:      write_system_sd
 **
 ** Descriptions:       write the system details to SD card
 **
 ** parameters:         none
 ** 				 
 ** Returned value:     none
 ** 
 ******************************************************************************/
void write_system_sd(void)
{   
	sys_erase_flg=1;
	SectorErase(SYS_BLOCK);
   memcpy((void *)writevalue, (void *) &sys_t, sizeof(SYSTEM));
  WrBytes(SYS_BLOCK, writevalue, sizeof(SYSTEM));
	sys_erase_flg=0;
}

/******************************************************************************
 ** Function name:      read_mac_sd
 **
 ** Descriptions:       read the MAC address from SD card
 **
 ** parameters:         none
 ** 				 
 ** Returned value:     none
 ** 
 ******************************************************************************/
void read_mac_sd(void)
{
	  ReadBytes(MAC_BLOCK, writevalue, sizeof(MAC));		
    memcpy((void *) &mac_t, (void *) writevalue, sizeof(MAC));
}

/******************************************************************************
 ** Function name:      write_mac_sd
 **
 ** Descriptions:       write the MAC address to SD card
 **
 ** parameters:         none
 ** 				 
 ** Returned value:     none
 ** 
 ******************************************************************************/
void write_mac_sd(void)
{   
		SectorErase(MAC_BLOCK);
    memcpy((void *)writevalue, (void *) &mac_t, sizeof(MAC));
		WrBytes(MAC_BLOCK, writevalue, sizeof(MAC));		

}
/******************************************************************************
 ** Function name:      read_comm_sd
 **
 ** Descriptions:       read the comm details from SD card
 **
 ** parameters:         none
 ** 				 
 ** Returned value:     none
 ** 
 ******************************************************************************/
void read_comm_sd(void)
{
	ReadBytes(COMM_BLOCK, writevalue, sizeof(COMM));		
	memcpy((void *) &comm_t, (void *) writevalue, sizeof(COMM));
}

/******************************************************************************
 ** Function name:      write_comm_sd
 **
 ** Descriptions:       write the comm details to SD card
 **
 ** parameters:         none
 ** 				 
 ** Returned value:     none
 ** 
 ******************************************************************************/
void write_comm_sd(void)
{   
	SectorErase(COMM_BLOCK);
   memcpy((void *)writevalue, (void *) &comm_t, sizeof(COMM));
  WrBytes(COMM_BLOCK, writevalue, sizeof(COMM));		

}

void delete_backup_sector(void)
{
	u8_t i;
	for(i=0;i<MAX_USERS_BLOCK;i++)
	{
		SectorErase(BACKUP_BLOCK+(i*1000));
	}
}
void delete_user_sector(void)
{
	u8_t i;
	for(i=0;i<MAX_USERS_BLOCK;i++)
	{
		SectorErase(USER_BLOCK+(i*1000));
	}
}
void self_user_allign_bio(void)
{
	u32_t i,count=0;
	lcd_str(0,"User bio self alignment");
	lcd_str(1,"Please wait");
	delay(MESSAGE_TIMEOUT);
	delete_backup_sector();
	for(i=0;i<MAX_USERS_LOCATION;i++)
	{
		read_user_sd(i);
		if(user_t.user_id ==-1)
			break ;
		write_userbkp_sd(i);
	}
	delete_user_sector();
	for(i=0;i<MAX_USERS_LOCATION;i++)
	{
		read_userbkp_sd(i);
		if(user_t.user_id !=0)
		{
			if(user_t.finger_enabled==0)
				user_t.finger_enabled=0xFF;
			write_user_sd(count);
			count++;
		}
		if(user_t.user_id ==0xFFFFFFFF)
			break;
	}
}

void self_user_allign(void)
{
	u32_t i,count=0;
	lcd_str(0,"User self alignment");
	lcd_str(1,"Please wait");
	delete_backup_sector();
	for(i=0;i<MAX_USERS_LOCATION;i++)
	{
		read_user_sd(i);
		if(user_t.user_id ==-1)
			break ;
		write_userbkp_sd(i);
	}
	delete_user_sector();
	for(i=0;i<MAX_USERS_LOCATION;i++)
	{
		read_userbkp_sd(i);
		if(user_t.user_id !=0)
		{
			write_user_sd(count);
			count++;
		}
		if(user_t.user_id ==0xFFFFFFFF)
			break;
	}
}
void selftask_allign(void)
{
	u32_t i,count=0;
	lcd_str(0,"Task self alignment");
	lcd_str(1,"Please wait");
	delete_backup_sector();
	for(i=0;i<MAX_TASKS;i++)
	{
		read_task_sd(i);
		write_taskbkp_sd(i);
	}
	SectorErase(TASK_BLOCK);
	SectorErase(TASK_BLOCK+0x10000);
	for(i=0;i<MAX_TASKS;i++)
	{
		read_taskbkp_sd(i);
		if(task_t.task_id !=0)
		{
			write_task_sd(count);
			count++;
		}
		if(task_t.task_id==0xFFFF)
			break;

	}
}
int hex_val(int pos)
{
    char ch;
    ch = writevalue[pos];    
    ch = toupper(ch);    
    if (!isxdigit(ch))
        return 0;

    if (ch >= 'A')
    {
        return (ch - 55);
    }
    else
    {
        return (ch - '0');
    }
}
u32_t get_intvalue(int pos,int Cnt)
{
	int  i; 
	u32_t res = 0;

	for (i = 0; i < Cnt; i++)
	{
		res <<= 4;
		res |= hex_val(pos + i);
	}

	return res;
}


/******************************************************************************
 ** Function name:      view_mac_address
 **
 ** Descriptions:       this function used for view MAC address
 **  
 ** parameters:         none
 **
 ** Returned value:     none
 ** 
 ******************************************************************************/
void view_mac_address(void)
{
	s32_t itemp_key;
	char szmac_str[32]={""};
	char szstr[32]={""};
	u8_t i,return_flg;
	
	read_mac_sd();
	sprintf(szstr,"%04X",mac_t.add_MAC0);
	strcpy(szmac_str,szstr);
	sprintf(szstr,"%04X",mac_t.add_MAC1);
	strcat(szmac_str,szstr);
	sprintf(szstr,"%04X",mac_t.add_MAC2);
	strcat(szmac_str,szstr);
	lcd_show(0,'n',9);
	lcd_str(1,szmac_str);
	if (!get_menu_key(&itemp_key))
	{
		lcd_show(1,'V',27);
		delay(MESSAGE_TIMEOUT);
		return;
	}
	lcd_show(0,'n',9);
	lcd_show(1, 'V', 13);
	if(read_code(HIDE_INPUT)==1704)
	{
		lcd_str(1,"");
		strcpy(writevalue,"");
		return_flg=read_hexavalue(writevalue,12);

		if(return_flg<12 || return_flg==-1)
			return;
		for( i=0;i<12;i++)
		{

		}
			mac_t.add_MAC0=get_intvalue(0,4);	
			mac_t.add_MAC1=get_intvalue(4,4);
			mac_t.add_MAC2=get_intvalue(8,4);
			write_mac_sd();
			system_restart();
	}
	else
		return;
}
/******************
 read total event

****************/

void read_totalevent_sd(void)
{
		read_system_sd();
}
/******************
write  total event

****************/
void write_totalevent_sd(void )
{ 
    write_system_sd();
}

/*-----------------------------------------end of file-------------------------------------------*/
