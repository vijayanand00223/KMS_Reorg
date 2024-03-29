/**
 * @file date_time.h
 * @brief Date and time management
 *
 * @section License
 *
 * Copyright (C) 2010-2021 Oryx Embedded SARL. All rights reserved.
 *
 * This file is part of CycloneTCP Ultimate.
 *
 * This software is provided under a commercial license. You may
 * use this software under the conditions stated in the license
 * terms. This source code cannot be redistributed.
 *
 * @author Oryx Embedded SARL (www.oryx-embedded.com)
 * @version 2.1.2
 **/

#ifndef _DATE_TIME_H
#define _DATE_TIME_H

//Dependencies
#include <time.h>
#include "os_port.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Date and time representation
 **/

typedef struct
{
   uint16_t year;
   uint8_t month;
   uint8_t day;
   uint8_t dayOfWeek;
   uint8_t hours;
   uint8_t minutes;
   uint8_t seconds;
   uint16_t milliseconds;
} DateTime;


//Date and time management
const char_t *formatSystemTime(systime_t time, char_t *str);
const char_t *formatDate(const DateTime *date, char_t *str);

void getCurrentDate(DateTime *date);
time_t getCurrentUnixTime(void);

void convertUnixTimeToDate(time_t t, DateTime *date);
time_t convertDateToUnixTime(const DateTime *date);

int_t compareDateTime(const DateTime *date1, const DateTime *date2);

uint8_t computeDayOfWeek(uint16_t y, uint8_t m, uint8_t d);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
