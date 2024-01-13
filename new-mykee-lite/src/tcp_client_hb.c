#include "core/net.h"
#include "core/bsd_socket.h"
#include "tcp_client_hb.h"
#include "debug.h"
#include "menu_main.h"
#include "adc.h"
#include "uart.h"
#include "CLOCK_CALNDR.h"
#include "lpc23xx.h"

//FSM states
	
#define TCP_CLIENT_STATE_INIT 				0
#define TCP_CLIENT_STATE_IDEAL     		1
#define TCP_CLIENT_STATE_CONNECT	 		2
#define TCP_CLIENT_STATE_SEND      		3
#define TCP_CLIENT_STATE_RECEIVE      4
#define TCP_CLIENT_STATE_CLOSE     		5
#define TCP_CLIENT_STATE_OPEN_DELAY		6
 
#define CMD_SET_REMOTE_IP				0x03
#define CMD_SET_REMOTE_IPDNS		0x04
#define CMD_SET_RTC							0x05
#define CMD_RESET_TARGET				0x07
#define CMD_ADD_MOD_USER				0x09
#define CMD_DEL_USER						0x0B
#define CMD_DEL_ALL_USER				0x0C
#define CMD_SET_CONFIG					0x1A
#define CMD_GET_FIRMWARE_VER		0x16
#define CMD_MOD_PORT						0x18
#define CMD_FW_UPLOAD						0x19
#define CMD_ADD_KEY							0x1B
#define CMD_DELETE_KEY					0x1C
#define CMD_DELETE_ALLKEY				0x1D
#define CMD_ADD_EDIT_USER_T			0x1E
#define CMD_ADD_TASK						0x1F
#define CMD_DELETE_TASK					0x20
#define CMD_DELETEALL_TASK			0x21
#define CMD_CLOSE_CONN					0xFF


#define RESP_SUCCESS						0xFF
#define RESP_FAILURE						0xFE
#define RESP_SERIALNO_NOTEXIST	0xFD
#define RESP_CMD_END						0xFC
#define RESP_USER_DUPLICATE			0xE8
#define RESP_DB_OVERFLOW				0xE7
#define RESP_NO_USER						0xE6

//u16_t const CCITT_CRC_TAB1[256] = {
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
static char_t buffer[256];
static char_t rxbuffer[1050];
static Socket *clientSockethb;
static u8_t serveripno=0;
u8_t flag_hb_network=0;
static size_t buffer_len;

u8_t crc_calculation_client(u16_t length)
{
	u16_t i;
	u8_t crc=0,remain_value;
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
	sprintf(szstr,"\rCRC:%02X%02X\r",buffer[length],buffer[length+1]);
	pstr(szstr);
	length+=2;
	if(length%16)
		remain_value=16-(length%16);
	else
		remain_value=0;
	sprintf(szstr,"\rRemain Val:%d Len:%d",remain_value,(length+remain_value+1) );
	pstr(szstr);
		for(i=length;i<length+remain_value;i++)
		{
			buffer [i]=0;
		}
		for(i=0;i<length+remain_value;i++)
		{

		}
		buffer [length+remain_value]=remain_value;
		return (length+remain_value+1);
}
u8_t checkcrc_server(u16_t length)
{
	u16_t i;
	u8_t crc=0;
	u16_t crc_tab_value,crc_received;
	char szstr[32];
	for(i=0;i<length-2;i++)
		crc^=rxbuffer[i];
	crc_tab_value=CCITT_CRC_TAB[crc];
	crc_received=(rxbuffer[length-2]<<8)|(rxbuffer[length-1]&0xFF);
	sprintf(szstr,"CRC:%X-%X\r",crc_tab_value,crc_received);
	pstr(szstr);
	if(crc_tab_value==crc_received)
		return 1;
	else
		return 0;
}
u8_t check_serial_no_server(void)
{
	u32_t serial_temp=0;
	char szstr[32];
	
	serial_temp |= (rxbuffer[7] & 0xFF) | ((rxbuffer[6] << 8) & 0x0000FF00) | ((rxbuffer[5] << 16) & 0x00ff0000) | ((rxbuffer[4] << 24) & 0xff000000);
	sprintf(szstr,"Ser:%d-%d-%02X\r",serial_temp,ks_serial,rxbuffer[8]);
	pstr(szstr);
	if(serial_temp==ks_serial)
		return 1;
	else
		return 0;
}

void tcpClientInit(void)
{
   //Debug message
   pstr("TCP client hb initialization...\r\n");
   //Switch to default state
   tcpClientChangeState(TCP_CLIENT_STATE_IDEAL);
}


void tcpClienthbTask(void)
{
  switch(state)
  {
		 case TCP_CLIENT_STATE_INIT:
			 break;
   case TCP_CLIENT_STATE_IDEAL:
      tcpClientStateInit();
      break;
   case TCP_CLIENT_STATE_CONNECT:
      tcpClientStateConnect();
      break;
	 case TCP_CLIENT_STATE_OPEN_DELAY:
      tcpClientStateopenwait();
      break;
   case TCP_CLIENT_STATE_SEND:
      tcpClientStateSend();
      break;
	 case TCP_CLIENT_STATE_RECEIVE:
      tcpClientStatereceive();
      break;
   case TCP_CLIENT_STATE_CLOSE:
      tcpClientStateClose();
      break;
   default:
      //Switch to default state
      tcpClientChangeState(TCP_CLIENT_STATE_IDEAL);
      break;
   }
}
void tcpClientStateInit(void)
{
 
   if(!ghb_time)
   {
	 pstr("\rHB Connect");

		 hb_event_loginflag=1;
		 	tcpClientChangeState(TCP_CLIENT_STATE_CONNECT);
   }
	 else
	 {
		 hb_event_loginflag=0;
      tcpClientChangeState(TCP_CLIENT_STATE_IDEAL);
	 }
}


