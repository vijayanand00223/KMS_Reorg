#ifndef _UART_H_
#define _UART_H_

#include "ticker_itrp.h"

/* Uart flags */
#define IER_RBR		0x01
#define IER_THRE	0x02
#define IER_RLS		0x04

extern u32_t rfid_count;

s32_t p1char (s32_t ch);
int p2char(s32_t ch);
void p2str(char *str);
void send_bledata(char *str);
void p1str(char *str);  
void pstr(char *str);
void teststr(char *str);
void UART0_Handler(void)__irq;
void UART0_init(u8_t flag);
void handle_protocol_ble(u8_t ch);
void UART1_Handler (void) __irq;
void UART1_init(u8_t flag);
void UART2_Handler(void)__irq;
void UART2_init(void); 

#endif /* End of define __UART_H__ */

