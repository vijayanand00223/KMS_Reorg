#ifndef _TCP_CLIENT_TEST_H
#define _TCP_CLIENT_TEST_H

void tcpClientInit(void);
void tcpClienthbTask(void);

void tcpClientStateInit(void);
void tcpClientStateResolve(void);
void tcpClientStateConnect(void);
void tcpClientStateSend(void);
void tcpClientStatewait(void);
void tcpClientStateClose(void);
void tcpClientStateopenwait(void);
void tcpClientStatereceive(void);
void tcpClientChangeState(uint_t newState);
#endif
