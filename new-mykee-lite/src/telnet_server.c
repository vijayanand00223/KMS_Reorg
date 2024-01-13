/**
 * @file telnet_server.c
 * @brief Telnet server example
 *
 * @section License
 *
 * Copyright (C) 2010-2015 Oryx Embedded SARL. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * @author Oryx Embedded SARL (www.oryx-embedded.com)
 * @version 1.6.1
 **/

//Dependencies
#include "core/net.h"
#include "telnet_server.h"
#include "debug.h"
#include "Ticker_itrp.h"
#include "menu_main.h"
#include "uart.h"
#include "EEPROM.h"
#include "LCD.h"
#include "CLOCK_CALNDR.h"
//Telnet port
#define TELNET_SERVER_PORT 23

//Maximum time to wait before closing the connection (30s)
#define TELNET_SERVER_TIMEOUT 300000

/**
 * @brief TCP server states
 **/

typedef enum
{
   TELNET_SERVER_STATE_LISTEN  = 0,
   TELNET_SERVER_STATE_RECEIVE = 1,
   TELNET_SERVER_STATE_EXECUTE = 2,
   TELNET_SERVER_STATE_SEND    = 3,
	 TELNET_SERVER_STATE_WAIT    = 4,
	 TELNET_SERVER_CLOSE				 = 5
} telnetServerState;

#define GET_CONNECTION_CONFIQ 0x01
#define SET_RTC  							0x03
#define RESET_TARGET 					0x04
#define FACT_RESET 						0x05
#define GET_FIRMWARE_INFO 		0x06
#define FIRMWARE_UPGRADE 			0x07
#define SET_SERVER_IP1     		0x08
#define SET_SERIAL_NO     		0x09
#define SET_MAC_ID						0x0A
#define GET_MAC_ID						0x0B
#define SET_SERVER_IP2     		0x0C
#define SET_DNS_IP1			   		0x0D
#define SET_DNS_IP2     			0x0E
#define FIRMWARE_UPGRADE_WEB	0x0F

//Variables
static systime_t lastEvent;
static telnetServerState state;
static Socket *serverSocket;
static Socket *clientSocket;
static char_t rxBuffer[530];
static size_t rxBufferLen;
static size_t txBufferPos;
static size_t txBufferLen;
static unsigned char wait_status=0;

u8_t ftp_option;
u8_t fw_flag;
u16_t fw_datalen;
u16_t packet_num;
u8_t check_restart;

u32_t crc_calculation_fw(u16_t length)
{
	
   int i, j,count=0,test_count;
        u32_t byte1, crc, mask;
	       
    crc = 0xFFFFFFFF;
		for(count=1;count<length;count++)
		{
				read_boot_sd(count);
					test_count=256;
					i = 0;
					while (test_count>0) 
					{
							byte1 = Boot_memBlock_t.code[i];            
							crc = crc ^ byte1;
							for (j = 7; j >= 0; j--) 
							{  
									mask = (unsigned int)-(crc & 0x00000001);
									crc = (crc >> 1) ^ (0xEDB88340 & mask);
							}
							i = i + 1;
							test_count--;
					}
			}
       return ~crc;
}


