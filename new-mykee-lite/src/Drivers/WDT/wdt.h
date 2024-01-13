#ifndef _WDT_H_
#define _WDT_H_

#include "ticker_itrp.h"

/*Watchdog Mode Register - Bit Description*/
#define WDEN		(1<<0)
#define WDRESET		(1<<1)
#define WDTOF		(1<<2)
#define WDINT		(1<<3)

extern u8_t rstflag;

void wdt_3S_Init(void);
void wdt_2S_Init(void);
void wdt_reset(void);

#endif

