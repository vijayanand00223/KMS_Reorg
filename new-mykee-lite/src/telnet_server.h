/**
 * @file telnet_server.h
 * @brief Telnet server example
 *
 * @section License
 *
 * Copyright (C) 2010-2015 Oryx Embedded SARL. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * @author Oryx Embedded SARL (www.oryx-embedded.com)
 * @version 1.6.1
 **/

#ifndef _TELNET_SERVER_H
#define _TELNET_SERVER_H
#include "core/net.h"
//TCP server related functions
//error_t telnetServerInit(void);
void telnetServerTask(void);

void telnetServerStateListen(void);
void telnetServerStateReceive(void);
void telnetServerStateExecute(void);
void telnetServerStateSend(void);
void telnetserverwait(void);
#endif
