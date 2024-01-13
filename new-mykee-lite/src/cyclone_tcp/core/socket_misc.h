/**
 * @file socket_misc.h
 * @brief Helper functions for sockets
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

#ifndef _SOCKET_MISC_H
#define _SOCKET_MISC_H

//Dependencies
#include "core/net.h"
#include "core/socket.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//Socket related functions
Socket *socketAllocate(uint_t type, uint_t protocol);

void socketRegisterEvents(Socket *socket, OsEvent *event, uint_t eventMask);
void socketUnregisterEvents(Socket *socket);
uint_t socketGetEvents(Socket *socket);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
