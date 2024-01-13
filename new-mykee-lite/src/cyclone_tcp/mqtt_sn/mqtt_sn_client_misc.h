/**
 * @file mqtt_sn_client_misc.h
 * @brief Helper functions for MQTT-SN client
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

#ifndef _MQTT_SN_CLIENT_MISC_H
#define _MQTT_SN_CLIENT_MISC_H

//Dependencies
#include "core/net.h"
#include "mqtt_sn/mqtt_sn_client.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//MQTT-SN client related functions
error_t mqttSnClientProcessEvents(MqttSnClientContext *context,
   systime_t timeout);

MqttSnReturnCode mqttSnDeliverPublishMessage(MqttSnClientContext *context,
   MqttSnFlags flags, uint16_t topicId, const uint8_t *data, size_t dataLen);

error_t mqttSnClientAddTopic(MqttSnClientContext *context,
   const char_t *topicName, uint16_t topicId);

error_t mqttSnClientDeleteTopic(MqttSnClientContext *context,
   const char_t *topicName);

const char_t *mqttSnClientFindTopicId(MqttSnClientContext *context,
   uint16_t topicId);

uint16_t mqttSnClientFindTopicName(MqttSnClientContext *context,
   const char_t *topicName);

const char_t *mqttSnClientFindPredefTopicId(MqttSnClientContext *context,
   uint16_t topicId);

uint16_t mqttSnClientFindPredefTopicName(MqttSnClientContext *context,
   const char_t *topicName);

uint16_t mqttSnClientGenerateMessageId(MqttSnClientContext *context);

error_t mqttSnClientStoreMessageId(MqttSnClientContext *context,
   uint16_t msgId);

error_t mqttSnClientDiscardMessageId(MqttSnClientContext *context,
   uint16_t msgId);

bool_t mqttSnClientIsDuplicateMessageId(MqttSnClientContext *context,
   uint16_t msgId);

bool_t mqttSnClientIsShortTopicName(const char_t *topicName);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
