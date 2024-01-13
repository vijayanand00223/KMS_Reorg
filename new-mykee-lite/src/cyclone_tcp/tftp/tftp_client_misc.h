/**
 * @file tftp_client_misc.h
 * @brief Helper functions for TFTP client
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

#ifndef _TFTP_CLIENT_MISC_H
#define _TFTP_CLIENT_MISC_H

//Dependencies
#include "core/net.h"
#include "tftp/tftp_client.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//TFTP client related functions
error_t tftpClientOpenConnection(TftpClientContext *context);
void tftpClientCloseConnection(TftpClientContext *context);

error_t tftpClientProcessEvents(TftpClientContext *context);

void tftpClientProcessPacket(TftpClientContext *context,
   const IpAddr *srcIpAddr, uint16_t srcPort);

void tftpClientProcessDataPacket(TftpClientContext *context,
   uint16_t srcPort, const TftpDataPacket *dataPacket, size_t length);

void tftpClientProcessAckPacket(TftpClientContext *context,
   uint16_t srcPort, const TftpAckPacket *ackPacket, size_t length);

void tftpClientProcessErrorPacket(TftpClientContext *context,
   uint16_t srcPort, const TftpErrorPacket *errorPacket, size_t length);

error_t tftpClientSendRrqPacket(TftpClientContext *context,
   const char_t *filename, const char_t *mode);

error_t tftpClientSendWrqPacket(TftpClientContext *context,
   const char_t *filename, const char_t *mode);

error_t tftpClientSendDataPacket(TftpClientContext *context);
error_t tftpClientSendAckPacket(TftpClientContext *context);

error_t tftpClientSendErrorPacket(TftpClientContext *context,
   uint16_t errorCode, const char_t *errorMsg);

error_t tftpClientRetransmitPacket(TftpClientContext *context);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
