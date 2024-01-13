/**
 * @file web_socket_transport.h
 * @brief WebSocket transport layer
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

#ifndef _WEB_SOCKET_TRANSPORT_H
#define _WEB_SOCKET_TRANSPORT_H

//Dependencies
#include "core/net.h"
#include "web_socket/web_socket.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//WebSocket related functions
error_t webSocketOpenConnection(WebSocket *webSocket);

error_t webSocketEstablishConnection(WebSocket *webSocket,
   const IpAddr *serverIpAddr, uint16_t serverPort);

error_t webSocketShutdownConnection(WebSocket *webSocket);
void webSocketCloseConnection(WebSocket *webSocket);

error_t webSocketSendData(WebSocket *webSocket, const void *data,
   size_t length, size_t *written, uint_t flags);

error_t webSocketReceiveData(WebSocket *webSocket, void *data,
   size_t size, size_t *received, uint_t flags);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
