#ifndef _RFID_H_
#define _RFID_H_

#include "ticker_itrp.h"

extern volatile int   card_id_available;
extern volatile u32_t card_id;

int regular_card(void);

#endif

