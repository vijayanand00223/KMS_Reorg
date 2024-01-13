/**
 * @file main.c
 * @brief Main routine
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
#include <stdio.h>
#include "lpc23xx.h"
#include "os_port.h"
#include "core/net.h"
#include "drivers/mac/lpc23xx_eth_driver.h"
#include "drivers/phy/dp83848_driver.h"
#include "ipv6/slaac.h"
#include "dhcp/dhcp_client.h"
#include "dhcpv6/dhcpv6_client.h"
#include "telnet_server.h"
#include "error.h"
#include "debug.h"
#include "i2c.h"
#include "uart.h"
#include "wdt.h"
#include "timer.h"
#include "RFID.h"
#include "menu_main.h"
#include "LCD.h"
#include "LED.h"
#include "BUZZER.h"
#include "CLOCK_CALNDR.h"
#include "DOOR.h"

#define APP_MAC_ADDR "00-AB-CD-EF-23-78"
#define APP_USE_DHCP ENABLED
#define APP_IPV4_HOST_ADDR "192.168.16.38"
#define APP_IPV4_SUBNET_MASK "255.255.255.0"
#define APP_IPV4_DEFAULT_GATEWAY "192.168.16.254"
#define APP_IPV4_PRIMARY_DNS "8.8.8.8"
#define APP_IPV4_SECONDARY_DNS "8.8.4.4"

#define APP_USE_SLAAC DISABLED
#define APP_USE_DHCPV6 DISABLED
#define APP_IPV6_LINK_LOCAL_ADDR "fe80::2378"
#define APP_IPV6_PREFIX "2001:db8::"
#define APP_IPV6_PREFIX_LENGTH 64
#define APP_IPV6_GLOBAL_ADDR "2001:db8::2378"
#define APP_IPV6_ROUTER "fe80::1"
#define APP_IPV6_PRIMARY_DNS "2001:4860:4860::8888"
#define APP_IPV6_SECONDARY_DNS "2001:4860:4860::8844"
#define APP_IPV6_SERVER_ADDR "fc00::1:3b75:66a0:8134:6ad4"

KEYS key1_t[MAX_KEYS];
MAC mac_t;
SYSTEM sys_t;
TIME_STAMP TskTimeout_stamp_t;
USERS user_t;
COMM	comm_t;
TASKS task_t;
const uint8_t config[24] 			={0x06,0x06,0x12,0x00,0x00,0x00,0x0D,0x56,0x49,0x00,0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x00};
	
//Global variables
DhcpClientSettings dhcpClientSettings;
DhcpClientCtx dhcpClientContext;
Dhcpv6ClientSettings dhcpv6ClientSettings;
Dhcpv6ClientCtx dhcpv6ClientContext;

SlaacSettings slaacSettings;
SlaacContext slaacContext;
u16_t mac1,mac2,mac3;
u32_t ks_serial;

u32_t   ULALARMS;       // used for alarm setting
u16_t glcd_refresh_time=(u16_t) 0; // used to refresh the LCD for particular time interval


u8_t rstflag=1;
u8_t bio_delete_flag=0;
u8_t sys_erase_flg=0;
u32_t gcurrent_date;    // used to get the current date
s32_t gcurrent_dow;     // used to identify the day of week
s32_t gcurrent_time;    // used to get the current time
s32_t gupdate_chart=0;  // flag for update task chart for every day
u8_t hb_event_loginflag=0;



u8_t comcount=0;

u32_t   ULALARMS;       // used for alarm setting

u32_t gdashboard_trg;	//flag for dashboard trigger
u16_t uci;

char szstr[50];
u32_t rmip1,rmip2,rmip3;


u8_t fw_flg=0;


u8_t flag_init=0;
char url[32];
char url1[32];
char lock_str[50];
u8_t	arrow_r_flg=0;
u8_t serial_flg=0;


u32_t passcode=0;
char wbin[16];
u8_t  fingerprint=0;
u8_t finger_user=0;

u8_t uck;
s32_t user_code_pin;
u8_t max_keys_available;
u8_t Enter_flag;
u16_t ev_port,hb_port;
u8_t  org_key_data[MAX_KEYS][12];
u8_t  key_not_reg[MAX_KEYS];      //1-not reg,2-reg
u8_t j=0;
u8_t lock_flg=0;
u8_t lock_flg1=0;
u8_t lock_flg2=0;
u8_t conn_flg=0;
u8_t disconn_flg=0;
u8_t ble_user_flg=0;
u8_t ble_key_no=0;
u8_t ble_task_flg=0;
void delay(u32_t ulticks)
{
	u32_t ultimeout;
	ultimeout = set_timeout(ulticks);

	while (!chk_timeout(ultimeout))				//upto the seconds complete,it rotates with in the while loop.
	{
	if(!rstflag)
		{
			wdt_reset_feed();
		}
		netTask();
		telnetServerTask();
		tcpClientfwTask();
		tcpClienthbTask();
		tcpClienteventTask();	
	}
}
/******************************************************************************
 ** Function name:      key1
 **
 ** Descriptions:       it also wait for some delay. but if any key pressed with in the delay
 **						it returns corresponding values.
 **
 ** parameters:         ultimeout=>It is a value.That explain, how much time to wait here
 **                                during this operation we can press any key for any entering operation 
 ** 				 
 ** Returned value:     none
 ** 
 ******************************************************************************/
