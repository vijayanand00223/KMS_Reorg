/**
 * @file http_server_auth.h
 * @brief HTTP authentication
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

#ifndef _HTTP_SERVER_AUTH_H
#define _HTTP_SERVER_AUTH_H

//Dependencies
#include "http/http_server.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//HTTP authentication related functions
bool_t httpCheckPassword(HttpConnection *connection,
   const char_t *password, HttpAuthMode mode);

void httpParseAuthorizationField(HttpConnection *connection, char_t *value);
size_t httpAddAuthenticateField(HttpConnection *connection, char_t *output);

error_t httpGenerateNonce(HttpServerContext *context,
   char_t *output, size_t *length);

error_t httpVerifyNonce(HttpServerContext *context,
   const char_t *nonce, const char_t *nc);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
