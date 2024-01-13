#ifndef _KEYPAD_H_
#define _KEYPAD_H_

#include "ticker_itrp.h"

#define KBD_E       (1 << 6)

u32_t kbd_byte(u32_t uicol1);
void lcd_keypad();
#endif

