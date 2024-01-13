	/*
	File name :menu_main.h
	This file contain the macros structure and function declarations.
	Auther:sterna Security.
*/
#ifndef __MENU_H
#define __MENU_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ticker_itrp.h"
#include "sbl_config.h"
#include "compiler_port.h"
#include "aes.h"

/*IMPORTANT NOTE:
Please use the following 3 Prepocessors Definitions to make the right Code
ABLOY - Commenting this removes all modifications done for ABLOY. Keep this for ABLOY customer

DOOR_SENSE_NC - Commenting this will make a code for BOX with NO Door Sensor.
				Best way of making the code for only Door sensor change is, keep this definition and make once,
				Flash the code and check remove keys. If it is not working the proper way, then comment this line and make code.
				It is work in any of this 2 condition.

NO_DOOR_NO_SOLENOID - Keeping this will make a code for a BOX with no Front door and Locking Solenoid.
						Remember to comment this line when using a Front door with Door sensor... (Important)
*/

#define DOOR_SENSE_NC
//#define NO_DOOR_NO_SOLENOID
#define TCP_COMM
//#define UART0_DEBUG
#define TASK_MANAGEMENT
#define BIOMETRIC

#define IBUTTON

#ifdef IBUTTON 
//	#define IBUT_4KEY
#endif

#ifndef IBUTTON 
	#define TRUEKEY32
#endif


#define REMOTEIP	0x01
#define HOSTADDR	0x02
#define SL018			0x01
#define HID				0x02
#define NEXKEY		0x03

// sd card storage
#define BOOT_BLOCK_SIZE		 	4 
#define MAX_EVENTS         	28000
#define MAX_KEYEVENTS			 	1000
#ifdef TRUEKEY32
#define MAX_KEYS           	32
#else
#define MAX_KEYS           	16
#endif
#define MAX_USERS_BLOCK		  3
#define MAX_USERS_LOCATION 	1500
#define MAX_TASKS            800
#define MAX_USER_TASKS       17

#define SYS_BLOCK               0x000000        //Sector 0        [SYSTEM data]
#define MAC_BLOCK             	0x010000				//Sector 1	      [MAC Address]
#define BOOT_BLOCK              0x020000        //Sector 2 to 4	  [Bootloader]
#define COMM_BLOCK							0x060000				//Sector 5        [Communication IP & Port Details]
#define USER_BLOCK							0x070000				//Sector 6 to 9   (Each sector 512 users)	[User Details]
#define TASK_BLOCK							0x0A0000				//sector 9 to 10
#define KEYS_EVNT_BLOCK					0x0C0000        //Sector 11 		  [Key Events]
#define KEY_BLOCK								0x0D0000				//Sector 12			  [Key Details]
#define EVENT_BLOCK							0x0E0000				//Sector 14 to 27 (14 sectors, each 2047 events)total 28658 events [Event Details]
#define BACKUP_BLOCK						0x1C0000				//Sector 28 to 31 [User Backup during self allign]


#define UART_BAUDRATE_115200 1
#define UART_BAUDRATE_38400	 2
#define UART_BAUDRATE_9600   3

// version of fimware 
#ifndef TRUEKEY32
#define BOX_VERSION    	     "V00.00.16"
#else
#define BOX_VERSION    	     "V01.00.01"
#endif

// user level
#define INSTALLER 			 		 1
#define ADMINISTRATOR 		 	 2
#define END_USER			 			 4

// input menthod
#define SHOW_INPUT           0
#define HIDE_INPUT           1

// time
#define ONE_SEC			     		 1000                    // 1 second to show message
#define MESSAGE_TIMEOUT      2000                    // 2 seconds to show message
#define KEY_TIMEOUT          30000                   // 30 seconds key press timeout

#define EXIST                '1234'
#define NOT_EXIST            '0000'

#define TRUE  1
#define FALSE 0

#define DATA_COMM			    0x01
#define HB_COMM				    0x02
#define BEACON_COMM				0x03
#define CLIQ_COMM				  0x09
#define KEY_RETURN 				0x05