s32_t key1(u32_t ultimeout)
{
	u32_t ultt;
	s32_t iret_key = 0;
	ultt = set_timeout(ultimeout);
	while (1)
	{
	if(!rstflag)
		{
			wdt_reset_feed();
		}
		netTask();
		telnetServerTask();
		tcpClientfwTask();
		tcpClienthbTask();
		tcpClienteventTask();	
		if (pressed_key)
		{
			iret_key = pressed_key;
			pressed_key =(char) 0;
			return iret_key;
		}
		if (chk_timeout(ultt))
		{
			return 0;
		}
	}
}
/**
 * @brief Main entry point
 * @return Unused value
 **/

void structure_size()
{
	char szstr1[32];
	u16_t iuser_code;
	u16_t i;
	iuser_code=sizeof(sys_t);
	sprintf(szstr1,"size of system:%d\r",iuser_code);
	pstr(szstr1);
	sprintf(szstr1,"size of event:%d\r",sizeof(event_t));
	pstr(szstr1);
	sprintf(szstr1,"size of keyenent:%d-%d\r",sizeof(KEY_EVENTS),sizeof(key_events_t[i]));
	pstr(szstr1);	
	sprintf(szstr1,"size of key:%d\r",sizeof(key1_t[0]));
	pstr(szstr1);	
	sprintf(szstr1,"size of user:%d\r",sizeof(user_t));
	pstr(szstr1);
}

