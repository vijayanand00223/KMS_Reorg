/**
 * @file tftp_server_misc.h
 * @brief Helper functions for TFTP server
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

#ifndef _TFTP_SERVER_MISC_H
#define _TFTP_SERVER_MISC_H

//Dependencies
#include "core/net.h"
#include "tftp/tftp_server.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//TFTP server related functions
void tftpServerTick(TftpServerContext *context);

TftpClientConnection *tftpServerOpenConnection(TftpServerContext *context,
   const IpAddr *clientIpAddr, uint16_t clientPort);

void tftpServerCloseConnection(TftpClientConnection *connection);

void tftpServerAcceptRequest(TftpServerContext *context);

void tftpServerProcessPacket(TftpServerContext *context,
   TftpClientConnection *connection);

void tftpServerProcessRrqPacket(TftpServerContext *context, const IpAddr *clientIpAddr,
   uint16_t clientPort, const TftpRrqPacket *rrqPacket, size_t length);

void tftpServerProcessWrqPacket(TftpServerContext *context, const IpAddr *clientIpAddr,
   uint16_t clientPort, const TftpWrqPacket *wrqPacket, size_t length);

void tftpServerProcessDataPacket(TftpClientConnection *connection,
   const TftpDataPacket *dataPacket, size_t length);

void tftpServerProcessAckPacket(TftpClientConnection *connection,
   const TftpAckPacket *ackPacket, size_t length);

void tftpServerProcessErrorPacket(TftpClientConnection *connection,
   const TftpErrorPacket *errorPacket, size_t length);

error_t tftpServerSendDataPacket(TftpClientConnection *connection);
error_t tftpServerSendAckPacket(TftpClientConnection *connection);

error_t tftpServerSendErrorPacket(TftpClientConnection *connection,
   uint16_t errorCode, const char_t *errorMsg);

error_t tftpServerRetransmitPacket(TftpClientConnection *connection);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
