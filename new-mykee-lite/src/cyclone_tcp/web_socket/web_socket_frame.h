/**
 * @file web_socket_frame.h
 * @brief WebSocket frame parsing and formatting
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

#ifndef _WEB_SOCKET_FRAME_H
#define _WEB_SOCKET_FRAME_H

//Dependencies
#include "core/net.h"
#include "web_socket/web_socket.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//WebSocket related functions
error_t webSocketFormatFrameHeader(WebSocket *webSocket,
   bool_t fin, WebSocketFrameType type, size_t payloadLen);

error_t webSocketParseFrameHeader(WebSocket *webSocket,
   const WebSocketFrame *frame, WebSocketFrameType *type);

error_t webSocketFormatCloseFrame(WebSocket *webSocket);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
