#ifndef _LCD_H_
#define _LCD_H_

#include "ticker_itrp.h"

#define LCD_E       (1 << 18)
#define LCD_RS      (1 << 22)
#define LCD_RW      (1 << 23)
#define LCD_E1      (1 << 7)
#define LCD_E2      (1 << 8)
#define LCD_W       (1 << 9)
#define LCD_DMASK   (0xFF << 24)
#define LCD_DSHIFT  (24)

enum LCD_PORTS
{
    LCD_CONTROL,
    LCD_DATA
};

extern int glcd_x;
extern int glcd_y;
extern volatile u32_t lcd_changed[2];
extern char lcd_string[2][32];

void lcd_byte(u32_t reg, u32_t info);
void lcd_init(void);
void lcd_ch(u32_t uiline1, u32_t uix, u32_t uiinfo);
void lcd_char(char ch);
void lcd_back(s8_t scamount);
void lcd_forward(s8_t scamount);
void lcd_delete(s8_t scamount);
void lcd_str(u8_t ucline, char *p);
void lcd_center(u8_t ucline, char *p);
void lcd_clr(u8_t ucline);
char *find_string(char attribute, s32_t idx);
void lcd_show(u8_t ucline, char attribute, u8_t idx);
void lcd_show_int(u8_t ucline, u32_t uipar);
void lcd_show_str(u8_t ucline, char attribute, u8_t idx, char *par);
void lcd_show_index(u8_t ucline, char attribute, u8_t idx);
void lcd_show_index_int(u8_t ucline, char attribute, u8_t idx, s32_t par);
void lcd_show_index_str(u8_t ucline, char attribute, u8_t idx, char *par);
void lcd_dis(char a);
#endif
