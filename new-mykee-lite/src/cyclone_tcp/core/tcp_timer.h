/**
 * @file tcp_timer.h
 * @brief TCP timer management
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

#ifndef _TCP_TIMER_H
#define _TCP_TIMER_H

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//TCP timer related functions
void tcpTick(void);

void tcpCheckRetransmitTimer(Socket *socket);
void tcpCheckPersistTimer(Socket *socket);
void tcpCheckKeepAliveTimer(Socket *socket);
void tcpCheckOverrideTimer(Socket *socket);
void tcpCheckFinWait2Timer(Socket *socket);
void tcpCheckTimeWaitTimer(Socket *socket);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
