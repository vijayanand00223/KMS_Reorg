/**
 * @file dhcp_client_fsm.h
 * @brief DHCP client finite state machine
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

#ifndef _DHCP_CLIENT_FSM_H
#define _DHCP_CLIENT_FSM_H

//Dependencies
#include "core/net.h"
#include "dhcp/dhcp_client.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//DHCP client related functions
void dhcpClientStateInit(DhcpClientContext *context);
void dhcpClientStateSelecting(DhcpClientContext *context);
void dhcpClientStateRequesting(DhcpClientContext *context);
void dhcpClientStateInitReboot(DhcpClientContext *context);
void dhcpClientStateRebooting(DhcpClientContext *context);
void dhcpClientStateProbing(DhcpClientContext *context);
void dhcpClientStateBound(DhcpClientContext *context);
void dhcpClientStateRenewing(DhcpClientContext *context);
void dhcpClientStateRebinding(DhcpClientContext *context);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
