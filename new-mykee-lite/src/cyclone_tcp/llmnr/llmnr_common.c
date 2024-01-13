/**
 * @file llmnr_common.c
 * @brief Definitions common to LLMNR client and responder
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

//Switch to the appropriate trace level
#define TRACE_LEVEL LLMNR_TRACE_LEVEL

//Dependencies
#include "core/net.h"
#include "llmnr/llmnr_client.h"
#include "llmnr/llmnr_responder.h"
#include "llmnr/llmnr_common.h"
#include "debug.h"

//Check TCP/IP stack configuration
#if (LLMNR_CLIENT_SUPPORT == ENABLED || LLMNR_RESPONDER_SUPPORT == ENABLED)

//LLMNR IPv6 multicast group (ff02::1:3)
const Ipv6Addr LLMNR_IPV6_MULTICAST_ADDR =
   IPV6_ADDR(0xFF02, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0001, 0x0003);

#endif
