/*
 * File Name:pin_menu.c
 * 		This file used to change the pin numbers.
 * 		This file contains edit and change user pin menu functions 
 * 		and edit admin pin function.
 * 
 * Version:V02.02.00 -->code restructure.
 * changes from old version: 1. code restructure and detail history 
 *                              included for files and functions
 * changes done by:   K.Sivakumar
 * 
 * Version:V02.00.00 -->code restructure. 
 * 
 * Copyright (c) 2004-2011 Sterna Security.
 * All rights reserved.
 * Author: R.Narayanan
 */


#include "menu_main.h"

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
/******************************************************************************
 ** Function name:      user_change_pin
 **
 ** Descriptions:       this function explains user has to change 
 ** 					pin number for first time login
 **
 ** parameters:         None
 ** 				 
 ** Returned value:     None
 ** 
 ******************************************************************************/
//s32_t user_change_pin(u32_t usercode)
//{
//    u32_t uifuser_pin,uisuser_pin;
//	s16_t sdlocation,location_temp;
//	while(1)
//	{	
//		lcd_show(0U,'U',5U);	
//    	lcd_show(1U, 'S', 4U);     
//		uifuser_pin = read_code(HIDE_INPUT);
//		if(uifuser_pin==user_t.PIN)
//		{     
//		    lcd_show(0U, 'S', 14U);      //same as current pin
//			lcd_show(1U, 'S', 15U);      //please enter new pin
//			delay((u32_t) ONE_SEC);		
//		}
//		else if(uifuser_pin==-1)
//		{
//			return 0;
//		}	
//        else if(uifuser_pin==0)
//        {
//            lcd_show(1, 'S', 3); 
//            Wrong_entry();
//        }
//		else
//		{
//			break;
//		}
//						
//	}
//	while(1)
//	{
//		lcd_show(0U,'U',5U);
//	    lcd_show(1U, 'S', 5U);                            // Re enter PIN
//		uisuser_pin=read_code(HIDE_INPUT);
//		if(uisuser_pin==-1)
//		{
//			return 0;
//		}		
//	    else if (uisuser_pin ==uifuser_pin)
//		{
//			sdlocation=read_user_status(usercode);
//			location_temp=free_user(usercode);
//			read_user_sd(sdlocation);
//			user_t.PIN = uifuser_pin;	
//			user_t.first_login=0U;			// for user pin change for first login
//			write_user_sd((s32_t)location_temp);
//			read_user_sd(sdlocation);
//			user_t.code=user_t.card=0;
//			write_user_sd(sdlocation);
//			lcd_show(1U, 'S', 6U);                       // New PIN saved 
//			prot_log_event((s32_t) EVT_PSWD_CHANGED,(s32_t) user_code_main,0, 0U, 0U,"EV");
//			delay((u32_t) ONE_SEC);
//			break;
//		}		
//		else
//		{
//	        lcd_show(1U, 'S', 7U);                       // PIN Mismatched
//			delay((u32_t) ONE_SEC);
//		}		
//	}
//	return 1;
//}

///******************************************************************************
// ** Function name:      edit_user_pin
// **
// ** Descriptions:       the installer can edit the user pin number
// **
// ** parameters:         None
// ** 				 
// ** Returned value:     None
// ** 
// ******************************************************************************/
//void edit_user_pin(void)
//{
//  u32_t uiuser_code, uiuser_pin;
//	s16_t sdlocation=0,location_temp=0;

//	lcd_show(1U, 'I', 3U);
//    lcd_show(0U, 'S', 0U);                            // Enter user code
//    uiuser_code = read_code(SHOW_INPUT);
//    if(uiuser_code ==(u32_t) gadmin_code)
//    {
//        lcd_show(0U, 'S', 2U);	//Invalid login code
//        lcd_clr(1);
//        delay((u32_t) ONE_SEC);
//		return;
//    }
//    if (uiuser_code==-1)
//    {
//        return;
//    }
//	if(!uiuser_code)
//	{
//        lcd_show(0U, 'S', 2U);                        // Invalid user code
//        lcd_clr(1U);
//        delay((u32_t) MESSAGE_TIMEOUT);
//		return;
//	}
//	sdlocation=read_user_status(uiuser_code);

