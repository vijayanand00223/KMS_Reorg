/**
 * @file snmp_vacm_mib_impl.h
 * @brief SNMP VACM MIB module implementation
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

#ifndef _SNMP_VACM_MIB_IMPL_H
#define _SNMP_VACM_MIB_IMPL_H

//Dependencies
#include "mibs/mib_common.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//SNMP VACM MIB related functions
error_t snmpVacmMibInit(void);
error_t snmpVacmMibLoad(void *context);
void snmpVacmMibUnload(void *context);
void snmpVacmMibLock(void);
void snmpVacmMibUnlock(void);

error_t snmpVacmMibGetContextEntry(const MibObject *object,
   const uint8_t *oid, size_t oidLen, MibVariant *value, size_t *valueLen);

error_t snmpVacmMibGetNextContextEntry(const MibObject *object,
   const uint8_t *oid, size_t oidLen, uint8_t *nextOid, size_t *nextOidLen);

error_t snmpVacmMibSetSecurityToGroupEntry(const MibObject *object, const uint8_t *oid,
   size_t oidLen, const MibVariant *value, size_t valueLen, bool_t commit);

error_t snmpVacmMibGetSecurityToGroupEntry(const MibObject *object,
   const uint8_t *oid, size_t oidLen, MibVariant *value, size_t *valueLen);

error_t snmpVacmMibGetNextSecurityToGroupEntry(const MibObject *object,
   const uint8_t *oid, size_t oidLen, uint8_t *nextOid, size_t *nextOidLen);

error_t snmpVacmMibSetAccessEntry(const MibObject *object, const uint8_t *oid,
   size_t oidLen, const MibVariant *value, size_t valueLen, bool_t commit);

error_t snmpVacmMibGetAccessEntry(const MibObject *object,
   const uint8_t *oid, size_t oidLen, MibVariant *value, size_t *valueLen);

error_t snmpVacmMibGetNextAccessEntry(const MibObject *object,
   const uint8_t *oid, size_t oidLen, uint8_t *nextOid, size_t *nextOidLen);

error_t snmpVacmMibSetViewSpinLock(const MibObject *object, const uint8_t *oid,
   size_t oidLen, const MibVariant *value, size_t valueLen, bool_t commit);

error_t snmpVacmMibGetViewSpinLock(const MibObject *object,
   const uint8_t *oid, size_t oidLen, MibVariant *value, size_t *valueLen);

error_t snmpVacmMibSetViewTreeFamilyEntry(const MibObject *object, const uint8_t *oid,
   size_t oidLen, const MibVariant *value, size_t valueLen, bool_t commit);

error_t snmpVacmMibGetViewTreeFamilyEntry(const MibObject *object,
   const uint8_t *oid, size_t oidLen, MibVariant *value, size_t *valueLen);

error_t snmpVacmMibGetNextViewTreeFamilyEntry(const MibObject *object,
   const uint8_t *oid, size_t oidLen, uint8_t *nextOid, size_t *nextOidLen);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