void fw_download_sd()
{
 char read_str[24]={""};
 char check_str1[24]={"STERNA- BOOTLOAD"};
 u16_t usi;
 char  szstr[50];
 	u32_t crc_check=0,crc_test=0;
 gcommunication_time=20;
 for(usi=0U;usi<(u16_t)256;usi++)
 {
  if(usi >= 240U)
  {
   sprintf(szstr, "%c", Boot_memBlock_t.code[usi]);
   strcat(read_str,szstr);
   }
 }
		if(packet_num==0)
		{
		if(strcmp(read_str,check_str1))                //Tag for valid firmware in SD card "STERNA- BOOTLOAD"
		{
		  packet_num = 0;
		  p1str("Not valid protocol\r");
		  return;
		}
		else
		{
		 p1str("valid protocol\r");
		 for (usi=0; usi<BOOT_BLOCK_SIZE;usi++)
		 {
		 SectorErase(BOOT_BLOCK + (usi*0x010000));			//Erasing boot sector 0x020000-0x040000
		 delay(1);
		 }
		}
		packet_num++;
		}
	else if(packet_num != 0)
	{
		if((packet_num+1) == fw_datalen)
		{
			if(!strcmp(read_str,check_str1))
			{
				for(usi=0U;usi<(u16_t)256;usi++)
				{
					if(usi >= 236U && usi <= 239U)
					{
						sprintf(szstr, "%02X", Boot_memBlock_t.code[usi]);
						p1str(szstr);
					}
				}
				crc_check =	((Boot_memBlock_t.code[236]&0xFF)|((Boot_memBlock_t.code[237]<<8)&0xFF00)|((Boot_memBlock_t.code[238]<<16)&0xFF0000)|((Boot_memBlock_t.code[239]<<24)&0xFF000000));
				crc_test=crc_calculation_fw(fw_datalen-1);
				sprintf(szstr,"\r%04X,%04X,%d",crc_check,crc_test,fw_datalen);
				p1str(szstr);
				if(crc_check==crc_test)
				{
					packet_num = 0;
					Boot_memBlock_t.code[25]=fw_datalen&0xFF;
					Boot_memBlock_t.code[26]=fw_datalen>>8&0xFF;
					Boot_memBlock_t.code[24]='1';
					write_boot_sd(0);
					p1str("Firmware upload!!!");
					fw_flag=3;
					system_restart();
					delay(2000);
				}
				else
				{
					p1str("CRC Mismatch!!!");	
					packet_num = 0;
					fw_flag=0x00;
					p1str("Code not for us\r");			
					system_restart();
					delay(2000);			
				}
			}
			else
			{
				packet_num = 0;
			  fw_flag=0x00;
			  p1str("Code not for us\r");   
				system_restart();
				delay(2000);			
			}
   }
		else
		{
			write_boot_sd(packet_num);
			for(usi=0U;usi<(u16_t)256;usi++)
				Boot_memBlock_t.code[usi]='\0';
		}
		packet_num++;
  }
	return;
}
void handle_ftp_char(u8_t ch)
{
	static u16_t count=0;

	if(ftp_option==FW_UPLOAD_TCP)
	{
		if(count < 256)
		{
			Boot_memBlock_t.code[count]=(char)ch;
			count++;
			if (count == 256)
			{
				count =0;
				if(fw_flag!=2)
					fw_download_sd();				
			}
		}
		else
		{
			count = 0;
		}
	}
}

void get_Connection_confiq(u8_t check)
{
	char szstr[32];

	rxBuffer[0]=sys_t.serial&0xFF;
	rxBuffer[1]=(sys_t.serial>>8)&0xFF;
	rxBuffer[2]=(sys_t.serial>>16)&0xFF;
	rxBuffer[3]=(sys_t.serial>>24)&0xFF;			
		
	rxBuffer[4]=GET_CONNECTION_CONFIQ;
	
	rxBuffer[5]=255;
	
	rxBuffer[6]=sys_t.serial&0xFF;
	rxBuffer[7]=(sys_t.serial>>8)&0xFF;
	rxBuffer[8]=(sys_t.serial>>16)&0xFF;
	rxBuffer[9]=(sys_t.serial>>24)&0xFF;
	sprintf(szstr,"\rSysSerial:%d,%d\r",sys_t.serial,ks_serial);
	pstr(szstr);
	crc_calculation(11);
}