int_t main(void)
{
	
	s32_t iuser_code=0;
	s32_t i=0;
	char szstr1[50];	
  error_t error;
	u32_t user_code;
	NetInterface *interface;
	MacAddr macAddr;
	Ipv4Addr ipv4Addr;
  
	
#if (APP_USE_SLAAC == DISABLED && IPV6_SUPPORT == ENABLED)
	 Ipv6Addr ipv6Addr;
#endif	
	glogin_user = 0;
	gcomm_busy =0;
	memset(&TskTimeout_stamp_t, 0, sizeof(TIME_STAMP)); // clear the timestamp
	TICKER_Start(); //inializing
	endtick_value=get_ticks_value();
	TICKER_100ms_Start();
	lock_on(0);
	for(i=0;i<MAX_KEYS;i++)//initializing with green LED in off condition
	{
		green_led[i]=0;
		solenoid[i]=1;
	}
	lcd_init();	
	lcd_dis(1);          //lcd brightness on
	lcd_clr(0);
	lcd_clr(1);
	

   if (WDMOD & WDTOF)
	{
		buzzer(500);
		WDMOD &= ~(WDTOF);
	}
	delay(2000);
		//reset flag 
	rstflag=0x00;
	wdt_3S_Init();
	
	i2c_lpc_init(0);		
  pressed_key=0;
  read_mac_sd();
	mac1=mac_t.add_MAC0;
	mac2=mac_t.add_MAC1;
	mac3=mac_t.add_MAC2;
	TRACE_INFO("MAC:%04X%04X%04X\r",mac1,mac2,mac3);
	lcd_str(0, "Sterna Mykee Systems ");
	lcd_show(1,'e',14);//Initializing...
	read_comm_sd();		

	read_system_sd();
	if(sys_t.Total_Keys != SIXTEEN_KEYS_MODE  && sys_t.Total_Keys != THIRTYTWO_KEYS_MODE )
	{
		sys_t.Total_Keys = SIXTEEN_KEYS_MODE ;
 		write_system_sd();
	}

	max_keys_available=MAX_KEYS;
	if(sys_t.Intelligent_mode!=INTELLIGENT	&&	sys_t.Intelligent_mode!=NON_INTELLIGENT)
	{
		sys_t.Intelligent_mode=NON_INTELLIGENT;
		write_system_sd();
	}

	
	
	for(i=0;i<max_keys_available;i++)
	{
		read_keyevents_sd(i);
		sprintf(szstr,"uc=%d\r",key_events_t[i].user);
	}
	
	if(sys_t.hb_duration == 0xff || sys_t.hb_duration==0)
	{
		sys_t.hb_duration=2;
		write_system_sd();
	}
	

	
	
	
	if(sys_t.ip_type==0xff)
	{
		sys_t.ip_type=STATIC;
		write_system_sd();
	}
	if(sys_t.conn_type==0xff)
	{
		sys_t.conn_type=REMOTEIP ;
		write_system_sd();
	}	
	read_system_sd();	
	ks_serial=sys_t.serial;
	factory=sys_t.factory;
	if(factory ==0xFF)
	{
		sys_t.factory=0;
		factory =sys_t.factory ;
		write_system_sd();
	}
	prot_init_event();
	
	structure_size();
	for(i=0;i<max_keys_available;i++)
	{
		read_keyevents_sd(i);
		sprintf(szstr,"uc=%d\r",key_events_t[i].user);
	}
#ifdef IBUTTON
	for(i=0;i<MAX_KEYS;i++)    //key data restore
	 {
		 read_key_sd(i);
			for(j=0;j<12;j++)
			{
				org_key_data[i][j]=key1_t[i].keyvalue[j];
				sprintf(szstr,"%02X",org_key_data[i][j]);
				pstr(szstr);
			}	

			for(j=0;j<6;j++)
			{ 				
				if(((org_key_data[i][j]==0xFF)&&(org_key_data[i][j+1]==0xFF)) || org_key_data[i][0]==0x00)
        {
          key_not_reg[i]=1;  
        }
				else if(((org_key_data[i][j]!=0XFF))&&(org_key_data[i][0]!=0x00))
				{
          key_not_reg[i]=2;
        }				
			}
			if( key_not_reg[i]==1)
			{
				sprintf(szstr,"\rkey%d",i);
	      pstr(szstr);
			}
  	}
#endif
#ifdef BIOMETRIC

#endif	
		
	user_code=key1(MESSAGE_TIMEOUT);
	
	if(user_code=='8')
	{
	user_code=0;
		buzzer(ONE_SEC);
		lcd_clr(0);
		lcd_clr(1);
		service_menu();
	}
	
	 //TCP/IP stack initialization
	 error = netInit();
	 //Any error to report?
	 if(error)
	 {
			//Debug message
			TRACE_ERROR("Failed to initialize TCP/IP stack!\r\n");
	 }
	 //Configure the first Ethernet interface
	 interface = &netInterface[0];
	 interface->id=(((mac2<<16)&0xFFFF0000)|(mac3));
	
		
	 //Set interface name
	 netSetInterfaceName(interface, "TKM");
	 //Set host name
	 netSetHostname(interface, szstr1);
		 //Select the relevant network adapter
	 netSetDriver(interface, &lpc23xxEthDriver);
	 netSetPhyDriver(interface, &dp83848PhyDriver);
	 //Set host MAC address
	 macAddr.w[0]=((mac1>>8)&0xFF)|((mac1<<8)&0xFF00);
	 macAddr.w[1]=((mac2>>8)&0xFF)|((mac2<<8)&0xFF00);
	 macAddr.w[2]=((mac3>>8)&0xFF)|((mac3<<8)&0xFF00);
	 TRACE_INFO("MAC address = %s\r\n", macAddrToString(&macAddr, NULL));
	 netSetMacAddr(interface, &macAddr);

	 //Initialize network interface
	 error = netConfigInterface(interface);
	 //Any error to report?
	 if(error)
	 {
			//Debug message
		TRACE_ERROR("Failed to configure interface %s!\r\n", interface->name);
	 }
	read_comm_sd();

	rmip1=(comm_t.remote_ip1>>24&0xFF)|(comm_t.remote_ip1>>8&0xFF00)|(comm_t.remote_ip1<<8&0xFF0000)|(comm_t.remote_ip1<<24&0xFF000000);
	rmip2=(comm_t.remote_ip2>>24&0xFF)|(comm_t.remote_ip2>>8&0xFF00)|(comm_t.remote_ip2<<8&0xFF0000)|(comm_t.remote_ip2<<24&0xFF000000);
	rmip3=(comm_t.remote_ip3>>24&0xFF)|(comm_t.remote_ip3>>8&0xFF00)|(comm_t.remote_ip3<<8&0xFF0000)|(comm_t.remote_ip3<<24&0xFF000000);
	rmip2=rmip3=rmip1;

#if (IPV4_SUPPORT == ENABLED)
//#if (APP_USE_DHCP == ENABLED)
 if(sys_t.ip_type==DYNAMIC)
 {
   //Get default settings
   dhcpClientGetDefaultSettings(&dhcpClientSettings);
   //Set the network interface to be configured by DHCP
   dhcpClientSettings.interface = interface;
   //Disable rapid commit option
   dhcpClientSettings.rapidCommit = FALSE;

   //DHCP client initialization
   error = dhcpClientInit(&dhcpClientContext, &dhcpClientSettings);
   //Failed to initialize DHCP client?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to initialize DHCP client!\r\n");
   }

   //Start DHCP client
   error = dhcpClientStart(&dhcpClientContext);
   //Failed to start DHCP client?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to start DHCP client!\r\n");
   }
 }
