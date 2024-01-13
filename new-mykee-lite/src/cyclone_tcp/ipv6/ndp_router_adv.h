/**
 * @file ndp_router_adv.h
 * @brief Router advertisement service
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

#ifndef _NDP_ROUTER_ADV_H
#define _NDP_ROUTER_ADV_H

//Dependencies
#include "core/net.h"
#include "ipv6/ipv6.h"

//RA service support
#ifndef NDP_ROUTER_ADV_SUPPORT
   #define NDP_ROUTER_ADV_SUPPORT DISABLED
#elif (NDP_ROUTER_ADV_SUPPORT != ENABLED && NDP_ROUTER_ADV_SUPPORT != DISABLED)
   #error NDP_ROUTER_ADV_SUPPORT parameter is not valid
#endif

//RA service tick interval
#ifndef NDP_ROUTER_ADV_TICK_INTERVAL
   #define NDP_ROUTER_ADV_TICK_INTERVAL 100
#elif (NDP_ROUTER_ADV_TICK_INTERVAL < 10)
   #error NDP_ROUTER_ADV_TICK_INTERVAL parameter is not valid
#endif

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief IPv6 prefix information
 **/

typedef struct
{
   Ipv6Addr prefix;
   uint8_t length;
   bool_t onLinkFlag;
   bool_t autonomousFlag;
   uint32_t validLifetime;
   uint32_t preferredLifetime;
} NdpRouterAdvPrefixInfo;


/**
 * @brief Route information
 **/

typedef struct
{
   Ipv6Addr prefix;
   uint8_t length;
   uint8_t preference;
   uint32_t routeLifetime;
} NdpRouterAdvRouteInfo;


/**
 * @brief Context information for 6LoWPAN header compression
 **/

typedef struct
{
   uint8_t cid;
   Ipv6Addr prefix;
   uint8_t length;
   bool_t compression;
   uint16_t validLifetime;
} NdpRouterAdvContextInfo;


/**
 * @brief RA service settings
 **/

typedef struct
{
   NetInterface *interface;
   systime_t maxRtrAdvInterval;
   systime_t minRtrAdvInterval;
   uint8_t curHopLimit;
   bool_t managedFlag;
   bool_t otherConfigFlag;
   bool_t homeAgentFlag;
   uint8_t preference;
   bool_t proxyFlag;
   uint16_t defaultLifetime;
   uint32_t reachableTime;
   uint32_t retransTimer;
   uint32_t linkMtu;
   NdpRouterAdvPrefixInfo *prefixList;
   uint_t prefixListLength;
   NdpRouterAdvRouteInfo *routeList;
   uint_t routeListLength;
   NdpRouterAdvContextInfo *contextList;
   uint_t contextListLength;
} NdpRouterAdvSettings;


/**
 * @brief RA service context
 **/

typedef struct
{
   NdpRouterAdvSettings settings; ///<RA service settings
   bool_t running;                ///<This flag tells whether the RA service is running
   systime_t timestamp;           ///<Timestamp to manage retransmissions
   systime_t timeout;             ///<Timeout value
   uint_t routerAdvCount;         ///<Router Advertisement message counter
} NdpRouterAdvContext;


//Tick counter to handle periodic operations
extern systime_t ndpRouterAdvTickCounter;

//RA service related functions
void ndpRouterAdvGetDefaultSettings(NdpRouterAdvSettings *settings);

error_t ndpRouterAdvInit(NdpRouterAdvContext *context,
   const NdpRouterAdvSettings *settings);

error_t ndpRouterAdvStart(NdpRouterAdvContext *context);
error_t ndpRouterAdvStop(NdpRouterAdvContext *context);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