void get_firmware_info(u8_t check)
{
	u8_t i=0;
	u32_t j=0;
	u8_t strdata[10];
	u8_t count=0;
	
	if(check==1)
	{
		rxBuffer[5]=254;
		
		rxBuffer[6]=sys_t.serial&0xff;
		rxBuffer[7]=(sys_t.serial>>8)&0xff;
		rxBuffer[8]=(sys_t.serial>>16)&0xff;
		rxBuffer[9]=(sys_t.serial>>24)&0xff;
		
		crc_calculation(6);
	}
	else
	{
		rxBuffer[0]=sys_t.serial&0xff;
		rxBuffer[1]=(sys_t.serial>>8)&0xff;
		rxBuffer[2]=(sys_t.serial>>16)&0xff;
		rxBuffer[3]=(sys_t.serial>>24)&0xff;
		
		rxBuffer[4]=GET_FIRMWARE_INFO;
		
		rxBuffer[5]=255;
		
		rxBuffer[6]=sys_t.serial&0xff;
		rxBuffer[7]=(sys_t.serial>>8)&0xff;
		rxBuffer[8]=(sys_t.serial>>16)&0xff;
		rxBuffer[9]=(sys_t.serial>>24)&0xff;
		
			
		rxBuffer[13]=comm_t.remote_ip1 &0xff;
		rxBuffer[12]=(comm_t.remote_ip1>>8)&0xff;
		rxBuffer[11]=(comm_t.remote_ip1>>16)&0xff;
		rxBuffer[10]=(comm_t.remote_ip1>>24)&0xff;
		
		strcpy(strdata,BOX_VERSION);
		
		i=strlen(strdata);
		
		rxBuffer[14]=i;
		
		for(j=0;j<i;j++)
		{
			rxBuffer[j+15]=strdata[j];
		}
		i=j-1;
		rxBuffer[16+i]=3;
		rxBuffer[17+i]=3;
			
		for(j=0;j<MAX_USERS_LOCATION;j++)
		{
				read_user_sd(j);
				if(user_t.user_id &&(user_t.user_id !=0xFFFFFFFF))
				{
					count++;
				}
		}
		
		rxBuffer[i+18]=count&0xff;		
		count=0;	
		read_user_sd(1);
		for(j=0;j<8;j++)
		{
					count++;
		}
		rxBuffer[i+19]=count&0xff;
		count=0;	
		rxBuffer[i+20]=8;
		crc_calculation(i+21);
	}
}
void handle_protocol_configpacket(u8_t error)
{
	u32_t test=0;
	
	char szstr[50];
	
	switch(rxBuffer[4])
	{	
/*		case CONNECTION_CONFIQ:
		break;*/
		
		case GET_CONNECTION_CONFIQ:
		get_Connection_confiq(error);
		break;
		
		case SET_RTC:
		 {
					test=0;
			 		test|=(rxBuffer[5]);
					test|=(rxBuffer[6]<<8);
					test|=(rxBuffer[7]<<16);					
					test|=(rxBuffer[8]<<24);
					sprintf(szstr,"TIME%08X",test);
					pstr(szstr);
					set_date_time(test);				
					rxBuffer[5]=0xFF;
					crc_calculation(6);
			  break;
		 }
		
		case FACT_RESET:
		writedefaults_sdcard();
		break;
		
		case RESET_TARGET:
		
			check_restart=1;
			system_restart();
			check_restart=0;
			break;

		case GET_FIRMWARE_INFO:
		get_firmware_info(error);
		break;	
		
		case FIRMWARE_UPGRADE:
		{
				fw_datalen=0;
				fw_datalen=(rxBuffer[5]<<8);
				fw_datalen|=rxBuffer[6];                                         
				fw_flag=1;
				gcommunication_time=60;
				ftp_option=FW_UPLOAD_TCP;
				lcd_str(0,"  NEW FIRMWARE  ");
				lcd_str(1,"    UPLOADING   ");
				break;	
		}
		
		case SET_SERVER_IP1:	
		   
				  read_comm_sd();
				  test=0;
			 	  test|=(rxBuffer[5]<<24);
					test|=(rxBuffer[6]<<16);
					test|=(rxBuffer[7]<<8);					
					test|=(rxBuffer[8]);
					sprintf(szstr,"\rSERVER IP%08X",test);
					pstr(szstr);
				  comm_t.remote_ip1=test;
					test=0;
				  write_comm_sd();
					rxBuffer[5]=0xFF;
					crc_calculation(6);
			    check_restart=1;
		     	system_restart();
			    check_restart=0;
		  
	      break;
	
		case SET_SERVER_IP2:	
		   
				  read_comm_sd();
				  test=0;
			 	  test|=(rxBuffer[5]<<24);
					test|=(rxBuffer[6]<<16);
					test|=(rxBuffer[7]<<8);					
					test|=(rxBuffer[8]);
					sprintf(szstr,"\rSERVER IP1%08X",test);
					pstr(szstr);
				  comm_t.remote_ip2=test;
					test=0;
				  write_comm_sd();
					rxBuffer[5]=0xFF;
					crc_calculation(6);
			    check_restart=1;
		     	system_restart();
			    check_restart=0;
		  
	      break;
		case SET_DNS_IP1:	
		   
				  read_system_sd();
				  test=0;
			 	  test|=(rxBuffer[5]<<24);
					test|=(rxBuffer[6]<<16);
					test|=(rxBuffer[7]<<8);					
					test|=(rxBuffer[8]);
					sprintf(szstr,"\rDNS IP1%08X",test);
					pstr(szstr);
				  sys_t.dns_ip1 =test;
					test=0;
				  write_system_sd();
					rxBuffer[5]=0xFF;
					crc_calculation(6);
			    check_restart=1;
		     	system_restart();
			    check_restart=0;
		  
	      break;
		case SET_DNS_IP2:	
		   
				  read_system_sd();
				  test=0;
			 	  test|=(rxBuffer[5]<<24);
					test|=(rxBuffer[6]<<16);
					test|=(rxBuffer[7]<<8);					
					test|=(rxBuffer[8]);
					sprintf(szstr,"\rSERVER IP%08X",test);
					pstr(szstr);
				  sys_t.dns_ip2=test;
					test=0;
				  write_system_sd();
					rxBuffer[5]=0xFF;
					crc_calculation(6);
			    check_restart=1;
		     	system_restart();
			    check_restart=0;
		  
	      break;
	case SET_SERIAL_NO:	
		   
				  pstr("SET SERIAL NO");
					read_system_sd();
					test=0;
			 		test|=(rxBuffer[5]<<24);
					test|=(rxBuffer[6]<<16);
					test|=(rxBuffer[7]<<8);					
					test|=(rxBuffer[8]);
				  sprintf(szstr,"serial NO:%08X",test);
					pstr(szstr);
				  sys_t.serial=test;
					write_system_sd();
					rxBuffer[5]=0xFF;
					crc_calculation(6);
				  system_restart();
	     break;		
	case SET_MAC_ID:
					read_mac_sd();
					
					mac_t.add_MAC0=((rxBuffer[5]&0xFF)|(rxBuffer[6]<<8&0xFF00));	
					mac_t.add_MAC1=((rxBuffer[7]&0xFF)|(rxBuffer[8]<<8&0xFF00));	
					mac_t.add_MAC2=((rxBuffer[9]&0xFF)|(rxBuffer[10]<<8&0xFF00));	
					write_mac_sd();
					rxBuffer[5]=0xFF;
					crc_calculation(6);
				  system_restart();
			break;
	case GET_MAC_ID:
		read_mac_sd();
		rxBuffer[5]=0xFF;
			rxBuffer[5]=mac_t.add_MAC0&0xFF;
			rxBuffer[6]=(mac_t.add_MAC0>>8) &0xFF;
			rxBuffer[7]=mac_t.add_MAC1&0xFF;
			rxBuffer[8]=(mac_t.add_MAC1>>8) &0xFF;
			rxBuffer[9]=mac_t.add_MAC2&0xFF;
			rxBuffer[10]=(mac_t.add_MAC2>>8) &0xFF;	
			crc_calculation(11);
			break;
	}
}


