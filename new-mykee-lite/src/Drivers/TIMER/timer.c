#include "lpc23xx.h"
#include "timer.h"
#include "menu_main.h"
#include "ticker_itrp.h"
#include "BUZZER.h"
#include "LED.h"
#include "DOOR.h"
#include "KEYPAD.h"


volatile u32_t Cnt_TriggerTimeout;
volatile u32_t KeyTask_TimeLimit[64]; //used to store task due time	
volatile u32_t TICKER_time;//we can set time limet using TICKER_time.it'll increment for every 1ms
volatile u32_t TICKER_1ms_time; // it'll increment for 100ms
volatile u32_t TICKER_25us_time; // it'll increment for 25us
volatile u32_t g_gprs_poll_time;

volatile u32_t sound_buzzer;

static void led(char count)  //led function based count1 (count1==count_led)
{

           if(red_led[count])
             FIO1CLR= (1U << LOCK_DSHIFT) & LOCK_DMASK;
           else
             FIO1SET= (1U << LOCK_DSHIFT) & LOCK_DMASK;
					
					if(green_led[count])
             FIO1CLR= (2U << LOCK_DSHIFT) & LOCK_DMASK;
           else
            FIO1SET= (2U << LOCK_DSHIFT) & LOCK_DMASK;
						 
						if(solenoid[count]&0x01)
             FIO1CLR= (4U << LOCK_DSHIFT) & LOCK_DMASK;
            else
             FIO1SET= (4U << LOCK_DSHIFT) & LOCK_DMASK;


}

/*
   Function name : TICKER_Init
   This function initialize the ticker.
   We want a 1ms resolution

   PCLK is 48MHz.
   Timer Freq = PCLK / (T0PR * T0MR1) = 1000Hz

   T0PR  = 60
   T0MR1 = 1000
   */
// this function is used to initialize timer 0.
void TICKER_Init(void)
{
	u32_t pclksel;
	u8_t uci=0;
	pclksel= (PCLKSEL0 & (~(0x3<<2))) | (0x01 << 2);// Enable Timer 0 clock full speed (48 MHz)
    PCLKSEL0 = pclksel;

    T0TCR  = 2;                                     // Stop and reset the timer
    T0CTCR = 0;                                     // Timer mode
    T0PR   = 0;                                     // Prescaler
    T0MR1  = 4800;                                  // Match value 100 us
    T0MCR  = 3 << 3;                              // When match reset and interrupt
	for(uci=0; uci<MAX_KEYS; uci++)
	{
		KeyTask_TimeLimit[uci] = 0;
	}
}

//fuction for timer start
void TICKER_Start(void)
{
    TICKER_time  = 0;
    T0TCR        = 1;                               // Start timer
}

/******************************************************************************
 ** Function name:      TICKER_Handler
 **
 ** Descriptions:       Description: Interrupt service routine, for every 1ms.
 **						Implemented to get the value from the keypad at any time
 **
 ** parameters:          None
 ** Returned value:      None
 ** 
 ******************************************************************************/
