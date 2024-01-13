/**
 * @file mqtt_client_packet.h
 * @brief MQTT packet parsing and formatting
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

#ifndef _MQTT_CLIENT_PACKET_H
#define _MQTT_CLIENT_PACKET_H

//Dependencies
#include "core/net.h"
#include "mqtt/mqtt_client.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//MQTT client related functions
error_t mqttClientReceivePacket(MqttClientContext *context);
error_t mqttClientProcessPacket(MqttClientContext *context);

error_t mqttClientProcessConnAck(MqttClientContext *context,
   bool_t dup, MqttQosLevel qos, bool_t retain, size_t remainingLen);

error_t mqttClientProcessPubAck(MqttClientContext *context,
   bool_t dup, MqttQosLevel qos, bool_t retain, size_t remainingLen);

error_t mqttClientProcessPublish(MqttClientContext *context,
   bool_t dup, MqttQosLevel qos, bool_t retain, size_t remainingLen);

error_t mqttClientProcessPubRec(MqttClientContext *context,
   bool_t dup, MqttQosLevel qos, bool_t retain, size_t remainingLen);

error_t mqttClientProcessPubRel(MqttClientContext *context,
   bool_t dup, MqttQosLevel qos, bool_t retain, size_t remainingLen);

error_t mqttClientProcessPubComp(MqttClientContext *context,
   bool_t dup, MqttQosLevel qos, bool_t retain, size_t remainingLen);

error_t mqttClientProcessSubAck(MqttClientContext *context,
   bool_t dup, MqttQosLevel qos, bool_t retain, size_t remainingLen);

error_t mqttClientProcessUnsubAck(MqttClientContext *context,
   bool_t dup, MqttQosLevel qos, bool_t retain, size_t remainingLen);

error_t mqttClientProcessPingResp(MqttClientContext *context,
   bool_t dup, MqttQosLevel qos, bool_t retain, size_t remainingLen);

error_t mqttClientFormatConnect(MqttClientContext *context,
   bool_t cleanSession);

error_t mqttClientFormatPublish(MqttClientContext *context, const char_t *topic,
   const void *message, size_t length, MqttQosLevel qos, bool_t retain);

error_t mqttClientFormatPubAck(MqttClientContext *context, uint16_t packetId);
error_t mqttClientFormatPubRec(MqttClientContext *context, uint16_t packetId);
error_t mqttClientFormatPubRel(MqttClientContext *context, uint16_t packetId);
error_t mqttClientFormatPubComp(MqttClientContext *context, uint16_t packetId);

error_t mqttClientFormatSubscribe(MqttClientContext *context,
   const char_t *topic, MqttQosLevel qos);

error_t mqttClientFormatUnsubscribe(MqttClientContext *context,
   const char_t *topic);

error_t mqttClientFormatPingReq(MqttClientContext *context);
error_t mqttClientFormatDisconnect(MqttClientContext *context);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
