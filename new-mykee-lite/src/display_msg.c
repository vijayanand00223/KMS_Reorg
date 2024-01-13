/*
 * File name: Display_msg.c
 *   This file contains the display commands for administrator,Installer,Operator and general user.
 * 	 we can select 4 different languages(English,Polish,Spanish,Finnish) to display the commands.
 *   and also this file contains the SMS sentence format for each event and each language. 
 * 
 * Version:V02.02.00 -->code restructure.
 * changes from old version: 1. code restructure and detail history 
 *                              included for files and functions
 * changes done by:   K.Sivakumar
 * 
 * Version:V02.01.00 -->GSM features added. 
 * changes from old version: 1. Added gsms_message array to get the SMS sentence for selected events in english language.
 * 							 2. Added gsms_message_finnish array to get the SMS sentence for selected events in finnish language.
 * 							 3. Added gsms_message_polish array to get the SMS sentence for selected events in polish language.
 * 							 4. Added gsms_message_spanish array to get the SMS sentence for selected events in spanish language.
 * changes done by:  B.Sedhu sundaram  
 *        
 * Version:V02.00.00 -->code restructure. 
 * 
 * Copyright (c) 2004-2011 Sterna Security.
 * All rights reserved.
 * Author:sterna security.
 */

#include "menu_main.h"
#include "lpc23xx.H"
#include <stdio.h>

