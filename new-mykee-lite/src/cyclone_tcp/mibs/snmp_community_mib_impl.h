/**
 * @file snmp_community_mib_impl.h
 * @brief SNMP COMMUNITY MIB module implementation
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

#ifndef _SNMP_COMMUNITY_MIB_IMPL_H
#define _SNMP_COMMUNITY_MIB_IMPL_H

//Dependencies
#include "mibs/mib_common.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//SNMP COMMUNITY MIB related functions
error_t snmpCommunityMibInit(void);
error_t snmpCommunityMibLoad(void *context);
void snmpCommunityMibUnload(void *context);
void snmpCommunityMibLock(void);
void snmpCommunityMibUnlock(void);

error_t snmpCommunityMibSetCommunityEntry(const MibObject *object, const uint8_t *oid,
   size_t oidLen, const MibVariant *value, size_t valueLen, bool_t commit);

error_t snmpCommunityMibGetCommunityEntry(const MibObject *object, const uint8_t *oid,
   size_t oidLen, MibVariant *value, size_t *valueLen);

error_t snmpCommunityMibGetNextCommunityEntry(const MibObject *object, const uint8_t *oid,
   size_t oidLen, uint8_t *nextOid, size_t *nextOidLen);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