#define FW_UPLOAD_TCP			0x04

#define STATIC				0x01
#define DYNAMIC				0x02

#define NORMAL_MODE		0x00
#define BEACON_MODE		0x01
#define R2_R3_MODE		0x02
        
#define CMD_BEACON_READ				0x03
#define CMD_BEACON_WRITE			0x04
#define CMD_BEACON_RESET			0x05
#define CMD_BEACON_VERIFY 		0x06
#define CMD_BEACON_DISCONNECT	0x07

#define Master_Code			3271
#define Emergency_Code	6926
#define Serial_No_Code	9273
#define Firmware_Code		3256

//#define BEACON_ON			0x01
//#define BEACON_OFF		0x02

#define ENABLE					0x01
#define DISABLE					0x02

// event details
#define EVT_KEY_TAKEN        			81                    // A key has been given to a user.
#define EVT_KEY_RETURNED     			82                    // A key has been returned by a user.
#define EVT_KEY_NOT_RETURNED			3										 // Key not returned in time.
#define EVT_NOKEY_TAKEN		   			4										 // No Key taken after login
#define EVT_INVALID_USR_PIN  			83                    // An unknown user.
#define EVT_TAMPER           			5                    // Box has been tampered with.
#define EVT_MECH_OVERRIDE    			84                    // A mechanical key override has occurred.
#define EVT_BURGLARY		 		 			85										 // Service Door Opened.
#define EVT_LOGIN            			86                    // A user has logged in.
#define EVT_LOGOUT           			87                   // A user has logged out.
#define EVT_BATTERY_FAULT    			88                   // Lead Acid battery is faulty.
#define EVT_POWER_FAULT      			89                   // Main power failure.
#define EVT_POWER_RESTORED   			90                   // Main power restored.
#define EVT_DOOR_OPEN_TOUT 	 			91                   // Login timeout
#define EVT_DOOR_CLS_TOUT 	 			92                   // Out_Door Open Timeout occurred MLN
#define EVT_OUTDOOR_CLSD     			93					  			 // Out_Door Closed
#define EVT_BACKDOOR_CLOSED  			94									 // Service Door Closed
#define EVT_PROGRAM_KEY_TAKEN			6									 // Programming Key Taken
#define EVT_PROGRAM_KEY_RETURNED  7									 // Programming Key Return
#define EVT_KEY_REGISTERED				101
#define EVT_UNAUTH_KEYREMOVED			102
#define EVT_UNAUTH_KEYRETURNED		103
#define EVT_ALARM_KEYTAKEN				104
#define EVT_ERGY_KEY_TAKE	 	  		105                   // Keys taken by Admin at emergency
#define EVT_ERGY_KEY_RETURN  		  106									 // Keys return by Admin at emergency
#define EVT_KEY_NOT_REGISTERED		107
#define EVT_ALARM_KEYRETURNED			108
#define EVT_TSK_WRNG_TZ						109
#define EVT_HIGHSEC_REMOVAL				110
#define EVT_MULTIPLE_TASK					111
#define EVT_TASK_TIMEOUT					112

/*******************Please Don't Change these Event Code*****************************************/
#define EVT_CLIQSERVER_ERR_R1 		20   								 //get response from cliq server
#define EVT_SERVER_CONN_ERR				95									 // Server Connection failure
#define EVT_VTS_FAILURE						22									 // Truck VTS Failure
#define EVT_BEACON_FAILURE				23									 // Truck Beacon read failure
#define EVT_SWIPEIN_RQST_FAIL			24									 // Truck Swipe In Request Fail
#define EVT_SWIPEOUT_RQST_FAIL  	25									 // Truck Swipe Out Request Fail
#define EVT_MAX_ATTEMPTS					26									 // Max Attempts Reached
#define EVT_TRUCK_NOT_ENTER				27									 // Truck Not Entered into Terminal
#define EVT_PREV_TRIP_NOT_CLOSED	28									 // Previous Trip Not Closed
#define EVT_NO_AUDITRIAL_LOCK			29									 // No Audit trail for Lock
#define EVT_FILLING_INTERVAL  	  30									 // Filling Interval Not Closed
#define EVT_NO_FAN								31									 // No FAN generated for the truck
#define EVT_NO_INVOICE						32									 // No Invoice generated for the truck	

