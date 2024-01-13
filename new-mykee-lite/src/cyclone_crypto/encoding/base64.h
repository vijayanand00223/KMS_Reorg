/**
 * @file base64.h
 * @brief Base64 encoding scheme
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

#ifndef _BASE64_H
#define _BASE64_H

//Dependencies
#include "core/crypto.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//Base64 encoding related functions
void base64Encode(const void *input, size_t inputLen, char_t *output,
   size_t *outputLen);

error_t base64Decode(const char_t *input, size_t inputLen, void *output,
   size_t *outputLen);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
