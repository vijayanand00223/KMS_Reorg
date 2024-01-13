#include "lpc23xx.h"
#include "ticker_itrp.h"
#include "os_port.h"
#include "menu_main.h"
#include "adc.h"
#include "timer.h"
#include "LCD.h"
#include "BUZZER.h"
#include "KEYPAD.h"


volatile u32_t gkeyb_buf[KEYB_QUEUE];
volatile s32_t   gcomm_busy ;
volatile u8_t  lock_state[64];    //used to store the status of keys
volatile char  pressed_key = 0;
u8_t  lock_key_state[64];

u32_t endtick_value=0;
volatile u32_t gcommunication_time;  //we can set time for communication
volatile u32_t ghb_time;
u8_t i_button_read=0;
volatile u32_t gfw_time;

u8_t green_led[MAX_KEYS];
u8_t red_led[MAX_KEYS];
u8_t solenoid[MAX_KEYS];
u8_t ibutton_value[MAX_KEYS][13];
const char key_table[16] = 
{
    '1',     '2', '3',     ESC, 
    '4',     '5', '6',     ARROW_U, 
    '7',     '8', '9',     ARROW_D,
    ARROW_L, '0', ARROW_R, ENTER
};

u32_t kbd_byte(u32_t uicol1)
{
    u32_t uidata;

    FIO1DIR |= LOCK_AMASK;
    FIO1CLR = LOCK_AMASK;
    FIO1SET = (1 << (uicol1 + LOCK_ASHIFT)) & LOCK_AMASK;

    FIO1DIR &= ~LOCK_DMASK;                         // Databus should be input
    FIO0CLR = KBD_E;
    FIO0CLR = KBD_E;
    FIO0CLR = KBD_E;
    FIO0CLR = KBD_E;
    FIO0CLR = KBD_E;
    FIO0CLR = KBD_E;
    FIO0CLR = KBD_E;
    FIO0CLR = KBD_E;

    uidata = (FIO1PIN & LOCK_DMASK) >> LOCK_DSHIFT;
    FIO0SET = KBD_E;

    return (uidata);
}

