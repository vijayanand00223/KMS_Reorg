/**
 * @file modbus_client_pdu.h
 * @brief Modbus PDU formatting and parsing
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

#ifndef _MODBUS_CLIENT_PDU_H
#define _MODBUS_CLIENT_PDU_H

//Dependencies
#include "core/net.h"
#include "modbus/modbus_client.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//Modbus/TCP client related functions
error_t modbusClientFormatReadCoilsReq(ModbusClientContext *context,
   uint16_t address, uint_t quantity);

error_t modbusClientFormatReadDiscreteInputsReq(ModbusClientContext *context,
   uint16_t address, uint_t quantity);

error_t modbusClientFormatReadHoldingRegsReq(ModbusClientContext *context,
   uint16_t address, uint_t quantity);

error_t modbusClientFormatReadInputRegsReq(ModbusClientContext *context,
   uint16_t address, uint_t quantity);

error_t modbusClientFormatWriteSingleCoilReq(ModbusClientContext *context,
   uint16_t address, bool_t value);

error_t modbusClientFormatWriteSingleRegReq(ModbusClientContext *context,
   uint16_t address, uint16_t value);

error_t modbusClientFormatWriteMultipleCoilsReq(ModbusClientContext *context,
   uint16_t address, uint_t quantity, const uint8_t *value);

error_t modbusClientFormatWriteMultipleRegsReq(ModbusClientContext *context,
   uint16_t address, uint_t quantity, const uint16_t *value);

error_t modbusClientFormatMaskWriteRegReq(ModbusClientContext *context,
   uint16_t address, uint16_t andMask, uint16_t orMask);

error_t modbusClientFormatReadWriteMultipleRegsReq(ModbusClientContext *context,
   uint16_t readAddress, uint16_t readQuantity, uint16_t writeAddress,
   uint16_t writeQuantity, const uint16_t *writeValue);

error_t modbusClientParseReadCoilsResp(ModbusClientContext *context,
   uint_t quantity, uint8_t *value);

error_t modbusClientParseReadDiscreteInputsResp(ModbusClientContext *context,
   uint_t quantity, uint8_t *value);

error_t modbusClientParseReadHoldingRegsResp(ModbusClientContext *context,
   uint_t quantity, uint16_t *value);

error_t modbusClientParseReadInputRegsResp(ModbusClientContext *context,
   uint_t quantity, uint16_t *value);

error_t modbusClientParseWriteSingleCoilResp(ModbusClientContext *context,
   uint16_t address, bool_t value);

error_t modbusClientParseWriteSingleRegResp(ModbusClientContext *context,
   uint16_t address, uint16_t value);

error_t modbusClientParseWriteMultipleCoilsResp(ModbusClientContext *context,
   uint16_t address, uint_t quantity);

error_t modbusClientParseWriteMultipleRegsResp(ModbusClientContext *context,
   uint16_t address, uint_t quantity);

error_t modbusClientParseMaskWriteRegResp(ModbusClientContext *context,
   uint16_t address, uint16_t andMask, uint16_t orMask);

error_t modbusClientParseReadWriteMultipleRegsResp(ModbusClientContext *context,
   uint_t readQuantity, uint16_t *readValue);

error_t modbusClientParseExceptionResp(ModbusClientContext *context);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