void  TICKER_Handler(void)__irq
{
	static u32_t tick_100us=0;
	 static u8_t keycount=0;
		static u32_t uislice = 0U;

	u16_t us_bat_volt,us_ac_power_volt;
		
if(uislice<=8)
{

			uislice++;
			}
		else 
			uislice=0;	
#ifdef IBUTTON			
		FIO1SET= LOCK_E;
		if(!i_button_read)
		{
			lcd_keypad();
		}
#endif
#ifdef TRUEKEY32 
	lcd_keypad();
#endif
				if(!(tick_100us %10))//1ms
				{
					TICKER_time++;                              // 1ms
					//systemTicks++; 															//1ms
				}
					

			if(!(tick_100us % 10000)) 		//1 sec
			{
	
				endtick_value++;
				glcd_refresh_time++;
				if(g_gprs_poll_time)
					g_gprs_poll_time--;
				if(gcommunication_time)
				{
					gcommunication_time--;
				}
				if(ghb_time)
				{
					ghb_time--;
				}
					
				if(KeyTask_TimeLimit[keycount])
				{
					if(KeyTask_TimeLimit[keycount] <= endtick_value) //Raise Alarm when Counter exceeds the timelimit
					{
						ULALARMS |= TASK_OVERDUE;
						KeyTask_TimeLimit[keycount]=0xFFFFFFFF;	//Cleared to avoid checking again for the same key for which the alarm has been raised
						TskTimeout_stamp_t.ticks[keycount]=endtick_value;
					}
				}
				keycount++;
				keycount&=(MAX_KEYS-1);

#ifdef FWUPLOAD_SERVER 			
		if(gfw_time)
			gfw_time--;			
#endif					

						if(AD0DR1 & (1<<31))
						{
							us_ac_power_volt = read_ac_adc_ch();
							if(!us_ac_power_volt)
							{
								ULALARMS |= AC_POWER_OFF;
								if(AD0DR0 & (1<<31))
								{
									us_bat_volt = read_dc_adc_ch();
									if(us_bat_volt < 350) // gives alarm at 9v (  for 1V ~= adc value 38.8)
									{
										ULALARMS |= BATTERY_LOW;
									}
									else
									{
										ULALARMS &= ~(BATTERY_LOW);
									}
								}
							}
							else
							{
								ULALARMS &= ~(AC_POWER_OFF);
							}
						}
#ifndef TRUEKEY32						
						if(trg(1))
#else
						if(trg(0))
#endif						
						{
							ULALARMS |= SERVICE_DOOR_OPEN;
						}
								
#ifndef NO_DOOR_NO_SOLENOID
						if(!(ULALARMS & DOOR_OPEN_TIMEOUT))
						{
#ifndef TRUEKEY32							
							if(trg(2))
#else
							if(trg(1))
#endif														
							{
								ULALARMS |= MECH_OVERRIDE;
							}
						}
						#endif
#ifdef TRUEKEY32 						
						if((lock_state[uilock1]&(LOCK_CLOSED|LOCK_OPEN))!=(lock_key_state[uilock1]&(LOCK_CLOSED|LOCK_OPEN)))
						{
							lock_key_state[uilock1]= lock_state[uilock1];
							if(!ucflag_keystatus)
							{
								uilock1= uilock1-1;
								ucflag_keystatus=0xFFU;
							}
							else
							{
								ucflag_keyoverride=0xFFU;
								ucflag_keystatus=0x00;
							}
						}
						else
						{
							if(ucflag_keystatus)
							{
								ucflag_keyoverride=0xffU;
								ucflag_keystatus=0x00;
								
							}
						}
							uilock1 = (uilock1 + 1) & 0x3F; 

						if((uilock1 ==0x3F) && ucflag_keyoverride)
						{
							ULALARMS |=KEY_OVERRIDE;
							ucflag_keyoverride=0x00;
						}
#endif						
				}


		
#ifdef SOUND
    if (sound_buzzer && (!(uislice & 0x03)))
    {
        sound_buzzer--;
        if ((!sound_buzzer) ||( uislice & (0x04)))
        {
            buzzer_On();
        }
        else
        {
            buzzer_Off();
        }
    }
#endif

tick_100us++;

   T0IR        = 2;
    VICVectAddr = 0;
}

/******************************************************************************
 ** Function name:      TICKER_100ms_Init
 **
 ** Descriptions:      Interrupt service routine, for every 100ms.
 **					   Implemented to take care of Task overdue.
 **
 ** parameters:          None
 ** Returned value:      None
 ** 
 ******************************************************************************/
void TICKER_100ms_Init(void)
{
	u32_t pclksel;
	pclksel = (PCLKSEL0 & (~(0x3<<4))) | (0x01 << 4);	// Enable Timer 1 clock full speed (48 MHz)
	PCLKSEL0 = pclksel;
    T1TCR  = 2;                                     // Stop and reset the timer
    T1CTCR = 0;                                     // Timer mode
    T1PR   = 0;                                    // Prescaler
    T1MR1  = 480;                                   // Match value 25 us
    T1MCR  = 3 << 3;                              // When match reset and interrupt

}

void TICKER_100ms_Start(void)
/*
Description: Interrupt service routine, for every 100ms.
Implemented to take care of Task overdue.
*/
{
    TICKER_25us_time  = 0;
    T1TCR        = 1;                               // Start timer
}


