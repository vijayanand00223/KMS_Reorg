/**
 * @file mime.h
 * @brief MIME (Multipurpose Internet Mail Extensions)
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

#ifndef _MIME_H
#define _MIME_H

//Dependencies
#include "core/net.h"

//Custom MIME types
#ifndef MIME_CUSTOM_TYPES
   #define MIME_CUSTOM_TYPES
#endif

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief MIME type
 **/

typedef struct
{
   const char_t *extension;
   const char_t *type;
} MimeType;


//MIME related functions
const char_t *mimeGetType(const char_t *filename);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
