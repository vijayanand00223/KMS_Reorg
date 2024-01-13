/**
 * @file mqtt_sn_client_transport.h
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

#ifndef _MQTT_SN_CLIENT_TRANSPORT_H
#define _MQTT_SN_CLIENT_TRANSPORT_H

//Dependencies
#include "core/net.h"
#include "mqtt_sn/mqtt_sn_client.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//MQTT-SN related functions
error_t mqttSnClientOpenConnection(MqttSnClientContext *context, bool_t secure);
error_t mqttSnClientEstablishConnection(MqttSnClientContext *context);
error_t mqttSnClientShutdownConnection(MqttSnClientContext *context);
void mqttSnClientCloseConnection(MqttSnClientContext *context);

error_t mqttSnClientBroadcastDatagram(MqttSnClientContext *context,
   const IpAddr *destIpAddr, uint16_t destPort, const void *data,
   size_t length);

error_t mqttSnClientSendDatagram(MqttSnClientContext *context,
   const void *data, size_t length);

error_t mqttSnClientReceiveDatagram(MqttSnClientContext *context,
   IpAddr *srcIpAddr, uint16_t *srcPort, void *data, size_t size,
   size_t *received, systime_t timeout);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
