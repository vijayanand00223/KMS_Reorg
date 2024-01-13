/**
 * @file sntp_client.h
 * @brief SNTP client (Simple Network Time Protocol)
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

#ifndef _SNTP_CLIENT_H
#define _SNTP_CLIENT_H

//Dependencies
#include "core/net.h"
#include "sntp/ntp_common.h"

//SNTP client support
#ifndef SNTP_CLIENT_SUPPORT
   #define SNTP_CLIENT_SUPPORT ENABLED
#elif (SNTP_CLIENT_SUPPORT != ENABLED && SNTP_CLIENT_SUPPORT != DISABLED)
   #error SNTP_CLIENT_SUPPORT parameter is not valid
#endif

//Default timeout
#ifndef SNTP_CLIENT_DEFAULT_TIMEOUT
   #define SNTP_CLIENT_DEFAULT_TIMEOUT 30000
#elif (SNTP_CLIENT_DEFAULT_TIMEOUT < 1000)
   #error SNTP_CLIENT_DEFAULT_TIMEOUT parameter is not valid
#endif

//Initial retransmission timeout
#ifndef SNTP_CLIENT_INIT_RETRANSMIT_TIMEOUT
   #define SNTP_CLIENT_INIT_RETRANSMIT_TIMEOUT 2000
#elif (SNTP_CLIENT_INIT_RETRANSMIT_TIMEOUT < 1000)
   #error SNTP_CLIENT_INIT_RETRANSMIT_TIMEOUT parameter is not valid
#endif

//Maximum retransmission timeout
#ifndef SNTP_CLIENT_MAX_RETRANSMIT_TIMEOUT
   #define SNTP_CLIENT_MAX_RETRANSMIT_TIMEOUT 15000
#elif (SNTP_CLIENT_MAX_RETRANSMIT_TIMEOUT < 1000)
   #error SNTP_CLIENT_MAX_RETRANSMIT_TIMEOUT parameter is not valid
#endif

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief SNTP client states
 **/

typedef enum
{
   SNTP_CLIENT_STATE_INIT      = 0,
   SNTP_CLIENT_STATE_SENDING   = 2,
   SNTP_CLIENT_STATE_RECEIVING = 3,
   SNTP_CLIENT_STATE_COMPLETE  = 4
} SntpClientState;


/**
 * @brief SNTP client context
 **/

typedef struct
{
   SntpClientState state;             ///<SNTP client state
   NetInterface *interface;           ///<Underlying network interface
   IpAddr serverIpAddr;               ///<NTP server address
   uint16_t serverPort;               ///<NTP server port
   systime_t timeout;                 ///<Timeout value
   Socket *socket;                    ///<Underlying socket
   systime_t startTime;               ///<Request start time
   systime_t retransmitStartTime;     ///<Time at which the last request was sent
   systime_t retransmitTimeout;       ///<Retransmission timeout
   uint8_t message[NTP_MAX_MSG_SIZE]; ///<Buffer that holds the NTP request/response
   size_t messageLen;                 ///<Length of the NTP message, in bytes
   uint32_t kissCode;                 ///<Kiss code
} SntpClientContext;


//SNTP client related functions
error_t sntpClientInit(SntpClientContext *context);

error_t sntpClientSetTimeout(SntpClientContext *context, systime_t timeout);

error_t sntpClientBindToInterface(SntpClientContext *context,
   NetInterface *interface);

error_t sntpClientSetServerAddr(SntpClientContext *context,
   const IpAddr *serverIpAddr, uint16_t serverPort);

error_t sntpClientGetTimestamp(SntpClientContext *context,
   NtpTimestamp *timestamp);

uint32_t sntpClientGetKissCode(SntpClientContext *context);

void sntpClientDeinit(SntpClientContext *context);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
