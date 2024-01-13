/**
 * @file smtp_client_misc.h
 * @brief Helper functions for SMTP client
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

#ifndef _SMTP_CLIENT_MISC_H
#define _SMTP_CLIENT_MISC_H

//Dependencies
#include "core/net.h"
#include "smtp/smtp_client.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//SMTP client related functions
void smtpClientChangeState(SmtpClientContext *context,
   SmtpClientState newState);

error_t smtpClientSendCommand(SmtpClientContext *context,
   SmtpClientReplyCallback callback);

error_t smtpClientFormatCommand(SmtpClientContext *context,
   const char_t *command, const char_t *argument);

error_t smtpClientParseEhloReply(SmtpClientContext *context,
   char_t *replyLine);

error_t smtpClientFormatMailHeader(SmtpClientContext *context,
   const SmtpMailAddr *from, const SmtpMailAddr *recipients,
   uint_t numRecipients, const char_t *subject);

error_t smtpClientFormatMultipartHeader(SmtpClientContext *context,
   const char_t *filename, const char_t *contentType,
   const char_t *contentTransferEncoding, bool_t last);

error_t smtpClientCheckTimeout(SmtpClientContext *context);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
