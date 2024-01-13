/**
 * @file snmp_community_mib_module.h
 * @brief SNMP COMMUNITY MIB module
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
 * @section Description
 *
 * The SNMP-MIB describes managed objects which describe the behavior
 * of an SNMP entity. Refer to RFC 3418 for more details
 *
 * @author Oryx Embedded SARL (www.oryx-embedded.com)
 * @version 2.1.2
 **/

#ifndef _SNMP_COMMUNITY_MIB_MODULE_H
#define _SNMP_COMMUNITY_MIB_MODULE_H

//Dependencies
#include "mibs/mib_common.h"
#include "snmp/snmp_agent.h"

//SNMP COMMUNITY MIB module support
#ifndef SNMP_COMMUNITY_MIB_SUPPORT
   #define SNMP_COMMUNITY_MIB_SUPPORT DISABLED
#elif (SNMP_COMMUNITY_MIB_SUPPORT != ENABLED && SNMP_COMMUNITY_MIB_SUPPORT != DISABLED)
   #error SNMP_COMMUNITY_MIB_SUPPORT parameter is not valid
#endif

//Support for SET operations
#ifndef SNMP_COMMUNITY_MIB_SET_SUPPORT
   #define SNMP_COMMUNITY_MIB_SET_SUPPORT DISABLED
#elif (SNMP_COMMUNITY_MIB_SET_SUPPORT != ENABLED && SNMP_COMMUNITY_MIB_SET_SUPPORT != DISABLED)
   #error SNMP_COMMUNITY_MIB_SET_SUPPORT parameter is not valid
#endif

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief SNMP COMMUNITY MIB base
 **/

typedef struct
{
   SnmpAgentContext *context;
   SnmpUserEntry tempCommunity;
} SnmpCommunityMibBase;


//SNMP COMMUNITY MIB related constants
extern SnmpCommunityMibBase snmpCommunityMibBase;
extern const MibObject snmpCommunityMibObjects[];
extern const MibModule snmpCommunityMibModule;

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
