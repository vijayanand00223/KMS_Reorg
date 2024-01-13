/**
 * @file igmp_snooping_misc.h
 * @brief Helper functions for IGMP snooping switch
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

#ifndef _IGMP_SNOOPING_MISC_H
#define _IGMP_SNOOPING_MISC_H

//Dependencies
#include "core/net.h"
#include "igmp/igmp_snooping.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//IGMP snooping related functions
void igmpSnoopingProcessMessage(IgmpSnoopingContext *context,
   Ipv4PseudoHeader *pseudoHeader, const IgmpMessage *message,
   size_t length, NetRxAncillary *ancillary);

void igmpSnoopingProcessMembershipQuery(IgmpSnoopingContext *context,
   Ipv4PseudoHeader *pseudoHeader, const IgmpMessage *message,
   size_t length, NetRxAncillary *ancillary);

void igmpSnoopingProcessMembershipReport(IgmpSnoopingContext *context,
   Ipv4PseudoHeader *pseudoHeader, const IgmpMessage *message,
   size_t length, NetRxAncillary *ancillary);

void igmpSnoopingProcessLeaveGroup(IgmpSnoopingContext *context,
   Ipv4PseudoHeader *pseudoHeader, const IgmpMessage *message,
   size_t length, NetRxAncillary *ancillary);

void igmpSnoopingProcessUnknownMessage(IgmpSnoopingContext *context,
   Ipv4PseudoHeader *pseudoHeader, const IgmpMessage *message,
   size_t length, NetRxAncillary *ancillary);

error_t igmpSnoopingForwardMessage(IgmpSnoopingContext *context,
   uint32_t forwardPorts, const MacAddr *destMacAddr,
   Ipv4PseudoHeader *pseudoHeader, const IgmpMessage *message,
   size_t length);

IgmpSnoopingGroup *igmpSnoopingCreateGroup(IgmpSnoopingContext *context,
   Ipv4Addr groupAddr, uint8_t port);

IgmpSnoopingGroup *igmpSnoopingFindGroup(IgmpSnoopingContext *context,
   Ipv4Addr groupAddr, uint8_t port);

void igmpSnoopingDeleteGroup(IgmpSnoopingContext *context,
   IgmpSnoopingGroup *group);

void igmpSnoopingEnableMonitoring(IgmpSnoopingContext *context, bool_t enable);

void igmpSnoopingUpdateStaticFdbEntry(IgmpSnoopingContext *context,
   Ipv4Addr groupAddr);

void igmpSnoopingSetUnknownMcastFwdPorts(IgmpSnoopingContext *context,
   bool_t enable, uint32_t forwardPorts);

uint32_t igmpSnoopingGetRouterPorts(IgmpSnoopingContext *context);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
