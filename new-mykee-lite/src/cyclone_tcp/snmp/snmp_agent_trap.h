/**
 * @file snmp_agent_trap.h
 * @brief SNMP trap notifications
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

#ifndef _SNMP_AGENT_TRAP_H
#define _SNMP_AGENT_TRAP_H

//Dependencies
#include "core/net.h"
#include "snmp/snmp_agent.h"

//Trap notification support
#ifndef SNMP_AGENT_TRAP_SUPPORT
   #define SNMP_AGENT_TRAP_SUPPORT ENABLED
#elif (SNMP_AGENT_TRAP_SUPPORT != ENABLED && SNMP_AGENT_TRAP_SUPPORT != DISABLED)
   #error SNMP_AGENT_TRAP_SUPPORT parameter is not valid
#endif

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Object descriptor for trap notifications
 **/

typedef struct
{
   uint8_t oid[SNMP_MAX_OID_SIZE];
   size_t oidLen;
} SnmpTrapObject;


//SNMP trap related functions
error_t snmpFormatTrapMessage(SnmpAgentContext *context, SnmpVersion version,
   const char_t *userName, uint_t genericTrapType, uint_t specificTrapCode,
   const SnmpTrapObject *objectList, uint_t objectListSize);

error_t snmpFormatTrapPdu(SnmpAgentContext *context, SnmpVersion version,
   const char_t *userName, uint_t genericTrapType, uint_t specificTrapCode,
   const SnmpTrapObject *objectList, uint_t objectListSize);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
