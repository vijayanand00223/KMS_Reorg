/**
 * @file snmp_mpd_mib_module.h
 * @brief SNMP MPD MIB module
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

#ifndef _SNMP_MPD_MIB_MODULE_H
#define _SNMP_MPD_MIB_MODULE_H

//Dependencies
#include "mibs/mib_common.h"

//SNMP MPD MIB module support
#ifndef SNMP_MPD_MIB_SUPPORT
   #define SNMP_MPD_MIB_SUPPORT DISABLED
#elif (SNMP_MPD_MIB_SUPPORT != ENABLED && SNMP_MPD_MIB_SUPPORT != DISABLED)
   #error SNMP_MPD_MIB_SUPPORT parameter is not valid
#endif

//Macro definitions
#if (SNMP_MPD_MIB_SUPPORT == ENABLED)
   #define SNMP_MPD_MIB_INC_COUNTER32(name, value) snmpMpdMibBase.name += value
#else
   #define SNMP_MPD_MIB_INC_COUNTER32(name, value)
#endif

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief SNMP MPD MIB base
 **/

typedef struct
{
   uint32_t snmpUnknownSecurityModels;
   uint32_t snmpInvalidMsgs;
   uint32_t snmpUnknownPDUHandlers;
} SnmpMpdMibBase;


//SNMP MPD MIB related constants
extern SnmpMpdMibBase snmpMpdMibBase;
extern const MibObject snmpMpdMibObjects[];
extern const MibModule snmpMpdMibModule;

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
