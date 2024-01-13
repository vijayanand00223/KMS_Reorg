#ifndef _BUZZER_H_
#define _BUZZER_H_

#include "ticker_itrp.h"

#define SOUND
#define BUZZER      (1 << 3)

extern volatile u32_t sound_buzzer;

void buzzer(s32_t time);
void buzzer_On(void);
void buzzer_Off(void);

#endif

