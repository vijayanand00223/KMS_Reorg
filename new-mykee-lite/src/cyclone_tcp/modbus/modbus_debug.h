/**
 * @file modbus_debug.h
 * @brief Data logging functions for debugging purpose (Modbus/TCP)
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

#ifndef _MODBUS_DEBUG_H
#define _MODBUS_DEBUG_H

//Dependencies
#include "core/net.h"
#include "modbus/modbus_common.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//Modbus/TCP related functions
error_t modbusDumpRequestPdu(const void *pdu, size_t length);
error_t modbusDumpResponsePdu(const void *pdu, size_t length);

error_t modbusDumpReadCoilsReq(const ModbusReadCoilsReq *request,
   size_t length);

error_t modbusDumpReadCoilsResp(const ModbusReadCoilsResp *response,
   size_t length);

error_t modbusDumpReadDiscreteInputsReq(const ModbusReadDiscreteInputsReq *request,
   size_t length);

error_t modbusDumpReadDiscreteInputsResp(const ModbusReadDiscreteInputsResp *response,
   size_t length);

error_t modbusDumpReadHoldingRegsReq(const ModbusReadHoldingRegsReq *request,
   size_t length);

error_t modbusDumpReadHoldingRegsResp(const ModbusReadHoldingRegsResp *response,
   size_t length);

error_t modbusDumpReadInputRegsReq(const ModbusReadInputRegsReq *request,
   size_t length);

error_t modbusDumpReadInputRegsResp(const ModbusReadInputRegsResp *response,
   size_t length);

error_t modbusDumpWriteSingleCoilReq(const ModbusWriteSingleCoilReq *request,
   size_t length);

error_t modbusDumpWriteSingleCoilResp(const ModbusWriteSingleCoilResp *response,
   size_t length);

error_t modbusDumpWriteSingleRegReq(const ModbusWriteSingleRegReq *request,
   size_t length);

error_t modbusDumpWriteSingleRegResp(const ModbusWriteSingleRegResp *response,
   size_t length);

error_t modbusDumpWriteMultipleCoilsReq(const ModbusWriteMultipleCoilsReq *request,
   size_t length);

error_t modbusDumpWriteMultipleCoilsResp(const ModbusWriteMultipleCoilsResp *response,
   size_t length);

error_t modbusDumpWriteMultipleRegsReq(const ModbusWriteMultipleRegsReq *request,
   size_t length);

error_t modbusDumpWriteMultipleRegsResp(const ModbusWriteMultipleRegsResp *response,
   size_t length);

error_t modbusDumpMaskWriteRegReq(const ModbusMaskWriteRegReq *request,
   size_t length);

error_t modbusDumpMaskWriteRegResp(const ModbusMaskWriteRegResp *response,
   size_t length);

error_t modbusDumpReadWriteMultipleRegsReq(const ModbusReadWriteMultipleRegsReq *request,
   size_t length);

error_t modbusDumpReadWriteMultipleRegsResp(const ModbusReadWriteMultipleRegsResp *response,
   size_t length);

error_t modbusDumpExceptionResp(const ModbusExceptionResp *response,
   size_t length);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
