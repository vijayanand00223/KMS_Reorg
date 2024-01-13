/**
 * @file dhcpv6_client_fsm.h
 * @brief DHCPv6 client finite state machine
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

#ifndef _DHCPV6_CLIENT_FSM_H
#define _DHCPV6_CLIENT_FSM_H

//Dependencies
#include "core/net.h"
#include "dhcpv6/dhcpv6_client.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//DHCPv6 client related functions
void dhcpv6ClientStateInit(Dhcpv6ClientContext *context);
void dhcpv6ClientStateSolicit(Dhcpv6ClientContext *context);
void dhcpv6ClientStateRequest(Dhcpv6ClientContext *context);
void dhcpv6ClientStateInitConfirm(Dhcpv6ClientContext *context);
void dhcpv6ClientStateConfirm(Dhcpv6ClientContext *context);
void dhcpv6ClientStateDad(Dhcpv6ClientContext *context);
void dhcpv6ClientStateBound(Dhcpv6ClientContext *context);
void dhcpv6ClientStateRenew(Dhcpv6ClientContext *context);
void dhcpv6ClientStateRebind(Dhcpv6ClientContext *context);
void dhcpv6ClientStateRelease(Dhcpv6ClientContext *context);
void dhcpv6ClientStateDecline(Dhcpv6ClientContext *context);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
