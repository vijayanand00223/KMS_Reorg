/**
 * @file ftp_server_misc.h
 * @brief Helper functions for FTP server
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

#ifndef _FTP_SERVER_MISC_H
#define _FTP_SERVER_MISC_H

//Dependencies
#include "core/net.h"
#include "ftp/ftp_server.h"

//Time constant
#define FTP_SERVER_180_DAYS (180 * 86400)

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//FTP server related functions
void ftpServerTick(FtpServerContext *context);

uint16_t ftpServerGetPassivePort(FtpServerContext *context);

error_t ftpServerGetPath(FtpClientConnection *connection,
   const char_t *inputPath, char_t *outputPath, size_t maxLen);

uint_t ftpServerGetFilePermissions(FtpClientConnection *connection,
   const char_t *path);

size_t ftpServerFormatDirEntry(const FsDirEntry *dirEntry, uint_t perm,
   char_t *buffer);

const char_t *ftpServerStripRootDir(FtpServerContext *context,
   const char_t *path);

const char_t *ftpServerStripHomeDir(FtpClientConnection *connection,
   const char_t *path);

void ftpServerCloseConnection(FtpClientConnection *connection);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
