/**
 * @file ndp_cache.h
 * @brief Neighbor and destination cache management
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

#ifndef _NDP_CACHE_H
#define _NDP_CACHE_H

//Dependencies
#include "core/net.h"
#include "ipv6/ndp.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//NDP related functions
NdpNeighborCacheEntry *ndpCreateNeighborCacheEntry(NetInterface *interface);
NdpNeighborCacheEntry *ndpFindNeighborCacheEntry(NetInterface *interface, const Ipv6Addr *ipAddr);

void ndpUpdateNeighborCache(NetInterface *interface);
void ndpFlushNeighborCache(NetInterface *interface);

uint_t ndpSendQueuedPackets(NetInterface *interface, NdpNeighborCacheEntry *entry);
void ndpFlushQueuedPackets(NetInterface *interface, NdpNeighborCacheEntry *entry);

NdpDestCacheEntry *ndpCreateDestCacheEntry(NetInterface *interface);
NdpDestCacheEntry *ndpFindDestCacheEntry(NetInterface *interface, const Ipv6Addr *destAddr);
void ndpFlushDestCache(NetInterface *interface);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
