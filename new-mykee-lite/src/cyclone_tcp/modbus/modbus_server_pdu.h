/**
 * @file modbus_server_pdu.h
 * @brief Modbus PDU processing
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

#ifndef _MODBUS_SERVER_PDU_H
#define _MODBUS_SERVER_PDU_H

//Dependencies
#include "core/net.h"
#include "modbus/modbus_server.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//Modbus/TCP server related functions
error_t modbusServerProcessRequest(ModbusClientConnection *connection);

error_t modbusServerProcessReadCoilsReq(ModbusClientConnection *connection,
   const ModbusReadCoilsReq *request, size_t length);

error_t modbusServerProcessReadDiscreteInputsReq(ModbusClientConnection *connection,
   const ModbusReadDiscreteInputsReq *request, size_t length);

error_t modbusServerProcessReadHoldingRegsReq(ModbusClientConnection *connection,
   const ModbusReadHoldingRegsReq *request, size_t length);

error_t modbusServerProcessReadInputRegsReq(ModbusClientConnection *connection,
   const ModbusReadInputRegsReq *request, size_t length);

error_t modbusServerProcessWriteSingleCoilReq(ModbusClientConnection *connection,
   const ModbusWriteSingleCoilReq *request, size_t length);

error_t modbusServerProcessWriteSingleRegReq(ModbusClientConnection *connection,
   const ModbusWriteSingleRegReq *request, size_t length);

error_t modbusServerProcessWriteMultipleCoilsReq(ModbusClientConnection *connection,
   const ModbusWriteMultipleCoilsReq *request, size_t length);

error_t modbusServerProcessWriteMultipleRegsReq(ModbusClientConnection *connection,
   const ModbusWriteMultipleRegsReq *request, size_t length);

error_t modbusServerProcessMaskWriteRegReq(ModbusClientConnection *connection,
   const ModbusMaskWriteRegReq *request, size_t length);

error_t modbusServerProcessReadWriteMultipleRegsReq(ModbusClientConnection *connection,
   const ModbusReadWriteMultipleRegsReq *request, size_t length);

error_t modbusServerFormatExceptionResp(ModbusClientConnection *connection,
   ModbusFunctionCode functionCode, ModbusExceptionCode exceptionCode);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