/************************************************************************************************/

#define EVT_INVALID_LOGIN					96									 // Invalid Login
#define EVT_MASTER_MENU_LOGIN			97									 // Login into Master Menu
#define EVT_MASTER_MENU_LOGOUT		98									 // Logout from Master Menu
#define EVT_MASTER_MENU_ACC_FAIL	99									 // Master Munu Access Failed

#define EVT_CLIQSERVER_SUCCESS 	  39   								 //get response from cliq server
#define EVT_POWERUP							  100

#define EVT_NO_LOCK								54					
#define EVT_NO_CYLINDER						55
#define EVT_CYLINDER_UPDATE_FAIL	56

#define EVT_WRONG_SHEDULED_TIME   57
#define EVT_USER_NOT_FOUND			  58
#define EVT_SHOW_CARD_ID 				  59
#define EVT_NOKEY_MAPPED					60
#define EVT_NO_SHIFT_TIME					61
#define EVT_NO_KMS_MAP						62


#define RESP_SUCCESS											0xFF
#define RESP_FAILED												0xFE
#define RESP_UNKNOWN                    	0xFD
//#define RESP_USER_NOTFOUND        				0xFC
#define RESP_TASK_NOTFOUND        				0xFB
#define RESP_NOMEMORY                    	0xFA
#define RESP_DUPL_EMP                    	0xEF
#define RESP_NO_FEATURE                		0xEE
#define RESP_NOKEY                        0xED
#define RESP_SERIAL_MISMATCH    					0xEC
#define RESP_KEY_EVENT_END       				 	0xEB 
#define RESP_EVENT_END            				0xEA
#define RESP_EVENT_DEL_FAIL        				0xE9



//FLAGs for ALARM - 32Bits MLN ...
#define KEY_EVENT_CODE	     0xA5A50000					//CODE FOR KEY EVENTs: The lower word will contain the key number
extern u32_t   ULALARMS;
#define ALARM_VIOLATION		   0x80000000
#define ENDUSERALARM         (1 << 30)
#define ARCHIVE_EVENTS		 	 (1 << 15)
#define KEY_OVERRIDE		 		 (1 << 14)
#define COMMUNICATION_LOST	 (1 << 13)
#define KEY_PROBLEM			 		 (1 << 12)
#define KEY_NOT_TAKEN		 		 (1 << 11)
#define KEY_REL_EMERGENCY	   (1 << 10)
#define TASK_OVERDUE		     (1 << 9)
#define LOGIN_TIMEOUT		     (1 << 8)
#define SD_CARD_FULL		     (1 << 7)
#define ENTRY_IN_WRONG_TZ	   (1 << 6)
#define PIN_TRIES_FAIL		   (1 << 5)
#define BATTERY_LOW			     (1 << 4)
#define AC_POWER_OFF		     (1 << 3)
#define SERVICE_DOOR_OPEN	   (1 << 2)
#define MECH_OVERRIDE		     (1 << 1)
#define DOOR_OPEN_TIMEOUT	   1

#define USER_ALARMS			 ( TASK_OVERDUE		\
							 | SERVICE_DOOR_OPEN\
							 | MECH_OVERRIDE	\
							 | DOOR_OPEN_TIMEOUT)
/*RELAY 2 TRIGGER FOR EVENT */	