//#else
 else
 {
	 //Set IPv4 host addressr
	 pstr("ipv4");
	 
	if(!sys_t.ip||sys_t.ip==0xFFFFFFFF)
	{
		sys_t.ip=(0xC0A81000|(mac3&0x0f))+1;
		sys_t.netmask=0xFFFFFF00;
		sys_t.gateway=0xC0A81006;
		write_system_sd();
		pstr("IPSET\r");
	}
	
	//ipv4StringToAddr(APP_IPV4_HOST_ADDR, &ipv4Addr);
	
	ipv4Addr=(sys_t.ip>>24&0xFF)|(sys_t.ip>>8&0xFF00)|(sys_t.ip<<8&0xFF0000)|(sys_t.ip<<24&0xFF000000);
	ipv4SetHostAddr(interface, ipv4Addr);
	
	 //Set subnet mask

	ipv4Addr=(sys_t.netmask>>24&0xFF)|(sys_t.netmask>>8&0xFF00)|(sys_t.netmask<<8&0xFF0000)|(sys_t.netmask<<24&0xFF000000);
	ipv4SetSubnetMask(interface, ipv4Addr);
		
	 //Set default gateway

	ipv4Addr=(sys_t.gateway>>24&0xFF)|(sys_t.gateway>>8&0xFF00)|(sys_t.gateway<<8&0xFF0000)|(sys_t.gateway<<24&0xFF000000);
	ipv4SetDefaultGateway(interface, ipv4Addr);

	 //Set primary and secondary DNS servers
	

		ipv4Addr=(sys_t.dns_ip1>>24&0xFF)|(sys_t.dns_ip1>>8&0xFF00)|(sys_t.dns_ip1<<8&0xFF0000)|(sys_t.dns_ip1<<24&0xFF000000);
	 ipv4SetDnsServer(interface, 0, ipv4Addr);

		ipv4Addr=(sys_t.dns_ip2>>24&0xFF)|(sys_t.dns_ip2>>8&0xFF00)|(sys_t.dns_ip2<<8&0xFF0000)|(sys_t.dns_ip2<<24&0xFF000000);
	 ipv4SetDnsServer(interface, 1, ipv4Addr);
 }
	sprintf(szstr,"\rmyip:%X,nm:%X,gw:%X\r",sys_t.ip,sys_t.netmask,sys_t.gateway);
	pstr(szstr);
