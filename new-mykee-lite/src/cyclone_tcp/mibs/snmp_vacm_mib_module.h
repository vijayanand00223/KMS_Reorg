/**
 * @file snmp_vacm_mib_module.h
 * @brief SNMP VACM MIB module
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

#ifndef _SNMP_VACM_MIB_MODULE_H
#define _SNMP_VACM_MIB_MODULE_H

//Dependencies
#include "mibs/mib_common.h"
#include "snmp/snmp_agent.h"

//SNMP VACM MIB module support
#ifndef SNMP_VACM_MIB_SUPPORT
   #define SNMP_VACM_MIB_SUPPORT DISABLED
#elif (SNMP_VACM_MIB_SUPPORT != ENABLED && SNMP_VACM_MIB_SUPPORT != DISABLED)
   #error SNMP_VACM_MIB_SUPPORT parameter is not valid
#endif

//Support for SET operations
#ifndef SNMP_VACM_MIB_SET_SUPPORT
   #define SNMP_VACM_MIB_SET_SUPPORT DISABLED
#elif (SNMP_VACM_MIB_SET_SUPPORT != ENABLED && SNMP_VACM_MIB_SET_SUPPORT != DISABLED)
   #error SNMP_VACM_MIB_SET_SUPPORT parameter is not valid
#endif

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief SNMP VACM MIB base
 **/

typedef struct
{
   SnmpAgentContext *context;
   int32_t vacmViewSpinLock;
   SnmpGroupEntry tempGroupEntry;
   SnmpAccessEntry tempAccessEntry;
   SnmpViewEntry tempViewEntry;
} SnmpVacmMibBase;


//SNMP VACM MIB related constants
extern SnmpVacmMibBase snmpVacmMibBase;
extern const MibObject snmpVacmMibObjects[];
extern const MibModule snmpVacmMibModule;

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
