/**
 * @file mqtt_client_misc.h
 * @brief Helper functions for MQTT client
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

#ifndef _MQTT_CLIENT_MISC_H
#define _MQTT_CLIENT_MISC_H

//Dependencies
#include "core/net.h"
#include "mqtt/mqtt_client.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//MQTT client related functions
void mqttClientChangeState(MqttClientContext *context,
   MqttClientState newState);

error_t mqttClientProcessEvents(MqttClientContext *context, systime_t timeout);
error_t mqttClientCheckKeepAlive(MqttClientContext *context);

error_t mqttSerializeHeader(uint8_t *buffer, size_t *pos, MqttPacketType type,
   bool_t dup, MqttQosLevel qos, bool_t retain, size_t remainingLen);

error_t mqttSerializeByte(uint8_t *buffer, size_t bufferLen,
   size_t *pos, uint8_t value);

error_t mqttSerializeShort(uint8_t *buffer, size_t bufferLen,
   size_t *pos, uint16_t value);

error_t mqttSerializeString(uint8_t *buffer, size_t bufferLen,
   size_t *pos, const void *string, size_t stringLen);

error_t mqttSerializeData(uint8_t *buffer, size_t bufferLen,
   size_t *pos, const void *data, size_t dataLen);

error_t mqttDeserializeHeader(uint8_t *buffer, size_t bufferLen, size_t *pos,
   MqttPacketType *type, bool_t *dup, MqttQosLevel *qos, bool_t *retain, size_t *remainingLen);

error_t mqttDeserializeByte(uint8_t *buffer, size_t bufferLen,
   size_t *pos, uint8_t *value);

error_t mqttDeserializeShort(uint8_t *buffer, size_t bufferLen,
   size_t *pos, uint16_t *value);

error_t mqttDeserializeString(uint8_t *buffer, size_t bufferLen,
   size_t *pos, char_t **string, size_t *stringLen);

error_t mqttClientCheckTimeout(MqttClientContext *context);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