void tcpClientStateConnect(void)
{
   int_t error;
   IpAddr serverIpAddr;
	char szstr[32];
	static u8_t fail_cnt=0;
	 
	 	clientSockethb = socketOpen(SOCKET_TYPE_STREAM, SOCKET_IP_PROTO_TCP);
				if(!clientSockethb) 
				{

					tcpClientChangeState(TCP_CLIENT_STATE_CLOSE);
					return;
					
				}
pstr("\rHB send  init");
  	if(sys_t.conn_type==HOSTADDR)
		{
			read_comm_sd();
		  sprintf(szstr,"\rHost Addr:%s",comm_t.url);
		  pstr(szstr);
		 
		  error = getHostByName(NULL, comm_t.url, &serverIpAddr, 0);
			if(error)
			{
				sprintf(szstr,"error%d\r",error);
				pstr(szstr);
				 //Debug message
				
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
			
			tcpClientChangeState(TCP_CLIENT_STATE_CLOSE);
		}
		else
		{
			 //Debug message
			 pstr("\rConnecting to HTTP_TRACE_LEVEL serverIpAddr");

			}
		}
		else
		{
			pstr("\rRemote IP");
		
						
					serverIpAddr.length = sizeof(Ipv4Addr);
					
				serverIpAddr.ipv4Addr=rmip1;
						
		}
			read_comm_sd();
			hb_port=comm_t.hb_port;
				sprintf(szstr,"HB_IP:%X-Port:%d\r",serverIpAddr.ipv4Addr,hb_port);
					pstr(szstr);
					error = socketConnect(clientSockethb, &serverIpAddr, hb_port);
				if(error==NO_ERROR || error==ERROR_TIMEOUT)
				{
					tcpClientChangeState(TCP_CLIENT_STATE_SEND);
					pstr("HB Socket coneect error\r");		
				}
				else
				{
						if(serveripno<5)
							serveripno++;
						else
							serveripno=0;
						tcpClientChangeState(TCP_CLIENT_STATE_CLOSE);		
				}
}
void tcpClientStateopenwait(void)
{
	   systime_t time;
   
   //Get current time
		time = osGetSystemTime();
		if(timeCompare(time, lastEvent + 50000) >= 0)	
		{
			if(serveripno<5)
				serveripno++;
			else
				serveripno=0;
				pstr("Socket coneect timedout\r");
				tcpClientChangeState(TCP_CLIENT_STATE_CLOSE);		
		}
		if(clientSockethb->state==4)
		{
			tcpClientChangeState(TCP_CLIENT_STATE_SEND);
		}
	
}
void  handle_hb_protocol_packet()
{
	u32_t test=0;
	u16_t us_volt;
	u8_t uci;

		pstr("hb data sending\r");
	if(!flag_init)
	{
		test =get_ticks_value();
		flag_init=0x01;
		read_system_sd();			
		buffer[0]=(sys_t.cus_id&0xFF000000)>>24;
		buffer[1]=(sys_t.cus_id&0xFF0000)>>16;
		buffer[2]=(sys_t.cus_id &0xFF00)>>8;
		buffer[3]=(sys_t.cus_id &0xFF);
		buffer[4]=(sys_t.serial&0xFF000000)>>24;
		buffer[5]=(sys_t.serial&0xFF0000)>>16;
		buffer[6]=(sys_t.serial &0xFF00)>>8;
		buffer[7]=(sys_t.serial &0xFF);	
		if(sys_t.factory==0x01)
		{
			buffer[8]=0xFE;
		}
		else
			buffer[8]=0xFF;
		buffer[9]=0x03;				//APP_ID for Mykee
		buffer[10]=MAX_KEYS;
		buffer[11]=sys_t.time_zone;
		buffer[12]=(test&0xFF000000)>>24;
		buffer[13]=(test&0xFF0000)>>16;
		buffer[14]=(test&0xFF00)>>8;
		buffer[15]=(test&0xFF);
		
		us_volt=0;
		if (AD0DR1 & (1<<31)) //AC
		{
			us_volt = read_ac_adc_ch();
		}
		buffer[16]=(us_volt&0xFF00)>>8;
		buffer[17]=(us_volt&0xFF);
		us_volt=0;
		if (AD0DR0 & (1<<31)) //BATTERY
		{
			us_volt = read_dc_adc_ch();
		}
		buffer[18]=(us_volt&0xFF00)>>8;
		buffer[19]=(us_volt&0xFF);
		
		buffer_len=crc_calculation_client(20);
		
		EncyptPIAMPacket_hb(buffer_len-1,buffer);
	}
	else
	{
	}
}
void handle_protocol_configpacket_event(void)
{	
	u8_t flag_return=0,i;
	u32_t test_code=0,test=0;
	s16_t sd_location=0;
	u32_t bkp_var=0;
	u16_t facility_temp=0;
	u32_t card_numtemp=0;
	s32_t suser_code=0;
	u32_t user_id_temp=0;
	u32_t user_pin_temp=1234;
	char szstr[32];
	u8_t uci=0,key_no=0;
	u8_t finger_flag=0xFF;
	pstr("\rChecking\r");

	switch(rxbuffer[8])
	{
		case CMD_SET_REMOTE_IP:
		test=0;
					test |= (rxbuffer[12]);
					test |= (rxbuffer[11] << 8);
					test |= (rxbuffer[10] << 16);
					test |= (rxbuffer[9] << 24);
					rmip1=comm_t.remote_ip1=test;
					sprintf(szstr,"sip1:%08X\r",test);
					pstr(szstr);
					test=0;
					test |= (rxbuffer[16]);
					test |= (rxbuffer[15] << 8);
					test |= (rxbuffer[14] << 16);
					test |= (rxbuffer[13] << 24);
					rmip2=comm_t.remote_ip2=test;
					sprintf(szstr,"Sip2:%08X\r",test);
					pstr(szstr);
					test=0;
					test |= (rxbuffer[20]);
					test |= (rxbuffer[19] << 8);
					test |= (rxbuffer[18] << 16);
					test |= (rxbuffer[17] << 24);
					rmip3=comm_t.remote_ip3=test;
					write_comm_sd();
					sprintf(szstr,"Sip3:%08X\r",test);
					pstr(szstr);
				
				buffer[0]=(sys_t.cus_id&0xFF000000)>>24;
		buffer[1]=(sys_t.cus_id&0xFF0000)>>16;
		buffer[2]=(sys_t.cus_id &0xFF00)>>8;
		buffer[3]=(sys_t.cus_id &0xFF);
		buffer[4]=(sys_t.serial&0xFF000000)>>24;
		buffer[5]=(sys_t.serial&0xFF0000)>>16;
		buffer[6]=(sys_t.serial &0xFF00)>>8;
		buffer[7]=(sys_t.serial &0xFF);	
				buffer[8]=CMD_SET_REMOTE_IP;
				buffer[9]=RESP_SUCCESS;
					buffer_len=crc_calculation_client(10);
				EncyptPIAMPacket_hb(buffer_len-1,buffer);
		break;
		case CMD_SET_RTC:
				test=0;
				test|=(rxbuffer[12]);
				test|=(rxbuffer[11]<<8);
				test|=(rxbuffer[10]<<16);					
				test|=(rxbuffer[9]<<24);
				sprintf(szstr,"TIME%08X",test);
				pstr(szstr);
				set_date_time(test);				
				endtick_value=get_ticks_value();
				buffer[0]=(sys_t.cus_id&0xFF000000)>>24;
		buffer[1]=(sys_t.cus_id&0xFF0000)>>16;
		buffer[2]=(sys_t.cus_id &0xFF00)>>8;
		buffer[3]=(sys_t.cus_id &0xFF);
		buffer[4]=(sys_t.serial&0xFF000000)>>24;
		buffer[5]=(sys_t.serial&0xFF0000)>>16;
		buffer[6]=(sys_t.serial &0xFF00)>>8;
		buffer[7]=(sys_t.serial &0xFF);	
				buffer[8]=CMD_SET_RTC;
				buffer[9]=RESP_SUCCESS;
				buffer_len=crc_calculation_client(10);
				EncyptPIAMPacket_hb(buffer_len-1,buffer);
		break;
	case CMD_RESET_TARGET:
			if(rxbuffer[9]==0x01)
			{
				system_restart();
			}
			else if(rxbuffer [9]==0x02)
			{
				writedefaults_sdcard();
				system_restart();
			}
			else
				system_restart();	
	break;
	case CMD_ADD_MOD_USER:
	
	test=0;
				test |=(rxbuffer[12] & 0xFF)|((rxbuffer[11]<<8)&0xFF00)|((rxbuffer[10]<<16)&0xFF0000)|((rxbuffer[9]<<24)&0xFF000000);
				user_id_temp=test;
				test=0;
			test|=(rxbuffer[16] & 0xFF)|((rxbuffer[15]<<8)&0xFF00)|((rxbuffer[14]<<16)&0xFF0000)|((rxbuffer[13]<<24)&0xFF000000);
			user_pin_temp =test;
			test=0;
			test|=(rxbuffer[20] & 0xFF)|((rxbuffer[19]<<8)&0xFF00)|((rxbuffer[18]<<16)&0xFF0000)|((rxbuffer[17]<<24)&0xFF000000);
			
			facility_temp = 0x01;
			card_numtemp=test;
			sprintf(szstr,"User ID:%X\r\n",user_id_temp);
			pstr(szstr);
			if(check_user_bio(user_id_temp))
			{
				pstr("Finger enabled\r\n");
				finger_flag=1;
			}
			else
			{
				finger_flag=0xFF;
				pstr("Finger Disabeld\r\n");
			}
			suser_code=modify_user(user_id_temp);
			if(suser_code>=0)
			{
				pstr("User added\r");
				sprintf(szstr,"location:%d\r",suser_code);
				pstr(szstr);
				read_user_sd(suser_code);
				
				user_t.user_id=user_id_temp ;
				user_t.card=card_numtemp;
				user_t.enabled=rxbuffer[36];
				user_t.facility_code=facility_temp;
				user_t.issue_lvl=rxbuffer[9];
				user_t.issue_type=0x00;
				user_t.user_pin=1234;
				user_t.finger_enabled=finger_flag;
				test=0;
				test =(rxbuffer[35] & 0xFF)|((rxbuffer[34]<<8)&0xFF00)|((rxbuffer[33]<<16)&0xFF0000)|((rxbuffer[32]<<24)&0xFF000000);
				user_t.end_tickvalue=test;
				if(test<user_end_tickvalue)
					user_end_tickvalue=test;
				sprintf(szstr,"ET:%X,TI:%X\r",user_end_tickvalue,endtick_value);
				pstr(szstr);
				user_t.login_type=rxbuffer[21];
				test=rxbuffer[23]&0x7F;
				user_t.dow= (test&0x01)| ((test&0x02)<<5)|((test&0x04)<<3)|((test&0x08)<<1)|((test&0x10)>>1)|((test&0x20)>>3)|((test&0x40)>>5);
				user_t.holiday=(rxbuffer[23]>>7)&0x01;
				sprintf(szstr,"DOW:%X,H:%X\r",user_t.dow,user_t.holiday);
				pstr(szstr);
				user_t.access_type=rxbuffer[22];
				test=0;
				test =(rxbuffer[27] & 0xFF)|((rxbuffer[26]<<8)&0xFF00)|((rxbuffer[25]<<16)&0xFF0000)|((rxbuffer[24]<<24)&0xFF000000);
				user_t.user_key =test;
				sprintf(szstr,"key:%X\r",test);
				pstr(szstr);
				test=0;
				test =(rxbuffer[31] & 0xFF)|((rxbuffer[30]<<8)&0xFF00)|((rxbuffer[29]<<16)&0xFF0000)|((rxbuffer[28]<<24)&0xFF000000);
				user_t.start_date=test;
				user_t.start_time=get_sec_time_value(test);
				sprintf(szstr,"ST:%X\r",test);
				pstr(szstr);
				verify_date_time(test);
				test=0;
				test =(rxbuffer[35] & 0xFF)|((rxbuffer[34]<<8)&0xFF00)|((rxbuffer[33]<<16)&0xFF0000)|((rxbuffer[32]<<24)&0xFF000000);
				user_t.end_date=test;
				user_t.end_time=get_sec_time_value(test);
				sprintf(szstr,"ET:%X\r",test);
				pstr(szstr);
				verify_date_time(test);
				write_user_sd(suser_code);
				sprintf(szstr,"Id:%d Pin:%d card%X\r",user_t.user_id,user_t.user_pin,user_t.card);
				pstr(szstr);					
		 		buffer[0]=(sys_t.cus_id&0xFF000000)>>24;
				buffer[1]=(sys_t.cus_id&0xFF0000)>>16;
				buffer[2]=(sys_t.cus_id &0xFF00)>>8;
				buffer[3]=(sys_t.cus_id &0xFF);
				buffer[4]=(sys_t.serial&0xFF000000)>>24;
				buffer[5]=(sys_t.serial&0xFF0000)>>16;
				buffer[6]=(sys_t.serial &0xFF00)>>8;
				buffer[7]=(sys_t.serial &0xFF);	
				buffer[8]=CMD_ADD_MOD_USER;
				buffer[9]=RESP_SUCCESS;
				buffer[10]=rxbuffer[9];
				buffer[11]=rxbuffer[10];
				buffer[12]=rxbuffer[11];
				buffer[13]=rxbuffer[12];
				buffer_len=crc_calculation_client(14);
				EncyptPIAMPacket_hb(buffer_len-1,buffer);
					
			}
			else
			{
				
				if(suser_code==-1)
				{
					pstr("User Duplicated\r");						
		 			buffer[0]=(sys_t.cus_id&0xFF000000)>>24;
					buffer[1]=(sys_t.cus_id&0xFF0000)>>16;
					buffer[2]=(sys_t.cus_id &0xFF00)>>8;
					buffer[3]=(sys_t.cus_id &0xFF);
					buffer[4]=(sys_t.serial&0xFF000000)>>24;
					buffer[5]=(sys_t.serial&0xFF0000)>>16;
					buffer[6]=(sys_t.serial &0xFF00)>>8;
					buffer[7]=(sys_t.serial &0xFF);	
					buffer[8]=CMD_ADD_MOD_USER;
					buffer[9]=RESP_USER_DUPLICATE;
					buffer[10]=rxbuffer[10];
					buffer[11]=rxbuffer[11];
					buffer[12]=rxbuffer[12];
					buffer[13]=rxbuffer[13];
					buffer_len=crc_calculation_client(14);
				
					EncyptPIAMPacket_hb(buffer_len-1,buffer);
				}
				else
				{
					pstr("DB Overflow\r");
					buffer[0]=(sys_t.cus_id&0xFF000000)>>24;
					buffer[1]=(sys_t.cus_id&0xFF0000)>>16;
					buffer[2]=(sys_t.cus_id &0xFF00)>>8;
					buffer[3]=(sys_t.cus_id &0xFF);
					buffer[4]=(sys_t.serial&0xFF000000)>>24;
					buffer[5]=(sys_t.serial&0xFF0000)>>16;
					buffer[6]=(sys_t.serial &0xFF00)>>8;
					buffer[7]=(sys_t.serial &0xFF);	
					buffer[8]=CMD_ADD_MOD_USER;
					buffer[9]=RESP_DB_OVERFLOW;
					buffer[10]=rxbuffer[10];
					buffer[11]=rxbuffer[11];
					buffer[12]=rxbuffer[12];
					buffer[13]=rxbuffer[13];
					buffer_len=crc_calculation_client(14);
				
					EncyptPIAMPacket_hb(buffer_len-1,buffer);
				}
			}
	break;
	case CMD_DEL_USER:
		test=0;
		test|=(rxbuffer[12] & 0xFF)|((rxbuffer[11]<<8)&0xFF00)|((rxbuffer[10]<<16)&0xFF0000)|((rxbuffer[9]<<24)&0xFF000000);
		user_id_temp=test;
		sprintf(szstr,"delete User ID:%d",user_id_temp );
		pstr(szstr);
		if(delete_user(user_id_temp))
		{
			buffer[9]=RESP_SUCCESS;
  	}			
		else
		buffer[9]=RESP_NO_USER;
		buffer[0]=(sys_t.cus_id&0xFF000000)>>24;
		buffer[1]=(sys_t.cus_id&0xFF0000)>>16;
		buffer[2]=(sys_t.cus_id &0xFF00)>>8;
		buffer[3]=(sys_t.cus_id &0xFF);
		buffer[4]=(sys_t.serial&0xFF000000)>>24;
		buffer[5]=(sys_t.serial&0xFF0000)>>16;
		buffer[6]=(sys_t.serial &0xFF00)>>8;
		buffer[7]=(sys_t.serial &0xFF);	
		buffer[8] =CMD_DEL_USER;
		buffer[10] =rxbuffer[9];
		buffer[11] =rxbuffer[10];
		buffer[12] =rxbuffer[11];
		buffer[13] =rxbuffer[12];
		buffer_len=crc_calculation_client(14);
		EncyptPIAMPacket_hb(buffer_len-1,buffer);
	break;
	case CMD_ADD_KEY:
		flag_return= rxbuffer[9];			
		if(flag_return<=MAX_KEYS)
		{
			for(uci=0;uci<flag_return;uci++)
			{
				key_no=rxbuffer[10+(uci*2)]-1;
				read_key_sd((s32_t) key_no);
				key1_t[key_no].exist =(u32_t) EXIST;
				key1_t[key_no].keyid=key_no;
				key1_t[key_no].enabled=rxbuffer[11+(uci*2)];
				if(key1_t[key_no].enabled==3 || key1_t[key_no].enabled==4)
					key1_t[key_no].enabled=0;
				sprintf(szstr,"\rKey:%d Key Status:%d",key1_t[key_no].keyid,key1_t[key_no].enabled);
				pstr(szstr);
			}
			write_key_sd();
			
				buffer[9]=RESP_SUCCESS;			
		}
		else
		{
			pstr("No key1 response\r");
			buffer[9]=RESP_NOKEY;
		}
		read_system_sd();
		buffer[0]=(sys_t.cus_id&0xFF000000)>>24;
		buffer[1]=(sys_t.cus_id&0xFF0000)>>16;
		buffer[2]=(sys_t.cus_id &0xFF00)>>8;
		buffer[3]=(sys_t.cus_id &0xFF);
		buffer[4]=(sys_t.serial&0xFF000000)>>24;
		buffer[5]=(sys_t.serial&0xFF0000)>>16;
		buffer[6]=(sys_t.serial &0xFF00)>>8;
		buffer[7]=(sys_t.serial &0xFF);
		buffer[8] =CMD_ADD_KEY;
		buffer_len=crc_calculation_client(10);
				EncyptPIAMPacket_hb(buffer_len-1,buffer);
		break;
	case CMD_DELETE_KEY:
		flag_return= rxbuffer[9];
		if(flag_return<MAX_KEYS)
		{
			for(uci=0;uci<flag_return;uci++)
			{
				key_no=rxbuffer[10+(uci*1)];
				read_key_sd((s32_t) key_no);
				key1_t[flag_return].exist =(u32_t) NOT_EXIST;	
			}
			write_key_sd();
			buffer[0]=(sys_t.cus_id&0xFF000000)>>24;
		buffer[1]=(sys_t.cus_id&0xFF0000)>>16;
		buffer[2]=(sys_t.cus_id &0xFF00)>>8;
		buffer[3]=(sys_t.cus_id &0xFF);
		buffer[4]=(sys_t.serial&0xFF000000)>>24;
		buffer[5]=(sys_t.serial&0xFF0000)>>16;
		buffer[6]=(sys_t.serial &0xFF00)>>8;
		buffer[7]=(sys_t.serial &0xFF);	
		buffer[8]=CMD_DELETE_KEY;
		buffer[9]=RESP_SUCCESS;
		buffer_len=crc_calculation_client(10);
		EncyptPIAMPacket_hb(buffer_len-1,buffer);
			
		}
		else
		{
			pstr("No delete key1 response\r");
			buffer[0]=(sys_t.cus_id&0xFF000000)>>24;
		buffer[1]=(sys_t.cus_id&0xFF0000)>>16;
		buffer[2]=(sys_t.cus_id &0xFF00)>>8;
		buffer[3]=(sys_t.cus_id &0xFF);
		buffer[4]=(sys_t.serial&0xFF000000)>>24;
		buffer[5]=(sys_t.serial&0xFF0000)>>16;
		buffer[6]=(sys_t.serial &0xFF00)>>8;
		buffer[7]=(sys_t.serial &0xFF);	
		buffer[8] =CMD_ADD_KEY;
		buffer[9]=RESP_NOKEY;
		buffer_len=crc_calculation_client(10);
		EncyptPIAMPacket_hb(buffer_len-1,buffer);
		}				
		break;		
	case CMD_DELETE_ALLKEY:
		buffer[0]=(sys_t.cus_id&0xFF000000)>>24;
		buffer[1]=(sys_t.cus_id&0xFF0000)>>16;
		buffer[2]=(sys_t.cus_id &0xFF00)>>8;
		buffer[3]=(sys_t.cus_id &0xFF);
		buffer[4]=(sys_t.serial&0xFF000000)>>24;
		buffer[5]=(sys_t.serial&0xFF0000)>>16;
		buffer[6]=(sys_t.serial &0xFF00)>>8;
		buffer[7]=(sys_t.serial &0xFF);	
		buffer[8] =CMD_DELETE_ALLKEY;
		buffer[9]=RESP_NOKEY;
		buffer_len=crc_calculation_client(10);
		EncyptPIAMPacket_hb(buffer_len-1,buffer);
		break;
	case CMD_SET_CONFIG:
	if(sys_t.factory==0x01)
	{
		sys_t.factory=0;
		write_system_sd();
	}
		read_system_sd();
			
		sys_t.alarm_enable=rxbuffer[9];	
		total_comm_time=sys_t.hb_duration =rxbuffer [10];
		if(total_comm_time)
			ghb_time=total_comm_time*60;
		else
			ghb_time=0xFFFF;
		sys_t.open_door_time=rxbuffer[11];
		sys_t.key_turn_time =rxbuffer[12];
		sys_t.close_door_time=rxbuffer[13];
		write_system_sd();
		sprintf(szstr,"\rHB:%d,DOT:%d,KAT:%d,DCT:%d",sys_t.hb_duration,sys_t.open_door_time,sys_t.key_turn_time,sys_t.close_door_time);
		pstr(szstr);
		buffer[0]=(sys_t.cus_id&0xFF000000)>>24;
		buffer[1]=(sys_t.cus_id&0xFF0000)>>16;
		buffer[2]=(sys_t.cus_id &0xFF00)>>8;
		buffer[3]=(sys_t.cus_id &0xFF);
		buffer[4]=(sys_t.serial&0xFF000000)>>24;
		buffer[5]=(sys_t.serial&0xFF0000)>>16;
		buffer[6]=(sys_t.serial &0xFF00)>>8;
		buffer[7]=(sys_t.serial &0xFF);
		buffer[8] =CMD_SET_CONFIG;
		buffer[9]=RESP_SUCCESS;
		buffer_len=crc_calculation_client(10);
		EncyptPIAMPacket_hb(buffer_len-1,buffer);
	break;
	case CMD_GET_FIRMWARE_VER:
	buffer[0]=(sys_t.cus_id&0xFF000000)>>24;
		buffer[1]=(sys_t.cus_id&0xFF0000)>>16;
		buffer[2]=(sys_t.cus_id &0xFF00)>>8;
		buffer[3]=(sys_t.cus_id &0xFF);
		buffer[4]=(sys_t.serial&0xFF000000)>>24;
		buffer[5]=(sys_t.serial&0xFF0000)>>16;
		buffer[6]=(sys_t.serial &0xFF00)>>8;
		buffer[7]=(sys_t.serial &0xFF);	
		buffer[8] =CMD_GET_FIRMWARE_VER;
		buffer[9]=RESP_SUCCESS ;
		sprintf(szstr,"%s",BOX_VERSION);
		buffer[10]=strlen(szstr);
		for(i=0;i<strlen(szstr);i++)
			buffer[11+i]=szstr[i];
		buffer_len=crc_calculation_client(12+i);
		EncyptPIAMPacket_hb(buffer_len-1,buffer);
	break;
	case CMD_MOD_PORT:
	test=0;
			test=rxbuffer[10]&0xFF;
			test|=((rxbuffer[9]<<8)&0xFF00);
			
			read_comm_sd();
				hb_port=comm_t.hb_port=(u16_t)test;
				test=0;
			test=rxbuffer[12]&0xFF;
			test|=((rxbuffer[11]<<8)&0xFF00);
	
				ev_port=comm_t.event_port=(u16_t)test;
			
			write_comm_sd();
			sprintf(szstr,"\rHB_Port:%d,Eve_Port:%d",hb_port,ev_port);
			pstr(szstr);
			buffer[0]=(sys_t.cus_id&0xFF000000)>>24;
			buffer[1]=(sys_t.cus_id&0xFF0000)>>16;
			buffer[2]=(sys_t.cus_id &0xFF00)>>8;
			buffer[3]=(sys_t.cus_id &0xFF);
			buffer[4]=(sys_t.serial&0xFF000000)>>24;
			buffer[5]=(sys_t.serial&0xFF0000)>>16;
			buffer[6]=(sys_t.serial &0xFF00)>>8;
			buffer[7]=(sys_t.serial &0xFF);	
			buffer[8] =CMD_MOD_PORT;
			buffer[9]=RESP_SUCCESS;	
			buffer_len=crc_calculation_client(10);
			EncyptPIAMPacket_hb(buffer_len-1,buffer);
	break;
	
		case CMD_ADD_EDIT_USER_T:
	
			test=0;
			test|=(rxbuffer[12] & 0xFF)|((rxbuffer[11]<<8)&0xFF00)|((rxbuffer[10]<<16)&0xFF0000)|((rxbuffer[9]<<24)&0xFF000000);
			user_id_temp=3333;
			test=0;
			test|=(rxbuffer[16] & 0xFF)|((rxbuffer[15]<<8)&0xFF00)|((rxbuffer[14]<<16)&0xFF0000)|((rxbuffer[13]<<24)&0xFF000000);
			user_pin_temp =1234;
			test=0;
			test|=(rxbuffer[20] & 0xFF)|((rxbuffer[19]<<8)&0xFF00)|((rxbuffer[18]<<16)&0xFF0000)|((rxbuffer[17]<<24)&0xFF000000);
					
			facility_temp = 0x01;
		
			card_numtemp=test;
			sprintf(szstr,"User ID:%X\r\n",user_id_temp);
			pstr(szstr);
			if(check_user_bio(user_id_temp))
			{
				pstr("Finger enabled\r\n");
				finger_flag=1;
			}
			else
			{
				finger_flag=0xFF;
				pstr("Finger Disabeld\r\n");
			}		
			suser_code=modify_user(user_id_temp);
			if(suser_code>=0)
			{
				pstr("User added\r");
				sprintf(szstr,"location:%d\r",suser_code);
				pstr(szstr);
				read_user_sd(suser_code);
				
				user_t.user_id=user_id_temp ;
				user_t.card=card_numtemp;
				user_t.enabled=rxbuffer[22];
				user_t.facility_code=facility_temp;
				user_t.issue_lvl=rxbuffer[9];
				user_t.issue_type=0x00;
				user_t.user_pin=1234;
				user_t.finger_enabled=finger_flag;
				user_t.login_type=rxbuffer[21];
				for(i=0;i<32;i++)
					user_t.name[i]=rxbuffer[23+i];
				pstr("\rUser Name: ");
				pstr(user_t.name);
				user_t.task_count=bkp_var=rxbuffer[55];
				sprintf(szstr,"\rTask Count:%d",user_t.task_count );
				pstr(szstr);
				if(bkp_var)
				{
					for(i=0;i<bkp_var;i++)
					{
						test_code= (rxbuffer[56+(i*2)]<<8)|(rxbuffer[57+(i*2)]&0xFF);
						read_task_status(test_code);
						if(task_t.exist==(u32_t) EXIST)
						{
							user_t.tasks[i]=test_code;
							sprintf(szstr,"\rTaskID:%d",user_t.tasks[i]);
							pstr(szstr);
						}
					}
				
				}
				sprintf(szstr,"\rLogintype:%d-fflag:%d",user_t.login_type,finger_flag );
				pstr(szstr);
				if(user_t.login_type!=6 && user_t.finger_enabled==1)
				{
					bio_delete_flag=1;
				}
				write_user_sd(suser_code);
				sprintf(szstr,"\rId:%d Pin:%d card%XLogin Type:%d\r",user_t.user_id,user_t.user_pin,user_t.card,user_t.login_type);
				pstr(szstr);
		 		
				buffer[0]=(sys_t.cus_id&0xFF000000)>>24;
				buffer[1]=(sys_t.cus_id&0xFF0000)>>16;
				buffer[2]=(sys_t.cus_id &0xFF00)>>8;
				buffer[3]=(sys_t.cus_id &0xFF);
				buffer[4]=(sys_t.serial&0xFF000000)>>24;
				buffer[5]=(sys_t.serial&0xFF0000)>>16;
				buffer[6]=(sys_t.serial &0xFF00)>>8;
				buffer[7]=(sys_t.serial &0xFF);
				buffer[8]=CMD_ADD_EDIT_USER_T;
				buffer[9]=RESP_SUCCESS;
				buffer[10]=rxbuffer[9];
				buffer[11]=rxbuffer[10];
				buffer[12]=rxbuffer[11];
				buffer[13]=rxbuffer[12];
				buffer_len=crc_calculation_client(14);
				
				EncyptPIAMPacket_hb(buffer_len-1,buffer);
					
			}
			else
			{

				if(suser_code==-1)
				{
					pstr("User Duplicated\r");						
		 			buffer[0]=(sys_t.cus_id&0xFF000000)>>24;
					buffer[1]=(sys_t.cus_id&0xFF0000)>>16;
					buffer[2]=(sys_t.cus_id &0xFF00)>>8;
					buffer[3]=(sys_t.cus_id &0xFF);
					buffer[4]=(sys_t.serial&0xFF000000)>>24;
					buffer[5]=(sys_t.serial&0xFF0000)>>16;
					buffer[6]=(sys_t.serial &0xFF00)>>8;
					buffer[7]=(sys_t.serial &0xFF);	
					buffer[8]=CMD_ADD_MOD_USER;
					buffer[9]=RESP_USER_DUPLICATE;
					buffer[10]=rxbuffer[10];
					buffer[11]=rxbuffer[11];
					buffer[12]=rxbuffer[12];
					buffer[13]=rxbuffer[13];
					buffer_len=crc_calculation_client(14);
				
					EncyptPIAMPacket_hb(buffer_len-1,buffer);
				}
				else
				{
					pstr("DB Overflow\r");
					buffer[0]=(sys_t.cus_id&0xFF000000)>>24;
					buffer[1]=(sys_t.cus_id&0xFF0000)>>16;
					buffer[2]=(sys_t.cus_id &0xFF00)>>8;
					buffer[3]=(sys_t.cus_id &0xFF);
					buffer[4]=(sys_t.serial&0xFF000000)>>24;
					buffer[5]=(sys_t.serial&0xFF0000)>>16;
					buffer[6]=(sys_t.serial &0xFF00)>>8;
					buffer[7]=(sys_t.serial &0xFF);	
					buffer[8]=CMD_ADD_MOD_USER;
					buffer[9]=RESP_DB_OVERFLOW;
					buffer[10]=rxbuffer[10];
					buffer[11]=rxbuffer[11];
					buffer[12]=rxbuffer[12];
					buffer[13]=rxbuffer[13];
					buffer_len=crc_calculation_client(14);
				
					EncyptPIAMPacket_hb(buffer_len-1,buffer);
				}
			}
	break;
				case CMD_ADD_TASK:
				
				test_code= (rxbuffer[9]<<8)|(rxbuffer[10]&0xFF);
				sd_location=add_task(test_code);
				if(sd_location!=-1)
				{
					task_t.exist =(u32_t) EXIST;
					task_t.task_id=test_code;
					for(i=0;i<31;i++)	
					{
						sprintf(szstr,"%X-",task_t.name[i]);
						pstr(szstr);
					
					}
					for(i=0;i<31;i++)
						task_t.name[i]=rxbuffer[11+i];
					task_t.name[31]='\0';
					pstr("\rtaskname:");
					pstr(task_t.name);
					task_t.time_limit =(rxbuffer[43]<<8)|(rxbuffer[44]&0xFF);
					
					task_t.access_type=rxbuffer[45];
					test=0;
					test=rxbuffer[45]&0x7F;
					task_t.dow= (test&0x01)| ((test&0x02)<<5)|((test&0x04)<<3)|((test&0x08)<<1)|((test&0x10)>>1)|((test&0x20)>>3)|((test&0x40)>>5);
					test=0;
					test =(rxbuffer[49] & 0xFF)|((rxbuffer[48]<<8)&0xFF00)|((rxbuffer[47]<<16)&0xFF0000)|((rxbuffer[46]<<24)&0xFF000000);
					task_t.start_date=test;
					task_t.start_time = get_sec_time_value(test);
					
					sprintf(szstr,"ST:%X\r",test);
					pstr(szstr);
					verify_date_time(test);
					test=0;
					test =(rxbuffer[53] & 0xFF)|((rxbuffer[52]<<8)&0xFF00)|((rxbuffer[51]<<16)&0xFF0000)|((rxbuffer[50]<<24)&0xFF000000);
					task_t.end_date=test;
					task_t.end_time=get_sec_time_value(test);
					sprintf(szstr,"ET:%X\r",test);
					pstr(szstr);
					verify_date_time(test);
					
					task_t.multiple		= rxbuffer[54];
					if(task_t.multiple)						//validation to be check after software protocol
					{
						for(i=0;i<5;i++)
						{			
							task_t.user_code[i] = ((rxbuffer[55+(i*4)]<<24)&0xFF000000)|((rxbuffer[56+(i*4)]<<16)&0xFF0000)|(rxbuffer[57+(i*4)]<<8)|(rxbuffer[58+(i*4)]&0xFF);								
						}					
					}
					bkp_var=((rxbuffer[75]<<24)&0xFF000000)|((rxbuffer[76]<<16)&0xFF0000)|((rxbuffer[77]<<8)&0xFF00)|(rxbuffer[78]&0xFF);
					sprintf(szstr,"TT%04X\r",bkp_var);
					pstr(szstr);
					for (i = 0; i < 32; i++)
					{
						task_t.key[i] = (bkp_var & (1 << i)) != 0;
						sprintf(szstr,"key%d\r",task_t.key[i]);
						pstr(szstr);
					}
					
					 
					write_task_sd(sd_location);
					read_task_sd(sd_location);
					sprintf(szstr,"\rID:%d",task_t.task_id);
					pstr(szstr);
					pstr("\rtaskname:");
					sprintf(szstr,"size:%d\r",strlen(task_t.name));
					pstr(szstr);
					buffer[0]=(sys_t.cus_id&0xFF000000)>>24;
					buffer[1]=(sys_t.cus_id&0xFF0000)>>16;
					buffer[2]=(sys_t.cus_id &0xFF00)>>8;
					buffer[3]=(sys_t.cus_id &0xFF);
					buffer[4]=(sys_t.serial&0xFF000000)>>24;
					buffer[5]=(sys_t.serial&0xFF0000)>>16;
					buffer[6]=(sys_t.serial &0xFF00)>>8;
					buffer[7]=(sys_t.serial &0xFF);	
					buffer[8]=CMD_ADD_TASK;
					buffer[9]=RESP_SUCCESS;
					buffer[10]=rxbuffer[9];
					buffer[11]=rxbuffer[10];
					
					buffer_len=crc_calculation_client(12);
				
					EncyptPIAMPacket_hb(buffer_len-1,buffer);
					if(!(sys_t.self_user_allign &0x02))
					{
						sys_t.self_user_allign|=0x02;
						write_system_sd();
					}
				}
				else
					pstr("Task add fails\r");
				break;
			case CMD_DELETE_TASK:
				test_code= (rxbuffer[9]<<8)|(rxbuffer[10]&0xFF);
				sd_location=delete_task(test_code);
				if(sd_location!=0)
				{
					buffer[0]=(sys_t.cus_id&0xFF000000)>>24;
					buffer[1]=(sys_t.cus_id&0xFF0000)>>16;
					buffer[2]=(sys_t.cus_id &0xFF00)>>8;
					buffer[3]=(sys_t.cus_id &0xFF);
					buffer[4]=(sys_t.serial&0xFF000000)>>24;
					buffer[5]=(sys_t.serial&0xFF0000)>>16;
					buffer[6]=(sys_t.serial &0xFF00)>>8;
					buffer[7]=(sys_t.serial &0xFF);	
					buffer[8]=CMD_DELETE_TASK;
					buffer[9]=RESP_SUCCESS;
						buffer[10]=rxbuffer[9];
					buffer[11]=rxbuffer[10];
					buffer_len=crc_calculation_client(12);
				
					EncyptPIAMPacket_hb(buffer_len-1,buffer);
					if(!(sys_t.self_user_allign &0x02))
					{
						sys_t.self_user_allign|=0x02;
						write_system_sd();
					}
				}
				else
				{
					buffer[0]=(sys_t.cus_id&0xFF000000)>>24;
					buffer[1]=(sys_t.cus_id&0xFF0000)>>16;
					buffer[2]=(sys_t.cus_id &0xFF00)>>8;
					buffer[3]=(sys_t.cus_id &0xFF);
					buffer[4]=(sys_t.serial&0xFF000000)>>24;
					buffer[5]=(sys_t.serial&0xFF0000)>>16;
					buffer[6]=(sys_t.serial &0xFF00)>>8;
					buffer[7]=(sys_t.serial &0xFF);	
					buffer[8]=CMD_DELETE_TASK;
					buffer[9]=RESP_TASK_NOTFOUND;	
						buffer[10]=rxbuffer[9];
					buffer[11]=rxbuffer[10];
					buffer_len=crc_calculation_client(12);
				
					EncyptPIAMPacket_hb(buffer_len-1,buffer);
				}
				break;
			case CMD_DELETEALL_TASK:
				buffer[0]=(sys_t.cus_id&0xFF000000)>>24;
					buffer[1]=(sys_t.cus_id&0xFF0000)>>16;
					buffer[2]=(sys_t.cus_id &0xFF00)>>8;
					buffer[3]=(sys_t.cus_id &0xFF);
					buffer[4]=(sys_t.serial&0xFF000000)>>24;
					buffer[5]=(sys_t.serial&0xFF0000)>>16;
					buffer[6]=(sys_t.serial &0xFF00)>>8;
					buffer[7]=(sys_t.serial &0xFF);	
					buffer[8]=CMD_DELETEALL_TASK;
					buffer[9]=RESP_NO_FEATURE;
				crc_calculation(10);
				break;	
	case CMD_CLOSE_CONN:
	if(sys_t.factory==0x01)
	{
		sys_t.factory=0;
		write_system_sd();
	}
		pstr("\rConnection Closed Command");
		tcpClientChangeState(TCP_CLIENT_STATE_CLOSE);
	break;
	case CMD_FW_UPLOAD:
	pstr("\rFW Upload command received");
	ghb_time =60;
	fw_flag=1;
	tcpClientfwInit();
	gfw_time=2;
	break;
	}
	
}