//#endif
#endif
//pg/0118/c/15/000047
#if (IPV6_SUPPORT == ENABLED)
#if (APP_USE_SLAAC == ENABLED)
   //Get default settings
   slaacGetDefaultSettings(&slaacSettings);
   //Set the network interface to be configured
   slaacSettings.interface = interface;

   //SLAAC initialization
   error = slaacInit(&slaacContext, &slaacSettings);
   //Failed to initialize SLAAC?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to initialize SLAAC!\r\n");
   }

   //Start IPv6 address autoconfiguration process
   error = slaacStart(&slaacContext);
   //Failed to start SLAAC process?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to start SLAAC!\r\n");
   }
#elif (APP_USE_DHCPV6 == ENABLED)
   //A link-local address is formed by combining the well-known
   //link-local prefix fe80::/10 with the interface identifier
   ipv6StringToAddr("fe80::", &ipv6Addr);

   //Copy the Organization Unique Identifier (OUI)
   ipv6Addr.b[8] = macAddr.b[0] ^ MAC_ADDR_FLAG_LOCAL;
   ipv6Addr.b[9] = macAddr.b[1];
   ipv6Addr.b[10] = macAddr.b[2];

   //The middle 16 bits are given the value 0xFFFE
   ipv6Addr.b[11] = 0xFF;
   ipv6Addr.b[12] = 0xFE;

   //Copy the right-most 24 bits of the MAC address
   ipv6Addr.b[13] = macAddr.b[3];
   ipv6Addr.b[14] = macAddr.b[4];
   ipv6Addr.b[15] = macAddr.b[5];

   //Set link-local address
   ipv6SetLinkLocalAddr(interface, &ipv6Addr);

   //Get default settings
   dhcpv6ClientGetDefaultSettings(&dhcpv6ClientSettings);
   //Set the network interface to be configured by DHCPv6
   dhcpv6ClientSettings.interface = interface;
   //Disable rapid commit option
   dhcpv6ClientSettings.rapidCommit = FALSE;

   //DHCPv6 client initialization
   error = dhcpv6ClientInit(&dhcpv6ClientContext, &dhcpv6ClientSettings);
   //Failed to initialize DHCPv6 client?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to initialize DHCPv6 client!\r\n");
   }

   //Start DHCPv6 client
   error = dhcpv6ClientStart(&dhcpv6ClientContext);
   //Failed to start DHCPv6 client?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to start DHCPv6 client!\r\n");
   }
#else
   //Set link-local address
   ipv6StringToAddr(APP_IPV6_LINK_LOCAL_ADDR, &ipv6Addr);
   ipv6SetLinkLocalAddr(interface, &ipv6Addr);

   //Set IPv6 prefix
   ipv6StringToAddr(APP_IPV6_PREFIX, &ipv6Addr);
   ipv6SetPrefix(interface, &ipv6Addr, APP_IPV6_PREFIX_LENGTH);

   //Set global address
   ipv6StringToAddr(APP_IPV6_GLOBAL_ADDR, &ipv6Addr);
   ipv6SetGlobalAddr(interface, &ipv6Addr);

   //Set router
   ipv6StringToAddr(APP_IPV6_ROUTER, &ipv6Addr);
   ipv6SetRouter(interface, &ipv6Addr);

   //Set primary and secondary DNS servers
   ipv6StringToAddr(APP_IPV6_PRIMARY_DNS, &ipv6Addr);
   ipv6SetDnsServer(interface, 0, &ipv6Addr);
   ipv6StringToAddr(APP_IPV6_SECONDARY_DNS, &ipv6Addr);
   ipv6SetDnsServer(interface, 1, &ipv6Addr);
	 dhcp6_ipreceived=0xFF;
#endif
#endif
			
		 //Telnet server initialization
	error = telnetServerInit();
	 //Failed to initialize Telnet server?
	 if(error)
	 {
			//Debug message
			TRACE_ERROR("Failed to initializa Telnet server!\r\n");
	 }	
	
	if (key1(MESSAGE_TIMEOUT)==ESC)
	{
		lcd_show(0, 'e', 10); //Service Mode
		lcd_show(1, 'V', 13);
		if (read_code(HIDE_INPUT)==1950)
		{
			lcd_show(0, 'e', 15); //Key events created
			lcd_show(1, 'e', 16); //All user can return keys
			create_keyevents();
			delay(MESSAGE_TIMEOUT);
		}
		else
		{
			lcd_show(1, 'S', 16);  //Invalid Service code
			delay(MESSAGE_TIMEOUT);
			lcd_show(1, 'V', 27); //Operation Aborted
			delay(MESSAGE_TIMEOUT);
		}
	}
	
	
	//display the serial number of the box and firmware version	
	sprintf(szstr, "KA%05d", sys_t.serial);
	lcd_show_str(0, 'e', 17, szstr);//Box serial:
	sprintf(szstr, "%s", BOX_VERSION);
	lcd_show_str(1, 'e', 18, szstr);//version:
	delay(MESSAGE_TIMEOUT);
	if(sys_t.self_user_allign)
	{
		if(sys_t.self_user_allign & 0x01)
			self_user_allign();
		if(sys_t.self_user_allign & 0x02)			
			selftask_allign();
		sys_t.self_user_allign=0;
		write_system_sd();			
	}
