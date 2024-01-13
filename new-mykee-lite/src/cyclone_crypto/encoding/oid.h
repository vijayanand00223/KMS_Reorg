/**
 * @file oid.h
 * @brief OID (Object Identifier)
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

#ifndef _OID_H
#define _OID_H

//Dependencies
#include "core/crypto.h"

//Mask definition
#define OID_MORE_FLAG  0x80
#define OID_VALUE_MASK 0x7F

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//OID related functions
error_t oidCheck(const uint8_t *oid, size_t oidLen);

int_t oidComp(const uint8_t *oid1, size_t oidLen1, const uint8_t *oid2,
   size_t oidLen2);

bool_t oidMatch(const uint8_t *oid, size_t oidLen, const uint8_t *subtree,
   size_t subtreeLen, const uint8_t *mask, size_t maskLen);

uint_t oidCountSubIdentifiers(const uint8_t *oid, size_t oidLen);

error_t oidEncodeSubIdentifier(uint8_t *oid, size_t maxOidLen,
   size_t *pos, uint32_t value);

error_t oidDecodeSubIdentifier(const uint8_t *oid, size_t oidLen,
   size_t *pos, uint32_t *value);

error_t oidFromString(const char_t *str, uint8_t *oid, size_t maxOidLen,
   size_t *oidLen);

char_t *oidToString(const uint8_t *oid, size_t oidLen, char_t *str,
   size_t maxStrLen);

error_t maskFromString(const char_t *str, uint8_t *mask, size_t maxMaskLen,
   size_t *maskLen);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
