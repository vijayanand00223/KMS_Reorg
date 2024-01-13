/**
 * @file coap_server_request.h
 * @brief CoAP request handling
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

#ifndef _COAP_SERVER_REQUEST_H
#define _COAP_SERVER_REQUEST_H

//Dependencies
#include "core/net.h"
#include "coap/coap_server.h"
#include "coap/coap_option.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

error_t coapServerGetMethodCode(CoapServerContext *context, CoapCode *code);

error_t coapServerGetUriPath(CoapServerContext *context, char_t *path,
   size_t maxLen);

error_t coapServerGetUriQuery(CoapServerContext *context, char_t *queryString,
   size_t maxLen);

error_t coapServerGetOpaqueOption(CoapServerContext *context, uint16_t optionNum,
   uint_t optionIndex, const uint8_t **optionValue, size_t *optionLen);

error_t coapServerGetStringOption(CoapServerContext *context, uint16_t optionNum,
   uint_t optionIndex, const char_t **optionValue, size_t *optionLen);

error_t coapServerGetUintOption(CoapServerContext *context, uint16_t optionNum,
   uint_t optionIndex, uint32_t *optionValue);

error_t coapServerGetPayload(CoapServerContext *context, const uint8_t **payload,
   size_t *payloadLen);

error_t coapServerReadPayload(CoapServerContext *context, void *data, size_t size,
   size_t *length);

error_t coapServerSetResponseCode(CoapServerContext *context, CoapCode code);

error_t coapServerSetLocationPath(CoapServerContext *context,
   const char_t *path);

error_t coapServerSetLocationQuery(CoapServerContext *context,
   const char_t *queryString);

error_t coapServerSetOpaqueOption(CoapServerContext *context, uint16_t optionNum,
   uint_t optionIndex, const uint8_t *optionValue, size_t optionLen);

error_t coapServerSetStringOption(CoapServerContext *context, uint16_t optionNum,
   uint_t optionIndex, const char_t *optionValue);

error_t coapServerSetUintOption(CoapServerContext *context, uint16_t optionNum,
   uint_t optionIndex, uint32_t optionValue);

error_t coapServerDeleteOption(CoapServerContext *context, uint16_t optionNum,
   uint_t optionIndex);

error_t coapServerSetPayload(CoapServerContext *context, const void *payload,
   size_t payloadLen);

error_t coapServerWritePayload(CoapServerContext *context, const void *data,
   size_t length);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
