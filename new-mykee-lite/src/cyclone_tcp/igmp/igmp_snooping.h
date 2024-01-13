/**
 * @file igmp_snooping.h
 * @brief IGMP snooping switch
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

#ifndef _IGMP_SNOOPING_H
#define _IGMP_SNOOPING_H

//Dependencies
#include "core/net.h"
#include "igmp/igmp_common.h"

//IGMP snooping support
#ifndef IGMP_SNOOPING_SUPPORT
   #define IGMP_SNOOPING_SUPPORT DISABLED
#elif (IGMP_SNOOPING_SUPPORT != ENABLED && IGMP_SNOOPING_SUPPORT != DISABLED)
   #error IGMP_SNOOPING_SUPPORT parameter is not valid
#endif

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Multicast group states
 **/

typedef enum
{
   IGMP_SNOOPING_GROUP_STATE_NO_MEMBERS_PRESENT  = 0,
   IGMP_SNOOPING_GROUP_STATE_MEMBERS_PRESENT     = 1,
   IGMP_SNOOPING_GROUP_STATE_CHECKING_MEMBERSHIP = 2
} IgmpSnoopingGroupState;


/**
 * @brief Snooping switch port
 **/

typedef struct
{
   bool_t routerPresent;
   NetTimer timer;
} IgmpSnoopingPort;


/**
 * @brief Multicast group
 **/

typedef struct
{
   IgmpSnoopingGroupState state; ///<Multicast group state
   Ipv4Addr addr;                ///<Multicast group address
   uint8_t port;
   NetTimer timer;
} IgmpSnoopingGroup;


/**
 * @brief IGMP snooping settings
 **/

typedef struct
{
   NetInterface *interface;             ///<Underlying network interface
   uint_t numPorts;                     ///<Number of ports
   IgmpSnoopingPort *ports;             ///<Ports
   uint_t numGroups;                    ///<Maximum number of multicast groups
   IgmpSnoopingGroup *groups;           ///<Multicast groups
   bool_t floodReports;                 ///<Flood IGMP report messages to all ports (not only to router ports)
   bool_t floodUnknownMulticastPackets; ///<Flood unregistered multicast traffic to all ports
   systime_t lastMemberQueryTime;       ///<Leave latency
} IgmpSnoopingSettings;


/**
 * @brief IGMP snooping switch context
 **/

typedef struct
{
   bool_t running;
   NetInterface *interface;             ///<The primary interface on an attached network
   uint_t numPorts;                     ///<Number of ports
   IgmpSnoopingPort *ports;             ///<Ports
   uint_t numGroups;                    ///<Maximum number of multicast groups
   IgmpSnoopingGroup *groups;           ///<Multicast groups
   bool_t floodReports;                 ///<Flood IGMP report messages to all ports (not only to router ports)
   bool_t floodUnknownMulticastPackets; ///<Flood unregistered multicast traffic to all ports
   systime_t lastMemberQueryTime;       ///<Leave latency
} IgmpSnoopingContext;


//IGMP snooping related functions
void igmpSnoopingGetDefaultSettings(IgmpSnoopingSettings *settings);

error_t igmpSnoopingInit(IgmpSnoopingContext *context,
   const IgmpSnoopingSettings *settings);

error_t igmpSnoopingStart(IgmpSnoopingContext *context);
error_t igmpSnoopingStop(IgmpSnoopingContext *context);

void igmpSnoopingTick(IgmpSnoopingContext *context);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
