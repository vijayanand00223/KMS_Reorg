/**
 * @file coap_server_transport.h
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

#ifndef _COAP_SERVER_TRANSPORT_H
#define _COAP_SERVER_TRANSPORT_H

//Dependencies
#include "core/net.h"
#include "coap/coap_server.h"

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif

//CoAP server related functions
error_t coapServerAcceptSession(CoapServerContext *context,
   CoapDtlsSession *session, const IpAddr *remoteIpAddr, uint16_t remotePort);

error_t coapServerDemultiplexSession(CoapServerContext *context);

void coapServerDeleteSession(CoapDtlsSession *session);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
