/**
 * @file http_client_misc.h
 * @brief Helper functions for HTTP client
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

#ifndef _HTTP_CLIENT_MISC_H
#define _HTTP_CLIENT_MISC_H

//Dependencies
#include "core/net.h"
#include "http/http_client.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//HTTP client related functions
void httpClientChangeState(HttpClientContext *context,
   HttpClientState newState);

void httpClientChangeRequestState(HttpClientContext *context,
   HttpRequestState newState);

error_t httpClientFormatRequestHeader(HttpClientContext *context);

error_t httpClientFormatChunkSize(HttpClientContext *context, size_t length);

error_t httpClientParseStatusLine(HttpClientContext *context, char_t *line,
   size_t length);

error_t httpClientParseHeaderField(HttpClientContext *context, char_t *line,
   size_t length);

error_t httpClientParseConnectionField(HttpClientContext *context,
   const char_t *value);

error_t httpClientParseTransferEncodingField(HttpClientContext *context,
   const char_t *value);

error_t httpClientParseContentLengthField(HttpClientContext *context,
   const char_t *value);

error_t httpClientParseChunkSize(HttpClientContext *context, char_t *line,
   size_t length);

error_t httpClientCheckTimeout(HttpClientContext *context);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
