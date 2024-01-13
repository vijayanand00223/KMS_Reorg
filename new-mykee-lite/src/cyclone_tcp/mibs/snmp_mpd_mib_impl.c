/**
 * @file snmp_mpd_mib_impl.c
 * @brief SNMP MPD MIB module implementation
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
 * @brief SNMP MPD MIB module initialization
 * @return Error code
 **/

error_t snmpMpdMibInit(void)
{
   //Debug message
   TRACE_INFO("Initializing SNMP MPD MIB base...\r\n");

   //Clear SNMP MPD MIB base
   osMemset(&snmpMpdMibBase, 0, sizeof(snmpMpdMibBase));

   //Successful processing
   return NO_ERROR;
}


/**
 * @brief Load SNMP MPD MIB module
 * @param[in] context Pointer to the SNMP agent context
 * @return Error code
 **/

error_t snmpMpdMibLoad(void *context)
{
   //Successful processing
   return NO_ERROR;
}


/**
 * @brief Unload SNMP MPD MIB module
 * @param[in] context Pointer to the SNMP agent context
 **/

void snmpMpdMibUnload(void *context)
{
}


/**
 * @brief Lock SNMP MPD MIB base
 **/

void snmpMpdMibLock(void)
{
}


/**
 * @brief Unlock SNMP MPD MIB base
 **/

void snmpMpdMibUnlock(void)
{
}

#endif
