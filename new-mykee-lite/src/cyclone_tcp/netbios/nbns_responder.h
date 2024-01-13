/**
 * @file nbns_responder.h
 * @brief NBNS responder (NetBIOS Name Service)
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

#ifndef _NBNS_RESPONDER_H
#define _NBNS_RESPONDER_H

//Dependencies
#include "core/net.h"
#include "core/udp.h"
#include "dns/dns_common.h"
#include "netbios/nbns_common.h"

//NBNS responder support
#ifndef NBNS_RESPONDER_SUPPORT
   #define NBNS_RESPONDER_SUPPORT ENABLED
#elif (NBNS_RESPONDER_SUPPORT != ENABLED && NBNS_RESPONDER_SUPPORT != DISABLED)
   #error NBNS_RESPONDER_SUPPORT parameter is not valid
#endif

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//NBNS related functions
void nbnsProcessQuery(NetInterface *interface, const Ipv4PseudoHeader *pseudoHeader,
   const UdpHeader *udpHeader, const NbnsHeader *message, size_t length);

error_t nbnsSendResponse(NetInterface *interface,
   const IpAddr *destIpAddr, uint16_t destPort, uint16_t id);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
