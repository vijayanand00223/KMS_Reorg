#ifndef _TCP_CLIENT_TEST_H
#define _TCP_CLIENT_TEST_H

void tcpClient_eventInit(void);
void tcpClienteventTask(void);

void tcpClienteventStateInit(void);
void tcpClienteventStateConnect(void);
void tcpClienteventStateSend(void);
void tcpClienteventStatereceive(void);
void tcpClienteventStateClose(void);
void tcpClienteventStateopenwait(void);
void tcpClientChangeeventState(uint_t newState);
#endif