#define TRG_BURGLARY_ALM     0x00000001
#define TRG_SRV_DOOR_OPEN 	 0x00000002
#define TRG_MECH_OVERRIDE    0x00000004
#define TRG_ACPOWER_OFF		 	 0x00000008
#define TRG_TASK_OVERDUE	 	 0x00000010
#define TRG_HIGHSEC_REMOVE   0x00000020
#define TRG_EMER_KEYTAKEN    0x00000040
#define TRG_EMER_KEYRETURN 	 0x00000080
#define	TRG_BATTERY_LOW		 	 0x00000100
#define TRG_PIN_TRIES        0x00000200
#define TRG_TASK_NOT_DONE    0x00000400
#define TRG_DOOR_CLOSETOUT   0x00000800
#define TRG_DOOR_OPENTOUT  	 0x00001000
#define TRG_WRONG_TASTTIME   0x00002000
#define TRG_NOKEY_TAKEN		 	 0x00004000
#define TRG_LOGIN	         	 0x00008000
#define TRG_LOGGEDOUT        0x00010000
#define TRG_LOGIN_MULTIPLE   0x00020000
#define TRG_KEY_TAKEN        0x00040000
#define TRG_KEY_RETURN       0x00080000
#define TRG_MODEM_NOTRESPOND 0x00100000

#define KEY_TAKEN_RQST						0x01
#define KEY_RETURN_RQST						0x02
#define KEY_NOT_TAKEN_RQST        0x03
#define CARDID_KEYTAKEN_RQST			0x04
#define KEY_NOT_RETURN_RQST				0x05
#define KEY_TAKEN_ACK_RQST				0x06	
#define KEY_RETURN_ACK_RQST				0x07
#define	USERCODE_KEYTAKEN_RQST		0x08



#define INTELLIGENT							  0x01
#define NON_INTELLIGENT					  0x02

#define	SIXTEEN_KEYS_MODE					0x01
#define THIRTYTWO_KEYS_MODE				0x02

#define UNIVERSAL									0x01
#define MIFARE										0x02

typedef union                                       // Union to facilitate reading 4 byte string as long
	{
	    u32_t num;
	    char  array[6];
	}   CODE;
	
	typedef struct
	{
		u32_t	remote_ip1;
		u32_t	remote_ip2;
		u32_t	remote_ip3;
		u16_t	firmware_port;
		u16_t	event_port;
		u16_t	hb_port;
		u8_t cur_key_pos;
		char url[32];
		u8_t url_cnt;
		char url1[32];
		u8_t url_cnt1;
		u8_t hold_on_key;//useless
		u8_t unused[42];
	}	COMM;
//	typedef struct 
//	{
//		u32_t PIN;
//		u32_t user_id;
//		u32_t card;
//		u32_t user_key;
//		u8_t  enabled;
//		u8_t  login_type;
//		u8_t  rights;
//		char  name[32];
//		u8_t  beacon_mac_id[6];
//		u8_t  unused[4];
//	}USERS;
	//128 bytes
	typedef struct 
	{
	    u16_t start;                                    // Start time (HH*60 + MM)
	    u16_t end;                                      // End time   (HH*60 + MM)
	}   TM;
	//4 bytes
	typedef struct 
	{
		u32_t exist;
		u32_t time_limit;                               // Time limit in minutes
		u32_t dow;
		s32_t user_code[5];
		u32_t start_date;
		u32_t end_date;
		u32_t start_time;
		u32_t end_time;
		u16_t task_id;	
		char  name[33];		
		u8_t  multiple;			
	  u8_t  key[32];
		u8_t  access_type;
		u8_t unused[8];
	}   TASKS;
typedef struct
{
	u32_t user_id;
	u32_t card;	
	u32_t end_tickvalue;
	u32_t start_date;
	u32_t end_date;
  u32_t dow;
	u32_t start_time;
	u32_t end_time;
	u32_t access_deneidtime;
	u32_t user_pin;
	u32_t user_key;
	u16_t facility_code;
	u16_t tasks[MAX_USER_TASKS];
	u8_t  enabled;
	u8_t issue_lvl;
	u8_t issue_type;
	u8_t  door;
  u8_t holiday;
	u8_t access_type;	
	u8_t login_type;
	u8_t task_count;
	u8_t	finger_enabled;
	u8_t  acc_type;
	u32_t PIN1;
	char name[32];
}   USERS;
	typedef struct
	{
		u32_t code;
		u32_t key11;
		s32_t user;
		u32_t ticks;
		u16_t task;
		u8_t save_event;
		u8_t sent_event;
		u8_t unused[12];
	}EVENTS;
	//32 bytes
