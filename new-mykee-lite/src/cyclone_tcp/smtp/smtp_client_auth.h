/**
 * @file smtp_client_auth.h
 * @brief SMTP authentication mechanism
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

#ifndef _SMTP_CLIENT_AUTH_H
#define _SMTP_CLIENT_AUTH_H

//Dependencies
#include "core/net.h"
#include "smtp/smtp_client.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//SMTP client related functions
error_t smtpClientLoginAuth(SmtpClientContext *context,
   const char_t *username, const char_t *password);

error_t smtpClientPlainAuth(SmtpClientContext *context,
   const char_t *username, const char_t *password);

error_t smtpClientCramMd5Auth(SmtpClientContext *context,
   const char_t *username, const char_t *password);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
