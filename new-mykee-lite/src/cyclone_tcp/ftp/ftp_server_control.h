/**
 * @file ftp_server_control.h
 * @brief FTP control connection
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

#ifndef _FTP_SERVER_CONTROL_H
#define _FTP_SERVER_CONTROL_H

//Dependencies
#include "core/net.h"
#include "ftp/ftp_server.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//FTP server related functions
void ftpServerRegisterControlChannelEvents(FtpClientConnection *connection,
   SocketEventDesc *eventDesc);

void ftpServerProcessControlChannelEvents(FtpClientConnection *connection,
   uint_t eventFlags);

void ftpServerAcceptControlChannel(FtpServerContext *context);
void ftpServerCloseControlChannel(FtpClientConnection *connection);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
