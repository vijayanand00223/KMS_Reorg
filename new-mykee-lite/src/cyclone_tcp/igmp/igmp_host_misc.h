/**
 * @file igmp_host_misc.h
 * @brief Helper functions for IGMP host
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

#ifndef _IGMP_HOST_MISC_H
#define _IGMP_HOST_MISC_H

//Dependencies
#include "core/net.h"
#include "igmp/igmp_host.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//IGMP host related functions
error_t igmpHostSendMembershipReport(NetInterface *interface, Ipv4Addr ipAddr);
error_t igmpHostSendLeaveGroup(NetInterface *interface, Ipv4Addr ipAddr);

void igmpHostProcessMessage(NetInterface *interface,
   const IgmpMessage *message, size_t length);

void igmpHostProcessMembershipQuery(NetInterface *interface,
   const IgmpMessage *message, size_t length);

void igmpHostProcessMembershipReport(NetInterface *interface,
   const IgmpMessage *message, size_t length);

uint32_t igmpRand(uint32_t max);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
