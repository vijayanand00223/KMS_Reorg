#include "lpc23xx.h"
#include "BUZZER.h"
#include "menu_main.h"
#include "ticker_itrp.h"

/******************************************************************************
 ** Function name:      buzzer
 **
 ** Descriptions:       sounds the buzzer for some time.
 **
 ** parameters:          time=>indicates howmuch time buzzer should be ON.
 ** Returned value:      None
 ** 
 ******************************************************************************/
void buzzer(s32_t time)
{
	s32_t inew_time;

    inew_time = 3 * time;
	if(sys_t.alarm_enable != 2)		//R&D_STERNA Alarm Disable checking
	{
    sound_buzzer = inew_time;
	}
	else
	{
		sound_buzzer = 0;		//R&D_STERNA - Sound disable
	}
}

void buzzer_On(void)
{
	FIO2SET = BUZZER;
}
void buzzer_Off(void)
{
	FIO2CLR = BUZZER;
}


