/**
 * @file snmp_mpd_mib_impl.h
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

#ifndef _SNMP_MPD_MIB_IMPL_H
#define _SNMP_MPD_MIB_IMPL_H

//Dependencies
#include "mibs/mib_common.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//SNMP MPD MIB related functions
error_t snmpMpdMibInit(void);
error_t snmpMpdMibLoad(void *context);
void snmpMpdMibUnload(void *context);
void snmpMpdMibLock(void);
void snmpMpdMibUnlock(void);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