u8_t check_config_receive(u16_t length)
{
	if(!checkcrc_server(length))
	{
		pstr("CRC Mismatch\r");
		return 0;	
	}
	if(!check_serial_no_server())
	{

	pstr("\rSerial Mismatch");
		return 0;
	}
	handle_protocol_configpacket_event();
	return 1;
}

void tcpClientStateSend(void)
{
	char szstr[32];
	u8_t uci;
  int_t error;
	static u8_t event_first_flag=0;
	systime_t time;
  size_t length;
   //Get current time
	time = osGetSystemTime();
	if(!event_first_flag)
	{
		event_first_flag=0x01;
		handle_hb_protocol_packet();
	}
	error = socketSend(clientSockethb , buffer, buffer_len, NULL, 0);
	if(error != NO_ERROR)
	{
		if(error==ERROR_TIMEOUT)
		{
			if(timeCompare(time, lastEvent +(20000)) >= 0)	
			{

			}
			else
				return;
	  }
		if(serveripno<5)
			serveripno++;
		else
			serveripno=0;
		pstr("send hb error");
		ghb_time=30;
		comcount=8;
		tcpClientChangeState(TCP_CLIENT_STATE_CLOSE);
		event_first_flag=0x00;
 }
 else
 {
	 event_first_flag=0x00;
	 pstr("datahb_sent\r");
	 pstr("\rTo PC:");
	 for(uci=0;uci<buffer_len;uci++)
	 {
		 sprintf(szstr,"%02X",buffer[uci]);
		 pstr(szstr);
	 }
	 pstr("\r");
	 while(1)
	 {
		 time = osGetSystemTime();
		 error = socketReceive(clientSockethb, buffer, sizeof(buffer),&length, 0);
		 if(error == NO_ERROR)
		 {
			 
		 }
		 else
		 {
			 if(timeCompare(time, lastEvent + (2000)) >= 0)	
			 {
					pstr("\rReady to receive");
				  tcpClientChangeState(TCP_CLIENT_STATE_RECEIVE);
				  break;
			 }
		 }
		}
  }
}


