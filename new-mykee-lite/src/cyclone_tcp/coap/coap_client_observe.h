/**
 * @file coap_client_observe.h
 * @brief CoAP observe
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

#ifndef _COAP_CLIENT_OBSERVE_H
#define _COAP_CLIENT_OBSERVE_H

//Dependencies
#include "core/net.h"
#include "coap/coap_client.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//CoAP client related functions
error_t coapClientProcessNotification(CoapClientRequest *request,
   const CoapMessage *response);

bool_t coapClientCheckSequenceNumber(CoapClientRequest *request,
   uint32_t v2, systime_t t2);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
