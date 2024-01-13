#ifndef _DOOR_H_
#define _DOOR_H_

#include "ticker_itrp.h"

#define KBD_E       (1 << 6)

void relay(s32_t num, s32_t state);
int  trg(s32_t num);
void lock_on(char a);

#endif

