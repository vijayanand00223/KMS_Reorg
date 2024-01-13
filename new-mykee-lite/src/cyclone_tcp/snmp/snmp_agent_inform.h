/**
 * @file snmp_agent_inform.h
 * @brief SNMP inform notifications
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

#ifndef _SNMP_AGENT_INFORM_H
#define _SNMP_AGENT_INFORM_H

//Dependencies
#include "core/net.h"
#include "snmp/snmp_agent.h"

//Inform notification support
#ifndef SNMP_AGENT_INFORM_SUPPORT
   #define SNMP_AGENT_INFORM_SUPPORT DISABLED
#elif (SNMP_AGENT_INFORM_SUPPORT != ENABLED && SNMP_AGENT_INFORM_SUPPORT != DISABLED)
   #error SNMP_AGENT_INFORM_SUPPORT parameter is not valid
#endif

//Maximum number of retransmissions of inform requests
#ifndef SNMP_AGENT_INFORM_MAX_RETRIES
   #define SNMP_AGENT_INFORM_MAX_RETRIES 5
#elif (SNMP_AGENT_INFORM_MAX_RETRIES < 1)
   #error SNMP_AGENT_INFORM_MAX_RETRIES parameter is not valid
#endif

//Inform request retransmission timeout
#ifndef SNMP_AGENT_INFORM_TIMEOUT
   #define SNMP_AGENT_INFORM_TIMEOUT 2000
#elif (SNMP_AGENT_INFORM_TIMEOUT < 1000)
   #error SNMP_AGENT_INFORM_TIMEOUT parameter is not valid
#endif

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief State of the inform sending process
 **/

typedef enum
{
   SNMP_AGENT_STATE_IDLE               = 0,
   SNMP_AGENT_STATE_SENDING_GET_REQ    = 1,
   SNMP_AGENT_STATE_WAITING_REPORT     = 2,
   SNMP_AGENT_STATE_SENDING_INFORM_REQ = 3,
   SNMP_AGENT_STATE_WAITING_GET_RESP   = 4
} SnmpAgentState;


//SNMP inform related functions
error_t snmpFormatInformRequestMessage(SnmpAgentContext *context,
   SnmpVersion version, const char_t *userName, uint_t genericTrapType,
   uint_t specificTrapCode, const SnmpTrapObject *objectList,
   uint_t objectListSize);

error_t snmpFormatInformRequestPdu(SnmpAgentContext *context,
   SnmpVersion version, const char_t *userName, uint_t genericTrapType,
   uint_t specificTrapCode, const SnmpTrapObject *objectList,
   uint_t objectListSize);

error_t snmpFormatGetRequestMessage(SnmpAgentContext *context,
   SnmpVersion version);

error_t snmpFormatGetRequestPdu(SnmpAgentContext *context, SnmpVersion version);

error_t snmpProcessGetResponsePdu(SnmpAgentContext *context);
error_t snmpProcessReportPdu(SnmpAgentContext *context);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