u8_t check_config_protocol(u8_t length)
{
	char szstr[32];
	u16_t check_length=0;

	u8_t i=0;
	check_length=get_protocol_length(rxBuffer[4]);

	sprintf(szstr,"\rcmd:%02X-%d-%d\r",rxBuffer[4],length,check_length);
	pstr(szstr);
	

	if(check_length!=length)
	{
		pstr("Length Mismatch\r");
		return 0;
	}	
	if(!serial_verification())
	{
		pstr("Serial Mismatch\r");
	}
	if(!checksum_verification(length))
	{
		pstr("CRC Mismatch\r");
		return 0;
	}
	
	handle_protocol_configpacket(i);
	return 1;
}


void crc_calculation(s32_t count)
{
	u8_t crc_value=0;
	u32_t i;
	char szstr[32];
	for(i=0;i<count;i++)
	{
		crc_value^=rxBuffer[i];
	}
	crc_value &=0x7F;
	crc_value+=1;
	rxBuffer[count]=crc_value;
	for(i=0;i<=count;i++)
	{
		sprintf(szstr,"%x",rxBuffer[i]);
		pstr(szstr);
	}
	pstr("end");
	tcpconfig_protocol_send(count+1);


}

u8_t serial_verification()
{
	 if((rxBuffer[0]!=((sys_t.serial)&0xFF))||(rxBuffer[1]!=((sys_t.serial>>8)&0xFF))||(rxBuffer[2]!=((sys_t.serial>>16) &0xFF))||(rxBuffer[3]!=((sys_t.serial>>24) &0xFF)))//||(gprotocol_packetconfig_in_out[4]!=((tcp_comm_tserial>>8) &0xFF))||(gprotocol_packetconfig_in_out[5]!=(tcp_comm_tserial &0xFF)))
		return 0;
	else
		return 1;
}





