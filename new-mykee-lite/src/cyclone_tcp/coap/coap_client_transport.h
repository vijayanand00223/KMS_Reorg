/**
 * @file coap_client_transport.h
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

#ifndef _COAP_CLIENT_TRANSPORT_H
#define _COAP_CLIENT_TRANSPORT_H

//Dependencies
#include "core/net.h"
#include "coap/coap_client.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//CoAP client related functions
error_t coapClientOpenConnection(CoapClientContext *context);

error_t coapClientEstablishConnection(CoapClientContext *context,
   const IpAddr *serverIpAddr, uint16_t serverPort);

error_t coapClientShutdownConnection(CoapClientContext *context);

void coapClientCloseConnection(CoapClientContext *context);

error_t coapClientSendDatagram(CoapClientContext *context,
   const void *data, size_t length);

error_t coapClientReceiveDatagram(CoapClientContext *context,
   void *data, size_t size, size_t *received);

error_t coapClientWaitForDatagram(CoapClientContext *context,
   systime_t timeout);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
