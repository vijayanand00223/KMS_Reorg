/**
 * @file modbus_server_security.h
 * @brief Modbus/TCP security layer
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

#ifndef _MODBUS_SERVER_SECURITY_H
#define _MODBUS_SERVER_SECURITY_H

//Dependencies
#include "core/net.h"
#include "modbus/modbus_server.h"

//TLS supported?
#if (MODBUS_SERVER_TLS_SUPPORT == ENABLED)
   #include "core/crypto.h"
   #include "encoding/asn1.h"
   #include "encoding/oid.h"
   #include "pkix/x509_cert_parse.h"
#endif

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//Modbus/TCP server related functions
error_t modbusServerParseRoleOid(ModbusClientConnection *connection,
   const uint8_t *data, size_t length);

error_t modbusServerOpenSecureConnection(ModbusServerContext *context,
   ModbusClientConnection *connection);

error_t modbusServerEstablishSecureConnection(ModbusClientConnection *connection);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
