/**
 * @file md5.h
 * @brief MD5 (Message-Digest Algorithm)
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

#ifndef _MD5_H
#define _MD5_H

//Dependencies
#include "core/crypto.h"

//MD5 block size
#define MD5_BLOCK_SIZE 64
//MD5 digest size
#define MD5_DIGEST_SIZE 16
//Minimum length of the padding string
#define MD5_MIN_PAD_SIZE 9
//MD5 algorithm object identifier
#define MD5_OID md5Oid
//Common interface for hash algorithms
#define MD5_HASH_ALGO (&md5HashAlgo)

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief MD5 algorithm context
 **/

typedef struct
{
   union
   {
      uint32_t h[4];
      uint8_t digest[16];
   };
   union
   {
      uint32_t x[16];
      uint8_t buffer[64];
   };
   size_t size;
   uint64_t totalSize;
#if ((defined(STM32F2XX_CRYPTO_HASH_SUPPORT) && STM32F2XX_CRYPTO_HASH_SUPPORT == ENABLED) || \
   (defined(STM32F4XX_CRYPTO_HASH_SUPPORT) && STM32F4XX_CRYPTO_HASH_SUPPORT == ENABLED) || \
   (defined(STM32F7XX_CRYPTO_HASH_SUPPORT) && STM32F7XX_CRYPTO_HASH_SUPPORT == ENABLED) || \
   (defined(STM32H7XX_CRYPTO_HASH_SUPPORT) && STM32H7XX_CRYPTO_HASH_SUPPORT == ENABLED) || \
   (defined(STM32L4XX_CRYPTO_HASH_SUPPORT) && STM32L4XX_CRYPTO_HASH_SUPPORT == ENABLED) || \
   (defined(STM32L5XX_CRYPTO_HASH_SUPPORT) && STM32L5XX_CRYPTO_HASH_SUPPORT == ENABLED) || \
   (defined(STM32MP1XX_CRYPTO_HASH_SUPPORT) && STM32MP1XX_CRYPTO_HASH_SUPPORT == ENABLED))
   uint32_t hwContext[58];
#endif
} Md5Context;


//MD5 related constants
extern const uint8_t md5Oid[8];
extern const HashAlgo md5HashAlgo;

//MD5 related functions
error_t md5Compute(const void *data, size_t length, uint8_t *digest);
void md5Init(Md5Context *context);
void md5Update(Md5Context *context, const void *data, size_t length);
void md5Final(Md5Context *context, uint8_t *digest);
void md5FinalRaw(Md5Context *context, uint8_t *digest);
void md5ProcessBlock(Md5Context *context);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
