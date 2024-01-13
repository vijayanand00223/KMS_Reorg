#include "lpc23xx.h"
#include "LCD.h"
#include "ticker_itrp.h"
#include "menu_main.h"
#include <stdio.h>
#include <stdlib.h>

char lcd_string[2][32];
volatile u32_t lcd_changed[2] ={0,0};

s32_t glcd_x;               // used for lcd purpose
s32_t glcd_y;               // used for lcd purpose


// This function is used for write the initializing address to LCD
void lcd_byte(u32_t reg, u32_t info)
{
    VICIntEnClr = INT_TIMER0;
    if (reg)
    {
        FIO1SET = LCD_RS;
    }
    else
    {
        FIO1CLR = LCD_RS;
    }
    FIO0CLR = LCD_E1;
    FIO1CLR = LCD_E;
    FIO1DIR |= LCD_DMASK;
    FIO1CLR = LCD_DMASK;
    FIO1CLR = LCD_RW;
    FIO1SET = (info << LCD_DSHIFT) & LCD_DMASK;
    FIO0CLR = LCD_E1;                               // Data setup time 
    FIO0CLR = LCD_E1;
    FIO0SET = LCD_E1;                               // E pulse width
    FIO0SET = LCD_E1;                               // 2: 290 ns
    FIO0CLR = LCD_E1;                               // Data hold time
    FIO0CLR = LCD_E1;
    VICIntEnable  = INT_TIMER0;
}

// LCD initialization function
void lcd_init(void)
{
    u8_t uci;
    lcd_changed[0] = 0;
	 lcd_changed[1] = 0;
	glcd_refresh_time=0;
    for (uci = 0; uci < 32; uci++)
    {
        lcd_string[0][uci] = ' ';
        lcd_string[1][uci] = ' ';
    }
    delay(200);
    lcd_byte(LCD_CONTROL, 0x38);   delay(20);       // Eight bits mode, set eight bits
    lcd_byte(LCD_CONTROL, 0x38);   delay(20);       // Eight bits mode, set eight bits
    lcd_byte(LCD_CONTROL, 0x0C);   delay(5);        // Display on, no cursor
    lcd_byte(LCD_CONTROL, 0x01);   delay(20);       // CLS and Home
    lcd_byte(LCD_CONTROL, 0x06);   delay(5);        // Entry Mode: Increment, cursor shift
}

void lcd_ch(u32_t uiline1, u32_t uix, u32_t uiinfo)
{
    if ((uiline1 < 2) && (uix < 24) && (lcd_string[uiline1][uix] != uiinfo))
    {
        lcd_string[uiline1][uix] =(char) uiinfo;

        VICIntEnClr = INT_TIMER0;

        if (uiline1)                                   // Indicate which pair of characters changed
        {
            lcd_changed[1] |= 0x0001 << uix;
        }
        else
        {
            lcd_changed[0] |= 0x0001 << uix;
        }

        VICIntEnable = INT_TIMER0;
    }
}

// to display a single charector
void lcd_char(char ch)
{
    lcd_ch((u32_t)glcd_y,(u32_t) glcd_x, (u32_t)ch);
    glcd_x++;
}

// to go one step back
void lcd_back(s8_t scamount)
{
    if (glcd_x >= scamount)
    {
        glcd_x -= scamount;
    }
    else
    {
        glcd_x = 0;
    }
}

// to go one step forward
void lcd_forward(s8_t scamount)
{
    glcd_x += scamount;
}

//go one step back and delete the charector
void lcd_delete(s8_t scamount)
{
    s8_t sci;

    for (sci = 0; sci < scamount; sci++)
    {
         
        lcd_back(1);
        lcd_char(' ');
        lcd_back(1);
    }
}

// Prints string on line of LCD, clears also area after string
void lcd_str(u8_t ucline, char *p)
{
    u8_t uci;
    
    for (uci = 0U; uci < 24U; uci++)
    {
        if (p[uci])
        {
            lcd_ch((u32_t)ucline, (u32_t)uci, (u32_t)p[uci]);
        }
        else
        {
            break;
        }
    }

    glcd_x =(s32_t) uci;
    glcd_y =(s32_t) ucline;
    
    for (; uci < 24U; uci++)
    {
        lcd_ch((u32_t) ucline,(u32_t) uci, ' ');
    }
}

