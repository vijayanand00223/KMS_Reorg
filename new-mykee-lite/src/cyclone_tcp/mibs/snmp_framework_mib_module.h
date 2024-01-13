/**
 * @file snmp_framework_mib_module.h
 * @brief SNMP FRAMEWORK MIB module
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

#ifndef _SNMP_FRAMEWORK_MIB_MODULE_H
#define _SNMP_FRAMEWORK_MIB_MODULE_H

//Dependencies
#include "mibs/mib_common.h"
#include "snmp/snmp_agent.h"

//SNMP FRAMEWORK MIB module support
#ifndef SNMP_FRAMEWORK_MIB_SUPPORT
   #define SNMP_FRAMEWORK_MIB_SUPPORT DISABLED
#elif (SNMP_FRAMEWORK_MIB_SUPPORT != ENABLED && SNMP_FRAMEWORK_MIB_SUPPORT != DISABLED)
   #error SNMP_FRAMEWORK_MIB_SUPPORT parameter is not valid
#endif

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief SNMP FRAMEWORK MIB base
 **/

typedef struct
{
   SnmpAgentContext *context;
} SnmpFrameworkMibBase;


//SNMP FRAMEWORK MIB related constants
extern SnmpFrameworkMibBase snmpFrameworkMibBase;
extern const MibObject snmpFrameworkMibObjects[];
extern const MibModule snmpFrameworkMibModule;

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
