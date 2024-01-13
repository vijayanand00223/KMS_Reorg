/**
 * @file llmnr_responder.h
 * @brief LLMNR responder (Link-Local Multicast Name Resolution)
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

#ifndef _LLMNR_RESPONDER_H
#define _LLMNR_RESPONDER_H

//Dependencies
#include "core/net.h"
#include "core/udp.h"
#include "dns/dns_common.h"
#include "llmnr/llmnr_common.h"

//LLMNR responder support
#ifndef LLMNR_RESPONDER_SUPPORT
   #define LLMNR_RESPONDER_SUPPORT DISABLED
#elif (LLMNR_RESPONDER_SUPPORT != ENABLED && LLMNR_RESPONDER_SUPPORT != DISABLED)
   #error LLMNR_RESPONDER_SUPPORT parameter is not valid
#endif

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//LLMNR related functions
error_t llmnrResponderInit(NetInterface *interface);

void llmnrProcessQuery(NetInterface *interface,
   const IpPseudoHeader *pseudoHeader, const UdpHeader *udpHeader,
   const NetBuffer *buffer, size_t offset, const NetRxAncillary *ancillary,
   void *param);

error_t llmnrSendResponse(NetInterface *interface, const IpAddr *destIpAddr,
   uint16_t destPort, uint16_t id, uint16_t qtype, uint16_t qclass);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
