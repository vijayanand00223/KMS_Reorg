/**
 * @file ipv6_pmtu.h
 * @brief Path MTU Discovery for IPv6
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

#ifndef _IPV6_PMTU_H
#define _IPV6_PMTU_H

//Dependencies
#include "core/net.h"

//Path MTU discovery support
#ifndef IPV6_PMTU_SUPPORT
   #define IPV6_PMTU_SUPPORT ENABLED
#elif (IPV6_PMTU_SUPPORT != ENABLED && IPV6_PMTU_SUPPORT != DISABLED)
   #error IPV6_PMTU_SUPPORT parameter is not valid
#endif

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//Path MTU discovery related functions
size_t ipv6GetPathMtu(NetInterface *interface, const Ipv6Addr *destAddr);

void ipv6UpdatePathMtu(NetInterface *interface,
   const Ipv6Addr *destAddr, size_t tentativePathMtu);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
