/**
 * @file ipv4_routing.h
 * @brief IPv4 routing
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

#ifndef _IPV4_ROUTING_H
#define _IPV4_ROUTING_H

//Dependencies
#include "core/net.h"
#include "ipv4/ipv4.h"

//IPv4 routing support
#ifndef IPV4_ROUTING_SUPPORT
   #define IPV4_ROUTING_SUPPORT DISABLED
#elif (IPV4_ROUTING_SUPPORT != ENABLED && IPV4_ROUTING_SUPPORT != DISABLED)
   #error IPV4_ROUTING_SUPPORT parameter is not valid
#endif

//Size of the IPv4 routing table
#ifndef IPV4_ROUTING_TABLE_SIZE
   #define IPV4_ROUTING_TABLE_SIZE 8
#elif (IPV4_ROUTING_TABLE_SIZE < 1)
   #error IPV4_ROUTING_TABLE_SIZE parameter is not valid
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
   Ipv4Addr networkDest;    ///<Network destination
   Ipv4Addr networkMask;    ///<Subnet mask for this route
   NetInterface *interface; ///<Outgoing network interface
   Ipv4Addr nextHop;        ///<Next hop
   uint_t metric;           ///<Metric value
} Ipv4RoutingTableEntry;


//IPv4 routing related functions
error_t ipv4InitRouting(void);
error_t ipv4EnableRouting(NetInterface *interface, bool_t enable);

error_t ipv4AddRoute(Ipv4Addr networkDest, Ipv4Addr networkMask,
   NetInterface *interface, Ipv4Addr nextHop, uint_t metric);

error_t ipv4DeleteRoute(Ipv4Addr networkDest, Ipv4Addr networkMask);
error_t ipv4DeleteAllRoutes(void);

error_t ipv4ForwardPacket(NetInterface *srcInterface, const NetBuffer *ipPacket,
   size_t ipPacketOffset);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
