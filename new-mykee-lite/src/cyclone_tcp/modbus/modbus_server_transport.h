/**
 * @file modbus_server_transport.h
 * @brief Transport protocol abstraction layer
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

#ifndef _MODBUS_SERVER_TRANSPORT_H
#define _MODBUS_SERVER_TRANSPORT_H

//Dependencies
#include "core/net.h"
#include "modbus/modbus_server.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//Modbus/TCP server related functions
void modbusServerAcceptConnection(ModbusServerContext *context);

error_t modbusServerShutdownConnection(ModbusClientConnection *connection);
void modbusServerCloseConnection(ModbusClientConnection *connection);

error_t modbusServerSendData(ModbusClientConnection *connection, const void *data,
   size_t length, size_t *written, uint_t flags);

error_t modbusServerReceiveData(ModbusClientConnection *connection, void *data,
   size_t size, size_t *received, uint_t flags);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