/** USER PIN NEED TO ADD**/
	typedef struct
	{
		u32_t exist;//not used
		u32_t code;
		u32_t ticks;		
		u32_t card_id;
		u32_t	user_pin;
		u16_t user;
		u8_t  task;//not used
		u8_t  status;
		//u8_t  flag;//not used
		//char  description[15];	
//		char user_name[13]; 
//		u8_t  key_id;//not used

		u8_t unused[8];
	}   KEY_EVENTS;
	//52 bytes

	typedef struct
	{
		u32_t ticks[8];
	}TIME_STAMP;
	//192 bytes for 32 keys.
	//3072 bytes for 512 keys.

	typedef struct 
	{
	  u32_t exist;
		u32_t task_removal_time;
		u8_t flag;
	 	u8_t enabled;
		u8_t keyid;
		u8_t keyvalue[12];
		u8_t status;
//		u8_t max_keys_reg;
		u8_t unused[8];
	}KEYS;
	//42 bytes

	typedef struct
	{
	    char code[256];
	}   BOOTSECTOR;
	//260 bytes

	typedef struct
	{
		u32_t serial;
		u32_t network;
		u32_t dhcp;
		u32_t ip;
		u32_t netmask;
		u32_t gateway;
		u32_t trigger;
		u32_t dashbrd_trigger;
		u32_t dns_ip1;
		u32_t dns_ip2;
		u32_t cus_id;
		u8_t  key_return_time;
		u16_t telnet_port;
		u16_t e_count; //event receive count for conn log 
		u8_t  alarm_enable;
		u8_t  key_turn_time;
		u8_t  open_door_time;
		u8_t  close_door_time;
		u8_t  standalone;			//1->standalone,0->PC mode
		u8_t  date_display;
		u8_t  time_display;
		u8_t  dst;
		u8_t  self_user_allign;
		u8_t	hb_duration;
		u8_t	fw_upload;
		u8_t	sbfw_size;
		u8_t	factory;
		u8_t	ip_type;
		u8_t 	hooter_enable;
		u8_t  conn_type;
		u8_t  beacon_reader;
		u8_t	Key[8];
    u8_t  Key_return_flag[8];		
		u8_t  Intelligent_mode;
		u8_t	multiple_key_access;
		u8_t	Total_Keys;
		u8_t	Main_door;
		u8_t reader;
		u8_t time_zone;
		//u8_t	unused[2];
		
//		u8_t  Key_validation_time;
	}   SYSTEM;
	//75 bytes

	typedef struct
	{
		u16_t add_MAC0;
		u16_t add_MAC1;
		u16_t add_MAC2;	
	}   MAC;
	//6 bytes


extern BOOTSECTOR  Boot_memBlock_t;	
extern EVENTS 	   event_t;
extern KEY_EVENTS  key_events_t[MAX_KEYS];
extern KEYS        key1_t[MAX_KEYS];
extern MAC	       mac_t;
extern SYSTEM      sys_t;
extern TIME_STAMP  TskTimeout_stamp_t;
extern USERS       user_t;
extern COMM				 comm_t;
extern TASKS       task_t;


