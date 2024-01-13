/**
 * @file udp_mib_module.c
 * @brief UDP MIB module
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
 * The UDP-MIB describes managed objects used for implementations of
 * the User Datagram Protocol (UDP) in an IP version independent manner.
 * Refer to the following RFCs for complete details:
 * - RFC 4113: MIB for the User Datagram Protocol (UDP)
 * - RFC 4001: Textual Conventions for Internet Network Addresses
 *
 * @author Oryx Embedded SARL (www.oryx-embedded.com)
 * @version 2.1.2
 **/

//Switch to the appropriate trace level
#define TRACE_LEVEL SNMP_TRACE_LEVEL

//Dependencies
#include "core/net.h"
#include "mibs/mib_common.h"
#include "mibs/udp_mib_module.h"
#include "mibs/udp_mib_impl.h"
#include "core/crypto.h"
#include "encoding/asn1.h"
#include "encoding/oid.h"
#include "debug.h"

//Check TCP/IP stack configuration
#if (UDP_MIB_SUPPORT == ENABLED && UDP_SUPPORT == ENABLED)


/**
 * @brief UDP MIB base
 **/

UdpMibBase udpMibBase;


/**
 * @brief UDP MIB objects
 **/

const MibObject udpMibObjects[] =
{
   //udpInDatagrams object (1.3.6.1.2.1.7.1)
   {
      "udpInDatagrams",
      {43, 6, 1, 2, 1, 7, 1},
      7,
      ASN1_CLASS_APPLICATION,
      MIB_TYPE_COUNTER32,
      MIB_ACCESS_READ_ONLY,
      &udpMibBase.udpInDatagrams,
      NULL,
      sizeof(uint32_t),
      NULL,
      NULL,
      NULL
   },
   //udpNoPorts object (1.3.6.1.2.1.7.2)
   {
      "udpNoPorts",
      {43, 6, 1, 2, 1, 7, 2},
      7,
      ASN1_CLASS_APPLICATION,
      MIB_TYPE_COUNTER32,
      MIB_ACCESS_READ_ONLY,
      &udpMibBase.udpNoPorts,
      NULL,
      sizeof(uint32_t),
      NULL,
      NULL,
      NULL
   },
   //udpInErrors object (1.3.6.1.2.1.7.3)
   {
      "udpInErrors",
      {43, 6, 1, 2, 1, 7, 3},
      7,
      ASN1_CLASS_APPLICATION,
      MIB_TYPE_COUNTER32,
      MIB_ACCESS_READ_ONLY,
      &udpMibBase.udpInErrors,
      NULL,
      sizeof(uint32_t),
      NULL,
      NULL,
      NULL
   },
   //udpOutDatagrams object (1.3.6.1.2.1.7.4)
   {
      "udpOutDatagrams",
      {43, 6, 1, 2, 1, 7, 4},
      7,
      ASN1_CLASS_APPLICATION,
      MIB_TYPE_COUNTER32,
      MIB_ACCESS_READ_ONLY,
      &udpMibBase.udpOutDatagrams,
      NULL,
      sizeof(uint32_t),
      NULL,
      NULL,
      NULL
   },
   //udpEndpointProcess object (1.3.6.1.2.1.7.7.1.8)
   {
      "udpEndpointProcess",
      {43, 6, 1, 2, 1, 7, 7, 1, 8},
      9,
      ASN1_CLASS_APPLICATION,
      MIB_TYPE_UNSIGNED32,
      MIB_ACCESS_READ_ONLY,
      NULL,
      NULL,
      sizeof(uint32_t),
      NULL,
      udpMibGetUdpEndpointEntry,
      udpMibGetNextUdpEndpointEntry
   },
   //udpHCInDatagrams object (1.3.6.1.2.1.7.8)
   {
      "udpHCInDatagrams",
      {43, 6, 1, 2, 1, 7, 8},
      7,
      ASN1_CLASS_APPLICATION,
      MIB_TYPE_COUNTER64,
      MIB_ACCESS_READ_ONLY,
      &udpMibBase.udpHCInDatagrams,
      NULL,
      sizeof(uint64_t),
      NULL,
      NULL,
      NULL
   },
   //udpHCOutDatagrams object (1.3.6.1.2.1.7.9)
   {
      "udpHCOutDatagrams",
      {43, 6, 1, 2, 1, 7, 9},
      7,
      ASN1_CLASS_APPLICATION,
      MIB_TYPE_COUNTER64,
      MIB_ACCESS_READ_ONLY,
      &udpMibBase.udpHCOutDatagrams,
      NULL,
      sizeof(uint64_t),
      NULL,
      NULL,
      NULL
   }
};


/**
 * @brief UDP MIB module
 **/

const MibModule udpMibModule =
{
   "UDP-MIB",
   {43, 6, 1, 2, 1, 50},
   6,
   udpMibObjects,
   arraysize(udpMibObjects),
   udpMibInit,
   NULL,
   NULL,
   NULL,
   NULL
};

#endif