/*
Characters free for flaging strings
 'q','i','w','v','R','C','O','K','N'
0,2 to 9.
*/
//the following are LCD messages in English language.
const char glcd_msgs[][24] =
{
	" String Not Found",
	"SEnter User Code: ",	//0
	" Enter User PIN : ", 	//1
	" Invalid login code",	//2
	" Invalid PIN",			//3
	" Enter new PIN: ",		//4
	" Re-enter PIN : ",		//5
	" New PIN saved",		//6
	" PIN Mismatched",		//7
	" Not allowed",			//8
	" USER  : %s",	//9
	" Show the RFID",		//10
	" Wrong Task Time",		//11
	" Check with the Chart",//12
	" User Disabled",		//13
	" Same as current PIN",	//14
	" Please enter new PIN",//15
	" Invalid service code",//16
	" BIO ENABLED USER", //17
	"DPlease open door",					//D
	" Please close door",
	" Open door timeout    ",
	" Close door timeout   ",
	" Timer: %s",
	" RETURN TO MAIN",//5
#ifndef TRUEKEY32	
	" 1-TOTAL FOBS REG",
	" 2-ADD FOBS REG",
	" [%s] FOBS",
#else	
	" 1-TOTAL KEYS REG",
	" 2-ADD KEYS REG",
	" [%s] KEYS",
#endif	
	" REGISTERED",
	" [%s] NEW OR",
	" CHANGED KEYS",
	" NOT REGISTERED",
#ifndef TRUEKEY32	
	"KTake Fob %s          ", 						//K
	" Return Fob %s        ",		//1
	" Fob handling timeout",//2
	" Fob %s taken         ",		//3
	" Fob %s returned      ",		//4
	" Fob Remove or Return",
	" Return indicated keys",//6
	" No Fobs to remove",	//7
	" No Fobs to return",	//8
#else
	"KTake Key %s          ", 						//K
	" Return Key %s        ",		//1
	" Key handling timeout",//2
	" Key %s taken         ",		//3
	" Key %s returned      ",		//4
	" Key Remove or Return",
	" Return indicated keys",//6
	" No Keys to remove",	//7
	" No Keys to return",	//8
#endif	
	" User Already Logged IN", //9
	" %s Users Remaining",	//10
	" %s User Remaining",	//11
	" Enter/Show RFID/Finger",//12
	" !Max Entries Reached",//13
	" Process Cancelled",	//14
#ifndef TRUEKEY32		
	" Please keep the fob%s",
#else
	" Please keep the key%s",
#endif	
	" as previous",
	" timed out",
	"GNot saved",							//G
	" Enter New Value",		//1
	" Illegal Value",		//2
	"IInstaller Menu",						//I
	" Utilities",			  //1
	" Diagnostics",			//2
	" Communications",	//6
	" FW Upload   ",            //8
	"AAdministrator Menu",					//A
	" System Setup",   
	" Diagnostics",
	" Utilities",
	" Emergency Access",
	" FW Upload   ",            //8
	" BIOMETRIC   ",
	"UUser menu ",							//U
#ifndef TRUEKEY32	
	" 1.Remove Fobs*",
	" 1.Remove Fobs",
	" 2.Return Fobs",
	" 2.Return Fobs*",		
#else
	" 1.Remove Keys*",
	" 1.Remove Keys",
	" 2.Return Keys",
	" 2.Return Keys*",		
#endif	
	" Change PIN code",
	"sSystem setup",	//0					//s
	" Time and Date",	//1	
	" Operating Parameters",//2
	" Alarm Sound [%s]",	//3 
	" Communications",	    //4
	"oOperating Parameters",				//o
	" Door OpenTime [%dsec]",	
	" Door CloseTime[%dsec]",
#ifndef TRUEKEY32	
	" Fob Access Time[%dsec]",	
	"cTotal Fobs", 						//c
#else
	" Key Access Time[%dsec]",
	"cTotal Keys", 						//c
#endif	
	" 16",
	" 32",
	" RFID [%s]",
	"aAlarm Sound", 						//a
	" On",
	" Off",
	"pKey [%s]",
	" Enable",
	" Disable",
#ifndef TRUEKEY32	
	"kFOBS",
	" 4 FOBS",
	" 8 FOBS",
#else
	"kKEYS",
	" 4 KEYS",
	" 8 KEYS",
#endif	
	"rReader",
	" UNIVERSAL",
	" MIFARE",
	"qBeacon Reader",
	" On",
	" Off",
	"nCommunications",						//n
	" IP  [%s]",
	" SNM [%s]",
	" GW  [%s]",
	" HOST[%s]",
	" HOSTFW[%s]",
	" RMIP[%s]",
	" DNS1[%s]",
	" DNS2[%s]",
	" MAC Address",    
	" Telnet Port Setting",
	" HB Port Setting",
	" Event Port Setting",
	" FIRMWARE Port Setting",
	" IP Type Setting[%s]",
	" Connection Type[%s]",	
	"wIP Type Setting",
	"	STATIC",
	" DYNAMIC",
	" S",
	" D",
	"lConnection Type",							//l
	" Remote IP",
	" Host Address",
	" R",
	" H",
	"tTime and Date",   					//t
	" Time Format [%s]",
	" Set Time [%s]",
	" Date Format [%s]",
	" Set Date [%s]",	
	"fTime Format", 						//f
	" 12H",
	" 24H",
	"dDate Format", 						//d
	" D-M-Y",
	" M-D-Y",
	" Y-M-D",
	"   -  -   (DMY)",
	"BKey %d:Removed",						//B
	" User:%s  Task:%03d",	//1
#ifndef TRUEKEY32	
	" Fob %d:Not Sensed",	//2
	" Fob %d:Available",	//3
#else
	" Key %d:Not Sensed",	//2
	" Key %d:Available",	//3
#endif	
	"xMon", 								//x
	" Tue",
	" Wed",
	" Thu",
	" Fri",
	" Sat",
	" Sun",
	"EALARM MESSAGE",						//E
	" EXCEEDED MAX PIN TRIES",	
	" AC POWER OFF",			
	" MECHANICAL OVERRIDE", 	
	" DOOR CLOSE TIMEOUT",	
#ifndef TRUEKEY32
	" FOB %s -TASK OVERDUE",       //5
#else
	" KEY %s -TASK OVERDUE",       //5
#endif	
	" Error read user %d",
	" Error write user %d",
	" Error read Task %d",
	" Error write Task %d",
	" Error read key event %d",    //10
	" Error write Key evt %d",
	" Error read event %d",
	" Error write event %d",
	" Error read key %d",
	" Error read sys ",            //15
	" Error write sys",
	" BURGLARY ALARM",	
	"WDiagnostics", 						//W
	" Check Outputs",
	" Check LED-s",
#ifndef TRUEKEY32	
	" Check Solenoid",
#endif	
	" Check Keypad",
	" Check RFID Reader",
	" Check Key Control",           //6
	" Check Key Status", 
	" Checking Solenoids...",
	" Checking LEDs...", 
	" Please Turn Key ",		
	" No RFID present",             //11
	"ZCheck Keypad",						//Z
	" 0 Pressed",
	" 1 Pressed",
	" 2 Pressed",
	" 3 Pressed",
	" 4 Pressed",        //5
	" 5 Pressed",
	" 6 Pressed",
	" 7 Pressed",
	" 8 Pressed",
	" 9 Pressed",        //10
	" Enter Key",
	" Pressed",           //Down Arrow
	" * or DEL",	      //Left Arrow
	" # or Space",	      //Right Arrow
	" Press Any Key",   //15
	"QCheck Key Control",					//Q
	" Turn Key %s Open",
	" Turn Key %s Close",
	" Key %s OPEN",
	" Key %s CLOSE",
	"JUtilities",							//J	   
	" System Identity",
	" System Restart",
	" Write Factory defaults",
#ifdef IBUTTON	
	" Key Registeration",
#endif	
	" Change Serial No",
	" Change Customer ID",
	"VRFID reader Disabled!",				//V
	" No tasks for this user",
	" Login By RFID Only",
	" Task Expired",			
	" !User have RFID",
	" Please Show it first",  //5
	" Choose Task",
	" Are you sure?",
	" Press ESC to cancel",
	" No Rights Allotted!",
	" Factory Dft Cancelled",  //10
	" Setting Factory Default",
	" Format SD Cancelled",
	" Enter service code:",
	" System Restarting after",
	" LAN Setting",                //15
	" Battery Low",
	" AC Power ON",
	" Mains ON",
	" Mains OFF",
	" Box Reset Initiated",        //20   
	" System Restarting",
	" Check the key position",
	" Uploading Successfull",
	" New firmware in SD",
	" No PIN for this user",        //25   
	" Completed-%s",
	" Operation Cancelled",			//27	
	"PCheck Board Status ", 				//P
	" AC Power Status", 		
	" Battery Power Status",
	" Outer Door status",
	" Service Door Status",
	" Buzzer Status",              //5
	" AC Voltage:%s V",
	" Battery Voltage:%s V",
	" Closed",
	" Opened",
	" Buzzer checking.....",
	" No outer door",
	"HUsers",								//H
	" Tasks",
#ifndef TRUEKEY32	
	" Creating Fob Events",
	" Clearing Users",
	" Clearing Tasks",
	" Creating Fob data",	
#else
	" Creating Key Events",
	" Clearing Users",
	" Clearing Tasks",
	" Creating Key data",	
#endif	
	" Clearing Events",
	"eTask Can access from",				//e
	" User deactivated",
	" Please wait/ask admin",
#ifndef TRUEKEY32	
	" No Fobs to remove",
	" or return",
	" Fob disabled/Not sensed",  //5
#else
	" No Keys to remove",
	" or return",
	" Key disabled/Not sensed",  //5
#endif
	" Please check with Admin",
	" Enter 5 to 30 ",
	" Rights restricted",
	" Check with Admin",
	" Service Mode",            //10
	" Enter Box serial No",
	" Illegal Value",
	" Firmware Uploading...",
	" Initializing...",
#ifdef TRUEKEY32	
	" Fob events created",        //15
	" All user return fobs",
#else
	" Key events created",        //15
	" All user return keys",
#endif	
	" Box serial:%s",
	" Version   :%s",
	" COMM ERROR!!!!!!!",
	" System restarting",           //20
	" Mail sent successfully",
	" Cannot send mail",
	" Modem not connected",
	" Properly",
	" Please wait..",              //25
	" Reset From software",
	" System will restart",
	" and update FW",
	" No boot code in SD",
	" Upload from sotware",        //30
	" Create task chart..",
	" System restarting for ",
	" FW Uploading...",
	"g     Mykee System      ",
	"RBiometric             ",
	" Add Enrollment        ",
	" Delete Enrollment     ",
	" Delete DB             ",
	" Set Work DB           ",
	" Biometric Initializing",
	" Select User:          ",//6
	" NO USER               ",
	" Place 1st finger      ",
	" User added            ",
	" User add failed       ",//10
	" User_deleted          ",
	" SUCCESS!!!            ",
	" Timedout              ",
	" Failed                ",	//14
	" Invalid User          ",
	" Place 2nd finger      ",
	" Lift and place        ",//17
	" First finger added    ",
	" Second finger added   ",
	" Progress...           ",
	" Biometric Initializing",//21
	" Creating DB           ",
	" Deleting DB           ",		
	" The end"								//z
};
/*-----------------------------------------end of file-------------------------------------------*/
