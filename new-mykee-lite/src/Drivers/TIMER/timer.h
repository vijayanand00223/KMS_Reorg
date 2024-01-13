#ifndef _TIMER_H_
#define _TIMER_H_

#include "ticker_itrp.h"

extern volatile u32_t  Cnt_TriggerTimeout;
extern volatile u32_t KeyTask_TimeLimit[64]; 		//Added for Task Overdue R&D_STERNA
extern volatile u32_t TICKER_time;
extern volatile u32_t TICKER_1ms_time;		//Added for Task Overdue R&D_STERNA
extern volatile u32_t TICKER_25us_time;



void TICKER_Init(void);
void TICKER_Start(void);
void TICKER_Handler(void) __irq;
void TICKER_100ms_Init(void);
void TICKER_100ms_Start(void);
void TICKER_25us_Handler(void)__irq;
u32_t set_timeout(u32_t ticks);
s32_t chk_timeout(u32_t timeout);
u32_t Set_Trigger_Timeout(u32_t ticks);
s32_t Chk_Trigger_Timeout(u32_t timeout);

#endif

