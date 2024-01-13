/**
 * @file ip.h
 * @brief IPv4 and IPv6 common routines
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

#ifndef _IP_H
#define _IP_H

//Dependencies
#include "ipv4/ipv4.h"
#include "ipv6/ipv6.h"

//DiffServ support
#ifndef IP_DIFF_SERV_SUPPORT
   #define IP_DIFF_SERV_SUPPORT DISABLED
#elif (IP_DIFF_SERV_SUPPORT != ENABLED && IP_DIFF_SERV_SUPPORT != DISABLED)
   #error IP_DIFF_SERV_SUPPORT parameter is not valid
#endif

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief IP supported protocols
 **/

typedef enum
{
   IP_PROTOCOL_TCP  = 6,
   IP_PROTOCOL_UDP  = 17
} IpProtocol;


/**
 * @brief Flags used by I/O functions
 **/

typedef enum
{
   IP_FLAG_DONT_ROUTE = 0x0400,
   IP_FLAG_TTL        = 0x00FF,
   IP_FLAG_HOP_LIMIT  = 0x00FF
} IpFlags;


/**
 * @brief IP network address
 **/

typedef struct
{
   size_t length;
   union
   {
#if (IPV4_SUPPORT == ENABLED)
      Ipv4Addr ipv4Addr;
#endif
#if (IPV6_SUPPORT == ENABLED)
      Ipv6Addr ipv6Addr;
#endif
   };
} IpAddr;


/**
 * @brief IP pseudo header
 **/

typedef struct
{
   size_t length;
   union
   {
#if (IPV4_SUPPORT == ENABLED)
      Ipv4PseudoHeader ipv4Data;
#endif
#if (IPV6_SUPPORT == ENABLED)
      Ipv6PseudoHeader ipv6Data;
#endif
      uint8_t data[4];
   };
} IpPseudoHeader;


//IP related constants
extern const IpAddr IP_ADDR_ANY;
extern const IpAddr IP_ADDR_UNSPECIFIED;

//IP related functions
error_t ipSendDatagram(NetInterface *interface, IpPseudoHeader *pseudoHeader,
   NetBuffer *buffer, size_t offset, NetTxAncillary *ancillary);

error_t ipSelectSourceAddr(NetInterface **interface,
   const IpAddr *destAddr, IpAddr *srcAddr);

bool_t ipCompAddr(const IpAddr *ipAddr1, const IpAddr *ipAddr2);
bool_t ipIsUnspecifiedAddr(const IpAddr *ipAddr);
bool_t ipIsMulticastAddr(const IpAddr *ipAddr);

error_t ipJoinMulticastGroup(NetInterface *interface, const IpAddr *groupAddr);
error_t ipLeaveMulticastGroup(NetInterface *interface, const IpAddr *groupAddr);

uint16_t ipCalcChecksum(const void *data, size_t length);
uint16_t ipCalcChecksumEx(const NetBuffer *buffer, size_t offset, size_t length);

uint16_t ipCalcUpperLayerChecksum(const void *pseudoHeader,
   size_t pseudoHeaderLen, const void *data, size_t dataLen);

uint16_t ipCalcUpperLayerChecksumEx(const void *pseudoHeader,
   size_t pseudoHeaderLen, const NetBuffer *buffer, size_t offset, size_t length);

NetBuffer *ipAllocBuffer(size_t length, size_t *offset);

error_t ipStringToAddr(const char_t *str, IpAddr *ipAddr);
char_t *ipAddrToString(const IpAddr *ipAddr, char_t *str);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
