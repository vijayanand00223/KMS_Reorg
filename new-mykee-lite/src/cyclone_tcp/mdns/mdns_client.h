/**
 * @file mdns_client.h
 * @brief mDNS client (Multicast DNS)
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

#ifndef _MDNS_CLIENT_H
#define _MDNS_CLIENT_H

//Dependencies
#include "core/net.h"
#include "core/socket.h"
#include "core/udp.h"
#include "dns/dns_cache.h"
#include "dns/dns_common.h"

//mDNS client support
#ifndef MDNS_CLIENT_SUPPORT
   #define MDNS_CLIENT_SUPPORT DISABLED
#elif (MDNS_CLIENT_SUPPORT != ENABLED && MDNS_CLIENT_SUPPORT != DISABLED)
   #error MDNS_CLIENT_SUPPORT parameter is not valid
#endif

//Maximum number of retransmissions of mDNS queries
#ifndef MDNS_CLIENT_MAX_RETRIES
   #define MDNS_CLIENT_MAX_RETRIES 3
#elif (MDNS_CLIENT_MAX_RETRIES < 1)
   #error MDNS_CLIENT_MAX_RETRIES parameter is not valid
#endif

//Initial retransmission timeout
#ifndef MDNS_CLIENT_INIT_TIMEOUT
   #define MDNS_CLIENT_INIT_TIMEOUT 1000
#elif (MDNS_CLIENT_INIT_TIMEOUT < 1000)
   #error MDNS_CLIENT_INIT_TIMEOUT parameter is not valid
#endif

//Maximum retransmission timeout
#ifndef MDNS_CLIENT_MAX_TIMEOUT
   #define MDNS_CLIENT_MAX_TIMEOUT 1000
#elif (MDNS_CLIENT_MAX_TIMEOUT < 1000)
   #error MDNS_CLIENT_MAX_TIMEOUT parameter is not valid
#endif

//Maximum cache lifetime for mDNS entries
#ifndef MDNS_MAX_LIFETIME
   #define MDNS_MAX_LIFETIME 60000
#elif (MDNS_MAX_LIFETIME < 1000)
   #error MDNS_MAX_LIFETIME parameter is not valid
#endif

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//mDNS related functions
error_t mdnsClientResolve(NetInterface *interface, const char_t *name,
   HostType type, IpAddr *ipAddr);

error_t mdnsClientSendQuery(DnsCacheEntry *entry);

void mdnsClientParseAnRecord(NetInterface *interface,
   const MdnsMessage *message, size_t offset, const DnsResourceRecord *record);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