u16_t get_protocol_length(u8_t ch)
{
	u16_t return_value;
	
	
	switch(ch)
	{
		
			case GET_CONNECTION_CONFIQ:
			return_value=6;
			break;
			
			case SET_RTC:
			return_value=10;
			break;
	
			case RESET_TARGET:
			return_value=6;
			break;
			
			case FACT_RESET:
			return_value=6;
			break;
			
			case GET_FIRMWARE_INFO:
			return_value=6;
			break;
			
			case FIRMWARE_UPGRADE:	
			return_value=8;
			break;			
			
			case SET_SERVER_IP1:	
				 return_value=10;
			 break;
			
			case SET_SERVER_IP2:	
				 return_value=10;
			 break;
			
			case SET_DNS_IP1:	
				 return_value=10;
			 break;
			
			case SET_DNS_IP2:	
				 return_value=10;
			 break;
			
			case SET_SERIAL_NO:
			   return_value=10;
			 break;
			case GET_MAC_ID:
				return_value=6;
			break;
			case SET_MAC_ID:
				return_value=12;
			break;
			default:
			return_value=0;
			break;
	}
	return return_value;
	
}


u8_t checksum_verification(u16_t length)
{
	u32_t i;
	u8_t crc_value=0;
		
	for(i=0;i<length-1;i++)
	{
		crc_value ^=rxBuffer[i];
	}

	crc_value &=0x7F;          
	crc_value +=1;
	if(crc_value!=rxBuffer[length-1])
	return 0;

	return 1;

}

s32_t tcpconfig_protocol_send(u32_t pro_length)
{
	char szstr[50];
	pstr("TCP config");
	sprintf(szstr,"State:%dTelnet port:%d\r",state,sys_t.telnet_port );
	pstr(szstr);
	txBufferLen=pro_length;
	txBufferPos = 0;
	state=TELNET_SERVER_STATE_SEND;
	return 1;
}
/**
 * @brief TCP server initialization
 * @return Error code
 **/ 

error_t telnetServerInit(void)
{
   error_t error;
		u16_t telnet_port;
	telnet_port= sys_t.telnet_port;
   //Open a TCP socket
   serverSocket = socketOpen(SOCKET_TYPE_STREAM, SOCKET_IP_PROTO_TCP);
   //Failed to open socket?
   if(serverSocket == NULL)
      return ERROR_OPEN_FAILED;

   //Bind newly created socket to config port 
   error = socketBind(serverSocket, &IP_ADDR_ANY, telnet_port);
   //Failed to bind socket to port 23?
   if(error)
      return error;

   //Place socket in listening state
   error = socketListen(serverSocket, 0);
   //Any failure to report?
   if(error)
      return error;

   //Switch to default state
   state = TELNET_SERVER_STATE_LISTEN;
   
   //Initialize variables
   clientSocket = NULL;
   rxBufferLen = 0;
   txBufferPos = 0;
   txBufferLen = 0;

   //Successful initialization
   return NO_ERROR;
}


