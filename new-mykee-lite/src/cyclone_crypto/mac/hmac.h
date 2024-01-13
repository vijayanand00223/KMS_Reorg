/**
 * @file hmac.h
 * @brief HMAC (Keyed-Hashing for Message Authentication)
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

#ifndef _HMAC_H
#define _HMAC_H

//Dependencies
#include "core/crypto.h"
#include "hash/hash_algorithms.h"

//Inner padding (ipad)
#define HMAC_IPAD 0x36
//Outer padding (opad)
#define HMAC_OPAD 0x5C

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief HMAC algorithm context
 **/

typedef struct
{
   const HashAlgo *hash;
   HashContext hashContext;
   uint8_t key[MAX_HASH_BLOCK_SIZE];
   uint8_t digest[MAX_HASH_DIGEST_SIZE];
} HmacContext;


//HMAC related constants
extern const uint8_t HMAC_WITH_MD5_OID[8];
extern const uint8_t HMAC_WITH_TIGER_OID[8];
extern const uint8_t HMAC_WITH_RIPEMD160_OID[8];
extern const uint8_t HMAC_WITH_SHA1_OID[8];
extern const uint8_t HMAC_WITH_SHA224_OID[8];
extern const uint8_t HMAC_WITH_SHA256_OID[8];
extern const uint8_t HMAC_WITH_SHA384_OID[8];
extern const uint8_t HMAC_WITH_SHA512_OID[8];
extern const uint8_t HMAC_WITH_SHA3_224_OID[9];
extern const uint8_t HMAC_WITH_SHA3_256_OID[9];
extern const uint8_t HMAC_WITH_SHA3_384_OID[9];
extern const uint8_t HMAC_WITH_SHA3_512_OID[9];

//HMAC related functions
error_t hmacCompute(const HashAlgo *hash, const void *key, size_t keyLen,
   const void *data, size_t dataLen, uint8_t *digest);

error_t hmacInit(HmacContext *context, const HashAlgo *hash,
   const void *key, size_t keyLen);

void hmacUpdate(HmacContext *context, const void *data, size_t length);
void hmacFinal(HmacContext *context, uint8_t *digest);
void hmacFinalRaw(HmacContext *context, uint8_t *digest);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
