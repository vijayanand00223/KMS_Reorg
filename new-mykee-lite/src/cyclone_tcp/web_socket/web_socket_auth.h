/**
 * @file web_socket_auth.h
 * @brief HTTP authentication for WebSockets
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

#ifndef _WEB_SOCKET_AUTH_H
#define _WEB_SOCKET_AUTH_H

//Dependencies
#include "core/net.h"
#include "web_socket/web_socket.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//WebSocket related functions
error_t webSocketParseAuthenticateField(WebSocket *webSocket, char_t *value);

size_t webSocketAddAuthorizationField(WebSocket *webSocket, char_t *output);

void webSocketConvertArrayToHexString(const uint8_t *input,
   size_t inputLen, char_t *output);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
