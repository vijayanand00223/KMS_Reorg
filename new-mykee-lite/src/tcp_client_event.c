#include "core/net.h"
#include "core/bsd_socket.h"
#include "tcp_client_event.h"
#include "debug.h"
#include "menu_main.h"
#include "lpc23xx.h"
#include "wdt.h"
#include "uart.h"

//Communication timeout
//Buffer size
#define TCP_CLIENT_BUFFER_SIZE 256

//FSM states
#define TCP_CLIENT_EVENT_STATE_INIT 				0
#define TCP_CLIENT_EVENT_STATE_IDEAL     		1
#define TCP_CLIENT_EVENT_STATE_CONNECT	 		2
#define TCP_CLIENT_EVENT_STATE_SEND      		3
#define TCP_CLIENT_EVENT_STATE_RECEIVE      4
#define TCP_CLIENT_EVENT_STATE_CLOSE     		5

//u16_t const CCITT_CRC_TAB2[256] = {
//  0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
//  0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
//  0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
//  0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
//  0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
//  0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
//  0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
//  0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
//  0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
//  0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
//  0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
//  0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
//  0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
//  0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
//  0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
//  0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
//  0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
//  0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
//  0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
//  0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
//  0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
//  0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
//  0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
//  0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
//  0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
//  0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
//  0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
//  0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
//  0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
//  0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
//  0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
//  0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
//};
//Global variables
static uint_t state;
static systime_t lastEvent;
static char_t buffer[33];
static Socket *clientSocketevent;
static u8_t serveripno=0;
static size_t buffer_eve_len;
u8_t buffer_len;
 IpAddr serverIpAddr;
void tcpClient_eventInit(void)
{
   //Debug message
   pstr("TCP client event initialization...\r\n");
   //Switch to default state
   tcpClientChangeeventState(TCP_CLIENT_EVENT_STATE_IDEAL);
}


void tcpClienteventTask(void)
{
		if(fw_flag)
		return;
   switch(state)
   {
		 case TCP_CLIENT_EVENT_STATE_INIT:
			 break;
   case TCP_CLIENT_EVENT_STATE_IDEAL:
      tcpClienteventStateInit();
      break;
   case TCP_CLIENT_EVENT_STATE_CONNECT:
      tcpClienteventStateConnect();
      break;
//	 case TCP_CLIENT_EVENT_STATE_OPEN_DELAY:
//      tcpClienteventStateopenwait();
//      break;
   case TCP_CLIENT_EVENT_STATE_SEND:
      tcpClienteventStateSend();
      break;
	 case TCP_CLIENT_EVENT_STATE_RECEIVE:
      tcpClienteventStatereceive();
      break;
   case TCP_CLIENT_EVENT_STATE_CLOSE:
      tcpClienteventStateClose();
      break;
   default:
      //Switch to default state
      tcpClientChangeeventState(TCP_CLIENT_EVENT_STATE_IDEAL);
      break;
   }
}

s32_t tcpconfig_client_protocol_send(u32_t pro_length)
{
	buffer_len =pro_length;
	gcommunication_time=0;
	return 1;
}
void tcpClienteventStateInit(void)
{
	char szstr[32];
		int_t error;
		static u8_t fail_cnt=0;
   //User button pressed?
   if(glast_event_check!=glast_event && hb_event_loginflag==0)
   {
			read_event_sd(glast_event_check);
			if(event_t.sent_event==0xAA)
			{
				sprintf(szstr,"EVTCONNECT:%d\r",glast_event_check);
				pstr(szstr);
				read_comm_sd();
				ev_port=comm_t.event_port;
			if(sys_t.conn_type==HOSTADDR)
			{
				read_comm_sd();
				sprintf(szstr,"\rHost Addr:%s",comm_t.url);
				pstr(szstr);
			 
				error = getHostByName(NULL, comm_t.url, &serverIpAddr, 0);
				if(error)
				{
					 TRACE_INFO("Failed to resolve server name!\r\n");
					//return;
					
					if(error==ERROR_IN_PROGRESS )
					{
						fail_cnt++;
						if(fail_cnt >= 30)	
						{
							fail_cnt=0;
								pstr("\rError_Timeout_\r");
						}
						else
					return;
					}
					pstr("send event error 1");
					
					tcpClientChangeeventState(TCP_CLIENT_EVENT_STATE_CLOSE);
				}
				else
				{
					 //Debug message
					sprintf (szstr,"Conn Host %s...\r\n",
						 ipAddrToString(&serverIpAddr, NULL));
						 pstr(szstr);
					}
				}
				else
				{
					pstr("\rRemote IP");	
					serverIpAddr.length = sizeof(Ipv4Addr);							
					serverIpAddr.ipv4Addr=rmip1;
				}			
				sprintf(szstr,"IP:%X-Port:%d\r",serverIpAddr.ipv4Addr,ev_port);
				pstr(szstr);
				tcpClientChangeeventState(TCP_CLIENT_EVENT_STATE_CONNECT);
				return;
			}
			else
			{
				if(glast_event_check<MAX_EVENTS)
				{
					glast_event_check++;
				}
				else
				{
					glast_event_check=0;
				}
			}
			sprintf(szstr,"EVTERROR:%d\r",glast_event_check);
			pstr(szstr);
			tcpClientChangeeventState(TCP_CLIENT_EVENT_STATE_IDEAL); 		
	 }
	 else
	 {
      tcpClientChangeeventState(TCP_CLIENT_EVENT_STATE_IDEAL);
	 }
}