#ifdef TRUEKEY32
	for (i=0; i<max_keys_available; i++)
	{
		lock_key_state[i]=lock_state[i];
		iKey_status=lock_key_state[i];
		read_keyevents_sd(i);
		if ((iKey_status & LOCK_OPEN)&&(key_events_t[i].status!=LOCK_OPEN))
		{
			update_key_log(ginstaller_code, 0, i, LOCK_OPEN);
		}
		else if ((iKey_status & LOCK_CLOSED)&&(key_events_t[i].status!=LOCK_CLOSED))
		{
			update_key_log(ginstaller_code, 0, i, LOCK_CLOSED);
		}
		else if ((!(iKey_status & (LOCK_OPEN | LOCK_CLOSED))) && ((key_events_t[i].status == LOCK_OPEN)||(key_events_t[i].status == LOCK_CLOSED)))
		{
			pstr("data 3\r");
			update_key_log(ginstaller_code, 0, i, LOCK_NOT_FOUND);
		}
		else
		{
			//
		}
	}
	
	for(i=0;i<max_keys_available;i++)
	{
	lock_state[i] &= (u8_t) ~(LOCK_RED | LOCK_SOL | LOCK_GREEN);
	lock_key_state[i]=lock_state[i];
	//delay(1000);
	}
#endif
#ifdef IBUTTON 
	for(i=0;i<MAX_KEYS;i++)
	{
		if (key_events_t[i].status ==LOCK_OPEN)
		{
			read_key_sd(i);
			if (key1_t[i].task_removal_time)
			{
				KeyTask_TimeLimit[i]=key1_t[i].task_removal_time;
			}
			lock_key_state[i] =LOCK_OPEN;

		}
	}
	for(i=0;i<MAX_KEYS;i++)
	{
		green_led[i]=0;
		red_led[i]=0;
	}
#endif
	total_comm_time=sys_t.hb_duration;
	gcommunication_time=10;
	ghb_time=10;
	tcpClient_eventInit();   //event tcp communication init
	rfid_count=0;
	tcpClientInit();
	sprintf(lock_str,"user size %d\r\n",sizeof(user_t));
	p1str(lock_str);
#ifdef BIOMETRIC	

#endif	

  while(1)
	{
#ifdef BIOMETRIC		
		if(bio_delete_flag)
		{
			hb_event_loginflag=1;
			bio_verification();
		}
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
				pstr("pin -3\r");
				lcd_str(0,"Access Denied");
				lcd_clr(1);
				buzzer(MESSAGE_TIMEOUT);
				delay(MESSAGE_TIMEOUT);
				fingerprint=1;
			}
			else if(iuser_code==-4)
			{
				fingerprint=2;
			}
			else if(iuser_code!=0)
			{
				fingerprint=1;
				finger_user=1;
				if(check_user_bio(iuser_code))
				{
					 if (user_t.enabled == 0)
					 {
						 lcd_show(0,'S',13);
						 lcd_show(1, 'S', 8);						 // Not allowed					 
						 key1(MESSAGE_TIMEOUT);
					 }
					 else
					 {
						 hb_event_loginflag=1;
							user_code_main=iuser_code;
							menu_loginuser();
					 }
				}
			}
			else
				fingerprint=1;
		}
#endif	
		
