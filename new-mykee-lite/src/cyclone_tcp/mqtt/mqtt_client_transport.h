/**
 * @file mqtt_client_transport.h
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

#ifndef _MQTT_CLIENT_TRANSPORT_H
#define _MQTT_CLIENT_TRANSPORT_H

//Dependencies
#include "core/net.h"
#include "mqtt/mqtt_client.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//MQTT client related functions
error_t mqttClientOpenConnection(MqttClientContext *context);

error_t mqttClientEstablishConnection(MqttClientContext *context,
   const IpAddr *serverIpAddr, uint16_t serverPort);

error_t mqttClientShutdownConnection(MqttClientContext *context);

void mqttClientCloseConnection(MqttClientContext *context);

error_t mqttClientSendData(MqttClientContext *context,
   const void *data, size_t length, size_t *written, uint_t flags);

error_t mqttClientReceiveData(MqttClientContext *context,
   void *data, size_t size, size_t *received, uint_t flags);

error_t mqttClientWaitForData(MqttClientContext *context, systime_t timeout);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