//... FLAGs for ALARM - 32Bits
//extern u32_t galm_trg;			// flag for relay trigger
extern u32_t gdashboard_trg;	//flag for dashboard trigger
extern u32_t gcurrent_date;
extern u32_t gmul_code[5];
extern u32_t ks_serial;
extern u16_t glcd_refresh_time;
extern int gcurrent_dow;
extern int gcurrent_time;
extern u32_t glast_event;
extern u32_t glast_event_check;
extern int gcheck_task_time;
extern int gupdate_chart;
extern s16_t ginstaller_pin;
extern s16_t ginstaller_code;
extern s16_t gadmin_code;
extern u32_t user_code_main;
extern u32_t user_code_ble;
extern int gpacket_count;
extern int gpacket_in;
//extern int glcd_x;
//extern int glcd_y;
extern u8_t fw_flag;
//extern u8_t rstflag;
extern u8_t sys_erase_flg;
extern u8_t ftp_option;
extern u8_t glogin_user;		
//extern char gprotocol_packet_in[512];
extern unsigned char gprotocol_packetconfig_in_out[600];
extern char config_value[32];
extern volatile u32_t gkeyb_buf[KEYB_QUEUE];
extern volatile int	  gcomm_busy;
extern const char glcd_msgs[][24];
extern struct tcp_pcb *protocol_tpcb;
extern u32_t rmip1;
extern u32_t rmip2;
extern u32_t rmip3;
extern u8_t	 comcount;
extern u8_t hb_event_loginflag;



extern u8_t evtackcount;
extern u16_t system_count,fw_count,fwdata_count,user_count;
extern u16_t total_comm_time;
extern u8_t segnr;
extern u8_t tpnr;
extern u8_t factory;
extern u8_t bluk_flg;
extern u16_t fw_datalen;
extern u16_t packet_num;
//extern char g_protocol_packet_out1[150];
extern u8_t flg;
extern u8_t user_key_id;
extern u8_t fw_flg;




extern char ble_protocol_packet_in[256];
//extern u8_t ble_protocol_packet_out[32];
//extern char rfid_protocol_packet_in[32];

//extern u8_t reader_config_data[4][15]; // reader configuration data array
//extern u8_t reader_config_flag;
//extern u8_t line_check;
//extern u32_t ble_count;
//extern u32_t rfid_count;
extern u8_t flag_init;
extern char url[32];
extern char url1[32];
extern u8_t	arrow_r_flg;
extern u8_t serial_flg;
extern u8_t Enter_flag;
extern u16_t login_user_pin;


//extern volatile char data_w[30];
extern s32_t user_code_pin;
extern u8_t max_keys_available;


extern u8_t solenoid[MAX_KEYS];
extern u8_t ibutton_value[MAX_KEYS][13];
extern u8_t i_button_read;
extern u16_t ev_port,hb_port;
extern volatile u32_t gfw_time;
extern long int user_end_tickvalue;
extern u8_t key_no;
extern u8_t  org_key_data[MAX_KEYS][12];
extern u8_t  key_not_reg[MAX_KEYS];      //1-not reg,2-reg
extern u32_t key_pos; 
extern u32_t key_chk;  
extern u8_t  check_key[MAX_KEYS];
extern u8_t lock_flg;
extern u8_t lock_flg1;
extern u8_t lock_flg2;
extern u8_t conn_flg;
extern u8_t disconn_flg;
extern u8_t ble_user_flg;
extern u8_t ble_key_no;
extern u8_t ble_task_flg;
extern u16_t const CCITT_CRC_TAB[256];
extern u8_t  fingerprint;
extern u8_t finger_user;
extern u16_t lumi_count;
extern u8_t bio_delete_flag;
//communication
extern void self_user_allign(void);
extern void decryption(void);
extern void encryption(void);
extern void handle_protocol_char(s32_t ch);
//extern void handle_protocol_suprema(s32_t ich);
extern void handle_protocol_packet(void);
extern void httpd_init(void);
extern void prot_user_add(s32_t code);
extern void prot_init_event(void);
//extern void pstr(char *str);      
//extern void teststr(char *str);
//extern void reader_config(void);
//extern void p1str(char *str);  
//extern void send_bledata(char *str);
//extern int pchar(s32_t ch); 
//extern void p2str(char *str);                               
//extern int p2char(s32_t ch);     
extern int pchar_one(s32_t ich);  
extern s32_t find_user_sd(s32_t user_code);
// lcd 
//extern void lcd_back(s8_t scamount);
//extern void lcd_center(u8_t ucline, char *p);
//extern void lcd_char(char ch);
//extern void lcd_clr(u8_t ucline);
//extern void lcd_delete(s8_t scamount);
//extern void lcd_forward(s8_t scamount);
//extern void lcd_show(u8_t ucline, char attribute, u8_t idx);
//extern void lcd_show_index(u8_t ucline, char attribute, u8_t idx);
//extern void lcd_show_int(u8_t ucline, u32_t uipar);
//extern void lcd_show_index_int(u8_t ucline, char attribute, u8_t idx, s32_t par);
//extern void lcd_show_index_str(u8_t ucline, char attribute, u8_t idx, char *par);
//extern void lcd_show_str(u8_t ucline, char attribute, u8_t idx, char *par);
//extern void lcd_str(u8_t ucline, char *p);
//extern void lock_on(char a);
//extern void lcd_dis(char a);

