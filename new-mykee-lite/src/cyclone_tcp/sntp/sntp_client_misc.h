/**
 * @file sntp_client.h
 * @brief Helper functions for SNTP client
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

#ifndef _SNTP_CLIENT_MISC_H
#define _SNTP_CLIENT_MISC_H

//Dependencies
#include "core/net.h"
#include "sntp/sntp_client.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//SNTP client related functions
error_t sntpClientOpenConnection(SntpClientContext *context);
void sntpClientCloseConnection(SntpClientContext *context);

error_t sntpClientSendRequest(SntpClientContext *context);
error_t sntpClientReceiveResponse(SntpClientContext *context);

error_t sntpClientCheckResponse(SntpClientContext *context,
   const IpAddr *ipAddr, uint16_t port, const uint8_t *message,
   size_t length);

error_t sntpClientParseResponse(SntpClientContext *context,
   NtpTimestamp *timestamp);

error_t sntpClientCheckTimeout(SntpClientContext *context);

void sntpClientDumpMessage(const uint8_t *message, size_t length);
void sntpClientDumpTimestamp(const NtpTimestamp *timestamp);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
