/**
 * @file tcp_fsm.h
 * @brief TCP finite state machine
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

#ifndef _TCP_FSM_H
#define _TCP_FSM_H

//Dependencies
#include "core/tcp.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//TCP FSM related functions
void tcpProcessSegment(NetInterface *interface, IpPseudoHeader *pseudoHeader,
   const NetBuffer *buffer, size_t offset, NetRxAncillary *ancillary);

void tcpStateClosed(NetInterface *interface,
   IpPseudoHeader *pseudoHeader, TcpHeader *segment, size_t length);

void tcpStateListen(Socket *socket, NetInterface *interface,
   IpPseudoHeader *pseudoHeader, TcpHeader *segment, size_t length);

void tcpStateSynSent(Socket *socket, TcpHeader *segment, size_t length);

void tcpStateSynReceived(Socket *socket, TcpHeader *segment,
   const NetBuffer *buffer, size_t offset, size_t length);

void tcpStateEstablished(Socket *socket, TcpHeader *segment,
   const NetBuffer *buffer, size_t offset, size_t length);

void tcpStateCloseWait(Socket *socket, TcpHeader *segment, size_t length);

void tcpStateLastAck(Socket *socket, TcpHeader *segment, size_t length);

void tcpStateFinWait1(Socket *socket, TcpHeader *segment,
   const NetBuffer *buffer, size_t offset, size_t length);

void tcpStateFinWait2(Socket *socket, TcpHeader *segment,
   const NetBuffer *buffer, size_t offset, size_t length);

void tcpStateClosing(Socket *socket, TcpHeader *segment, size_t length);

void tcpStateTimeWait(Socket *socket, TcpHeader *segment, size_t length);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
