/**
 * @file coap_client_block.h
 * @brief CoAP block-wise transfer
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

#ifndef _COAP_CLIENT_BLOCK_H
#define _COAP_CLIENT_BLOCK_H

//Dependencies
#include "core/net.h"
#include "coap/coap_client.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//CoAP client related functions
error_t coapClientSetTxBlockSize(CoapClientRequest *request, uint_t blockSize);
error_t coapClientSetRxBlockSize(CoapClientRequest *request, uint_t blockSize);

error_t coapClientWriteBody(CoapClientRequest *request, const void *data,
   size_t length, size_t *written, bool_t last);

error_t coapClientReadBody(CoapClientRequest *request, void *data,
   size_t size, size_t *received);

CoapBlockSize coapClientGetMaxBlockSize(void);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