void tcpClientStatewait(void)
{
	   systime_t time;
   //Get current time
		time = osGetSystemTime();
		if(timeCompare(time, lastEvent + (1000)) >= 0)	
		{
			 //Debug message
			tcpClientChangeState(TCP_CLIENT_STATE_CLOSE);
			 
		}

}

void tcpClientStatereceive(void)
{
	error_t error;
	size_t length;
	char szstr[32];
	u8_t uci,flg=0;
	systime_t time;
 //Get current time
	time = osGetSystemTime();
	error = socketReceive(clientSockethb, rxbuffer, sizeof(rxbuffer),&length, 0);
	
	if(error == NO_ERROR)
	{
		pstr("No error to receive hb\r");		
		pstr("No error");	
		sprintf(szstr,"len:%d\r",length);
		pstr(szstr);
		for(uci=0;uci<length;uci++)
		{
			sprintf(szstr,"%02X",rxbuffer[uci]);
			pstr(szstr);
		}
		length=length-rxbuffer[length-1]-1;
		DecyptPIAMPacket_hb(rxbuffer,length);		

		flg=check_config_receive(length);

		if(rxbuffer[8]==CMD_CLOSE_CONN || rxbuffer[8]==CMD_FW_UPLOAD)
			flg=0;
		if(flg==1)
			tcpClientChangeState(TCP_CLIENT_STATE_SEND);
		else
			tcpClientChangeState(TCP_CLIENT_STATE_CLOSE);

	}
	else
	{
		if(timeCompare(time, lastEvent + (30000)) >= 0)	
		{
			if(serveripno<5)
				serveripno++;
			else
				serveripno=0;
			tcpClientChangeState(TCP_CLIENT_STATE_CLOSE);
		}
	}
}
void tcpClientStateClose(void)
{
	flag_init=0;
	ghb_time =total_comm_time*60;
	pstr("\rHB_Close\r");
	socketClose(clientSockethb);
  tcpClientChangeState(TCP_CLIENT_STATE_IDEAL);
}
void tcpClientChangeState(uint_t newState)
{
  //Enter the specified state
  state = newState;
  //Save current time
  lastEvent = osGetSystemTime();
}


