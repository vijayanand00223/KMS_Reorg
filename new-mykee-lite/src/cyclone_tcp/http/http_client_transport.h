/**
 * @file http_client_transport.h
 * @brief Transport protocol abstraction layer
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

#ifndef _HTTP_CLIENT_TRANSPORT_H
#define _HTTP_CLIENT_TRANSPORT_H

//Dependencies
#include "core/net.h"
#include "http/http_client.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//HTTP client related functions
error_t httpClientOpenConnection(HttpClientContext *context);

error_t httpClientEstablishConnection(HttpClientContext *context,
   const IpAddr *serverIpAddr, uint16_t serverPort);

error_t httpClientShutdownConnection(HttpClientContext *context);
void httpClientCloseConnection(HttpClientContext *context);

error_t httpClientSendData(HttpClientContext *context, const void *data,
   size_t length, size_t *written, uint_t flags);

error_t httpClientReceiveData(HttpClientContext *context, void *data,
   size_t size, size_t *received, uint_t flags);

error_t httpClientSaveSession(HttpClientContext *context);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
