/**
 * @file snmp_agent_object.h
 * @brief MIB object access
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

#ifndef _SNMP_AGENT_OBJECT_H
#define _SNMP_AGENT_OBJECT_H

//Dependencies
#include "core/net.h"
#include "snmp/snmp_agent.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//SNMP agent related functions
error_t snmpSetObjectValue(SnmpAgentContext *context,
   const SnmpMessage *message, SnmpVarBind *var, bool_t commit);

error_t snmpGetObjectValue(SnmpAgentContext *context,
   const SnmpMessage *message, SnmpVarBind *var);

error_t snmpGetNextObject(SnmpAgentContext *context,
   const SnmpMessage *message, SnmpVarBind *var);

error_t snmpFindMibObject(SnmpAgentContext *context,
   const uint8_t *oid, size_t oidLen, const MibObject **object);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