void  TICKER_25us_Handler(void)__irq
{
	
	static u32_t ticker_value=0;
	u8_t rom_command=0x33,i=0,j=0;
	static u8_t shift_rfid=0,delay_count=0;
	static u8_t flag_shift=0;
	static u8_t bit_read=0;
	static u32_t tick_100us=0;
	static u8_t count1=0;
	static u8_t temp_count=0;
	volatile register u16_t Volt_Value, Battery;

	TICKER_25us_time++;
	if(!(TICKER_25us_time %1))
	{
	#ifdef IBUTTON
	if(!(tick_100us %1))//100us                //i_button read
	{
		if(i_button_read==1)
		{
			ticker_value=0;
			i_button_read=2;
			delay_count=0;
			shift_rfid=0;		
		}
	else if(i_button_read ==2)
	{
		FIO1DIR |= LOCK_AMASK;           //111111-22-27-->o/p
		FIO1CLR=LOCK_AMASK;
		FIO1SET=((temp_count <<LOCK_ASHIFT) & LOCK_AMASK);
		FIO1CLR = LOCK_E;
    led(count1);                        //set d2,d3 for led function
		
	 if(ticker_value==1)
	 {
		FIO1CLR  = LOCK_RW;      //set rw low
		FIO1DIR |= LOCK_DMASK;                           //set d0 to d3 are o/p
		FIO1CLR  = (8 << LOCK_DSHIFT) & LOCK_DMASK;     
		led(count1); 
	 }
	 else if(ticker_value==10)
	 {
		 FIO1CLR= LOCK_RW;      //set rw low
		 FIO1DIR |= LOCK_DMASK;                           //set d0 to d3 are o/p                   
		 FIO1SET = (8 << LOCK_DSHIFT)& LOCK_DMASK;
		 delay_count=0;
		 shift_rfid=0;
		led(count1); 
	 }
	 else if(ticker_value>=11 && ticker_value<15)
   {
	
	 }
	 else if(ticker_value>=15 && ticker_value<31)
	 { 
	   
	
	   if(!delay_count)
	  {
		 if(shift_rfid<8)
		 {
				flag_shift=((rom_command>>shift_rfid)&0x01);
				shift_rfid++;
			  led(count1);
		 }
		 else
			shift_rfid=0;
	  }
	  if(flag_shift)
	  {
	  	if(!delay_count)
		 {
			 FIO1CLR= LOCK_RW;      //set rw low
			 FIO1DIR |= LOCK_DMASK;                           //set d0 to d3 are o/p
			 FIO1CLR= (8 << LOCK_DSHIFT)& LOCK_DMASK;
			 FIO1CLR= (8 << LOCK_DSHIFT)& LOCK_DMASK;
			
      
			 FIO1CLR= LOCK_RW;      //set rw low
			 FIO1DIR |= LOCK_DMASK;                           //set d0 to d3 are o/p
			 FIO1SET = (8 << LOCK_DSHIFT)& LOCK_DMASK ;
		   
		 }
		 delay_count++;
		 if(delay_count==2)
		 {
				delay_count=0;
		 }
	  }
	 else
	 {
		if(!delay_count)
		{
			FIO1CLR= LOCK_RW;      //set rw low
			FIO1DIR |= LOCK_DMASK;                           //set d0 to d3 are o/p
			FIO1CLR= (8 << LOCK_DSHIFT)& LOCK_DMASK; 
		 
		}
		else if(delay_count==1)
		{
				FIO1CLR= LOCK_RW;      //set rw low
				FIO1DIR |= LOCK_DMASK;                           //set d0 to d3 are o/p
			  FIO1SET = (8<< LOCK_DSHIFT)& LOCK_DMASK;     
			  
		}
		delay_count++;
		if(delay_count==2)
		{
			delay_count=0;
		}
	 }
	 bit_read=0;
	
	}
	else if(ticker_value>=31 && ticker_value<95)
  {
		
		FIO1CLR= LOCK_RW;      //set rw low
		FIO1DIR |= LOCK_DMASK;                           //set d0 to d3 are o/p
		FIO1CLR= (8 << LOCK_DSHIFT)& LOCK_DMASK;
		FIO1CLR= (8 << LOCK_DSHIFT)& LOCK_DMASK;
		 
		
		FIO1CLR= LOCK_RW;      //set rw low
		FIO1DIR |= LOCK_DMASK;                          
		FIO1SET = (8 << LOCK_DSHIFT)& LOCK_DMASK;
		

		FIO1DIR &= ~LOCK_DMASK; //set D0->D3 are input
		FIO1SET = LOCK_RW;     //set rw high
		FIO1SET = LOCK_RW;     //set rw high
		FIO1SET = LOCK_RW;     //set rw high
		lock_state[bit_read]=((FIO1PIN & LOCK_DMASK) >> (LOCK_DSHIFT));
		lock_state[bit_read]&=0x01;
		bit_read++;
  }
	else if(ticker_value==95)
	{
	
		FIO1CLR= LOCK_RW;      //set rw low
		FIO1DIR |= LOCK_DMASK;                           //set d0 to d3 are o/p
		FIO1SET = (8 << LOCK_DSHIFT)& LOCK_DMASK;
	  
	}
	else if(ticker_value==100)
	{
	   
   ticker_value=0;                                
                                                            
	if(count1<MAX_KEYS)
	{

		ibutton_value[count1][0]=0;
		for(i=63;i>55;i--)
		{
		 ibutton_value[count1][0]=ibutton_value[count1][0]<<1;
		 ibutton_value[count1][0]|=(lock_state[i]);                                                        
		}

		for(j=0;j<8;j++)
		{                                             
			ibutton_value[count1][1+j]=0;
			for(i=0;i<8;i++)
			{
		  	ibutton_value[count1][1+j]<<=1;
			  ibutton_value[count1][1+j]|=lock_state[55-(j*8)-i];
			}                                                             
		}
	}                             
   
       temp_count++;
			 count1++;
#ifdef IBUT_4KEY 			 
			if(temp_count<32)
			{
				if(!((temp_count) %4))
					temp_count=temp_count+4;
#else
			if(temp_count<16)
			{
#endif			
				i_button_read=1;
			  
			}
			else
			{
				count1=0;
				temp_count=0;
				i_button_read=0;

			}
	}

	}

	ticker_value++;
}
#endif	
	#ifdef TRUEKEY32	 
       
        FIO1DIR |= LOCK_AMASK;
        FIO1CLR = LOCK_AMASK;
        FIO1SET = (uilock << LOCK_ASHIFT) & LOCK_AMASK;
        
        FIO1DIR &= ~LOCK_DMASK;
        FIO1SET = LOCK_RW;
        FIO1CLR = LOCK_E;
        uils = lock_state[uilock];
        uidata_out = uils & 0x0F;                       // Grab write data in time of read pulse
        uidata_out ^= LOCK_RED | LOCK_GREEN;
        uidata_in = (FIO1PIN & LOCK_DMASK) >> (LOCK_DSHIFT - 4);
        FIO1SET = LOCK_E;
        
        FIO1CLR = LOCK_RW;
        FIO1DIR |= LOCK_DMASK;
        FIO1CLR = LOCK_DMASK;
        FIO1SET = (uidata_out << LOCK_DSHIFT) & LOCK_DMASK;
        FIO1CLR = LOCK_E;
        uidata_in ^= LOCK_OPEN | LOCK_CLOSED;       // This write-back functions also as delay for E pulse
        uidata_in &= LOCK_OPEN | LOCK_CLOSED;
        uils = (uils & 0x0F) | (uidata_in & (LOCK_OPEN | LOCK_CLOSED));
        lock_state[uilock] =(u8_t) uils; 
        FIO1SET = LOCK_E;
        uilock = (uilock + 1) & 0x3F;                   // Increment to next row and truncate to 0..63
				
#endif	
}

    T1IR        = 2;
    VICVectAddr = 0;
}

//// set the time delay to wait for some operation.
u32_t set_timeout(u32_t ticks)
{
    return (TICKER_time + ticks);
}

// check time delay to complete the set time.
s32_t chk_timeout(u32_t timeout)
{
    return ( ((s32_t)TICKER_time -(s32_t) timeout) > 0);     // This version handles also wrap-around
}

// set the time delay to wait for some operation.
u32_t Set_Trigger_Timeout(u32_t ticks)
{
    return (TICKER_time + ticks);
}

//check time delay to come out.
s32_t Chk_Trigger_Timeout(u32_t timeout)
{
    return ( ((s32_t)TICKER_time - (s32_t)timeout) > 0);     // This version handles also wrap-around
}




