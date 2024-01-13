/**
 * @file tcp_mib_module.h
 * @brief TCP MIB module
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

#ifndef _TCP_MIB_MODULE_H
#define _TCP_MIB_MODULE_H

//Dependencies
#include "mibs/mib_common.h"

//TCP MIB module support
#ifndef TCP_MIB_SUPPORT
   #define TCP_MIB_SUPPORT DISABLED
#elif (TCP_MIB_SUPPORT != ENABLED && TCP_MIB_SUPPORT != DISABLED)
   #error TCP_MIB_SUPPORT parameter is not valid
#endif

//Macro definitions
#if (TCP_MIB_SUPPORT == ENABLED)
   #define TCP_MIB_INC_COUNTER32(name, value) tcpMibBase.name += value
   #define TCP_MIB_INC_COUNTER64(name, value) tcpMibBase.name += value
#else
   #define TCP_MIB_INC_COUNTER32(name, value)
   #define TCP_MIB_INC_COUNTER64(name, value)
#endif

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief RTO calculation algorithm
 **/

typedef enum
{
   TCP_MIB_RTO_ALGORITHM_OTHER    = 1,
   TCP_MIB_RTO_ALGORITHM_CONSTANT = 2,
   TCP_MIB_RTO_ALGORITHM_RSRE     = 3,
   TCP_MIB_RTO_ALGORITHM_VANJ     = 4
} TcpMibRtoAlgorithm;


/**
 * @brief TCP connection states
 **/

typedef enum
{
   TCP_MIB_CONN_STATE_CLOSED       = 1,
   TCP_MIB_CONN_STATE_LISTEN       = 2,
   TCP_MIB_CONN_STATE_SYN_SENT     = 3,
   TCP_MIB_CONN_STATE_SYN_RECEIVED = 4,
   TCP_MIB_CONN_STATE_ESTABLISHED  = 5,
   TCP_MIB_CONN_STATE_FIN_WAIT_1   = 6,
   TCP_MIB_CONN_STATE_FIN_WAIT_2   = 7,
   TCP_MIB_CONN_STATE_CLOSE_WAIT   = 8,
   TCP_MIB_CONN_STATE_LAST_ACK     = 9,
   TCP_MIB_CONN_STATE_CLOSING      = 10,
   TCP_MIB_CONN_STATE_TIME_WAIT    = 11,
   TCP_MIB_CONN_STATE_DELETE_TCB   = 12
} TcpMibConnState;


/**
 * @brief TCP MIB base
 **/

typedef struct
{
   int32_t tcpRtoAlgorithm;
   int32_t tcpRtoMin;
   int32_t tcpRtoMax;
   int32_t tcpMaxConn;
   uint32_t tcpActiveOpens;
   uint32_t tcpPassiveOpens;
   uint32_t tcpAttemptFails;
   uint32_t tcpEstabResets;
   uint32_t tcpInSegs;
   uint32_t tcpOutSegs;
   uint32_t tcpRetransSegs;
   uint32_t tcpInErrs;
   uint32_t tcpOutRsts;
   uint64_t tcpHCInSegs;
   uint64_t tcpHCOutSegs;
} TcpMibBase;


//TCP MIB related constants
extern TcpMibBase tcpMibBase;
extern const MibObject tcpMibObjects[];
extern const MibModule tcpMibModule;

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
