/**
 * @file des.h
 * @brief DES (Data Encryption Standard)
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

#ifndef _DES_H
#define _DES_H

//Dependencies
#include "core/crypto.h"

//DES block size
#define DES_BLOCK_SIZE 8
//Common interface for encryption algorithms
#define DES_CIPHER_ALGO (&desCipherAlgo)

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief DES algorithm context
 **/

typedef struct
{
   uint32_t ks[32];
} DesContext;


//DES related constants
extern const CipherAlgo desCipherAlgo;

//DES related functions
error_t desInit(DesContext *context, const uint8_t *key, size_t keyLen);
void desEncryptBlock(DesContext *context, const uint8_t *input, uint8_t *output);
void desDecryptBlock(DesContext *context, const uint8_t *input, uint8_t *output);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
