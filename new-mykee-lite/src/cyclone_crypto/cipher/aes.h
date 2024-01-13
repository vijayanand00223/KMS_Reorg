/**
 * @file aes.h
 * @brief AES (Advanced Encryption Standard)
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

#ifndef _AES_H
#define _AES_H

//Dependencies
#include "core/crypto.h"

//AES block size
#define AES_BLOCK_SIZE 16
//Common interface for encryption algorithms
#define AES_CIPHER_ALGO (&aesCipherAlgo)

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief AES algorithm context
 **/

typedef struct
{
   uint_t nr;
   uint32_t ek[60];
   uint32_t dk[60];
} AesContext;


//AES related constants
extern const CipherAlgo aesCipherAlgo;

//AES related functions
error_t aesInit(AesContext *context, const uint8_t *key, size_t keyLen);
void aesEncryptBlock(AesContext *context, const uint8_t *input, uint8_t *output);
void aesDecryptBlock(AesContext *context, const uint8_t *input, uint8_t *output);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
