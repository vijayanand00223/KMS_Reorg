/**
 * @file path.h
 * @brief Path manipulation helper functions
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

#ifndef _PATH_H
#define _PATH_H

//Dependencies
#include "os_port.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//Path manipulation helper functions
bool_t pathIsAbsolute(const char_t *path);
bool_t pathIsRelative(const char_t *path);

const char_t *pathGetFilename(const char_t *path);
void pathRemoveFilename(char_t *path);

void pathCopy(char_t *dest, const char_t *src, size_t maxLen);

void pathCanonicalize(char_t *path);

void pathAddSlash(char_t *path, size_t maxLen);
void pathRemoveSlash(char_t *path);

void pathCombine(char_t *path, const char_t *more, size_t maxLen);

bool_t pathMatch(const char_t *path, const char_t *pattern);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
