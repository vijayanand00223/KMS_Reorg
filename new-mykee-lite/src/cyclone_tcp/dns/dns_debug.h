/**
 * @file dns_debug.h
 * @brief Data logging functions for debugging purpose (DNS)
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

#ifndef _DNS_DEBUG_H
#define _DNS_DEBUG_H

//Dependencies
#include "core/net.h"
#include "dns/dns_common.h"
#include "debug.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//Check current trace level
#if (DNS_TRACE_LEVEL >= TRACE_LEVEL_DEBUG)
   void dnsDumpMessage(const DnsHeader *message, size_t length);
   size_t dnsDumpQuestion(const DnsHeader *message, size_t length, size_t pos, char_t *buffer);
   size_t dnsDumpResourceRecord(const DnsHeader *message, size_t length, size_t pos, char_t *buffer);
#else
   #define dnsDumpMessage(message, length)
#endif

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
