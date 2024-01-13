/**
 * @file dhcp_client_misc.h
 * @brief Helper functions for DHCP client
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

#ifndef _DHCP_CLIENT_MISC_H
#define _DHCP_CLIENT_MISC_H

//Dependencies
#include "core/net.h"
#include "dhcp/dhcp_client.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//Tick counter to handle periodic operations
extern systime_t dhcpClientTickCounter;

//DHCP client related functions
void dhcpClientTick(DhcpClientContext *context);
void dhcpClientLinkChangeEvent(DhcpClientContext *context);

error_t dhcpClientSendDiscover(DhcpClientContext *context);
error_t dhcpClientSendRequest(DhcpClientContext *context);
error_t dhcpClientSendDecline(DhcpClientContext *context);

void dhcpClientProcessMessage(NetInterface *interface,
   const IpPseudoHeader *pseudoHeader, const UdpHeader *udpHeader,
   const NetBuffer *buffer, size_t offset, const NetRxAncillary *ancillary,
   void *param);

void dhcpClientParseOffer(DhcpClientContext *context,
   const DhcpMessage *message, size_t length);

void dhcpClientParseAck(DhcpClientContext *context,
   const DhcpMessage *message, size_t length);

void dhcpClientParseNak(DhcpClientContext *context,
   const DhcpMessage *message, size_t length);

void dhcpClientCheckTimeout(DhcpClientContext *context);

uint16_t dhcpClientComputeElapsedTime(DhcpClientContext *context);

void dhcpClientChangeState(DhcpClientContext *context,
   DhcpState newState, systime_t delay);

void dhcpClientResetConfig(DhcpClientContext *context);
void dhcpClientDumpConfig(DhcpClientContext *context);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