#ifdef BLUETOOTH			
		if(lock_flg)
		{
			lock_flg=0;
			buzzer(500);
			user_code_main	=((ble_protocol_packet_in[6]<<24)&0xFF000000)|((ble_protocol_packet_in[7]<<16)&0xFF0000)|((ble_protocol_packet_in[8]<<8)&0xFF00)|(ble_protocol_packet_in[9]&0xFF);
			user_code_pin	=((ble_protocol_packet_in[10]<<24)&0xFF000000)|((ble_protocol_packet_in[11]<<16)&0xFF0000)|((ble_protocol_packet_in[12]<<8)&0xFF00)|(ble_protocol_packet_in[13]&0xFF);
			delay(1000);
			ble_user_flg=1;
#ifdef TASK_MANAGEMENT 
		main_menu_user_task();
#else		
		main_menu_user();
#endif		
		prot_log_event((s32_t) EVT_LOGOUT,(s32_t) user_code_main , 0,0);
		}
	
		if(conn_flg)
		{
			conn_flg=0;
			buzzer(500);
			lcd_clr(0);
			lcd_clr(1);
			lcd_str(0,"BLE Device Connected");
			delay(1000);
		}
		if(disconn_flg)
		{
			disconn_flg=0;
			buzzer(500);
			lcd_clr(0);
			lcd_clr(1);
			lcd_str(0,"BLE Device Disconnected");
			delay(1000);
		}
#endif
		if(fw_flag) // firmware download
		{
			hb_event_loginflag=1;
			lcd_show(0U,'e',13U);//Firmware Uploading...
			sprintf(szstr1, "%d%%", ((packet_num*100)/fw_datalen));
			lcd_show_str(1, 'V', 26, szstr1);
			delay(ONE_SEC);
			if(fw_flag == 2)
			{
				lcd_show(0, 'e', 19); //COMM ERROR
				lcd_show(1, 'e', 20); //System Restarting
				buzzer(MESSAGE_TIMEOUT);
				delay(MESSAGE_TIMEOUT);
				eraseboot_sdcard();
				system_restart();
			}
			if(fw_flag == 3)
			{
				system_restart();
			}
		}
		else
		{
			if(!(FIO2PIN & 0x00000004))
			{
				hb_event_loginflag=1;
				if(regular_card()==1)
				{
					
					pstr("regular card ok!!!");
					card_id_available=0;

					if(card_id)
					{
					  menu_login(0);
						fingerprint=1;
					}
					glogin_user=0;
				}
			}
			glogin_user=0;
			
				iuser_code = read_code(0xFD); // get the four digit value
				if(iuser_code==-2)//Up Arrow
				{
					pstr("Communicating to server using TCP\r");
					flag_init=0;
					ghb_time=1;
				}
				else if(iuser_code==-3)//Down Arrow
				{
					// Generate all passwords using box serial no and box current ticks
					// Current Ticks will display in LCD
					endtick_value=get_ticks_value();
					sprintf(szstr,"Ticks:%d",endtick_value);
					lcd_str(0,szstr);
					lcd_clr(1);
					Wrong_entry();
					passcode=myRand(Master_Code ,1);
					sprintf(szstr,"MP1:%d",passcode);
					passcode=myRand(Master_Code ,2);
					sprintf(szstr,"MP2:%d",passcode);
					passcode=myRand(Emergency_Code ,1);
					sprintf(szstr,"\rEmergency Password:%d",passcode);
					pstr(szstr);
					passcode=myRand(Serial_No_Code ,2);
					sprintf(szstr,"SP:%d",passcode);
					passcode=myRand(Firmware_Code ,1);

					delay(500);
				}
				else if ((iuser_code !=-1) && iuser_code)
				{
					if(arrow_r_flg==1)
 					 {
						arrow_r_flg=0;
						arrow_right_pressed(iuser_code);
					 }
					 else
						{
							flag_init=0;							
							user_code_pin=0;
							menu_login(iuser_code); // go to main login 
							glogin_user=0;
							card_id_available = 0;
							fingerprint=1;
						}
				}
				else
				{
	
				}
/***************************************************************/
			card_id_available = 0;
#ifdef IBUTTON
			key_check_2();
#endif
			hb_event_loginflag=1;
			alarm_handler();      //This function is used for alarm checking when events or problems occurs.
			hb_event_loginflag=0;
			lcd_clr(0);
			lcd_center(0, "Sterna Mykee"); // display the time
			get_timestamp(szstr, 4); // get the time stamp
		  lcd_center(1, szstr); // display the time
			delay(10);
	
		}
	}
}
/*-----------------------------------------end of file-------------------------------------------*/

