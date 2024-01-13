/**
 * @file smtp_client_transport.h
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

#ifndef _SMTP_CLIENT_TRANSPORT_H
#define _SMTP_CLIENT_TRANSPORT_H

//Dependencies
#include "core/net.h"
#include "smtp/smtp_client.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//SMTP client related functions
error_t smtpClientOpenConnection(SmtpClientContext *context);

error_t smtpClientEstablishConnection(SmtpClientContext *context,
   const IpAddr *serverIpAddr, uint16_t serverPort);

error_t smtpClientOpenSecureConnection(SmtpClientContext *context);
error_t smtpClientEstablishSecureConnection(SmtpClientContext *context);

error_t smtpClientShutdownConnection(SmtpClientContext *context);
void smtpClientCloseConnection(SmtpClientContext *context);

error_t smtpClientSendData(SmtpClientContext *context, const void *data,
   size_t length, size_t *written, uint_t flags);

error_t smtpClientReceiveData(SmtpClientContext *context, void *data,
   size_t size, size_t *received, uint_t flags);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
