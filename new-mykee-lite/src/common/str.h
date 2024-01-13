/**
 * @file str.h
 * @brief String manipulation helper functions
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

#ifndef _STR_H
#define _STR_H

//Dependencies
#include "os_port.h"
#include "error.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//String manipulation helper functions
char_t *strDuplicate(const char_t *s);
char_t *strTrimWhitespace(char_t *s);
void strRemoveTrailingSpace(char_t *s);
void strReplaceChar(char_t *s, char_t oldChar, char_t newChar);

error_t strSafeCopy(char_t *dest, const char_t *src, size_t destSize);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
