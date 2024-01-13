/**
 * @file asn1.h
 * @brief ASN.1 (Abstract Syntax Notation One)
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

#ifndef _ASN1_H
#define _ASN1_H

//Dependencies
#include "core/crypto.h"

//Multiple precision integer arithmetic supported?
#if (MPI_SUPPORT == ENABLED)
   #include "mpi/mpi.h"
#endif

//Tag number mask
#define ASN1_TAG_NUMBER_MASK        0x1F

//ASN.1 encoding
#define ASN1_ENCODING_MASK          0x20
#define ASN1_ENCODING_PRIMITIVE     0x00
#define ASN1_ENCODING_CONSTRUCTED   0x20

//ASN.1 class
#define ASN1_CLASS_MASK             0xC0
#define ASN1_CLASS_UNIVERSAL        0x00
#define ASN1_CLASS_APPLICATION      0x40
#define ASN1_CLASS_CONTEXT_SPECIFIC 0x80
#define ASN1_CLASS_PRIVATE          0xC0

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief ASN.1 data types
 **/

typedef enum
{
   ASN1_TYPE_BOOLEAN           = 1,
   ASN1_TYPE_INTEGER           = 2,
   ASN1_TYPE_BIT_STRING        = 3,
   ASN1_TYPE_OCTET_STRING      = 4,
   ASN1_TYPE_NULL              = 5,
   ASN1_TYPE_OBJECT_IDENTIFIER = 6,
   ASN1_TYPE_OBJECT_DESCRIPTOR = 7,
   ASN1_TYPE_EXTERNAL          = 8,
   ASN1_TYPE_REAL              = 9,
   ASN1_TYPE_ENUMERATED        = 10,
   ASN1_TYPE_UTF8_STRING       = 12,
   ASN1_TYPE_SEQUENCE          = 16,
   ASN1_TYPE_SET               = 17,
   ASN1_TYPE_NUMERIC_STRING    = 18,
   ASN1_TYPE_PRINTABLE_STRING  = 19,
   ASN1_TYPE_TELETEX_STRING    = 20,
   ASN1_TYPE_VIDEOTEX_STRING   = 21,
   ASN1_TYPE_IA5_STRING        = 22,
   ASN1_TYPE_UTC_TIME          = 23,
   ASN1_TYPE_GENERALIZED_TIME  = 24,
   ASN1_TYPE_GRAPHIC_STRING    = 25,
   ASN1_TYPE_VISIBLE_STRING    = 26,
   ASN1_TYPE_GENERAL_STRING    = 27,
   ASN1_TYPE_UNIVERSAL_STRING  = 28,
   ASN1_TYPE_BMP_STRING        = 30,
} Asn1Type;


/**
 * @brief ASN.1 tag
 **/

typedef struct
{
   bool_t constructed;
   uint_t objClass;
   uint_t objType;
   size_t length;
   const uint8_t *value;
   size_t totalLength;
} Asn1Tag;


//ASN.1 related functions
error_t asn1ReadTag(const uint8_t *data, size_t length, Asn1Tag *tag);
error_t asn1ReadSequence(const uint8_t *data, size_t length, Asn1Tag *tag);
error_t asn1ReadOctetString(const uint8_t *data, size_t length, Asn1Tag *tag);
error_t asn1ReadOid(const uint8_t *data, size_t length, Asn1Tag *tag);

error_t asn1ReadBoolean(const uint8_t *data, size_t length, Asn1Tag *tag,
   bool_t *value);

error_t asn1ReadInt32(const uint8_t *data, size_t length, Asn1Tag *tag,
   int32_t *value);

error_t asn1WriteTag(Asn1Tag *tag, bool_t reverse, uint8_t *data,
   size_t *written);

error_t asn1WriteInt32(int32_t value, bool_t reverse, uint8_t *data,
   size_t *written);

#if (MPI_SUPPORT == ENABLED)

error_t asn1ReadMpi(const uint8_t *data, size_t length, Asn1Tag *tag,
   Mpi *value);

error_t asn1WriteMpi(const Mpi *value, bool_t reverse, uint8_t *data,
   size_t *written);

#endif

error_t asn1CheckTag(const Asn1Tag *tag, bool_t constructed, uint_t objClass,
   uint_t objType);

error_t asn1CheckOid(const Asn1Tag *tag, const uint8_t *oid, size_t length);

error_t asn1DumpObject(const uint8_t *data, size_t length, uint_t level);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