// prints the string on the center of the line.
void lcd_center(u8_t ucline, char *p)
{
    u8_t uci, uclen;

    for (uclen = 0U; uclen < 24U; uclen++)
    {
        if (!p[uclen])
        {
            break;
        }
    }

    for (uci = 0U; uci < 24U; uci++)
    {
        lcd_ch((u32_t) ucline,(u32_t) uci, ' ');
    }

    for (uci = 0U; uci < uclen; uci++)
    {
        lcd_ch((u32_t) ucline,(((24U -(u32_t) uclen) / 2U) +((u32_t) uci)), p[uci]);
    }
}

//clear the LCD line
void lcd_clr(u8_t ucline)
{
    lcd_str(ucline, "");
}

/******************************************************************************
 ** Function name:      find_string
 **
 ** Descriptions:       Search lcd strings from the given array list
 **
 ** parameters:         attribute, idx
 ** 				 
 ** Returned value:     return the string which we want to display in LCD
 ** 
 ******************************************************************************/
char *find_string(char attribute, s32_t idx)
{
	u16_t usi;


			// If not found in local language, take english string
			for (usi = 0U; usi < 1000U; usi++)
			{
				if (glcd_msgs[usi][0] == attribute)
				{
					return ((char *) &glcd_msgs[usi + idx][1]);
				}
			
				if (glcd_msgs[usi][0] == 'z')
				{
					break;
				}
			}
			return ((char *) &glcd_msgs[0][1]);
}

/******************************************************************************
 ** Function name:      lcd_show
 **
 ** Descriptions:       search and show the corresponding sentence from the given array list
 **
 ** parameters:         ucline, attribute, idx
 ** 				 
 ** Returned value:     none
 ** 
 ******************************************************************************/
void lcd_show(u8_t ucline, char attribute, u8_t idx)
{
    char *p="";

    p = find_string(attribute,(s32_t) idx);
    lcd_str(ucline, p);
}

// to display integer variable values to LCD, use this function
void lcd_show_int(u8_t ucline, u32_t uipar)
{
    char szstr[32];

    sprintf(szstr, "%d", uipar);
    lcd_str(ucline, szstr);
}

//to show string with the related detemined string, use this function
void lcd_show_str(u8_t ucline, char attribute, u8_t idx, char *par)
{
    char *p ="";
    char szstr[32];

    p = find_string(attribute,(s32_t) idx);
    sprintf(szstr, p, par);
    lcd_str(ucline, szstr);
}

//select the value from the given index and display the sentence
void lcd_show_index(u8_t ucline, char attribute, u8_t idx)
{
    char *p="";
    char szstr[32];

    p = find_string(attribute,(s32_t) idx);
    sprintf(szstr, "%d. %s", idx, p);
    lcd_str(ucline, szstr);
}

//select the value from the given index and display the corresponding integer with the sentence
void lcd_show_index_int(u8_t ucline, char attribute, u8_t idx, s32_t par)
{
    char *p="";
    char szstr[32];

    p = find_string(attribute,(s32_t) idx);
    sprintf(szstr, "%d. ", idx);
    sprintf(szstr + 3, p, par);
    lcd_str(ucline, szstr);
}

//select the value from the given index and display the corresponding string with the sentence
void lcd_show_index_str(u8_t ucline, char attribute, u8_t idx, char *par)
{
    char *p="";
    char szstr[32];

    p = find_string(attribute,(s32_t) idx);

    sprintf(szstr, "%d. ", idx);
    sprintf(szstr + 3, p, par);
    lcd_str(ucline, szstr);
}

void lcd_dis(char a) //lcd display on and off
{
  if(a==1)
	{
      FIO0SET= LCD_W;
		  
  }
	if(a==0)
	{
		 FIO0CLR = LCD_W; 
  }

}



