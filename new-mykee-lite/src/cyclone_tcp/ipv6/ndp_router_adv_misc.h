/**
 * @file ndp_router_adv_misc.h
 * @brief Helper functions for router advertisement service
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

#ifndef _NDP_ROUTER_ADV_MISC_H
#define _NDP_ROUTER_ADV_MISC_H

//Dependencies
#include "core/net.h"
#include "ipv6/ndp_router_adv.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//Tick counter to handle periodic operations
extern systime_t ndpRouterAdvTickCounter;

//RA service related functions
void ndpRouterAdvTick(NdpRouterAdvContext *context);
void ndpRouterAdvLinkChangeEvent(NdpRouterAdvContext *context);

void ndpProcessRouterSol(NetInterface *interface, Ipv6PseudoHeader *pseudoHeader,
   const NetBuffer *buffer, size_t offset, uint8_t hopLimit);

error_t ndpSendRouterAdv(NdpRouterAdvContext *context, uint16_t routerLifetime);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
