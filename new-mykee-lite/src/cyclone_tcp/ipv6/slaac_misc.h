/**
 * @file slaac_misc.h
 * @brief Helper functions for SLAAC
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

#ifndef _SLAAC_MISC_H
#define _SLAAC_MISC_H

//Dependencies
#include "core/net.h"
#include "ipv6/slaac.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//SLAAC related functions
void slaacLinkChangeEvent(SlaacContext *context);

void slaacParseRouterAdv(SlaacContext *context,
   NdpRouterAdvMessage *message, size_t length);

void slaacParsePrefixInfoOption(SlaacContext *context,
   NdpPrefixInfoOption *option);

error_t slaacGenerateLinkLocalAddr(SlaacContext *context);

void slaacDumpConfig(SlaacContext *context);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
