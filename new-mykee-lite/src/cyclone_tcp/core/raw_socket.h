/**
 * @file raw_socket.h
 * @brief TCP/IP raw sockets
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

#ifndef _RAW_SOCKET_H
#define _RAW_SOCKET_H

//Dependencies
#include "core/net.h"
#include "core/ip.h"
#include "core/socket.h"

//Raw socket support
#ifndef RAW_SOCKET_SUPPORT
   #define RAW_SOCKET_SUPPORT DISABLED
#elif (RAW_SOCKET_SUPPORT != ENABLED && RAW_SOCKET_SUPPORT != DISABLED)
   #error RAW_SOCKET_SUPPORT parameter is not valid
#endif

//Receive queue depth for raw sockets
#ifndef RAW_SOCKET_RX_QUEUE_SIZE
   #define RAW_SOCKET_RX_QUEUE_SIZE 4
#elif (RAW_SOCKET_RX_QUEUE_SIZE < 1)
   #error RAW_SOCKET_RX_QUEUE_SIZE parameter is not valid
#endif

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//Raw socket related functions
error_t rawSocketProcessIpPacket(NetInterface *interface,
   IpPseudoHeader *pseudoHeader, const NetBuffer *buffer, size_t offset,
   NetRxAncillary *ancillary);

void rawSocketProcessEthPacket(NetInterface *interface, EthHeader *header,
   const uint8_t *data, size_t length, NetRxAncillary *ancillary);

error_t rawSocketSendIpPacket(Socket *socket, const SocketMsg *message,
   uint_t flags);

error_t rawSocketSendEthPacket(Socket *socket, const SocketMsg *message,
   uint_t flags);

error_t rawSocketReceiveIpPacket(Socket *socket, SocketMsg *message,
   uint_t flags);

error_t rawSocketReceiveEthPacket(Socket *socket, SocketMsg *message,
   uint_t flags);

void rawSocketUpdateEvents(Socket *socket);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
