/**
 * @file sha1.h
 * @brief SHA-1 (Secure Hash Algorithm 1)
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

#ifndef _SHA1_H
#define _SHA1_H

//Dependencies
#include "core/crypto.h"

//SHA-1 block size
#define SHA1_BLOCK_SIZE 64
//SHA-1 digest size
#define SHA1_DIGEST_SIZE 20
//Minimum length of the padding string
#define SHA1_MIN_PAD_SIZE 9
//SHA-1 algorithm object identifier
#define SHA1_OID sha1Oid
//Common interface for hash algorithms
#define SHA1_HASH_ALGO (&sha1HashAlgo)

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief SHA-1 algorithm context
 **/

typedef struct
{
   union
   {
      uint32_t h[5];
      uint8_t digest[20];
   };
   union
   {
      uint32_t w[16];
      uint8_t buffer[64];
   };
   size_t size;
   uint64_t totalSize;
#if ((defined(MIMXRT1050_CRYPTO_HASH_SUPPORT) && MIMXRT1050_CRYPTO_HASH_SUPPORT == ENABLED) || \
   (defined(MIMXRT1060_CRYPTO_HASH_SUPPORT) && MIMXRT1060_CRYPTO_HASH_SUPPORT == ENABLED))
   uint32_t dcpHandle[11];
   uint32_t dcpContext[64];
#elif (defined(MIMXRT1170_CRYPTO_HASH_SUPPORT) && MIMXRT1170_CRYPTO_HASH_SUPPORT == ENABLED)
   uint32_t caamHandle[3];
   uint32_t caamContext[58];
#elif ((defined(STM32F2XX_CRYPTO_HASH_SUPPORT) && STM32F2XX_CRYPTO_HASH_SUPPORT == ENABLED) || \
   (defined(STM32F4XX_CRYPTO_HASH_SUPPORT) && STM32F4XX_CRYPTO_HASH_SUPPORT == ENABLED) || \
   (defined(STM32F7XX_CRYPTO_HASH_SUPPORT) && STM32F7XX_CRYPTO_HASH_SUPPORT == ENABLED) || \
   (defined(STM32H7XX_CRYPTO_HASH_SUPPORT) && STM32H7XX_CRYPTO_HASH_SUPPORT == ENABLED) || \
   (defined(STM32L4XX_CRYPTO_HASH_SUPPORT) && STM32L4XX_CRYPTO_HASH_SUPPORT == ENABLED) || \
   (defined(STM32L5XX_CRYPTO_HASH_SUPPORT) && STM32L5XX_CRYPTO_HASH_SUPPORT == ENABLED) || \
   (defined(STM32MP1XX_CRYPTO_HASH_SUPPORT) && STM32MP1XX_CRYPTO_HASH_SUPPORT == ENABLED))
   uint32_t hwContext[58];
#endif
} Sha1Context;


//SHA-1 related constants
extern const uint8_t sha1Oid[5];
extern const HashAlgo sha1HashAlgo;

//SHA-1 related functions
error_t sha1Compute(const void *data, size_t length, uint8_t *digest);
void sha1Init(Sha1Context *context);
void sha1Update(Sha1Context *context, const void *data, size_t length);
void sha1Final(Sha1Context *context, uint8_t *digest);
void sha1FinalRaw(Sha1Context *context, uint8_t *digest);
void sha1ProcessBlock(Sha1Context *context);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