/**
 * @bief TCP server task
**********/

void telnetServerTask(void)
{
   switch(state)
   {
   case TELNET_SERVER_STATE_LISTEN:
      //The TCP server is waiting for a connection attempt
      telnetServerStateListen();
      break;
   case TELNET_SERVER_STATE_RECEIVE:
      //The TCP server is waiting for a command from the client
      telnetServerStateReceive();
      break;
   case TELNET_SERVER_STATE_EXECUTE:
      //A command has been received and is about to be executed
      telnetServerStateExecute();
      break;
   case TELNET_SERVER_STATE_SEND:
      //The TCP server is sending the response back the client
      telnetServerStateSend();
      break;
	 case TELNET_SERVER_STATE_WAIT:
			telnetserverwait();
		 break;
	case TELNET_SERVER_CLOSE:
		 telnetserverclose();
	 break;
   default:
      //Switch to default state
      state = TELNET_SERVER_STATE_LISTEN;
      break;
   }
}

void  telnetserverclose(void)
{
		   systime_t time;
   
   //Get current time
   time = osGetSystemTime();

		  if(timeCompare(time, lastEvent + 1000) >= 0)	
      {
         pstr("send close...\r\n");
         //The connection has been closed or reset by the peer
         socketClose(clientSocket);
         //Wait for a new connection attempt
         state = TELNET_SERVER_STATE_LISTEN;
      }
}
/**
 * @bief LISTEN state
 *
 * The TCP server is waiting for a connection attempt
 *
 **/
void telnetserverwait(void)
{
	 systime_t time;

   //Get current time
   time = osGetSystemTime();

			if(wait_status &&!glogin_user)
			{
				wait_status=0;
				state = TELNET_SERVER_STATE_EXECUTE;
				return;
			}
		  if(timeCompare(time, lastEvent + TELNET_SERVER_TIMEOUT) >= 0)	
      {
         //Debug message
         TRACE_INFO("Timeout1...\r\n");
         
         //The connection has been closed or reset by the peer
         socketClose(clientSocket);
         //Wait for a new connection attempt
         state = TELNET_SERVER_STATE_LISTEN;
      }
}
void telnetServerStateListen(void)
{
   IpAddr clientIpAddr;
   uint16_t clientPort;
   //Accept an incoming connection attempt
   clientSocket = tcpAccept(serverSocket, &clientIpAddr, &clientPort);
   
   //Check whether a connection has been accepted
   if(clientSocket != NULL)
   {
      lastEvent = osGetSystemTime();

      //Initialize variables
      rxBufferLen = 0;
      txBufferPos = 0;
      txBufferLen = 0;
      //Enter the RECEIVE state
      state = TELNET_SERVER_STATE_RECEIVE;
   }
}


/**
 * @bief RECEIVE state
 *
 * The TCP server is waiting for a command from the client
 *
 **/

void telnetServerStateReceive(void)
{
   error_t error;
   size_t n;
   systime_t time;
   
   //Get current time
   time = osGetSystemTime();

   //Read data 
   error = socketReceive(clientSocket, rxBuffer + rxBufferLen,
      sizeof(rxBuffer) - rxBufferLen - 1, &n, 0);

   //Check status code
   if(error == NO_ERROR)
   {		 
     //Save current time
     lastEvent = time;
     //Some data have been received
     rxBufferLen += n;
     //Execute the command
     state = TELNET_SERVER_STATE_EXECUTE;
   }
   else if(error == ERROR_TIMEOUT || error == ERROR_WOULD_BLOCK)
   {
      //Manage timeout

			 if(timeCompare(time, lastEvent + TELNET_SERVER_TIMEOUT) >= 0)	
      {
					if(fw_flag)
					{
						fw_flag=2;
					}
         //Debug message
         TRACE_INFO("Timeout1...\r\n");
         
         //The connection has been closed or reset by the peer
         socketClose(clientSocket);
         //Wait for a new connection attempt
         state = TELNET_SERVER_STATE_LISTEN;
      }
   }
   else
   {
      //Debug message
      TRACE_INFO("Connection lostr2...\r\n");
      
      //The connection has been closed or reset by the peer
      socketClose(clientSocket);
      //Wait for a new connection attempt
      state = TELNET_SERVER_STATE_LISTEN;
   }
}

