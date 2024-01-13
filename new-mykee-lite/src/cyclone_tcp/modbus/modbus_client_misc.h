/**
 * @file modbus_client_misc.h
 * @brief Helper functions for Modbus/TCP client
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

#ifndef _MODBUS_CLIENT_MISC_H
#define _MODBUS_CLIENT_MISC_H

//Dependencies
#include "core/net.h"
#include "modbus/modbus_client.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//Modbus/TCP client related functions
error_t modbusClientTransaction(ModbusClientContext *context);
error_t modbusClientCheckResp(ModbusClientContext *context);

error_t modbusClientFormatMbapHeader(ModbusClientContext *context,
   size_t length);

error_t modbusClientParseMbapHeader(ModbusClientContext *context);

void *modbusClientGetRequestPdu(ModbusClientContext *context);
void *modbusClientGetResponsePdu(ModbusClientContext *context, size_t *length);

error_t modbusClientCheckTimeout(ModbusClientContext *context);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
