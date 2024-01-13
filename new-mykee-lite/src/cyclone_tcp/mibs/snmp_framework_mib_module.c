/**
 * @file snmp_framework_mib_module.c
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
 * @section Description
 *
 * The SNMP-FRAMEWORK-MIB defines managed objects for SNMP management
 * frameworks. Refer to RFC 3411 for more details
 *
 * @author Oryx Embedded SARL (www.oryx-embedded.com)
 * @version 2.1.2
 **/

//Switch to the appropriate trace level
#define TRACE_LEVEL SNMP_TRACE_LEVEL

//Dependencies
#include "core/net.h"
#include "mibs/mib_common.h"
#include "mibs/snmp_framework_mib_module.h"
#include "mibs/snmp_framework_mib_impl.h"
#include "core/crypto.h"
#include "encoding/asn1.h"
#include "encoding/oid.h"
#include "debug.h"

//Check TCP/IP stack configuration
#if (SNMP_FRAMEWORK_MIB_SUPPORT == ENABLED)


/**
 * @brief SNMP FRAMEWORK MIB base
 **/

SnmpFrameworkMibBase snmpFrameworkMibBase;


/**
 * @brief SNMP FRAMEWORK MIB objects
 **/

const MibObject snmpFrameworkMibObjects[] =
{
   //snmpEngineID object (1.3.6.1.6.3.10.2.1.1)
   {
      "snmpEngineID",
      {43, 6, 1, 6, 3, 10, 2, 1, 1},
      9,
      ASN1_CLASS_UNIVERSAL,
      ASN1_TYPE_OCTET_STRING,
      MIB_ACCESS_READ_ONLY,
      NULL,
      NULL,
      0,
      NULL,
      snmpFrameworkMibGetSnmpEngineID,
      NULL
   },
   //snmpEngineBoots object (1.3.6.1.6.3.10.2.1.2)
   {
      "snmpEngineBoots",
      {43, 6, 1, 6, 3, 10, 2, 1, 2},
      9,
      ASN1_CLASS_UNIVERSAL,
      ASN1_TYPE_INTEGER,
      MIB_ACCESS_READ_ONLY,
      NULL,
      NULL,
      sizeof(int32_t),
      NULL,
      snmpFrameworkMibGetSnmpEngineBoots,
      NULL
   },
   //snmpEngineTime object (1.3.6.1.6.3.10.2.1.3)
   {
      "snmpEngineTime",
      {43, 6, 1, 6, 3, 10, 2, 1, 3},
      9,
      ASN1_CLASS_UNIVERSAL,
      ASN1_TYPE_INTEGER,
      MIB_ACCESS_READ_ONLY,
      NULL,
      NULL,
      sizeof(int32_t),
      NULL,
      snmpFrameworkMibGetSnmpEngineTime,
      NULL
   },
   //snmpEngineMaxMessageSize object (1.3.6.1.6.3.10.2.1.4)
   {
      "snmpEngineMaxMessageSize",
      {43, 6, 1, 6, 3, 10, 2, 1, 4},
      9,
      ASN1_CLASS_UNIVERSAL,
      ASN1_TYPE_INTEGER,
      MIB_ACCESS_READ_ONLY,
      NULL,
      NULL,
      sizeof(int32_t),
      NULL,
      snmpFrameworkMibGetSnmpEngineMaxMessageSize,
      NULL
   }
};


/**
 * @brief SNMP FRAMEWORK MIB module
 **/

const MibModule snmpFrameworkMibModule =
{
   "SNMP-FRAMEWORK-MIB",
   {43, 6, 1, 6, 3, 10},
   6,
   snmpFrameworkMibObjects,
   arraysize(snmpFrameworkMibObjects),
   snmpFrameworkMibInit,
   snmpFrameworkMibLoad,
   snmpFrameworkMibUnload,
   snmpFrameworkMibLock,
   snmpFrameworkMibUnlock
};

#endif