//	if (sdlocation==-1)
//	{
//        lcd_show(0U, 'S', 2U);                        // Invalid user code
//        lcd_clr(1U);
//        delay((u32_t) MESSAGE_TIMEOUT);
//		return;
//	}
//	
//	if(user_t.login_type != 0U)						  // if no PIN for users
//	{
//		lcd_show(0U,'V',25U);	                      // No PIN for user
//		lcd_clr(1U);
//		delay((u32_t) MESSAGE_TIMEOUT);
//		return;
//	}
//    lcd_show(1U, 'S', 4U);                            // Enter New PIN
//	uiuser_pin = read_code(HIDE_INPUT);
//    if (uiuser_pin==-1)
//    {
//        return;
//    }
//    else if(uiuser_pin==0)
//    {
//        lcd_show(1, 'S', 3); 
//        Wrong_entry();
//        return;
//    }
//    lcd_show(1U, 'S', 5U);                            // Re enter PIN
//    if (uiuser_pin == read_code(HIDE_INPUT))
//	{
//		location_temp=free_user(uiuser_code);
//		read_user_sd((s32_t) sdlocation);
//		user_t.PIN = uiuser_pin;	
////		user_t.pintries_count=0;
////		user_t.pintries_flag=0;
////		user_t.pintries_date=0U;	
//		write_user_sd((s32_t) location_temp);
//		read_user_sd( sdlocation);
//			user_t.truck_id =user_t.card=0;
//		write_user_sd(sdlocation);
//        lcd_show(1U, 'S', 6U);                       // New PIN saved 
////        prot_log_event((s32_t) EVT_PSWD_CHANGED,(s32_t) user_t.truck_id , 0U);
//		delay((u32_t) ONE_SEC);
//	}
//	else
//	{
//        lcd_show(1U, 'S', 7U);                       // PIN Mismatched
//		delay((u32_t) ONE_SEC);
//	}
//}

///******************************************************************************
// ** Function name:      edit_admin_pin
// **
// ** Descriptions:       the admin can change the pin number itself.
// **
// ** parameters:         None
// ** 				 
// ** Returned value:     None
// ** 
// ******************************************************************************/
//void edit_admin_pin(void)
//{
//  u32_t uiuser_pin;
//	s16_t sdlocation=0,location_temp=0;
//	
//	lcd_show(0U, 'I', 4U);            // Edit Admin PIN
//	lcd_show(1U, 'V', 13U);           // Enter service code:
//	if(read_code(HIDE_INPUT)!= 1947U) // if entered code not equal to 1947	
//	{
//		lcd_show(0U, 'I', 4U);         // Edit Admin PIN
//		lcd_show(1U, 'S', 16U);         // Invalid Service code 
//		delay((u32_t) MESSAGE_TIMEOUT);
//		return;
//	}
//  lcd_show(1U, 'S', 4U);                            // Enter New PIN
//  uiuser_pin = read_code(0xFF);
//  if (uiuser_pin==-1)
//  {
//		return;
//  }
//  else if(uiuser_pin==0)
//  {
//    lcd_show(1, 'S', 3); 
//    Wrong_entry();
//    return;
//  }
//  lcd_show(1U, 'S', 5U);                            // Re enter PIN
//  if (uiuser_pin == read_code(0xFF))
//	{
//		sdlocation=read_user_status(gadmin_code);
//		location_temp=free_user(gadmin_code);
//		read_user_sd((s32_t) sdlocation);
//		user_t.PIN = uiuser_pin;	
////		user_t.pintries_count=0;
////		user_t.pintries_flag=0;
////		user_t.pintries_date=0U;
//		write_user_sd(location_temp);
//		read_user_sd((s32_t) sdlocation);
//		user_t.truck_id =user_t.card=0;
//		write_user_sd(sdlocation);
//        lcd_show(1U, 'S', 6U);                       // New PIN saved 
////        prot_log_event((s32_t) EVT_PSWD_CHANGED,(s32_t) user_code_main , 0U);
//		delay((u32_t) ONE_SEC);
//	}
//	else
//	{
//        lcd_show(1U, 'S', 7U);                       // PIN Mismatched
//		delay((u32_t) ONE_SEC);
//	}
//}
/*-----------------------------------------end of file-------------------------------------------*/