void tcpClienteventStateConnect(void)
{
  int_t error;	
	pstr("Socket event connect\r");
	clientSocketevent = socketOpen(SOCKET_TYPE_STREAM, SOCKET_IP_PROTO_TCP);
	if(!clientSocketevent) 
	{
		pstr("Event Socket open error\r");
		tcpClientChangeeventState(TCP_CLIENT_EVENT_STATE_CLOSE);
		return;		
	}
	error = socketConnect(clientSocketevent, &serverIpAddr, ev_port);		
	if(error==NO_ERROR || error==ERROR_TIMEOUT)
	{
		tcpClientChangeeventState(TCP_CLIENT_EVENT_STATE_SEND);
	}
	else
	{
			if(serveripno<5)
				serveripno++;
			else
				serveripno=0;
			tcpClientChangeeventState(TCP_CLIENT_EVENT_STATE_CLOSE);		
	}
}
void crc_calculation_client_event(u16_t length)
{
	u16_t i;
	u8_t crc=0;
	u16_t crc_tab_value;
	char szstr[32];
	for(i=0;i<length;i++)
	{
		sprintf(szstr,"%02X",buffer[i]);
		pstr(szstr);
		crc^=buffer[i];
	}
	crc_tab_value=CCITT_CRC_TAB[crc];
	buffer[length]=(crc_tab_value>>8)&0xFF;
	buffer[length+1]=(crc_tab_value)&0xFF;	
}
/******************************************************************************
 ** Function name:      handle_protocol_packet 
 **
 ** Descriptions:       this function explains the protocol communication.
 **						here we can uploading bin/html file in to sd card
 **						code_xfer=1-->firmware file, code_xfer =2 -->html upload
 **
 ** parameters:         None
 ** 				 
 ** Returned value:     None
 ** 
 ******************************************************************************/
