/**
 * @file ipv6_routing.h
 * @brief IPv6 routing
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

#ifndef _IPV6_ROUTING_H
#define _IPV6_ROUTING_H

//Dependencies
#include "core/net.h"
#include "ipv6/ipv6.h"

//IPv6 routing support
#ifndef IPV6_ROUTING_SUPPORT
   #define IPV6_ROUTING_SUPPORT DISABLED
#elif (IPV6_ROUTING_SUPPORT != ENABLED && IPV6_ROUTING_SUPPORT != DISABLED)
   #error IPV6_ROUTING_SUPPORT parameter is not valid
#endif

//Size of the IPv6 routing table
#ifndef IPV6_ROUTING_TABLE_SIZE
   #define IPV6_ROUTING_TABLE_SIZE 8
#elif (IPV6_ROUTING_TABLE_SIZE < 1)
   #error IPV6_ROUTING_TABLE_SIZE parameter is not valid
#endif

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Routing table entry
 **/

typedef struct
{
   bool_t valid;            ///<Valid entry
   Ipv6Addr prefix;         ///<Destination
   uint_t prefixLen;        ///<IPv6 prefix length
   NetInterface *interface; ///<Outgoing network interface
   Ipv6Addr nextHop;        ///<Next hop
   uint_t metric;           ///<Metric value
} Ipv6RoutingTableEntry;


//IPv6 routing related functions
error_t ipv6InitRouting(void);
error_t ipv6EnableRouting(NetInterface *interface, bool_t enable);

error_t ipv6AddRoute(const Ipv6Addr *prefix, uint_t prefixLen,
   NetInterface *interface, const Ipv6Addr *nextHop, uint_t metric);

error_t ipv6DeleteRoute(const Ipv6Addr *prefix, uint_t prefixLen);
error_t ipv6DeleteAllRoutes(void);

error_t ipv6ForwardPacket(NetInterface *srcInterface, NetBuffer *ipPacket,
   size_t ipPacketOffset);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
