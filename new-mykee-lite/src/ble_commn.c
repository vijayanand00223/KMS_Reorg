/*********************************************************************
					BLE Communication
*********************************************************************/
#include "menu_main.h"
#include "uart.h"
#include "LED.h"

void ble_invalid_login()
{
	u8_t i=0;
	delay(1000);
	p1char('*');
	send_bledata("+LOGIN");
	p1char(0xFE);
		for(i=0;i<16;i++)
		p1char(0x00);
	p1char('#');
	
	
}
void ble_login_success(void)
{
	u8_t i=0;
	p1char('*');
	send_bledata("+LOGIN");
	p1char(0xFF);
	for(i=0;i<16;i++)
		p1char(0x00);
	p1char('#');

}
void ble_door_open(void)
{
	u8_t i=0;
	p1char('*');
	send_bledata("+DOPEN");
	p1char(0xFF);
	for(i=0;i<16;i++)
		p1char(0x00);
	p1char('#');
}
void ble_serial_no(void)
{
	u8_t i=0;
	read_system_sd();
	p1char('*');
	send_bledata("+SERIA");
	p1char((sys_t.serial&0xFF000000)>>24);
	p1char((sys_t.serial&0xFF0000)>>16);
	p1char((sys_t.serial &0xFF00)>>8);
	p1char((sys_t.serial &0xFF));
	for(i=0;i<13;i++)
		p1char(0x00);
	p1char('#');
	
}
void ble_task_select(u16_t task,u8_t resp)
{
	u8_t i=0,uci=0,j=0;
	p1char('*');
	send_bledata("+TASK:");
	if(!task)
	{
		p1char(resp);
		for(i=0;i<17;i++)
			p1char(0x00);
	}
	else
	{
		p1char(0xFF);
		read_task_status(task);
		for (uci = 0; uci < MAX_KEYS; uci++)
		{
			read_key_sd((s32_t) uci);
			read_keyevents_sd((s32_t) uci);
			
			if ((task_t.key[uci])==1 && (key1_t[uci].enabled && key1_t[uci].enabled!=0xFF))   // changed for task only for key removal
			{
				if(key_events_t[uci].status ==(u8_t) LOCK_CLOSED)
				{
					{
						green_led[uci]=1;
						p1char(0x01);
						j=1;
					}
					
				}
				else if((key_events_t[uci].status ==(u8_t) LOCK_OPEN)&&(key_events_t[uci].exist==(u32_t) EXIST))
				{
					red_led[uci]=1;
					p1char(0x02);
					j=2;
				}
				else
				{
					green_led[uci]=0;
					red_led[uci]=0;
					p1char(0x00);
					j=3;
				}
			}
			else
			{
				green_led[uci]=0;
				red_led[uci]=0;
				p1char(0x00);
				j=4;
			}	
		}		
	}
	p1char('#');

}

/***************************************************end of file****************************/
