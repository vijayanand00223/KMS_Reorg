/**
 * @file ndp_misc.h
 * @brief Helper functions for NDP (Neighbor Discovery Protocol)
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

#ifndef _NDP_MISC_H
#define _NDP_MISC_H

//Dependencies
#include "core/net.h"
#include "ipv6/ndp.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//NDP related functions
void ndpParsePrefixInfoOption(NetInterface *interface, NdpPrefixInfoOption *option);

void ndpUpdateAddrList(NetInterface *interface);
void ndpUpdatePrefixList(NetInterface *interface);
void ndpUpdateDefaultRouterList(NetInterface *interface);

error_t ndpSelectDefaultRouter(NetInterface *interface,
   const Ipv6Addr *unreachableAddr, Ipv6Addr *addr);

bool_t ndpIsFirstHopRouter(NetInterface *interface,
   const Ipv6Addr *destAddr, const Ipv6Addr *nextHop);

error_t ndpSelectNextHop(NetInterface *interface, const Ipv6Addr *destAddr,
   const Ipv6Addr *unreachableNextHop, Ipv6Addr *nextHop, bool_t dontRoute);

void ndpUpdateNextHop(NetInterface *interface, const Ipv6Addr *nextHop);

void ndpAddOption(void *message, size_t *messageLen, uint8_t type,
   const void *value, size_t length);

void *ndpGetOption(uint8_t *options, size_t length, uint8_t type);

error_t ndpCheckOptions(const uint8_t *options, size_t length);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
