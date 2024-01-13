#include"menu_main.h"
#include "uart.h"
#include "EEPROM.h"
#include "LCD.h"
#include "BUZZER.h"

#define CMD_DELETE_DB 		0x78
#define CMD_CREATE_DB			0x70
#define CMD_WORK_DB				0x71
#define CMD_CHECK_STATUS	0x8B
#define CMD_ENROLLMENT		0x76
#define CMD_GETRESULT			0x7B
#define CMD_IDENTITY			0x79	
#define CMD_DELETE_USER		0x7C
#define CMD_ERROR_DELETE	0xE0
#define CMD_GETCONFIG			0x49
#define CMD_SET_CMD				0x4D
#define CMD_SETCOMPORT		0x56
#define CMD_ARM_TRIGGER		0x47
#define CMD_GET_USERREC		0x74
#define CMD_SET_USERREC		0x75

u16_t lumi_count=0;

const uint8_t delete_db[24]			  ={0x06,0x06,0x12,0x00,0x00,0x00,0x0D,0x56,0x78,0x00,0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
const uint8_t create_db[52]			  ={0x06,0x06,0x2E,0x00,0x00,0x00,0x0D,0x56,0x70,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x64,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x11,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
const uint8_t work_db[24]				  ={0x06,0x06,0x12,0x00,0x00,0x00,0x0D,0x56,0x71,0x00,0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};	
const uint8_t check_status[20]	  ={0x06,0x06,0x0E,0x00,0x00,0x00,0x0D,0x56,0x8B,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
const uint8_t user_identity[20]   ={0x06,0x06,0x0E,0x00,0x00,0x00,0x0D,0x56,0x79,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
const uint8_t get_result[20]			={0x06,0x06,0x0E,0x00,0x00,0x00,0x0D,0x56,0x7B,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
const uint8_t get_config[24] 			={0x06,0x06,0x12,0x00,0x00,0x00,0x0D,0x56,0x49,0x00,0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x00};
const uint8_t setcommand[88]	    ={0x06,0x06,0x52,0x00,0x00,0x00,0x0D,0x56,0x4D,0x00,0x00,0x00,0x00,0x00,0x44,0x00,0x00,0x00,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x40,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
const uint8_t config_port[28]     ={0x06,0x06,0x16,0x00,0x00,0x00,0x0d,0x56,0x56,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x00,0xc2,0x01,0x00,0x00,0x00};	//baud rate 115200
const uint8_t arm_trigger[20]			={0x06,0x06,0x0E,0x00,0x00,0x00,0x0D,0x56,0x47,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

uint8_t user_enrollment[64]={0x06,0x06,0x3A,0x00,0x00,0x00,0x0D,0x56,0x76,0x00,0x00,0x00,0x00,0x00,0x2C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00};
uint8_t delete_userrecords[64]={0x06,0x06,0x3A,0x00,0x00,0x00,0x0D,0x56,0x7C,0x00,0x00,0x00,0x00,0x00,0x2C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00};	


void send_lumidign_command(u8_t command)
{
	u16_t i;
					
	 lumi_count=0;
	switch (command)
	{
		case CMD_DELETE_DB:
			for(i=0;i<24;i++)
				send_biometric(delete_db[i]);		
			break;
		case CMD_CREATE_DB:
			for(i=0;i<52;i++)
				send_biometric(create_db[i]);
		break;
		case CMD_WORK_DB:
			for(i=0;i<24;i++)
				send_biometric(work_db[i]);				
			break;
		case CMD_CHECK_STATUS:
			for(i=0;i<24;i++)
				send_biometric(check_status[i]);			
			break;
		case CMD_ENROLLMENT:
			for(i=0;i<64;i++)
					send_biometric(user_enrollment[i]);	
			break;
		case CMD_GETRESULT:
			for(i=0;i<20;i++)
					send_biometric(get_result[i]);
			break;
		case CMD_IDENTITY:
			for(i=0;i<20;i++)
				send_biometric(user_identity[i]);	
			break;
		case CMD_DELETE_USER:
			for(i=0;i<64;i++)
					send_biometric(delete_userrecords[i]);	
			break;
		case CMD_GETCONFIG:
				for(i=0;i<24;i++)
				send_biometric(get_config[i]);		
		break;	
		case CMD_SET_CMD:
				for(i=0;i<88;i++)
				send_biometric(setcommand[i]);				
		break;
		case CMD_SETCOMPORT:
				for(i=0;i<28;i++)
					send_biometric(config_port[i]);
		break;
		case CMD_ARM_TRIGGER:
				for(i=0;i<20;i++)
					send_biometric(arm_trigger[i]);				
			break;	
	}
}
u8_t get_length(u8_t  cmd)
{
	u16_t return_value;
	switch(cmd)
	{
		case CMD_DELETE_DB:
		case CMD_CREATE_DB:
		case CMD_WORK_DB:
		case CMD_ENROLLMENT:
		case CMD_IDENTITY:
		case CMD_DELETE_USER:
		case CMD_ERROR_DELETE:
		case CMD_SET_CMD:		
		case CMD_SETCOMPORT:	
		case CMD_ARM_TRIGGER:
			return_value= 0x0E;
			break;
		case CMD_CHECK_STATUS:
			return_value=0x1E;
			break;
		case CMD_GETRESULT:
			return_value=0x52;
			break;
		case CMD_GETCONFIG:
			return_value=0x8E;
			break;
		
	}	
	return return_value;

}
u8_t protocol_verification()
{
	u16_t test_var=0;
	
	test_var=(gprotocol_packetconfig_in_out[0]|((gprotocol_packetconfig_in_out[1]<<8)&0xFF00)|((gprotocol_packetconfig_in_out[2]<<16)&0xFF0000)|((gprotocol_packetconfig_in_out[3]<<24)&0xFF000000));
	if(get_length(gprotocol_packetconfig_in_out[6])!=test_var)
	{
		p1str("Length Mismatch\r");
		return 0;
	}
	
	if(gprotocol_packetconfig_in_out[4]!=0x0D||gprotocol_packetconfig_in_out[5]!=0x56)
	{
		p1str("Header Mismatch\r");
		return 0;
	}
	return 1;
}	
s8_t check_status_lumi(u8_t command)
{
	u8_t i,count=0;
	char szstr[32];
		send_lumidign_command(CMD_CHECK_STATUS);
			while(count++<20)
		{
			if(lumi_count==34)
			{				
				sprintf(szstr,"respstatus:%d\r",lumi_count);
				
				for(i=0;i<lumi_count;i++)
				{
						sprintf(szstr,"%02X",gprotocol_packetconfig_in_out[i]);
				}		
				lumi_count=0;
				if(!protocol_verification())
					return 0;
					sprintf(szstr,"command %02X-%02X\r",gprotocol_packetconfig_in_out[16],command);
						sprintf(szstr,"response %02X\r",gprotocol_packetconfig_in_out[20]);
				if(gprotocol_packetconfig_in_out[16]==CMD_IDENTITY &&gprotocol_packetconfig_in_out[20]==0x01)	
				{
					return -4;
				}
				if(gprotocol_packetconfig_in_out[16]==command &&gprotocol_packetconfig_in_out[20]==0x03)
				{
					if(command==CMD_IDENTITY)
					{
						if(gprotocol_packetconfig_in_out[24]==0x22)
						{
							return 1;
						}
						else
						{
							return -3;
						}
					
					}
					else
					{
						return 1;
					}
				}
				else if(gprotocol_packetconfig_in_out[16]==command &&gprotocol_packetconfig_in_out[20]==0x02)
				{
					return 2;
				}
				else
				{
					delay(1000);
					send_lumidign_command(CMD_CHECK_STATUS);
					
				}
			}
			delay(100);
		}
	return 0;
}
u8_t delete_database(void)
{
		u8_t i,count=0;
	char szstr[32];
		lcd_show(0,'R',23);			//Deleting DB
		lcd_show(1,'R',20);			//Progress
		p1str("\rDelete DB\r");
		arm_trigger_value();
		send_lumidign_command(CMD_DELETE_DB);
		while(count++<10)
		{
			if(lumi_count)
			{				
				sprintf(szstr,"resp:%d\r",lumi_count);
				p1str(szstr);
				delay(500);
				for(i=0;i<lumi_count;i++)
				{
						sprintf(szstr,"%02X",gprotocol_packetconfig_in_out[i]);
						p1str(szstr);
				}		
				if(!protocol_verification())
				{
					lcd_show(1,'R',14);			//Failed
					delay(MESSAGE_TIMEOUT);
					return 0;
				}
				if(check_status_lumi(CMD_DELETE_DB))
				{
					lcd_show(1,'R',12);			//Sucess
					delay(MESSAGE_TIMEOUT);				
					return 1;
				}
				else 
				{
					lcd_show(1,'R',14);			//Failed
					delay(MESSAGE_TIMEOUT);
					return 0;
				}
			}
			delay(100);
		}
		if(count>=10)
		{
			lcd_show(1,'R',13);			//Timeout
			delay(MESSAGE_TIMEOUT);				
			return 0;
		}
		return 0;
}

u8_t create_database(void)
{
		u8_t i,count=0;
		char szstr[32];	
		lcd_show(0,'R',22);//Creating DB
		lcd_show(1,'R',20);
		p1str("\rCreate DB\r");
		send_lumidign_command(CMD_CREATE_DB);
		count=0;
		while(count++<10)
		{
			if(lumi_count)
			{
				sprintf(szstr,"resp:%d\r",lumi_count);
				p1str(szstr);
				delay(500);
				for(i=0;i<lumi_count;i++)
				{
						sprintf(szstr,"%02X",gprotocol_packetconfig_in_out[i]);
						p1str(szstr);
				}		
				lumi_count=0;				
				if(!protocol_verification())
				{
					lcd_show(1,'R',14);
					delay(MESSAGE_TIMEOUT);
					return 0;
				}
 				if(check_status_lumi(CMD_CREATE_DB))
				{
					lcd_show(1,'R',12);
					delay(MESSAGE_TIMEOUT);				
					return 1;
				}			
				else 
				{
					lcd_show(1,'R',14);
					delay(MESSAGE_TIMEOUT);
					return 0;
				}
			}
			delay(100);
		}
		if(count>=10)
		{
			lcd_show(1,'R',13);
			delay(MESSAGE_TIMEOUT);				
			return 0;
		}
		return 0;
}

u8_t set_command(void)
{
		u8_t i,count=0;
	char szstr[32];
		lcd_show(0,'R',21);			//SET COMMAND
		lcd_show(1,'R',20);			//Progress...
			p1str("\rSet Command\r");
			send_lumidign_command(CMD_SET_CMD);
		count=0;
		while(count++<10)
		{
			if(lumi_count)
			{
				sprintf(szstr,"resp:%d\r",lumi_count);
				p1str(szstr);
				for(i=0;i<lumi_count;i++)
				{
						sprintf(szstr,"%02X",gprotocol_packetconfig_in_out[i]);
						p1str(szstr);
				}			
				lumi_count=0;
				if(!protocol_verification())
				{
					lcd_show(0,'R',14);
					delay(MESSAGE_TIMEOUT);		
					return 0;
				}
				else
				{
					lcd_show(1,'R',12);
					delay(MESSAGE_TIMEOUT);
					return 1;				
				}
			}
			delay(100);
		}
		if(count>=10)
		{
			lcd_show(0,'R',13);
			delay(MESSAGE_TIMEOUT);
			return 0;
		}
return 0;		
}

u8_t arm_trigger_value(void)
{
	u8_t count=0,i;
	char szstr[32];
	p1str("arm_trigger_value");
	send_lumidign_command(CMD_ARM_TRIGGER);
		while(count++<10)
		{
			if(lumi_count)
			{
				sprintf(szstr,"resp:%d\r",lumi_count);
				p1str(szstr);
				for(i=0;i<lumi_count;i++)
				{
						sprintf(szstr,"%02X",gprotocol_packetconfig_in_out[i]);
						p1str(szstr);
				}			
				lumi_count=0;
				if(!protocol_verification())
				{
					lcd_show(1,'R',14);		//Failed
					delay(MESSAGE_TIMEOUT);
					return 0;
				}
				else
				{
						return 1;
				}
			}
			delay(50);
		}
	return 0;
}

u8_t set_baudrate(void)
{
	u8_t count=0,i;
	char szstr[32];
	send_lumidign_command(CMD_SETCOMPORT);
		while(count++<10)
		{
			if(lumi_count)
			{
				sprintf(szstr,"resp:%d\r",lumi_count);
				p1str(szstr);
				for(i=0;i<lumi_count;i++)
				{
						sprintf(szstr,"%02X",gprotocol_packetconfig_in_out[i]);
						p1str(szstr);
				}			
				lumi_count=0;
				if(!protocol_verification())
				{
					lcd_show(1,'R',14);
					delay(MESSAGE_TIMEOUT);
					return 0;
				}
				else
				{
						return 1;
				}
			}
			delay(50);
		}
	return 0;
}
u8_t verify_identity(void)
{
	u8_t count=0,i;
	char szstr[32];

	send_lumidign_command(CMD_IDENTITY);
		while(count++<40)
		{
			if(lumi_count)
			{
				sprintf(szstr,"resp:%d\r",lumi_count);
				for(i=0;i<lumi_count;i++)
				{
						sprintf(szstr,"%02X",gprotocol_packetconfig_in_out[i]);
				}			
				lumi_count=0;
				if(!protocol_verification())
					return 0;			
				else
					return 1;					
			}
			delay(50);
		}	
		return 0;
}
s32_t getresult_identy(void)
{
		u8_t count=0,i;
		u32_t usercode=0;	
		char szstr[32];	

	delay(1000);
	usercode=check_status_lumi(CMD_IDENTITY);		

	if(usercode==1)
	{
		lcd_str(0,"PLEASE WAIT!!!         ");
		lcd_clr(1);
		delay(1000);						
		send_lumidign_command(CMD_GETRESULT);		
		count=0;
		while(count++<40)
		{
			if(lumi_count==86)
			{
				sprintf(szstr,"respget:%d\r",lumi_count);

				for(i=0;i<lumi_count;i++)
				{
						sprintf(szstr,"%02X",gprotocol_packetconfig_in_out[i]);
				}			
				lumi_count=0;								
				if(!protocol_verification())
				{
					return 0;
				}
				else
				{
					usercode=0;
					usercode|=gprotocol_packetconfig_in_out[20]&0xFF;
					usercode|=(gprotocol_packetconfig_in_out[21]<<8)&0xFF00;
					usercode|=(gprotocol_packetconfig_in_out[22]<<16)&0xFF0000;
					usercode|=(gprotocol_packetconfig_in_out[23]<<24)&0xFF000000;		
					sprintf(szstr,"\rUsercode:%X\r",usercode);		
					return usercode;
				}
			}
			delay(50);
			
		}			
		if(count>=40)
		{
			lcd_show(1,'R',13);//timeout
			delay(MESSAGE_TIMEOUT);				
			return 0;
		}
		
	}		
	else  if(usercode==-3)
	{
		//invalid user
		return -3;
	}
	else if(usercode==-4)
	{
		
		//no identity
		return -4;
	}	
	
	return 0;

}
u8_t set_workdb(void)
{
	u16_t count=0,i;
	char szstr[32];	
	lcd_show(0,'R',5);
	
		p1str("\rWORK DB\r");
			send_lumidign_command(CMD_WORK_DB);
		count=0;
		while(count++<10)
		{
			if(lumi_count)
			{
				sprintf(szstr,"resp:%d\r",lumi_count);
				p1str(szstr);
				for(i=0;i<lumi_count;i++)
				{
						sprintf(szstr,"%02X",gprotocol_packetconfig_in_out[i]);
						p1str(szstr);
				}			
				lumi_count=0;
				if(!protocol_verification())
					return 0;
				delay(500);
				if(check_status_lumi(gprotocol_packetconfig_in_out[6]))
				{
					lcd_show(0,'R',5);	
					delay(MESSAGE_TIMEOUT);		
					return 1;				
				}					
			}
			delay(100);
		}		
		if(count>=10)
		{
			
			lcd_show(0,'R',13);
			delay(MESSAGE_TIMEOUT);					
			return 0;
		}
		return 0;
}
void send_biometric(s32_t ch)
{
	pchar(ch);
}
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
s32_t set_option_bio(char cattribute, u8_t *popt, u8_t ucount,u8_t acc)
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
			*popt = imenu_i+(acc*2);
			return 1;
		}
	}
}

void save_user_details(u32_t user_code)
{
	u16_t i;
	for(i=0;i<MAX_USERS_LOCATION;i++)
	{
		 read_user_sd(i);
		 if(user_t.user_id   ==0xFFFFFFFF)
		 {
			 return;
		 }
		 if(user_t.user_id  ==user_code)
		 {
			 user_t.finger_enabled=0x01;
			 write_user_sd(i);
			 return;
		 }
	}
}
u8_t add_enrollment_user(u32_t user_code,u8_t finger)
{
	u16_t count=0,i,ret_flg,fing;
	char szstr[33];
	u32_t iuser_code;
//0D 56 header
//76 00 00 00  	-->CMD
//00 00 		-->ARG
//2C 00 00 00 	-->SIZE
//00 00 00 00 	-->GROUPID
//31 30 30 31 00 CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC -->USER ID
//06 00 00 00 	-->FINGER ID
//03 00 00 00 	-->NO OF INSTANCE
//00 00  		-->RESERVER
//00 00 00 00 00 00 00 00 3A 00 00 00
	user_enrollment[22]=user_code&0xFF;
	user_enrollment[23]=(user_code>>8)&0xFF;
	user_enrollment[24]=(user_code>>16)&0xFF;
	user_enrollment[25]=(user_code>>24)&0xFF;	
	user_enrollment[54]=finger;
	lcd_clr(0);
	lcd_clr(1);
			  sprintf(szstr,"\rCode:%X\r",user_code);
				p1str(szstr);
					if(finger==1)
				lcd_show(0,'R',8);		//Place 1st finger
			else if(finger==2)
				lcd_show(0,'R',16);   //Place 2nd finger
				if(verify_identity()==1)
				{
					fing=1;
					while(count++<10)
					{
		iuser_code=getresult_identy();
				lcd_str(1,"Verifying...       ");					
					delay(1000);
		if(iuser_code==-4)
		{
			fing=1;
		}
		else if(iuser_code==-3)
		{
			fing=0;
			break;
		}
		else if(iuser_code!=0)
		{
			fing=0;
			lcd_str(1,"Finger already exists");
			delay(1000);
			lcd_clr(1);
			return 0;
		}
		else
		{
			fing=0;
			break;
		}
		if(key1(2000))
			return 0;
	}
					if(count>=10 && fing)
						return 0;
	}
				count=0;
				arm_trigger_value();
				p1str("\rEnrollment1\r");
				send_lumidign_command(CMD_ENROLLMENT);
			while(count++<5)
		{
			if(finger==1)
				lcd_show(0,'R',8);		//Place 1st finger
			else if(finger==2)
				lcd_show(0,'R',16);   //Place 2nd finger
			if(lumi_count)
			{
				lcd_show(1,'R',17);
				sprintf(szstr,"resp:%d\r",lumi_count);
				p1str(szstr);
				delay(500);
				for(i=0;i<lumi_count;i++)
				{
						sprintf(szstr,"%02X",gprotocol_packetconfig_in_out[i]);
						p1str(szstr);
						
				}			
				lumi_count=0;
				if(!protocol_verification())
				{
					lcd_show(1,'R',14);
					delay(MESSAGE_TIMEOUT);
					return 0;
				}
				ret_flg=check_status_lumi(CMD_ENROLLMENT);
				if(ret_flg==1)
				{
				  lcd_show(1,'R',12);
					delay(MESSAGE_TIMEOUT);
					return 1;
				}
				else if(ret_flg==2)
				{
					lcd_show(1,'R',14);
					delay(MESSAGE_TIMEOUT);
					lcd_clr(1);
					return 0;
				}
				else	
				{					
				lcd_clr(1);	
				}
			}
			delay(50);
		}	
			if(count>=5)
		{
			lcd_show(1,'R',13);
			delay(MESSAGE_TIMEOUT);				
			return 0;
		}
		return 0;			
}
u8_t delete_userrecord(u32_t user_code,u8_t finger,u8_t flag)
{
	u16_t count=0,i;
	char szstr[32];
	sprintf(szstr,"\rdelete user records:%d-%d\r",user_code,finger);
	p1str(szstr);	
	delete_userrecords[22]=user_code&0xFF;
	delete_userrecords[23]=(user_code>>8)&0xFF;
	delete_userrecords[24]=(user_code>>16)&0xFF;
	delete_userrecords[25]=(user_code>>24)&0xFF;	
	delete_userrecords[54]=finger;	
	send_lumidign_command(CMD_DELETE_USER);	
	while(count++<50)
	{
		if(lumi_count)
		{
			sprintf(szstr,"resp:%d\r",lumi_count);
			p1str(szstr);
			for(i=0;i<lumi_count;i++)
			{
					sprintf(szstr,"%02X",gprotocol_packetconfig_in_out[i]);
					p1str(szstr);
			}			
			lumi_count=0;
			if(!protocol_verification())
				return 0;
			if(gprotocol_packetconfig_in_out[10]==0x01 || gprotocol_packetconfig_in_out[10]==0x9E || gprotocol_packetconfig_in_out[10]==0x00)
				return 1;
			else
				return 0;
		}
		delay(500);
	}
	if(count>=50)
	{
		if(flag)
		{
		lcd_show(1,'R',13);
		delay(MESSAGE_TIMEOUT);		
		}			
		return 0;
	}
	return 0;
}
u32_t search_user(u8_t *count,u8_t flag,u8_t type,u32_t limit)
{
	u8_t uci;
	char szstr[32];
	static u8_t flag_ex=0;
	u32_t user_id=0;
	static u16_t pre_cnt=0;
	if(flag)//Down Arrow
	{
		if(flag_ex==1)
		{
			*count+=1;
		}
		flag_ex=2;
		for (uci = *count; uci < limit; uci++) 		 //if user already have card and change to new card,
    {			
      read_user_sd(uci);
      if(user_t.user_id !=0 &&((user_t.finger_enabled==0xff &&type==1 && user_t.login_type==6 && user_t.enabled!=0)||(user_t.finger_enabled==0x01 &&type==2 && user_t.login_type!=6 &&user_t.enabled!=0)))
      {
				pre_cnt=*count;
        *count=uci+1;
				user_id=user_t.user_id ;
				return user_id;
			}						 
    }
	}
	else //Up Arrow
	{
		if(flag_ex==2)
		{
			if(*count<1)
				*count=1;
			else
				*count-=1;
		}
		flag_ex=1;
		if(*count<1)
			*count=1;
				
		for (uci = *count-1; uci >=0; uci--) 		 //if user already have card and change to new card,
    {
      read_user_sd(uci);
			sprintf(szstr,"\r%d.%X-%d\r",uci,user_t.user_id ,*count);
			p1str(szstr);		
		  if(user_t.user_id ==0xFFFFFFFF)
      {
				read_user_sd(pre_cnt);
				*count=pre_cnt;
				user_id=user_t.user_id ;
        return user_id;
      }
			else if(user_t.user_id !=0&&((user_t.finger_enabled==0xff &&type==1)||(user_t.finger_enabled==0x01 &&type==2)))
      {
        *count=uci;
				 pre_cnt=uci;
			   sprintf(szstr,"\r%d.%X-%d\r",uci,user_t.user_id ,*count);
				 p1str(szstr);
				 user_id= user_t.user_id;
				 return user_id;
      }
     }
	}
	return 0;
}
u32_t search_userenroll(u16_t *count,u16_t flag,u16_t type,u32_t limit,u32_t lowerlimit)
{
	s16_t uci;
	char szstr[32];
	u32_t user_id=0;
	if(flag)//Down Arrow
	{
		for (uci = *count+1; uci <= limit; uci++) 		 //if user already have card and change to new card,
    {			
      read_user_sd(uci);
      if(user_t.user_id !=0 &&((user_t.finger_enabled==0xff &&type==1&& user_t.login_type==6 && user_t.enabled!=0)||(user_t.finger_enabled==0x01 &&type==2&& user_t.login_type==6 &&user_t.enabled!=0)))
      {
				*count=uci;
				user_id=user_t.user_id ;
				return user_id;
			}						 
    }
	}
	else //Up Arrow
	{
			
		if(*count<1)
			*count=1;
		for (uci = *count-1; uci >=lowerlimit; uci--) 		 //if user already have card and change to new card,
    {
      read_user_sd(uci);
			sprintf(szstr,"\r%d.%X-%d-%d-%d\r",uci,user_t.user_id ,*count,user_t.finger_enabled,user_t.login_type);
			p1str(szstr);		
			 if(user_t.user_id ==0xFFFFFFFF)
      {
				p1str("stop here\r");
        return user_id;
      }
			if(user_t.user_id !=0&&((user_t.finger_enabled==0xff &&type==1 && user_t.login_type==6 && user_t.enabled!=0)||(user_t.finger_enabled==0x01 &&type==2&& user_t.login_type==6 &&user_t.enabled!=0)))
      {
        *count=uci;
			   sprintf(szstr,"\r%d.%X-%d\r",uci,user_t.user_id ,*count);
				 p1str(szstr);
				 user_id= user_t.user_id;
				 return user_id;
      }
     }
	}
	return 0;
}
u16_t check_biomtric(u8_t flag,u16_t *start_user,u16_t *max_user)
{
	u16_t uci=0;
	char szstr[50];
	u16_t user_count=0;
	for ( uci = 0; uci < MAX_USERS_LOCATION; uci++) 		 //if user already have card and change to new card,
	{
		read_user_sd(uci);
  	if(user_t.user_id ==0xFFFFFFFF ||user_t.user_id ==-1)
		{
			*max_user	=uci;		
			break;
		}
		else 
		{
			if(flag==0)
			{
				if(user_t.user_id  !=0 && (user_t.finger_enabled==0xFF)&&(user_t.login_type==6))
				{
					if(!user_count)
					{
						*start_user=uci;
					}
					user_count++;
				}
			}
		}
		sprintf(szstr,"uci:%d,User:%d FP:%d-%d-%d\r",uci,user_t.user_id,user_t.finger_enabled,user_t.login_type,user_count);
		p1str(szstr);
	}
	return user_count;
}
void add_enrollment(void)
{
	u32_t user_code=0;// for first user id 
	s32_t itemp_key;
	u16_t upperlimit,lowerlimit,start_user;//limit-->max user count
	char szstr[33];
	char test_str[50];
	u16_t usercount=0;
		usercount=check_biomtric(0,&start_user,&upperlimit);//0-->for add
		sprintf(test_str,"usr:%d-%d-%d-%s\r\n",usercount,start_user,upperlimit,user_t.name);
		pstr(test_str);
		if(usercount)
		{
			lowerlimit=usercount=start_user;
			read_user_sd(start_user);
			user_code=user_t.user_id;
			strncpy(szstr,user_t.name,24);
			p1str(szstr);
			lcd_show(0,'R',6);
			lcd_str(1,szstr);
			while(1)
			{
			if (get_menu_key(&itemp_key)==0)
			{
				return;
			}	
			switch(itemp_key)
			{
				case ARROW_U:
					user_code=search_userenroll(&usercount,0,1,upperlimit,lowerlimit);
				break;
				case ARROW_D:
					user_code=search_userenroll(&usercount,1,1,upperlimit,lowerlimit);
				break;
				case ENTER:
					sprintf(test_str,"%d-%d-%s",user_t.user_id,user_t.login_type,user_t.name);
					p1str(test_str);
					if(add_enrollment_user(user_code,1))
					{
						lcd_show(0,'R',18);						//First finger added
						delay(MESSAGE_TIMEOUT);
						if(add_enrollment_user(user_code,2))
						{
							lcd_show(0,'R',19);									//second finger added
							save_user_details(user_code);				
							delay(MESSAGE_TIMEOUT);	
							
							lcd_clr(0);
							lcd_show(0,'R',9);	
							delay(50);
							arm_trigger_value();
							return;
						}
						else
						{
							lcd_show(0,'R',14);		
							arm_trigger_value();	
							delay(MESSAGE_TIMEOUT);
							if(delete_userrecord(user_code,1,1))
								lcd_show(0,'X',6);	
							delay(100);
							lcd_clr(1);
							
							set_workdb();
							lcd_show(0,'R',10);						
							delay(MESSAGE_TIMEOUT);
							lcd_clr(0);
							arm_trigger_value();			
						}
					}
					else
					{
						delay(100);
						lcd_clr(1);
						lcd_show(0,'R',10);		
						delete_userrecord(user_code,1,1);					
						delay(MESSAGE_TIMEOUT);
						set_workdb();
						lcd_show(0,'R',10);
						delay(MESSAGE_TIMEOUT);
						lcd_clr(0);
						arm_trigger_value();
						return;
					}
					break;
			}
			if(user_code!=0)
			{
				lcd_show(0,'R',6);
				strncpy(szstr,user_t.name,24);
				p1str(szstr);
				lcd_str(1,szstr);
			}
		}
			
		
		}
		else
		{
			lcd_str(0,"No User to Enroll");
			lcd_clr(1);
			delay(MESSAGE_TIMEOUT);
			return;
		}	
	
}
void add_enrollment_old(void)
{
	u8_t count=0;
	u32_t user_code=0;// for first user id 
	s32_t itemp_key,uci,limit;//limit-->max user count
	char szstr[33];

	for ( uci = 0; uci < MAX_USERS_LOCATION; uci++) 		 //if user already have card and change to new card,
	{
		read_user_sd(uci);
		sprintf(szstr,"uci:%d,User:%d FP:%2X\r",uci,user_t.user_id,user_t.finger_enabled);
		p1str(szstr);
  	if(user_t.user_id ==0xFFFFFFFF ||user_t.user_id ==-1)
		{
			limit=uci;      
			break;
		}
		else if(user_t.user_id  !=0 && (user_t.finger_enabled==0xFF))
		{
			if(!count)
			{
			 user_code=uci;
			}
			count++;
		}
	}
	if(!count)
	{
		lcd_show(0,'R',7);
		lcd_clr(1);
		delay(MESSAGE_TIMEOUT);
		return;
	}
	lcd_clr(0);
	lcd_clr(1);
	count=user_code+1;
	read_user_sd(user_code);
	user_code=user_t.user_id ;
	sprintf(szstr,"%s",user_t.name );
	p1str(szstr);
	while(1)
	{
		sprintf(szstr,"count%d\r",count);
		p1str(szstr);
		lcd_show(0,'R',6);	
		if (get_menu_key(&itemp_key)==0)
		{
			p1str("ESC");
			return;
		}	
		switch(itemp_key)
		{
			case ARROW_U:
				user_code=search_user(&count,0,1,limit);
			break;
			case ARROW_D:
				user_code=search_user(&count,1,1,limit);
			break;
			case ENTER:
				if(user_code!=0)
				{
					if(add_enrollment_user(user_code,1))
					{
						lcd_show(0,'R',18);						//First finger added
						delay(MESSAGE_TIMEOUT);
						if(add_enrollment_user(user_code,2))
						{
							lcd_show(0,'R',19);									//second finger added
							save_user_details(user_code);				
							delay(MESSAGE_TIMEOUT);	
							
							lcd_clr(0);
							lcd_show(0,'R',9);	
							delay(50);
							arm_trigger_value();
							return;
						}
						else
						{
							lcd_show(0,'R',14);		
							arm_trigger_value();	
							delay(MESSAGE_TIMEOUT);
							if(delete_userrecord(user_code,1,1))
								lcd_show(0,'X',6);	
							delay(100);
							lcd_clr(1);
							
							set_workdb();
							lcd_show(0,'R',10);						
							delay(MESSAGE_TIMEOUT);
							lcd_clr(0);
							arm_trigger_value();			
						}
					}
					else
					{
						delay(100);
						lcd_clr(1);
						lcd_show(0,'R',10);		
						delete_userrecord(user_code,1,1);					
						delay(MESSAGE_TIMEOUT);
						set_workdb();
						lcd_show(0,'R',10);
						delay(MESSAGE_TIMEOUT);
						lcd_clr(0);
						arm_trigger_value();
					}
				}
				else
				{
					p1str("user code zero\r");
				}
				break;
		}
		
	
	if(user_code!=0)
	{
		p1str("user code\r");
		lcd_show(0,'R',6);
		strncpy(szstr,user_t.name,24);
		p1str(szstr);
		lcd_str(1,szstr);
		sprintf(szstr,"BE:%d-%d\r",user_t.finger_enabled,count);
		p1str(szstr);
	}
	else if(!count)
		lcd_show(1,'R',7);
	}
	p1str("Return to main menu");
}

void chage_bioflag(u16_t user_code)//if user code is 0 disable all user fingerbio
{
	u16_t uci;
	u8_t user_flag=0;
	lcd_str(0,"Deleting bio data");
	lcd_str(1,"Please wait!!!");
	delay(MESSAGE_TIMEOUT);
	for(uci=0;uci<MAX_USERS_LOCATION;uci++)
	{
		read_user_sd(uci);
		if(user_t.user_id ==0xFFFFFFFF)
		{
			break;
		}
		else
		{
			 if(!user_code)
			 {
				 if(user_t.finger_enabled==1)
				 {
					user_t.finger_enabled=0x00;
					write_user_sd(uci);
					user_flag=1;				
				 } 
			 
			 }
			 else if(user_t.user_id ==user_code)
			 {
				user_t.finger_enabled=0x00;
				write_user_sd(uci);
				user_flag=1;				
			 } 
			
  	}
	}
	if(user_flag)
	{
		
		self_user_allign_bio();
	}
}
void delete_user_details(u16_t user_code)
{
	char szstr[32];
 	sprintf(szstr,"User Code to delete:%d\r",user_code);
	lcd_str(1,szstr);
	chage_bioflag(user_code);//user code must be >0
	delay(1000);
	set_workdb();

}

void delete_enrollment(void)
{
	u32_t uci,upperlimit,lowerlimit;
	u8_t del_flg=0;
	u32_t user_code;
	s32_t itemp_key;
	char username[33];
	char szstr[33];
	u16_t usercount=0;
	p1str("delete enrollment");
	for ( uci = 0; uci < MAX_USERS_LOCATION; uci++) 		 //if user already have card and change to new card,
  {
		 read_user_sd(uci);
		 if(user_t.user_id ==0xFFFFFFFF || user_t.user_id ==-1)
		 {
			 upperlimit=uci;
			 break;
		 }
		 else if(user_t.user_id !=0 && user_t.finger_enabled==0x01)
		 {
			 if(!usercount)
			 {
				 lowerlimit=user_code=uci;
				 sprintf(szstr,"uciusername:%s",user_t.name);
				 p1str(szstr);
				 strncpy(username,user_t.name,32);
			 }
			 usercount++;
		 }	
	}
	if(!usercount)
	{
		lcd_show(0,'R',7);
		lcd_clr(1);
		delay(MESSAGE_TIMEOUT);
		return;
	}
	
	lcd_clr(0);
	lcd_clr(1);
	usercount=user_code;
	read_user_sd(user_code);
	user_code=user_t.user_id;
	sprintf(szstr,"%s",user_t.name );
	lcd_str(1,szstr);
	delay(500);
	while(1)
	{
		lcd_show(0,'R',6);	
		itemp_key=key1(5000);
		if(itemp_key== ARROW_U)
		{
			user_code=search_userenroll(&usercount,0,2,upperlimit,lowerlimit);
		}
		else if(itemp_key== ARROW_D)
		{
			user_code=search_userenroll(&usercount,1,2,upperlimit,lowerlimit);
		}
		else if(itemp_key== ENTER)
		{
			if(user_code!=0)
			{
				lcd_clr(1);
				lcd_str(0,"PLEASE WAIT!!! ");
				delay(500);
				if(delete_userrecord(user_code,1,1))
				{
					lcd_str(1,"Finger1 Deleted...");
					delay(MESSAGE_TIMEOUT);	
					if(delete_userrecord(user_code,2,1))
					{
						lcd_show(0,'R',11);
						delete_user_details(user_code);
						delay(MESSAGE_TIMEOUT);
						arm_trigger_value();
						del_flg=1;
						break;
					}
					else
					{
						lcd_show(0,'R',14);		
						delay(MESSAGE_TIMEOUT);
						arm_trigger_value();
					}
				}
				else
				{
						lcd_show(0,'R',14);		
						delay(MESSAGE_TIMEOUT);	
						arm_trigger_value();
				}
			}
		}
		else
			break;
		if(user_code!=0 && !del_flg)
		{
			lcd_clr(1);
			sprintf(szstr,"%s",user_t.name );
			lcd_str(1,szstr);
			sprintf(szstr,"BE:%d-%d\r",user_t.finger_enabled,usercount);
			p1str(szstr);
		}
		else if(!usercount)
			lcd_show(1,'R',13);
	}
}

u8_t delete_alluser(void)
{
	u16_t uci;
	s32_t temp_key=0;
	
	lcd_show(0,'V',7);//are you sure
	lcd_clr(1);
	arm_trigger_value();
	temp_key =key1((u32_t) MESSAGE_TIMEOUT);
	if(temp_key == ENTER)
	{
		lcd_str(0,"DELETING USERS!!!         ");
		lcd_str(1,"PLEASE WAIT!!!         ");
		delay(MESSAGE_TIMEOUT);
		for (uci = 0x07; uci < 0x0C; uci++)
		{
			SectorErase(uci*0x010000);
		}
		if(delete_database())
		{
			delay(1000);
			if(create_database())
			{
				delay(1000);
				set_workdb();
			}
			arm_trigger_value();
			temp_key=0;
			return 1;
		}
		temp_key=0;
		return 1;
	}
	else
		return 0;
}
void finger_module_menu(void)
{
	s32_t itemp_key;
	s32_t imenu_i = 1;
	lcd_show(0, 'R', 0);
	lcd_str(1,"Enter Code:");
  if(read_code(HIDE_INPUT)!= 4512)
	{
		lcd_show(0, 'R', 0);
		lcd_show(1, 'S', 16U);		//Invalid service code
		delay(MESSAGE_TIMEOUT);
		return;
	}
	while(1)
	{
		lcd_show(0, 'R', 0);                        // Administrator menu
		lcd_show_index(1U, 'R',(u8_t) imenu_i);

		if (get_menu_key(&itemp_key)==0)
		{
			return;
		}		
		if (select_menu(&imenu_i, itemp_key,5))		      
		{
			switch(imenu_i)					
			{
				case 1 : add_enrollment();			break;
				case 2 : delete_enrollment(); 	break;
				case 3 : delete_alluser(); 			break;
				case 4 : set_workdb();        	break;
				case 5 : set_command();				  break; 
				default:
											  break;
			}
		}
		p1str("main menu");
	}
}
s32_t check_bio(u8_t input)
{
	s32_t iuser_code=0;
	u8_t count1=0;
	while(count1++<10)
		{
			if(fingerprint==1)
			{
				if(verify_identity()==1)
				{
					fingerprint=2;
				}		
			}
			if(fingerprint==2)
			{
				glcd_refresh_time=0;
				iuser_code=getresult_identy();
				if(iuser_code==-3)//Access Denied
				{
					lcd_str(0,"Access Denied");
					lcd_clr(1);
					buzzer(MESSAGE_TIMEOUT);
					delay(MESSAGE_TIMEOUT);
					fingerprint=1;
					arm_trigger_value();
					return 0;
				}
				else if(iuser_code==-4)
				{
					fingerprint=2;
				}
				else if(iuser_code!=0)
				{
					fingerprint=1;
					finger_user=1;
					arm_trigger_value();
					return iuser_code;
				}
				else
				fingerprint=1;
			}
			if(input==1)
			{
				iuser_code=read_code(2);
				if ((iuser_code !=-1) && iuser_code)
				{
					arm_trigger_value();
					return iuser_code; // go to main login 
				}
				else if(!iuser_code)
				{
					arm_trigger_value();
					return 0;
				}
				else
				{
				}
			}
		}
		buzzer(MESSAGE_TIMEOUT);
		arm_trigger_value();
		return 0;
}
void biometric_init(void)
{
	UART0_init(UART_BAUDRATE_9600);
	send_lumidign_command(CMD_GETCONFIG);

	delay(500);
	if(lumi_count)
	{
		lumi_count=0;
		p1str("Get config done set\r");		
	}
	else
	{
		p1str("Get config failed!!\r");		
	}
		delay(500);
		if(set_workdb())
		{
			delay(500);
			if(set_command())
			{
			   delay(500);
				fingerprint=0x01;
			}
			else
				p1str("set commandfailure\r");		
		}
		else
			p1str("set work db failure\r");		
}

//For testing the bio we can call from main function
void test_biometric(void)
{
	s32_t iuser_code=0;
	while(1)
	{
		iuser_code=key1(5000);
	
		switch (iuser_code)
		{
			case '1':
					service_menu();
			break;
			case '2':
				 if(delete_database())
				{
					p1str("delete database success\r");
					delay(1000);
					if(create_database())
					{
						p1str("create database success\r");						
						delay(1000);
						set_workdb();
					}
					else
					{
						p1str("create database failed\r");
					}
					arm_trigger_value();
				}
				else
				{
					p1str("delete database failed\r");
				}
			break;
			case '3':
				add_enrollment();
			break;
			case '4':
				delete_enrollment();
			break;
			case '5':
			break;
			case '6':
			break;
			case '7':
			break;
			case '8':
				
			break;
			
		}
		if(iuser_code =='9')
			break;
	}
	
}
u8_t check_user_bio(u16_t user_code)
{
	u16_t uci;
	
	for(uci=0;uci<MAX_USERS_LOCATION;uci++)
	{
		read_user_sd(uci);
		if(user_t.user_id==user_code &&user_t.finger_enabled==0x01)
		{
			return 1;
		}
	}
	return 0;
}

#ifdef BIOMETRIC	
void bio_verification(void)
{
	u16_t uci;
	lcd_str(0,"User Bio Reconfiguring");
	lcd_str(1,"Please wait...");
	for(uci=0;uci<MAX_USERS_LOCATION;uci++)
	{
		delay(1);
		read_user_sd(uci);
		if(user_t.login_type!=6 && user_t.finger_enabled==1)
		{
			p1str("user bio to be deleted\r\n");
			if(delete_userrecord(user_t.user_id,1,0))
			{
				p1str("delete record finger 1  done\r\n");
				if(delete_userrecord(user_t.user_id,2,0))
				{
					p1str("user bio deleted\r\n");
					arm_trigger_value();
					user_t.finger_enabled=0;
					write_user_sd(uci);
				}
				else
				{
					p1str("user delete failed\r\n");
					arm_trigger_value();
				}
				
				
			}
			else
			{
				p1str("user bio1 to be deleted\r\n");
					arm_trigger_value();
			}
			}
			}
	bio_delete_flag=0;
}
#endif		


