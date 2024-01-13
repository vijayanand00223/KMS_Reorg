/**
 * @file snmp_framework_mib_impl.h
 * @brief SNMP FRAMEWORK MIB module implementation
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

#ifndef _SNMP_FRAMEWORK_MIB_IMPL_H
#define _SNMP_FRAMEWORK_MIB_IMPL_H

//Dependencies
#include "mibs/mib_common.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//SNMP FRAMEWORK MIB related functions
error_t snmpFrameworkMibInit(void);
error_t snmpFrameworkMibLoad(void *context);
void snmpFrameworkMibUnload(void *context);
void snmpFrameworkMibLock(void);
void snmpFrameworkMibUnlock(void);

error_t snmpFrameworkMibGetSnmpEngineID(const MibObject *object,
   const uint8_t *oid, size_t oidLen, MibVariant *value, size_t *valueLen);

error_t snmpFrameworkMibGetSnmpEngineBoots(const MibObject *object,
   const uint8_t *oid, size_t oidLen, MibVariant *value, size_t *valueLen);

error_t snmpFrameworkMibGetSnmpEngineTime(const MibObject *object,
   const uint8_t *oid, size_t oidLen, MibVariant *value, size_t *valueLen);

error_t snmpFrameworkMibGetSnmpEngineMaxMessageSize(const MibObject *object,
   const uint8_t *oid, size_t oidLen, MibVariant *value, size_t *valueLen);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
