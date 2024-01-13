/**
 * @file ssi.h
 * @brief SSI (Server Side Includes)
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

#ifndef _SSI_H
#define _SSI_H

//Dependencies
#include "http/http_server.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//SSI related functions
error_t ssiExecuteScript(HttpConnection *connection, const char_t *uri, uint_t level);

error_t ssiProcessCommand(HttpConnection *connection,
   const char_t *tag, size_t length, const char_t *uri, uint_t level);

error_t ssiProcessIncludeCommand(HttpConnection *connection,
   const char_t *tag, size_t length, const char_t *uri, uint_t level);

error_t ssiProcessEchoCommand(HttpConnection *connection, const char_t *tag, size_t length);
error_t ssiProcessExecCommand(HttpConnection *connection, const char_t *tag, size_t length);

error_t ssiSearchTag(const char_t *s, size_t sLen, const char_t *tag, size_t tagLen, uint_t *pos);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
