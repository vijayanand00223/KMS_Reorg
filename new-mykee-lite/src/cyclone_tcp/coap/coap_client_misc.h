/**
 * @file coap_client_misc.h
 * @brief Helper functions for CoAP client
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

#ifndef _COAP_CLIENT_MISC_H
#define _COAP_CLIENT_MISC_H

//Dependencies
#include "core/net.h"
#include "coap/coap_client.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//CoAP client related functions
error_t coapClientProcessEvents(CoapClientContext *context, systime_t timeout);
error_t coapClientProcessRequestEvents(CoapClientRequest *request);

error_t coapClientChangeRequestState(CoapClientRequest *request,
   CoapRequestState newState);

error_t coapClientMatchResponse(const CoapClientRequest *request,
   const CoapMessage *response);

error_t coapClientProcessResponse(CoapClientRequest *request,
   const CoapMessage *response);

error_t coapClientRejectResponse(CoapClientContext *context,
   const CoapMessage *response);

error_t coapClientSendAck(CoapClientContext *context, uint16_t mid);
error_t coapClientSendReset(CoapClientContext *context, uint16_t mid);

void coapClientGenerateMessageId(CoapClientContext *context,
   CoapMessageHeader *header);

void coapClientGenerateToken(CoapClientContext *context,
   CoapMessageHeader *header);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
