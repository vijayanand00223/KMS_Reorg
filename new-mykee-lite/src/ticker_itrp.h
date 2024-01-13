

#ifndef __TICKER_H
#define __TICKER_H

typedef unsigned char   u8_t;
typedef signed char     s8_t;
typedef unsigned short  u16_t;
typedef signed short    s16_t;
typedef unsigned int    u32_t;
typedef signed int      s32_t;
typedef double           f32_t;
typedef unsigned int    mem_ptr_t;





extern volatile u8_t  lock_state[64];
extern u8_t  lock_key_state[64];
extern volatile char  pressed_key;


extern u32_t endtick_value;
//extern void UART0_Handler (void) __irq;
//extern void UART0_init(u8_t flag);
//extern void UART1_Handler (void) __irq;
//extern void UART1_init(u8_t flag);
//extern void UART2_Handler (void) __irq;
//extern void UART2_init(void);
//extern void  TICKER_Handler(void) __irq;
//extern void TICKER_Init(void);
//extern void   TICKER_Start(void);
//extern void  TICKER_25us_Handler(void) __irq;
//extern void TICKER_100ms_Init(void);
//extern void   TICKER_100ms_Start(void);
extern void TargetResetInit(void);
extern void tcpClient_eventInit(void);
extern void tcpClienteventTask(void);
extern void tcpClientInit(void);
extern void tcpClienthbTask(void);


extern void   delay(u32_t ulticks);
//extern void   relay(s32_t num, s32_t state);
//extern int    trg(s32_t num);

#define ON           1
#define OFF          0



#define INT_TIMER0   (1 << 4) 
#define INT_TIMER1   (1 << 5) 			//Added for Task Overdue R&D_STERNA
#define INT_UART0    (1 << 6)
#define INT_UART1    (1 << 7)
#define INT_UART2    (1 << 28)
#define INT_MCI      (1 << 24)
#define KEYB_QUEUE  4






#define LOCK_RES    (1 << 19)
#define LOCK_RW     (1 << 20)
#define LOCK_E      (1 << 21)
#define LOCK_AMASK  (0x3F << 22)
#define LOCK_ASHIFT (22)
#define LOCK_DMASK  (0x0F << 28)
#define LOCK_DSHIFT (28)
#define LOCK_RED    0x01 
#define LOCK_GREEN  0x02
#define LOCK_SOL    0x04
#define LOCK_OPEN   0x10
#define LOCK_CLOSED 0x20

#define LOCK_NOT_FOUND 0xEF	//R&D_STERNA

//#define TRG0        (1 << 15)
//#define TRG1        (1 << 16)
//#define TRG2        (1 << 17)
//#define TRG3        (1 << 18)
#define TRG0        (1 << 10)
#define TRG1        (1 << 11)
#define TRG2        (1 << 28)
#define TRG3        (1 << 29)
#define LOCKD       (1 << 7)
#define ISP			    (1 << 10)

#define RLY1        (1 << 8)
#define RLY2        (1 << 9)

#define BAT_TEST    (1 << 25)


/* Special keys */
#define ESC            1
#define ENTER          2  
#define ARROW_L        3
#define ARROW_R        4
#define ARROW_U        5
#define ARROW_D        6

//#define PINSEL0VAL  0x00A00055
#define PINSEL0VAL  0x00500055
//#define PINSEL1VAL  0x00054800
#define PINSEL1VAL  0x00054000
#define PINSEL2VAL  0x50150105
#define PINSEL3VAL  0x00000005
//#define PINSEL4VAL  0x0000802A
#define PINSEL4VAL  0x0000002A	
#define PINSEL5VAL  0x00000000
#define PINSEL6VAL  0x00000000
#define PINSEL7VAL  0x00000000
#define PINSEL8VAL  0x00000000
#define PINSEL9VAL  0x00000000



#define IIR_PEND	0x01
#define IIR_RLS		0x03
#define IIR_RDA		0x02
#define IIR_CTI		0x06
#define IIR_THRE	0x01



#endif

