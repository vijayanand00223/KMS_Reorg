/**
 * @file snmp_agent_pdu.h
 * @brief SNMP agent (PDU processing)
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

#ifndef _SNMP_AGENT_PDU_H
#define _SNMP_AGENT_PDU_H

//Dependencies
#include "core/net.h"
#include "snmp/snmp_agent.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//SNMP agent related functions
error_t snmpProcessPdu(SnmpAgentContext *context);

error_t snmpProcessGetRequestPdu(SnmpAgentContext *context);
error_t snmpProcessGetBulkRequestPdu(SnmpAgentContext *context);
error_t snmpProcessSetRequestPdu(SnmpAgentContext *context);

error_t snmpFormatReportPdu(SnmpAgentContext *context, error_t errorIndication);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
