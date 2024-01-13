/**
 * @file loopback_driver.h
 * @brief Loopback interface driver
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

#ifndef _LOOPBACK_DRIVER_H
#define _LOOPBACK_DRIVER_H

//Dependencies
#include "core/nic.h"

//Queue size
#ifndef LOOPBACK_DRIVER_QUEUE_SIZE
   #define LOOPBACK_DRIVER_QUEUE_SIZE 6
#elif (LOOPBACK_DRIVER_QUEUE_SIZE < 1)
   #error LOOPBACK_DRIVER_QUEUE_SIZE parameter is not valid
#endif


/**
 * @brief Loopback interface queue entry
 **/

typedef struct
{
   size_t length;
   uint8_t data[ETH_MTU];
} LoopbackDriverQueueEntry;


//Loopback interface driver
extern const NicDriver loopbackDriver;

//Loopback interface related functions
error_t loopbackDriverInit(NetInterface *interface);

void loopbackDriverTick(NetInterface *interface);

void loopbackDriverEnableIrq(NetInterface *interface);
void loopbackDriverDisableIrq(NetInterface *interface);
void loopbackDriverEventHandler(NetInterface *interface);

error_t loopbackDriverSendPacket(NetInterface *interface,
   const NetBuffer *buffer, size_t offset, NetTxAncillary *ancillary);

error_t loopbackDriverReceivePacket(NetInterface *interface);

error_t loopbackDriverUpdateMacAddrFilter(NetInterface *interface);

#endif
