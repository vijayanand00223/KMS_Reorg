#ifndef _TCP_CLIENT_FW_H
#define _TCP_CLIENT_FW_H

void tcpClientfwInit(void);
void tcpClientfwTask(void);

void tcpClientStatefwInit(void);
void tcpClientStatefwResolve(void);
void tcpClientStatefwConnect(void);
void tcpClientStatefwSend(void);
void tcpClientStatefwwait(void);
void tcpClientStatefwClose(void);
void tcpClientStatefwopenwait(void);
void tcpClientChangeStatefw(uint_t newState);
void tcpClientStatefwreceive(void);
#endif
