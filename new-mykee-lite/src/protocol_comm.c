
#include "menu_main.h"
#include "ticker_itrp.h"
#include "lpc23xx.H"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include "endian.h"
#include "telnet_server.h"
#include "uart.h"
#include "EEPROM.h"
#include "CLOCK_CALNDR.h"

BOOTSECTOR  Boot_memBlock_t;
EVENTS      event_t;
KEY_EVENTS  key_events_t[MAX_KEYS];	

s32_t gpacket_count = 0;
s32_t gpacket_in = 0;
u16_t login_user_pin=0;
u16_t system_count,fw_count,fwdata_count,user_count;


u16_t const CCITT_CRC_TAB[256] = {
  0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
  0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
  0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
  0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
  0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
  0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
  0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
  0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
  0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
  0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
  0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
  0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
  0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
  0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
  0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
  0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
  0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
  0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
  0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
  0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
  0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
  0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
  0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
  0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
  0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
  0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
  0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
  0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
  0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
  0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
  0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
  0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
};


u32_t	glast_event;         //used to store the events for alarm settings
u32_t glast_event_check;   //used to check whether the event is last or not
u32_t	test_arr[32];

unsigned char gprotocol_packetconfig_in_out[600];

char ble_protocol_packet_in[256];


u16_t protocol_length=0;
u8_t evtackcount=0;
u16_t eventcount=0;            //total event receive count for postmethod
u16_t total_comm_time=0;
u8_t segnr=0,tpnr=0;
u8_t factory;
u8_t bluk_flg=0;




void prot_init_event(void)
{
	u32_t i;
	uint8_t flag=0;
	for(i=0;i<MAX_EVENTS;i++)
	{
		read_event_sd(i);
		
		if(event_t.save_event==0xFF)
		{
			glast_event=i;
			break;
		}
		
	}
	if(i==MAX_EVENTS)
		glast_event=0;

	glast_event_check=glast_event;

	for(i=MAX_EVENTS;i>0;i--)
	{
		read_event_sd(i);
		if(event_t.sent_event==0xAA)
		{
			flag=1;
			glast_event_check=i;	
		}
		if(flag==1 &&	event_t.sent_event!=0xAA)
		{
			break;
		}	
	}
}

/******************************************************************************
 ** Function name:      prot_log_event
 **
 ** Descriptions:       update the details related with the events
 **
 ** parameters:         event,user,task,k1,k2,description[2]
 ** 				 
 ** Returned value:     None
 ** 
 ******************************************************************************/
void prot_log_event(s32_t event, s32_t user,s32_t task, u32_t k1)
{
	u32_t evt_block;	
		evt_block=EVENT_BLOCK+(sizeof(EVENTS)*glast_event);  
		if(!(evt_block%0x10000))
		{
			SectorErase(evt_block);
		}
		event_t.ticks =get_ticks_value();
    event_t.code  =(u32_t) event;
    event_t.user  = user;
    event_t.key11  = k1;
		event_t.task=(u16_t)task;
		event_t.save_event=0;
		event_t.sent_event=0xAA;
		write_event_sd(glast_event);
		delay(10);
		read_event_sd(glast_event);
    if (glast_event < MAX_EVENTS)
    {
			glast_event++;
		}
		else 
			glast_event=0;
	  gcommunication_time=10;
}
/******************************************************************************
 ** Function name:      update_key_log
 **
 ** Descriptions:       used to update the status of keys for tasks
 **
 ** parameters:         user01, task, key_num, key_state
 ** 				 
 ** Returned value:     None
 ** 
 ******************************************************************************/
void update_key_log(s32_t ususer01, s32_t uctask,s32_t uckey_num, u8_t key_state)
{
	char szstr[50];
	if(uckey_num > max_keys_available)
	{
		return;
	}	

	read_keyevents_sd(uckey_num);
	

	if(ginstaller_code!=ususer01)
	{	
		key_events_t[uckey_num].user =(u16_t) ususer01;
		key_events_t[uckey_num].card_id=0;//truck_card_id;
		key_events_t[uckey_num].user_pin=login_user_pin;
	}
	else
	{
		key_events_t[uckey_num].user =(u16_t)ginstaller_code;
		key_events_t[uckey_num].card_id=0;	
	}
	key_events_t[uckey_num].exist =(u32_t) EXIST;
	key_events_t[uckey_num].code =((u32_t) (KEY_EVENT_CODE) | ((u32_t)uckey_num+1));
	key_events_t[uckey_num].task =(u8_t) uctask;
	key_events_t[uckey_num].ticks=endtick_value;
	lock_key_state[uckey_num]= lock_state[uckey_num]&(u8_t)(LOCK_CLOSED|LOCK_OPEN);	
	key_events_t[uckey_num].status = key_state;
	
	if(key_state ==(u8_t) LOCK_OPEN)
	{		
		pstr("Key Removed");
	}
	else if(key_state ==(u8_t) LOCK_CLOSED)
	{
		pstr("Key Available");
	}
	else if(key_state ==(u8_t) LOCK_NOT_FOUND)
	{
		pstr("Key Not Sensed");
	}
	else
	{
	}

	pstr(szstr);
	write_keyevents_sd();	
}