// sd card read and write
extern void read_boot_sd(s32_t block);
extern void read_card_sd(s32_t block);
extern void read_event_sd(s32_t block);
extern void read_keyevents_sd(s32_t block);
extern void read_key_sd(s32_t block);
extern void read_mac_sd(void);
extern void read_system_sd(void);
extern void read_user_sd(s32_t block);
extern void write_boot_sd(s32_t block);
extern void write_card_sd(s32_t block);
extern void write_event_sd(s32_t block);
extern void write_keyevents_sd(void);
extern void write_key_sd(void);
extern void write_mac_sd(void);
extern void write_system_sd(void);
extern void read_comm_sd(void);
extern void write_comm_sd(void);
extern void write_user_sd(s32_t block);

// menu function
extern int select_menu(s32_t *menu_i, s32_t temp_key, s32_t limit);
extern int set_port(void);
extern int set_port_hb(void);
extern int set_port_tcp(void);
extern int set_port_cliq(void);
extern void diagnostics(void);
extern void edit_admin_pin(void);
extern void edit_user_pin(void);
extern void emergency_keyaccess(void);
extern void network1(void);
extern void system_restart(void);  
extern void system_setup(void);
extern void utilities(void);
extern void view_mac_address(void);
// alarm and trigger
extern void alarm_handler(void);	
extern void service_menu(void);
void error_msg(void);//error msg for reader
//User
extern void delete_card_user(s32_t icode1,u32_t uicard);
extern void save_card_user(s32_t icode1,u32_t uicard);

extern void user_select(s32_t imode);

//key
extern void update_key_log(s32_t ususer01,s32_t uctask, s32_t uckey_num, u8_t key_state);

//general function
//extern char *find_string(char attribute, s32_t idx);
extern void create_keyevents(void);
extern void delete_location(void);
extern void eraseboot_sdcard(void);
extern void format_sdcard(void);
extern void not_implemented(void);
extern void writedefaults_sdcard(void);
extern void format_ip(char *str, u32_t ip1);
extern void get_timestamp(char *str, u8_t ucformat);
extern void menu_login(s32_t source_keyb);
extern uint8_t key_remove_return(uint32_t user_code,uint32_t card_id);
extern void prot_log_event(s32_t event, s32_t user,s32_t task, u32_t k1);
extern void Wrong_entry(void);
extern int compare_date(u32_t uifirst_date,u32_t uisec_date);
//extern void greenled_onoff(u8_t flag);
 
extern int dow1(s32_t iday, s32_t imonth, s32_t iyear);
extern s32_t enter_digits(s32_t len,u8_t a);
extern int get_menu_key(s32_t *itemp_key);
extern int key1(u32_t ultimeout);
extern int multiple_task_access(u8_t ucsource_keyb);
extern s32_t remove_return_multiple_key_1(s32_t option,s32_t user_code);
extern int emerg_remove_return_key();	
extern int set_option(char cattribute, u8_t *popt, u8_t ucount);
extern u8_t sets_enable_diable(char *p ,char cattribute, u8_t *popt, u8_t ucount);
extern u32_t read_code(s32_t hide_input);
extern u32_t read_string(char *in_str, s32_t max_len);

