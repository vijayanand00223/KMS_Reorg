/**
 * @file igmp_router_misc.h
 * @brief Helper functions fore IGMP router
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

#ifndef _IGMP_ROUTER_MISC_H
#define _IGMP_ROUTER_MISC_H

//Dependencies
#include "core/net.h"
#include "igmp/igmp_router.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//IGMP router related functions
error_t igmpRouterSendGeneralQuery(IgmpRouterContext *context);

error_t igmpRouterSendGroupSpecificQuery(IgmpRouterContext *context,
   Ipv4Addr groupAddr);

error_t igmpRouterSendMembershipQuery(IgmpRouterContext *context,
   Ipv4Addr destAddr, Ipv4Addr groupAddr, systime_t maxRespTime);

void igmpRouterProcessMessage(IgmpRouterContext *context,
   Ipv4PseudoHeader *pseudoHeader, const IgmpMessage *message, size_t length);

void igmpRouterProcessMembershipQuery(IgmpRouterContext *context,
   Ipv4PseudoHeader *pseudoHeader, const IgmpMessage *message, size_t length);

void igmpRouterProcessMembershipReport(IgmpRouterContext *context,
   Ipv4PseudoHeader *pseudoHeader, const IgmpMessage *message, size_t length);

void igmpRouterProcessLeaveGroup(IgmpRouterContext *context,
   Ipv4PseudoHeader *pseudoHeader, const IgmpMessage *message, size_t length);

IgmpRouterGroup *igmpRouterCreateGroup(IgmpRouterContext *context,
   Ipv4Addr groupAddr);

IgmpRouterGroup *igmpRouterFindGroup(IgmpRouterContext *context,
   Ipv4Addr groupAddr);

void igmpRouterDeleteGroup(IgmpRouterContext *context, IgmpRouterGroup *group);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
