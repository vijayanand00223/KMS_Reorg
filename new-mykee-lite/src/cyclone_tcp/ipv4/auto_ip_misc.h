/**
 * @file auto_ip_misc.h
 * @brief Helper functions for Auto-IP
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

#ifndef _AUTO_IP_MISC_H
#define _AUTO_IP_MISC_H

//Dependencies
#include "core/net.h"
#include "ipv4/auto_ip.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//Tick counter to handle periodic operations
extern systime_t autoIpTickCounter;

//Auto-IP related functions
void autoIpTick(AutoIpContext *context);
void autoIpLinkChangeEvent(AutoIpContext *context);

void autoIpChangeState(AutoIpContext *context,
   AutoIpState newState, systime_t delay);

void autoIpGenerateAddr(Ipv4Addr *ipAddr);

void autoIpResetConfig(AutoIpContext *context);
void autoIpDumpConfig(AutoIpContext *context);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