/**
 * @bief EXECUTE state
 *
 * A command has been received and is about to be executed
 *
 **/
 
void telnetServerStateExecute(void)
{
	char szstr[32];
	u32_t i;
	u8_t flag=0;
	state = TELNET_SERVER_STATE_WAIT;
	if(glogin_user)
	{
			wait_status=0x01;
	     lastEvent = osGetSystemTime();
				return;
	}
  rxBuffer[rxBufferLen] = '\0';
		sprintf(szstr,"Len:%d\r",rxBufferLen);
		pstr(szstr);	
	for(i=0;i<rxBufferLen;i++)
	{
		if(ftp_option == FW_UPLOAD_TCP)
			handle_ftp_char(rxBuffer[i]);
		if(ftp_option != FW_UPLOAD_TCP)
		{
			sprintf(szstr,"%02X\r",rxBuffer[i]);
			TRACE_INFO(szstr);
		}
	}

if(ftp_option != FW_UPLOAD_TCP)
	{
		flag=check_config_protocol(rxBufferLen);
		if(!flag)
		{
			//Debug message
			 TRACE_INFO("Connection closed1...\r\n");
			 
			 //Close the socket
			 socketClose(clientSocket);
			 //Wait for a new connection attempt
			 state = TELNET_SERVER_STATE_LISTEN;
			 //Exit immediately
			 return;
		}
		else if(flag==2)
		{
			state = TELNET_SERVER_STATE_RECEIVE;
		}
			
		rxBufferLen=0;
	}
	else 
	{
			rxBufferLen=0;
	}	
	if(ftp_option == FW_UPLOAD_TCP)
		state = TELNET_SERVER_STATE_RECEIVE;
}

/**
 * @bief SEND state
 *
 * The TCP server is sending the response back the client
 *
 **/

void telnetServerStateSend(void)
{
   error_t error;
   size_t n;
   systime_t time;
   //Get current time
   time = osGetSystemTime();

 error = socketSend(clientSocket,  rxBuffer + txBufferPos,
      txBufferLen - txBufferPos, &n, 0);  
   //Check status code
   if(error == NO_ERROR)
   {
		 pstr("No errors");
      //Save current time
      lastEvent = time;
      //Some data have been sent out
      txBufferPos += n;
      
		if(txBufferPos<txBufferLen)
			state = TELNET_SERVER_STATE_SEND;
		else
		{
         TRACE_INFO("Connection receive...\r\n");
          lastEvent = osGetSystemTime();
      
			 state = TELNET_SERVER_CLOSE;
         //Exit immediately
         return;
		}
   }
   else if(error == ERROR_TIMEOUT || error == ERROR_WOULD_BLOCK)
   {
			if(timeCompare(time, lastEvent + TELNET_SERVER_TIMEOUT) >= 0)	
			{
				 //Debug message
				 TRACE_INFO("Timeout2...\r\n");
				 
				 //The connection has been closed or reset by the peer
				 socketClose(clientSocket);
				 //Wait for a new connection attempt
				 state = TELNET_SERVER_STATE_LISTEN;
			}

   }
   else
   {
      //Debug message
      TRACE_INFO("Connection lostsend...\r\n");
      
      //The connection has been closed or reset by the peer
      socketClose(clientSocket);
      //Wait for a new connection attempt
      state = TELNET_SERVER_STATE_LISTEN;
   }
}
void close_connection(void)
{
	 socketClose(clientSocket);
	 //Wait for a new connection attempt
	 state = TELNET_SERVER_STATE_LISTEN;
}
/*-----------------------------------------end of file-------------------------------------------*/
