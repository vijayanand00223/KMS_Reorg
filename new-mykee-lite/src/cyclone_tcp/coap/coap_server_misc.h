/**
 * @file coap_server_misc.h
 * @brief Helper functions for CoAP server
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

#ifndef _COAP_SERVER_MISC_H
#define _COAP_SERVER_MISC_H

//Dependencies
#include "core/net.h"
#include "coap/coap_server.h"

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif

//CoAP server related functions
void coapServerTick(CoapServerContext *context);

error_t coapServerProcessRequest(CoapServerContext *context,
   const uint8_t *data, size_t length);

error_t coapServerRejectRequest(CoapServerContext *context);

error_t coapServerInitResponse(CoapServerContext *context);

error_t coapServerSendResponse(CoapServerContext *context,
   const void *data, size_t length);

error_t coapServerFormatReset(CoapServerContext *context, uint16_t mid);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
