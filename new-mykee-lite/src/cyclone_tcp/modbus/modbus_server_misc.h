/**
 * @file modbus_server_misc.h
 * @brief Helper functions for Modbus/TCP server
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

#ifndef _MODBUS_SERVER_MISC_H
#define _MODBUS_SERVER_MISC_H

//Dependencies
#include "core/net.h"
#include "modbus/modbus_server.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//Modbus/TCP server related functions
void modbusServerTick(ModbusServerContext *context);

void modbusServerRegisterConnectionEvents(ModbusClientConnection *connection,
   SocketEventDesc *eventDesc);

void modbusServerProcessConnectionEvents(ModbusClientConnection *connection);

error_t modbusServerParseMbapHeader(ModbusClientConnection *connection);

error_t modbusServerFormatMbapHeader(ModbusClientConnection *connection,
   size_t length);

void *modbusServerGetRequestPdu(ModbusClientConnection *connection,
   size_t *length);

void *modbusServerGetResponsePdu(ModbusClientConnection *connection);

void modbusServerLock(ModbusClientConnection *connection);
void modbusServerUnlock(ModbusClientConnection *connection);

error_t modbusServerReadCoil(ModbusClientConnection *connection,
   uint16_t address, bool_t *state);

error_t modbusServerReadDiscreteInput(ModbusClientConnection *connection,
   uint16_t address, bool_t *state);

error_t modbusServerWriteCoil(ModbusClientConnection *connection,
   uint16_t address, bool_t state, bool_t commit);

error_t modbusServerReadHoldingReg(ModbusClientConnection *connection,
   uint16_t address, uint16_t *value);

error_t modbusServerReadInputReg(ModbusClientConnection *connection,
   uint16_t address, uint16_t *value);

error_t modbusServerWriteReg(ModbusClientConnection *connection,
   uint16_t address, uint16_t value, bool_t commit);

ModbusExceptionCode modbusServerTranslateExceptionCode(error_t status);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
