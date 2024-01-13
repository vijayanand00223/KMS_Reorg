/**
 * @file dhcpv6_debug.h
 * @brief Data logging functions for debugging purpose (DHCPv6)
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

#ifndef _DHCPV6_DEBUG_H
#define _DHCPV6_DEBUG_H

//Dependencies
#include "core/net.h"
#include "dhcpv6/dhcpv6_common.h"
#include "debug.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//Check current trace level
#if (DHCPV6_TRACE_LEVEL >= TRACE_LEVEL_DEBUG)
   error_t dhcpv6DumpMessage(const void *message, size_t length);
   error_t dhcpv6DumpOptions(const uint8_t *options, size_t length, uint_t level);
   error_t dhcpv6DumpGenericOption(const Dhcpv6Option *option, uint_t level);
   error_t dhcpv6DumpClientIdOption(const Dhcpv6Option *option, uint_t level);
   error_t dhcpv6DumpServerIdOption(const Dhcpv6Option *option, uint_t level);
   error_t dhcpv6DumpIaNaOption(const Dhcpv6Option *option, uint_t level);
   error_t dhcpv6DumpIaTaOption(const Dhcpv6Option *option, uint_t level);
   error_t dhcpv6DumpIaAddrOption(const Dhcpv6Option *option, uint_t level);
   error_t dhcpv6DumpOroOption(const Dhcpv6Option *option, uint_t level);
   error_t dhcpv6DumpPreferenceOption(const Dhcpv6Option *option, uint_t level);
   error_t dhcpv6DumpElapsedTimeOption(const Dhcpv6Option *option, uint_t level);
   error_t dhcpv6DumpRelayMessageOption(const Dhcpv6Option *option, uint_t level);
   error_t dhcpv6DumpAuthOption(const Dhcpv6Option *option, uint_t level);
   error_t dhcpv6DumpServerUnicastOption(const Dhcpv6Option *option, uint_t level);
   error_t dhcpv6DumpStatusCodeOption(const Dhcpv6Option *option, uint_t level);
   error_t dhcpv6DumpRapidCommitOption(const Dhcpv6Option *option, uint_t level);
   error_t dhcpv6DumpUserClassOption(const Dhcpv6Option *option, uint_t level);
   error_t dhcpv6DumpVendorClassOption(const Dhcpv6Option *option, uint_t level);
   error_t dhcpv6DumpVendorSpecificInfoOption(const Dhcpv6Option *option, uint_t level);
   error_t dhcpv6DumpInterfaceIdOption(const Dhcpv6Option *option, uint_t level);
   error_t dhcpv6DumpReconfMessageOption(const Dhcpv6Option *option, uint_t level);
   error_t dhcpv6DumpReconfAcceptOption(const Dhcpv6Option *option, uint_t level);
   error_t dhcpv6DumpDnsServersOption(const Dhcpv6Option *option, uint_t level);
   error_t dhcpv6DumpDomainListOption(const Dhcpv6Option *option, uint_t level);
#else
   #define dhcpv6DumpMessage(message, length)
#endif

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
