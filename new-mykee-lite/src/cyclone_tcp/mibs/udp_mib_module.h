/**
 * @file udp_mib_module.h
 * @brief UDP MIB module
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

#ifndef _UDP_MIB_MODULE_H
#define _UDP_MIB_MODULE_H

//Dependencies
#include "mibs/mib_common.h"

//UDP MIB module support
#ifndef UDP_MIB_SUPPORT
   #define UDP_MIB_SUPPORT DISABLED
#elif (UDP_MIB_SUPPORT != ENABLED && UDP_MIB_SUPPORT != DISABLED)
   #error UDP_MIB_SUPPORT parameter is not valid
#endif

//Macro definitions
#if (UDP_MIB_SUPPORT == ENABLED)
   #define UDP_MIB_INC_COUNTER32(name, value) udpMibBase.name += value
   #define UDP_MIB_INC_COUNTER64(name, value) udpMibBase.name += value
#else
   #define UDP_MIB_INC_COUNTER32(name, value)
   #define UDP_MIB_INC_COUNTER64(name, value)
#endif

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief UDP MIB base
 **/

typedef struct
{
   uint32_t udpInDatagrams;
   uint32_t udpNoPorts;
   uint32_t udpInErrors;
   uint32_t udpOutDatagrams;
   uint64_t udpHCInDatagrams;
   uint64_t udpHCOutDatagrams;
} UdpMibBase;


//UDP MIB related constants
extern UdpMibBase udpMibBase;
extern const MibObject udpMibObjects[];
extern const MibModule udpMibModule;

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