extern  u16_t get_protocol_length(u8_t);
extern  u8_t serial_verification(void);
extern void crc_calculation(s32_t count);
//extern void crc_calculation_server(u16_t length);
extern u8_t checksum_verification(u16_t length);
extern s32_t set_ip(u32_t *ip);
extern void stringtohex_system(u8_t value);
extern void stringtohex_user(u8_t value);
extern void read_totalevent_sd(void);
extern void write_totalevent_sd(void);
//extern u8_t check_serial_no(void);
extern void date_time_display(char *szstr);
//extern void protocol_system(void);
//extern void protocol_user(void);
extern void comm_menu_dis(void);

extern void totalevent_erase(void);
extern void  telnetserverclose(void);
extern volatile u32_t gcommunication_time;  //we can set time for communication


extern volatile u32_t ghb_time;

//extern u32_t get_ticks_value(void);
extern void GPRS_Connect(void);
extern void check_ble(void);
//extern s32_t p1char (s32_t ch);
extern s32_t free_user(u32_t user_code);
//extern int regular_card(void);
//extern u8_t red_led_onoff(u8_t flag);
extern u32_t read_hexavalue(char*in_str,int max_len);
//extern void i2c_lpc_init(int Mode);
extern s32_t tcpconfig_protocol_send(u32_t pro_length);
//extern void set_date_time(u32_t diff_time);
extern void handle_ftp_char(u8_t ch);
extern void stringtohex_fw(u8_t value);
extern void fw_data(u8_t value);
extern void stringtohex_fwdata(u8_t value);
extern u8_t fw_upload_func(void);
//extern void handle_protocol_ble(u8_t ch);
extern s32_t tcpconfig_client_protocol_send(u32_t pro_length);

extern u8_t handle_config_protocol_char_aes(u16_t count_aes);
extern void arrow_right_pressed(u32_t user_pin1);
extern void Serial_no(void);
extern void disconnect_beacon_reader(void);
extern s32_t add_user(u32_t user_code);
extern u32_t myRand (u32_t,u8_t);
extern u32_t strbin_to_dec(const char * str);
extern u8_t check_config_protocol_enc(u16_t length);
extern s32_t remove_return_key(s32_t option);
extern void emergency_keyaccess1(void);
//extern u32_t get_sec_time_value(u32_t tick_value);
extern void total_register(void);
extern void register_keys(void);
extern void key_check_2(void);
extern void ble_login_success(void);
extern void ble_invalid_login(void);
extern u8_t ble_user_key_no(void);
extern void ble_user_key_status(void);
extern void ble_door_open();
extern s16_t add_task(u16_t taskid);
extern u8_t delete_task(u16_t task_id);
extern s16_t read_task_status(u16_t taskid);
extern void read_task_sd(s32_t block);
extern void write_task_sd(s32_t block);
extern void read_taskbkp_sd(s32_t);
extern void write_taskbkp_sd(s32_t);
extern void main_menu_user_task(void);
extern s32_t user_mykee(u8_t ucremove_mode,u16_t ustask,u16_t key_no);
extern s32_t user_mykee_task(u8_t ucremove_mode,u16_t ustask);
extern s16_t read_user_status(u16_t user_code);
extern void EncyptPIAMPacket_hb(u8_t count_value,uint8_t *buffer);
extern void ble_task_select(u16_t task,u8_t resp);
extern void wrong_unauth_key_check(u8_t);
extern void selftask_allign(void);
extern void clear_keyposition(void);
extern void ble_serial_no(void);
extern void send_biometric(s32_t ch);
extern u8_t arm_trigger_value(void);
extern void biometric_init(void);
extern void self_user_allign_bio(void);
extern u8_t check_user_bio(u16_t user_code);
extern void menu_loginuser(void);
extern u8_t verify_identity(void);
extern s32_t getresult_identy(void);
extern  int modify_user(u32_t uicode);
extern u8_t delete_userrecord(u32_t user_code,u8_t finger,u8_t flag);
extern u8_t delete_alluser(void);
#endif