void  handle_protocol_packet()
{
	u32_t test=0;
	u8_t i;

		pstr("event data sending\r");
		read_event_sd(glast_event_check);
		
		buffer[0]=(glast_event_check>>8 & 0xFF);
		buffer[1]=(glast_event_check & 0xFF); 
		buffer[2]=(sys_t.cus_id&0xFF000000)>>24;
		buffer[3]=(sys_t.cus_id&0xFF0000)>>16;
		buffer[4]=(sys_t.cus_id &0xFF00)>>8;
		buffer[5]=(sys_t.cus_id &0xFF);
		buffer[6]=(sys_t.serial&0xFF000000)>>24;
		buffer[7]=(sys_t.serial&0xFF0000)>>16;
		buffer[8]=(sys_t.serial &0xFF00)>>8;
		buffer[9]=(sys_t.serial &0xFF);
		buffer[10]=sys_t.time_zone;
		test=event_t.ticks;
		buffer[11]=(test&0xFF000000)>>24;
		buffer[12]=(test&0xFF0000)>>16;
		buffer[13]=(test &0xFF00)>>8;
		buffer[14]=(test&0xFF);
		buffer[15]=(event_t.code>>8) &0xFF;
		buffer[16]=(event_t.code)&0xFF;
		
		buffer[17]=(event_t.user>>24)&0xFF;
		buffer[18]=(event_t.user>>16)&0xFF;
		buffer[19]=(event_t.user>>8)&0xFF;
		buffer[20]=(event_t.user)&0xFF;
		buffer[21]=event_t.key11;//dummy for key in esto
		buffer[22]=(event_t.task>>8) &0xFF;
		buffer[23]=(event_t.task)&0xFF;
		
		crc_calculation_client_event(24);
		for(i=26;i<32;i++)
		{
			buffer[i]=0;
		}
		buffer[32]=0x07;	
		buffer_eve_len=33;
		EncyptPIAMPacket(buffer);
}
u8_t checksum_verification_event(u16_t length)
{
	u16_t i;
	u8_t crc=0;
	u16_t crc_tab_value,crc_received;
	char szstr[32];
	for(i=0;i<length-2;i++)
		crc^=buffer[i];
	crc_tab_value=CCITT_CRC_TAB[crc];
	crc_received=(buffer[length-2]<<8)|(buffer[length-1]&0xFF);
	sprintf(szstr,"CRC:%X-%X\r",crc_tab_value,crc_received);
	pstr(szstr);
	if(crc_tab_value==crc_received)
		return 1;
	else
		return 0;
}
void check_event_receive(void)
{
	u16_t message_id=0;
	char szstr[32];
	u32_t test=0;
	message_id=((buffer[1]&0xFF)|((buffer[0]<<8)&0xFF00));
	if(message_id!=glast_event_check)
	{
		sprintf(szstr,"\rMID:%X,%X\r",message_id,glast_event_check);
		pstr(szstr);
		return;
	}
	test |= (buffer[2] << 24);
	test |= (buffer[3] << 16);
	test |= (buffer[4] << 8);
	test |= (buffer[5]);
	sprintf(szstr,"\rSerialNo:%X - %X\r",test,ks_serial);
		pstr(szstr);
	if(test!=ks_serial)
		{
		
		return;
	}
	
	if(checksum_verification_event(8))
	{
		read_event_sd(glast_event_check);
		event_t.sent_event=0;
		write_event_sd(glast_event_check);
		if(glast_event_check<MAX_EVENTS)
		{
			glast_event_check++;
		}
		else
		{
			glast_event_check=0;
		}

	}
}
void tcpClienteventStateSend(void)
{
   int_t error;
	static u8_t event_first_flag=0;
	  systime_t time;
   		size_t length;
			char szstr[100];
	char charac[3];
	u8_t i;
	   static u32_t flag_timedout=0;
   //Get current time
		time = osGetSystemTime();
	if(!event_first_flag)
	{
		event_first_flag=0x01;
		handle_protocol_packet();
		pstr("Sending Event");
		sprintf(szstr,"%02X",buffer[0]);
		for(i=1;i<33;i++)
		{
			sprintf(charac,"%02X",buffer[i]);
			strcat(szstr,charac);
		}
	}
	error = socketSend(clientSocketevent, buffer, buffer_eve_len , NULL, 0);
   if(error != NO_ERROR)
   {
		  if(error==ERROR_TIMEOUT)
		 {
				if(timeCompare(time, lastEvent +(10000)) >= 0)	
				{

				}
				else
					return;
					
		 }
		 if(serveripno<5)
				serveripno++;
			else
				serveripno=0;
			pstr("send event error");
		  tcpClientChangeeventState(TCP_CLIENT_EVENT_STATE_CLOSE);
			event_first_flag=0x00;
	 }
   else
   {
		 	event_first_flag=0x00;
		 	pstr("dataevent_sent\r");
		 
		 while(1)
		 {
			 	time = osGetSystemTime();
		 		error = socketReceive(clientSocketevent, buffer, sizeof(buffer) - 1,&length, 0);
				if(error == NO_ERROR)
				{	
						pstr("No error to receive\r");		
						pstr("No error");	
						sprintf(szstr,"len:%d\r",length);
						pstr(szstr);
						if(length==16)
						{
						pstr("\rEvent Response Received:");
								for(i=0;i<17;i++)
								{
									sprintf(szstr,"%02X",buffer[i]);
									pstr(szstr);
								}						
								DecyptPIAMPacket(buffer,length);
   							check_event_receive();			
						}
						tcpClientChangeeventState(TCP_CLIENT_EVENT_STATE_CLOSE);
						break;
				}
				else
				{
					if(timeCompare(time, lastEvent + (20000)) >= 0)	
					{
						tcpClientChangeeventState(TCP_CLIENT_EVENT_STATE_RECEIVE);
							break;
					}		
					else
					{
						if(flag_timedout!=ghb_time)
						{
							flag_timedout=ghb_time;
							sprintf(szstr,"Receive err:%d\n",error);
							pstr(szstr);
						}
					}
				}
		 			netTask();
					telnetServerTask();
					if(!rstflag)
					{
						wdt_reset_feed();
					}
		 }
   }
}


void tcpClienteventStatereceive(void)
{
		error_t error;
		size_t length;
		char szstr[100];
		u8_t i;
		char charac[3];
		systime_t time;
   static u32_t flag_timedout=0;
		time = osGetSystemTime();
		error = socketReceive(clientSocketevent, buffer, sizeof(buffer) - 1,&length, 0);
		if(error == NO_ERROR)
		{	
				pstr("No error to receive\n");		
				pstr("No error");	
				sprintf(szstr,"len:%d\r",length);
				pstr(szstr);
				if(length==16)
				{
								for(i=0;i<17;i++)
								{
									sprintf(szstr,"%02X",buffer[i]);
									pstr(szstr);
								}
						pstr("\r");
						DecyptPIAMPacket(buffer,length);
						check_event_receive();
				}
				tcpClientChangeeventState(TCP_CLIENT_EVENT_STATE_CLOSE);
		}
		else
		{
			if(timeCompare(time, lastEvent + (1000)) >= 0)	
			{
				if(serveripno<5)
					serveripno++;
				else
					serveripno=0;
				tcpClientChangeeventState(TCP_CLIENT_EVENT_STATE_CLOSE);
			}		
			else
			{
				if(flag_timedout!=ghb_time)
				{
					flag_timedout=ghb_time;
							sprintf(szstr,"Receive err:%d\n",error);
							pstr(szstr);	
				}
			}
		}
}
void tcpClienteventStateClose(void)
{
	 socketClose(clientSocketevent);
   tcpClientChangeeventState(TCP_CLIENT_EVENT_STATE_IDEAL);
}
void tcpClientChangeeventState(uint_t newState)
{
   //Enter the specified state
   state = newState;
   //Save current time
   lastEvent = osGetSystemTime();
}


