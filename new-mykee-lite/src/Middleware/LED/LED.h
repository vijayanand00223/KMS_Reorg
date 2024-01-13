#ifndef _LED_H_
#define _LED_H_

#include "ticker_itrp.h"
#include "menu_main.h"

extern u8_t green_led[MAX_KEYS];
extern u8_t red_led[MAX_KEYS];

u8_t red_led_onoff(u8_t flag);
void greenled_onoff(u8_t flag);

#endif

