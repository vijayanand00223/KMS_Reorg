/**
 * @file llmnr_client.h
 * @brief LLMNR client (Link-Local Multicast Name Resolution)
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

#ifndef _LLMNR_CLIENT_H
#define _LLMNR_CLIENT_H

//Dependencies
#include "core/net.h"
#include "core/socket.h"
#include "core/udp.h"
#include "dns/dns_cache.h"
#include "dns/dns_common.h"
#include "llmnr/llmnr_common.h"

//LLMNR client support
#ifndef LLMNR_CLIENT_SUPPORT
   #define LLMNR_CLIENT_SUPPORT DISABLED
#elif (LLMNR_CLIENT_SUPPORT != ENABLED && LLMNR_CLIENT_SUPPORT != DISABLED)
   #error LLMNR_CLIENT_SUPPORT parameter is not valid
#endif

//Maximum number of retransmissions of LLMNR queries
#ifndef LLMNR_CLIENT_MAX_RETRIES
   #define LLMNR_CLIENT_MAX_RETRIES 3
#elif (LLMNR_CLIENT_MAX_RETRIES < 1)
   #error LLMNR_CLIENT_MAX_RETRIES parameter is not valid
#endif

//Initial retransmission timeout
#ifndef LLMNR_CLIENT_INIT_TIMEOUT
   #define LLMNR_CLIENT_INIT_TIMEOUT 1000
#elif (LLMNR_CLIENT_INIT_TIMEOUT < 1000)
   #error LLMNR_CLIENT_INIT_TIMEOUT parameter is not valid
#endif

//Maximum retransmission timeout
#ifndef LLMNR_CLIENT_MAX_TIMEOUT
   #define LLMNR_CLIENT_MAX_TIMEOUT 1000
#elif (LLMNR_CLIENT_MAX_TIMEOUT < 1000)
   #error LLMNR_CLIENT_MAX_TIMEOUT parameter is not valid
#endif

//Maximum cache lifetime for LLMNR entries
#ifndef LLMNR_MAX_LIFETIME
   #define LLMNR_MAX_LIFETIME 60000
#elif (LLMNR_MAX_LIFETIME < 1000)
   #error LLMNR_MAX_LIFETIME parameter is not valid
#endif

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//LLMNR related functions
error_t llmnrResolve(NetInterface *interface, const char_t *name,
   HostType type, IpAddr *ipAddr);

error_t llmnrSendQuery(DnsCacheEntry *entry);

void llmnrProcessResponse(NetInterface *interface,
   const IpPseudoHeader *pseudoHeader, const UdpHeader *udpHeader,
   const NetBuffer *buffer, size_t offset, const NetRxAncillary *ancillary,
   void *param);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
