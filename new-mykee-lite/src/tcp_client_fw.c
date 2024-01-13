#include "core/net.h"
#include "core/bsd_socket.h"
#include "tcp_client_fw.h"
#include "debug.h"
#include "menu_main.h"
#include "uart.h"
#include "BUZZER.h"
#include "CLOCK_CALNDR.h"

//FSM states
#define TCP_CLIENT_STATE_INIT 				0
#define TCP_CLIENT_STATE_IDEAL     		1
#define TCP_CLIENT_STATE_CONNECT			2
#define TCP_CLIENT_STATE_SEND      		3
#define TCP_CLIENT_STATE_WAIT		  		4
#define TCP_CLIENT_STATE_CLOSE     		5	
#define TCP_CLIENT_STATE_RECEIVE			6


//Global variables
static uint_t state;
static systime_t lastEvent;
static char_t buffer[530];
static Socket *clientSocketfw;
static u8_t serveripno=0;
u16_t fw_port;
static size_t rxBufferLen;
IpAddr serverIpAddr_fw;

void tcpClientfwInit(void)
{
   //Debug message
   pstr("TCP client fw initialization...\r\n");
   //Switch to default state
   tcpClientChangeStatefw(TCP_CLIENT_STATE_IDEAL);
}

void tcpClientfwTask(void)
{

   switch(state)
   {
		case TCP_CLIENT_STATE_INIT:
			 break;
    case TCP_CLIENT_STATE_IDEAL:
      tcpClientStatefwInit();
      break;
    case TCP_CLIENT_STATE_CONNECT:
      tcpClientStatefwConnect();
      break;
		case TCP_CLIENT_STATE_RECEIVE:
      tcpClientStatefwreceive();
      break;
		case TCP_CLIENT_STATE_SEND:
      tcpClientStatefwSend();
      break;
		case TCP_CLIENT_STATE_WAIT:
      tcpClientStatefwwait();
      break;
		case TCP_CLIENT_STATE_CLOSE:
      tcpClientStatefwClose();
      break;
		default:
      //Switch to default state
      tcpClientChangeStatefw(TCP_CLIENT_STATE_IDEAL);
      break;
   }
}


void tcpClientStatefwInit(void)
{
	static u16_t flag_comm=0;
	char szstr[32];
	int_t error;
	static u8_t fail_cnt=0;
	if(flag_comm!=gfw_time)
	{
		flag_comm=gfw_time;
		sprintf(szstr,"T:%d,state:%d\r",gfw_time,state);
				pstr(szstr);
	}
   if(gfw_time|| !total_comm_time)
   {
      tcpClientChangeStatefw(TCP_CLIENT_STATE_IDEAL);
   }
	 else
		pstr("\rFW send  init");
  	if(sys_t.conn_type==HOSTADDR)
		{
			read_comm_sd();
		  sprintf(szstr,"\rHost Addr:%s",comm_t.url1);
		  pstr(szstr);
		 
		  error = getHostByName(NULL, comm_t.url1, &serverIpAddr_fw, 0);
			if(error)
			{
  			if(error==ERROR_IN_PROGRESS )
				{
					fail_cnt++;
					if(fail_cnt >= 30)	
					{
						fail_cnt=0;
					}
					else
				return;
				}
			comcount=15;
			
			tcpClientChangeStatefw(TCP_CLIENT_STATE_CLOSE);
		}
		else
		{
			}
		}
		else
		{
			pstr("\rRemote IP");
			serverIpAddr_fw.ipv4Addr=rmip1;
		}
		tcpClientChangeStatefw(TCP_CLIENT_STATE_CONNECT);
		return;	 
}

