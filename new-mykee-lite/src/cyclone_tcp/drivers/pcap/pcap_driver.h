/**
 * @file pcap_driver.h
 * @brief PCAP driver
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

#ifndef _PCAP_DRIVER_H
#define _PCAP_DRIVER_H

//Dependencies
#include "core/nic.h"

//Maximum packet size
#ifndef PCAP_DRIVER_MAX_PACKET_SIZE
   #define PCAP_DRIVER_MAX_PACKET_SIZE 1536
#elif (PCAP_DRIVER_MAX_PACKET_SIZE < 1)
   #error PCAP_DRIVER_MAX_PACKET_SIZE parameter is not valid
#endif

//Maximum number of packets in the receive queue
#ifndef PCAP_DRIVER_QUEUE_SIZE
   #define PCAP_DRIVER_QUEUE_SIZE 64
#elif (PCAP_DRIVER_QUEUE_SIZE < 1)
   #error PCAP_DRIVER_QUEUE_SIZE parameter is not valid
#endif

//Receive timeout in milliseconds
#ifndef PCAP_DRIVER_TIMEOUT
   #define PCAP_DRIVER_TIMEOUT 1
#elif (PCAP_DRIVER_TIMEOUT < 1)
   #error PCAP_DRIVER_TIMEOUT parameter is not valid
#endif

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//PCAP driver
extern const NicDriver pcapDriver;

//PCAP related functions
error_t pcapDriverInit(NetInterface *interface);

void pcapDriverTick(NetInterface *interface);

void pcapDriverEnableIrq(NetInterface *interface);
void pcapDriverDisableIrq(NetInterface *interface);

void pcapDriverEventHandler(NetInterface *interface);

error_t pcapDriverSendPacket(NetInterface *interface,
   const NetBuffer *buffer, size_t offset, NetTxAncillary *ancillary);

error_t pcapDriverUpdateMacAddrFilter(NetInterface *interface);

void pcapDriverTask(NetInterface *interface);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
