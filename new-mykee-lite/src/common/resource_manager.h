/**
 * @file resource_manager.h
 * @brief Embedded resource management
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

#ifndef _RESOURCE_MANAGER_H
#define _RESOURCE_MANAGER_H

//Dependencies
#include "compiler_port.h"
#include "error.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Resource type
 **/

typedef enum
{
   RES_TYPE_DIR  = 1,
   RES_TYPE_FILE = 2
} ResType;


//CodeWarrior or Win32 compiler?
#if defined(__CWCC__) || defined(_WIN32)
   #pragma pack(push, 1)
#endif


/**
 * @brief Resource entry
 **/

typedef __start_packed struct
{
   char_t type;
   uint32_t dataStart;
   uint32_t dataLength;
   uint8_t nameLength;
   char_t name[];
} __end_packed ResEntry;


/**
 * @brief Root entry
 **/

typedef __start_packed struct
{
   char_t type;
   uint32_t dataStart;
   uint32_t dataLength;
   uint8_t nameLength;
} __end_packed ResRootEntry;


/**
 * @brief Resource header
 **/

typedef __start_packed struct
{
   uint32_t totalSize;
   ResRootEntry rootEntry;
} __end_packed ResHeader;


//CodeWarrior or Win32 compiler?
#if defined(__CWCC__) || defined(_WIN32)
   #pragma pack(pop)
#endif


typedef struct
{
   uint_t type;
   uint_t volume;
   uint32_t dataStart;
   uint32_t dataLength;
   uint8_t nameLength;
   char_t name[];
} DirEntry;


//Resource management
error_t resGetData(const char_t *path, const uint8_t **data, size_t *length);

error_t resSearchFile(const char_t *path, DirEntry *dirEntry);

//error_t resOpenDirectory(Directory *directory, const DirEntry *entry);
//error_t resReadDirectory(Directory *directory, DirEntry *entry);

#if 0
typedef struct
{
   uint_t mode;
   uint32_t start;
   uint32_t size;
   uint32_t offset;
} FsFile;

error_t resOpenFile(FsFile *file, const DirEntry *dirEntry, uint_t mode);
error_t resSeekFile(FsFile *file, uint32_t *position);
uint_t resReadFile(FsFile *file, void *data, size_t length);
#endif

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
