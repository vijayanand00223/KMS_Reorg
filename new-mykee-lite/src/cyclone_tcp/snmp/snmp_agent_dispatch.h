/**
 * @file snmp_agent_dispatch.h
 * @brief SNMP message dispatching
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

#ifndef _SNMP_AGENT_DISPATCH_H
#define _SNMP_AGENT_DISPATCH_H

//Dependencies
#include "core/net.h"
#include "snmp/snmp_agent.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//SNMP agent related functions
error_t snmpProcessMessage(SnmpAgentContext *context);

error_t snmpv1ProcessMessage(SnmpAgentContext *context);
error_t snmpv2cProcessMessage(SnmpAgentContext *context);
error_t snmpv3ProcessMessage(SnmpAgentContext *context);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
