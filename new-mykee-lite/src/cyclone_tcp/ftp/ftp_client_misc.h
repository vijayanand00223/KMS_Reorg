/**
 * @file ftp_client_misc.h
 * @brief Helper functions for FTP client
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

#ifndef _FTP_CLIENT_MISC_H
#define _FTP_CLIENT_MISC_H

//Dependencies
#include "core/net.h"
#include "ftp/ftp_client.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//FTP client related functions
void ftpClientChangeState(FtpClientContext *context, FtpClientState newState);

error_t ftpClientSendCommand(FtpClientContext *context);

error_t ftpClientFormatCommand(FtpClientContext *context,
   const char_t *command, const char_t *argument);

error_t ftpClientFormatPortCommand(FtpClientContext *context,
   const IpAddr *ipAddr, uint16_t port);

error_t ftpClientFormatPasvCommand(FtpClientContext *context);
error_t ftpClientParsePasvReply(FtpClientContext *context, uint16_t *port);

error_t ftpClientParsePwdReply(FtpClientContext *context, char_t *path,
   size_t maxLen);

error_t ftpClientParseDirEntry(char_t *line, FtpDirEntry *dirEntry);

error_t ftpClientInitDataTransfer(FtpClientContext *context, bool_t direction);
error_t ftpClientTerminateDataTransfer(FtpClientContext *context);

error_t ftpClientCheckTimeout(FtpClientContext *context);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
