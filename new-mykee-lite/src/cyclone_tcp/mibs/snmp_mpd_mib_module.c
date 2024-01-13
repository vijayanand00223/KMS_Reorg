/**
 * @file snmp_mpd_mib_module.c
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
 * @section Description
 *
 * The SNMP-MPD-MIB defines managed objects for SNMP message processing and
 * dispatching. Refer to RFC 3412 for more details
 *
 * @author Oryx Embedded SARL (www.oryx-embedded.com)
 * @version 2.1.2
 **/

//Switch to the appropriate trace level
#define TRACE_LEVEL SNMP_TRACE_LEVEL

//Dependencies
#include "core/net.h"
#include "mibs/mib_common.h"
#include "mibs/snmp_mpd_mib_module.h"
#include "mibs/snmp_mpd_mib_impl.h"
#include "core/crypto.h"
#include "encoding/asn1.h"
#include "encoding/oid.h"
#include "debug.h"

//Check TCP/IP stack configuration
#if (SNMP_MPD_MIB_SUPPORT == ENABLED)


/**
 * @brief SNMP MPD MIB base
 **/

SnmpMpdMibBase snmpMpdMibBase;


/**
 * @brief SNMP MPD MIB objects
 **/

const MibObject snmpMpdMibObjects[] =
{
   //snmpUnknownSecurityModels object (1.3.6.1.6.3.11.2.1.1)
   {
      "snmpUnknownSecurityModels",
      {43, 6, 1, 6, 3, 11, 2, 1, 1},
      9,
      ASN1_CLASS_APPLICATION,
      MIB_TYPE_COUNTER32,
      MIB_ACCESS_READ_ONLY,
      &snmpMpdMibBase.snmpUnknownSecurityModels,
      NULL,
      sizeof(uint32_t),
      NULL,
      NULL,
      NULL
   },
   //snmpInvalidMsgs object (1.3.6.1.6.3.11.2.1.2)
   {
      "snmpInvalidMsgs",
      {43, 6, 1, 6, 3, 11, 2, 1, 2},
      9,
      ASN1_CLASS_APPLICATION,
      MIB_TYPE_COUNTER32,
      MIB_ACCESS_READ_ONLY,
      &snmpMpdMibBase.snmpInvalidMsgs,
      NULL,
      sizeof(uint32_t),
      NULL,
      NULL,
      NULL
   },
   //snmpUnknownPDUHandlers object (1.3.6.1.6.3.11.2.1.3)
   {
      "snmpUnknownPDUHandlers",
      {43, 6, 1, 6, 3, 11, 2, 1, 3},
      9,
      ASN1_CLASS_APPLICATION,
      MIB_TYPE_COUNTER32,
      MIB_ACCESS_READ_ONLY,
      &snmpMpdMibBase.snmpUnknownPDUHandlers,
      NULL,
      sizeof(uint32_t),
      NULL,
      NULL,
      NULL
   }
};


/**
 * @brief SNMP MPD MIB module
 **/

const MibModule snmpMpdMibModule =
{
   "SNMP-MPD-MIB",
   {43, 6, 1, 6, 3, 11},
   6,
   snmpMpdMibObjects,
   arraysize(snmpMpdMibObjects),
   snmpMpdMibInit,
   snmpMpdMibLoad,
   snmpMpdMibUnload,
   snmpMpdMibLock,
   snmpMpdMibUnlock
};

#endif
