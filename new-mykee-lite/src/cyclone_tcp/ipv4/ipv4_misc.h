/**
 * @file ipv4_misc.h
 * @brief Helper functions for IPv4
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

#ifndef _IPV4_MISC_H
#define _IPV4_MISC_H

//Dependencies
#include <string.h>
#include "core/net.h"
#include "ipv4/ipv4.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//IPv4 related functions
error_t ipv4AddRouterAlertOption(NetBuffer *buffer, size_t *offset);

error_t ipv4CheckSourceAddr(NetInterface *interface, Ipv4Addr ipAddr);
error_t ipv4CheckDestAddr(NetInterface *interface, Ipv4Addr ipAddr);

error_t ipv4SelectSourceAddr(NetInterface **interface,
   Ipv4Addr destAddr, Ipv4Addr *srcAddr);

error_t ipv4SelectDefaultGateway(NetInterface *interface, Ipv4Addr srcAddr,
   Ipv4Addr *defaultGatewayAddr);

bool_t ipv4IsOnLink(NetInterface *interface, Ipv4Addr ipAddr);
bool_t ipv4IsBroadcastAddr(NetInterface *interface, Ipv4Addr ipAddr);
bool_t ipv4IsTentativeAddr(NetInterface *interface, Ipv4Addr ipAddr);
bool_t ipv4IsLocalHostAddr(Ipv4Addr ipAddr);

uint_t ipv4GetAddrScope(Ipv4Addr ipAddr);
uint_t ipv4GetPrefixLength(Ipv4Addr mask);

error_t ipv4GetBroadcastAddr(NetInterface *interface, Ipv4Addr *addr);
error_t ipv4MapMulticastAddrToMac(Ipv4Addr ipAddr, MacAddr *macAddr);

bool_t ipv4TrapIgmpPacket(Ipv4Header *header);

void ipv4UpdateInStats(NetInterface *interface, Ipv4Addr destIpAddr,
   size_t length);

void ipv4UpdateOutStats(NetInterface *interface, Ipv4Addr destIpAddr,
   size_t length);

void ipv4UpdateErrorStats(NetInterface *interface, error_t error);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