void tcpClientStatefwConnect(void)
{
   int_t error;
	 char szstr[32];	
	 pstr("Socketconnect\r");
	 
	 clientSocketfw = socketOpen(SOCKET_TYPE_STREAM, SOCKET_IP_PROTO_TCP);
				if(!clientSocketfw) 
				{
					pstr("Socket open error\r");
					tcpClientChangeStatefw(TCP_CLIENT_STATE_CLOSE);
					return;
					
				}
				serverIpAddr_fw.length = sizeof(Ipv4Addr);


				read_comm_sd();
				
		fw_port=comm_t.firmware_port;
				sprintf(szstr,"IP:%X-FW Port:%d\r",serverIpAddr_fw.ipv4Addr,fw_port);
					pstr(szstr);
					error = socketConnect(clientSocketfw, &serverIpAddr_fw, fw_port);
				if(error==NO_ERROR || error==ERROR_TIMEOUT)
				{
					gfw_time=10;
					tcpClientChangeStatefw(TCP_CLIENT_STATE_SEND);
				}
				else
				{
						if(serveripno<5)
							serveripno++;
						else
							serveripno=0;
						tcpClientChangeStatefw(TCP_CLIENT_STATE_CLOSE);
				}
}
void tcpClientStatefwSend(void)
{
	int_t error;
	u32_t test=0;
	u8_t crc_value=0;
	u16_t i;
	systime_t time;
  static u8_t fw_first_flag=0;
	char szstr[32];
  //Get current time
	time = osGetSystemTime();
	if(!fw_first_flag)
	{
		fw_first_flag=0x01;
		pstr("\rFW Socket send\r");
		buffer[0]=(sys_t.serial&0xFF000000)>>24;
		buffer[1]=(sys_t.serial&0xFF0000)>>16;
		buffer[2]=(sys_t.serial&0xFF00)>>8;
		buffer[3]=(sys_t.serial &0xFF);
		buffer[4]=0xFF;
		
		test=get_ticks_value();
		buffer[5]=(test&0xFF000000)>>24;
		buffer[6]=(test&0xFF0000)>>16;
		buffer[7]=(test&0xFF00)>>8;
		buffer[8]=(test&0xFF);

		for(i=0;i<9;i++)
		{
			crc_value^=buffer[i];
		}
		crc_value &=0x7F;
		crc_value+=1;
		buffer[9]=crc_value;
	}	
	error = socketSend(clientSocketfw, buffer, 10, NULL, 0);
   if(error != NO_ERROR)
   {
		 if(error==ERROR_TIMEOUT)
		 {
				if(timeCompare(time, lastEvent + 60000) >= 0)	
				{
						
				}
				else
					return;
		 }

		 if(serveripno<5)
			 serveripno++;
		 else
		 {
		 serveripno=0;
		 pstr("send error firmware\r");
		 
		 buzzer(3000);
		 gfw_time=10;
		 fw_first_flag=0x00;
		 tcpClientChangeStatefw(TCP_CLIENT_STATE_CLOSE);
		 system_restart();
		 }
	 }
   else
   {
		 	pstr("\rfw data_sent\r");
		 		gfw_time=(total_comm_time*60);
				sprintf(szstr,"\rgfw_time:%d",gfw_time);
				pstr(szstr);
      //Update FSM state
			tcpClientChangeStatefw(TCP_CLIENT_STATE_RECEIVE);
   }
}
u8_t check_config_protocol_fw(void)
{
	if(buffer[4]==0x0F)
	{
				fw_datalen=0;
				fw_datalen=(buffer[5]<<8);
				fw_datalen|=buffer[6];                                         
				fw_flag=1;
				ghb_time=60;
				ftp_option=FW_UPLOAD_TCP;
				pstr("\rNewFWUpload\r");
				return 1;	
	}
	else 
		return 0;
}
void tcpClientStatefwreceive(void)
{
	error_t error;
	size_t length;
	char szstr[32];
	u8_t flag=0;
	u32_t i;
	systime_t time;
	static u32_t flag_timedout=0;
 //Get current time
	time = osGetSystemTime();
	error = socketReceive(clientSocketfw, buffer+rxBufferLen, sizeof(buffer)  - rxBufferLen - 1,&length, 0);
	if(error == NO_ERROR)
	{
		pstr("No error to receive\r");		
		sprintf(szstr,"len:%d",length);
		pstr(szstr);
		 rxBufferLen += length;
		for(i=0;i<rxBufferLen;i++)
		{
			if(ftp_option == FW_UPLOAD_TCP)
				handle_ftp_char(buffer[i]);
			if(ftp_option != FW_UPLOAD_TCP)
			{
				sprintf(szstr,"%02X\r",buffer[i]);
				pstr(szstr);
			}
		}
				
		if(ftp_option != FW_UPLOAD_TCP)
		{
			flag=check_config_protocol_fw();
			if(!flag)
			{
				socketClose(clientSocketfw);
				//Wait for a new connection attempt
				state = TCP_CLIENT_STATE_WAIT;
				//Exit immediately
				return;
			}
			else if(flag==1)
			{
				pstr("\rReady to receive fw");
				state = TCP_CLIENT_STATE_RECEIVE;
			}
			rxBufferLen=0;
		}
		else 
		{
			rxBufferLen=0;
		}	
		if(ftp_option == FW_UPLOAD_TCP)
			state = TCP_CLIENT_STATE_RECEIVE;

	}
	else
	{
		if(timeCompare(time, lastEvent + (300000)) >= 0)	
		{
			if(fw_flag)
			{
				fw_flag=2;
			}
			if(serveripno<5)
				serveripno++;
			else
				serveripno=0;
			tcpClientChangeStatefw(TCP_CLIENT_STATE_CLOSE);
		}
		else
		{
			if(flag_timedout!=gfw_time)
			{
				flag_timedout=gfw_time;
			}
		}
	}
}

void tcpClientStatefwwait(void)
{
	systime_t time;
	//Get current time
	time = osGetSystemTime();
	if(timeCompare(time, lastEvent + (1000)) >= 0)	
	{
		 //Debug message
		gfw_time=10;
		tcpClientChangeStatefw(TCP_CLIENT_STATE_CLOSE);
	}
}

void tcpClientStatefwClose(void)
{
	 socketClose(clientSocketfw);
	 tcpClientChangeStatefw(TCP_CLIENT_STATE_IDEAL);
}

void tcpClientChangeStatefw(uint_t newState)
{
   //Enter the specified state
   state = newState;
   //Save current time
   lastEvent = osGetSystemTime();
}

