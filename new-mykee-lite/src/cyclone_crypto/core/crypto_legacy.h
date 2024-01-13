/**
 * @file crypto_legacy.h
 * @brief Legacy definitions
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

#ifndef _CRYPTO_LEGACY_H
#define _CRYPTO_LEGACY_H

//Deprecated functions
#define mpiReadRaw(r, data, length) mpiImport(r, data, length, MPI_FORMAT_BIG_ENDIAN)
#define mpiWriteRaw(a, data, length) mpiExport(a, data, length, MPI_FORMAT_BIG_ENDIAN)

#ifdef CURVE25519_SUPPORT
   #define X25519_SUPPORT CURVE25519_SUPPORT
#endif

#ifdef CURVE448_SUPPORT
   #define X448_SUPPORT CURVE448_SUPPORT
#endif

#define ecdsaGenerateKeyPair ecGenerateKeyPair
#define ecdsaGeneratePrivateKey ecGeneratePrivateKey
#define ecdsaGeneratePublicKey ecGeneratePublicKey

#define MAX_HASH_CONTEXT_SIZE sizeof(HashContext)
#define MAX_CIPHER_CONTEXT_SIZE sizeof(CipherContext)

#ifdef SAMD51_CRYPTO_PUKCC_SUPPORT
   #define SAMD51_CRYPTO_PKC_SUPPORT SAMD51_CRYPTO_PUKCC_SUPPORT
#endif

#ifdef SAME54_CRYPTO_PUKCC_SUPPORT
   #define SAME54_CRYPTO_PKC_SUPPORT SAME54_CRYPTO_PUKCC_SUPPORT
#endif

#endif