void lcd_keypad()
{
    static u32_t uicol = 0U;
    static u32_t uirow = 0U;
    static u32_t uikey_1 = 0U;
    static u32_t uikey2 = 0U;
    static u32_t uikey3 = 0U;
    u8_t uci;
static u32_t uislice = 0U;
    static u32_t uilcd_cycle[2] = {0U,0};
    static u32_t uipos[2] ={0U,0};
		static u8_t count_led=0;
		static u8_t temp_count_led=0;
if (uislice < 8)    
    {
			
		if (lcd_changed[0]|| uilcd_cycle[0])
		{
			switch (uilcd_cycle[0])
			{
			case 0:
				uipos[0] = lcd_changed[0];

				for (uci = 0; uci < 32; uci++)
				{
					if (uipos[0] & 1)
						break;
					uipos[0] >>= 1;
				}

				uipos[0] = uci;
				lcd_byte(LCD_CONTROL, 0x80 + uipos[0]);	// Address 

				
				lcd_changed[0] &= ~(1 << uipos[0]);
				uilcd_cycle[0] = 1;
				break;

			case 1:
				lcd_byte(LCD_DATA, lcd_string[0][(uipos[0] & 0x3F)]);
				uilcd_cycle[0] = 0;
				break;

     }
	 }
		else if (lcd_changed[1]|| uilcd_cycle[1])
		{
			switch (uilcd_cycle[1])
			{
			case 0:
				uipos[1] = lcd_changed[1];

				for (uci = 0; uci < 32; uci++)
				{
					if (uipos[1] & 1)
						break;
					uipos[1] >>= 1;
				}

				uipos[1] = uci;
				lcd_byte(LCD_CONTROL, 0xC0 + uipos[1]);	// Address 

				
				lcd_changed[1] &= ~(1 << uipos[1]);
				uilcd_cycle[1] = 1;
				break;

			case 1:
				lcd_byte(LCD_DATA, lcd_string[1][(uipos[1] & 0x3F)]);
				uilcd_cycle[1] = 0;
				break;

     }	
	 }		
	
	
#ifdef IBUTTON
		   FIO1DIR |= LOCK_AMASK;           //111111-22-27-->o/p
       FIO1CLR=LOCK_AMASK;
       FIO1SET= (temp_count_led<<LOCK_ASHIFT) & LOCK_AMASK;//set a0 to a5 is zero
       FIO1CLR = LOCK_E;
                               
        FIO1CLR= LOCK_RW;      //set rw low
        FIO1DIR |= LOCK_DMASK;                           //set d0 to d3 are o/p                   
        if(temp_count_led<32)
        {
           if(red_led[count_led])
             FIO1CLR= (1U << LOCK_DSHIFT) & LOCK_DMASK;
            else
             FIO1SET= (1U << LOCK_DSHIFT) & LOCK_DMASK;
					
					if(green_led[count_led])
             FIO1CLR= (2U << LOCK_DSHIFT) & LOCK_DMASK;
            else
             FIO1SET= (2U << LOCK_DSHIFT) & LOCK_DMASK;
					
					if(solenoid[count_led]&0x01)
             FIO1CLR= (4U << LOCK_DSHIFT) & LOCK_DMASK;
            else
						 FIO1SET= (4U << LOCK_DSHIFT) & LOCK_DMASK;
        }

        count_led++;
				temp_count_led++;
#ifdef IBUT_4KEY				
				if(!(temp_count_led %4))
					temp_count_led=temp_count_led+4;
        if(temp_count_led>=32)
#else
        if(temp_count_led>=MAX_KEYS)				
#endif				
				{
					count_led=0;
					temp_count_led=0;
				}
#endif
					uislice++;
    }
    else if (uislice == 8)
    {

        // Keyboard scan
        if (uicol < 4)
        {
            uirow = ~kbd_byte(uicol);
            for (uci = 0; uci < 4; uci++)
            {
                if (uirow & 1)
                {
                    if (uikey_1)
                    {
                        uikey_1 = 0xFFU;                // Multiple closures are not allowed
                    }
                    else
                    {
                        uikey_1 = (((u32_t)uci << 2) | uicol) + 1;// 20-10-07 swapped row and column so layout is clearer
                    }
                }
                uirow >>= 1;
            }
            uicol++;
        }
        else
        {
            if (uikey_1 && (uikey2 == uikey_1) && (uikey3 != uikey_1))
            {
                if (uikey_1 == 0xFF)
                {
                    if (sound_buzzer < 600)
                    {
                        sound_buzzer = 600U;
                    }
                }
                else
                {
                    for (uci = 0; uci < KEYB_QUEUE; uci++)
                    {
                        if (!gkeyb_buf[uci])
                        {
                            if (sound_buzzer < 30)
                            {
                                sound_buzzer = 30U;
                            }

                            gkeyb_buf[uci] = uikey_1;
                            break;
                        }
                    }
                }
            }
            uikey3 = uikey2; 
            uikey2 = uikey_1;
            uicol = 0; 
            uikey_1= 0;
        }
				
       
				
				uislice++;

    }
    else
    {
        // Keyboard queue update
        if ((!pressed_key) && (gkeyb_buf[0]))
        {
            if ((gkeyb_buf[0] - 1) < 16)
            {
                pressed_key = key_table[gkeyb_buf[0] - 1]; // get the value for pressed key
            }
            for (uci = 0; uci < KEYB_QUEUE - 1; uci++)
            {
                gkeyb_buf[uci] = gkeyb_buf[uci + 1];
            }
            gkeyb_buf[KEYB_QUEUE - 1] = 0;
        }
        uislice = 0;
				}
}



/*-----------------------------------------end of file-------------------------------------------*/

