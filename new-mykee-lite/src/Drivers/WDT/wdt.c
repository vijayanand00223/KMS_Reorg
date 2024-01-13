#include "lpc23xx.h"
#include "wdt.h"
#include "menu_main.h"
#include "ticker_itrp.h"

void wdt_3S_Init(void)
{
	WDCLKSEL = 0U;					//WDSEL : Bits 0&1 - Internal RC Oscillator as WDT Clk source.
	WDTC = 9100000;					// 3 secs. Watchdog Time-out Interval
	WDMOD |= WDEN | WDRESET;
}
void wdt_2S_Init(void)
{
	WDCLKSEL = 0U;					//WDSEL : Bits 0&1 - Internal RC Oscillator as WDT Clk source.
	WDTC = 2100000;					// 2.1 secs. Watchdog Time-out Interval
	WDMOD |= WDEN | WDRESET;		//Watchdog Reset Mode: Operate with WDT Initerrupt and WDRESET Enabled
}
void wdt_reset_feed(void)
{
	WDFEED = 0xAA;
	WDFEED = 0x55;
}