s32_t free_user(u32_t user_code)
{
	u16_t i;
	for(i=0;i<MAX_USERS_LOCATION;i++)
	{
		read_user_sd(i);
		if(user_t.user_id==0xFFFFFFFF)
			return i;
	}
	return -1;
}
u8_t delete_user(u32_t user_code)
{
	u16_t i;
	for(i=0;i<MAX_USERS_LOCATION;i++)
	{
		read_user_sd(i);
		if(user_t.user_id  ==user_code)
		{
			user_t.user_id  =user_t.card=0;
			write_user_sd(i);
			if(!(sys_t.self_user_allign &0x01))
			{
				sys_t.self_user_allign|=0x01;
				write_system_sd();
			}
			return 1;
		}
	}
	return 0;
}
 int modify_user(u32_t uicode)
{
  u16_t uci;	
   for (uci = 0; uci < MAX_USERS_LOCATION; uci++)			//if user already have card and change to new card,
   {
		read_user_sd((s32_t)uci);	
		if(user_t.user_id==uicode)					//modify card value  in the same location (no need to add in new location)
		{
			user_t.user_id=0;
			user_t.card=0;
			write_user_sd((s32_t)uci);

			if(!user_t.card && !(sys_t.self_user_allign &0x01))
			{
				pstr("zero card value\r");
				sys_t.self_user_allign=0x01;
				write_system_sd();
			}		
		}
			if (user_t.user_id  == 0xFFFFFFFF)
			{
				pstr("\rOK\r");
				return(uci);			
			}	
	}
		return -2;
	}

s16_t add_task(u16_t taskid)
{
	u16_t i;
	for(i=0;i<MAX_TASKS;i++)
	{
		read_task_sd(i);
		if(task_t.task_id==taskid)
		{
			task_t.task_id=0;
			write_task_sd(i);
		}
		if(task_t.task_id==0xFFFF)
		{
			return i;
		}
			
	}
	return -1;
}
u8_t delete_task(u16_t task_id)
{
	u16_t i;
	for(i=0;i<MAX_TASKS;i++)
	{
		read_task_sd(i);
		if(task_t.task_id==task_id)
		{
			task_t.task_id=0;
			write_task_sd(i);
			return 1;
		}

	}
	return 0;
}
s16_t read_task_status(u16_t taskid)
{
	u16_t i;
	for(i=0;i<MAX_TASKS;i++)
	{
		read_task_sd(i);
		if(task_t.task_id==taskid)
			return i;
		if(task_t.task_id==0xFFFF)
			return -1;
	}
	return -1;
}
s16_t read_user_status(u16_t user_code)
{
	u16_t i;
	for(i=0;i<MAX_USERS_LOCATION;i++)
	{
		read_user_sd(i);
		if(user_t.user_id ==user_code)
			return i;
	}
	return -1;
}
void fw_data(u8_t value)
{
	static u8_t fwval[2],i;
	u32_t fwack=0;
	char szstr[32];
	fwval[fw_count]=value;
	fw_count++;
	
	sprintf(szstr,"%X,%X\r",fwval[i],fwval[i+4]);

	if(fw_count==4)
	{
		for(i=0;i<4;i++)
		{
			fwack<<=4;
			if(fwval[i]>=0x30 && fwval[i]<=0x39 )
				fwack|=fwval[i]-0x30;
			else if(fwval[i]>=0x41 && fwval[i]<=0x46)
				fwack|=fwval[i]-0x37;
			else if(fwval[i]>=0x61 && fwval[i]<=0x66)
				fwack|=fwval[i]-0x61;
			else
				fwack|=0;
		}
	}
}


/*-----------------------------------------end of file-------------------------------------------*/
