#include "lpc23xx.h"
#include "uart.h"
#include "menu_main.h"
#include "ticker_itrp.h"

static u32_t ble_count;
u32_t rfid_count;


/******************************************************************************
 ** Function name:      p2char
 **
 ** Descriptions:       send a single charector through 2nd serial port
 **
 ** parameters:          ch
 ** Returned value:      ch
 ** 
 ******************************************************************************/
// Write character to Serial Port
s32_t p2char(s32_t ch)                                  
{ 
	u32_t timeout;


	timeout = set_timeout(5000);

	while (!(U2LSR & 0x20))
	{
		if (chk_timeout(timeout))
			return 0;
	}
	U2THR = ch;
	return 1;

}

/******************************************************************************
 ** Function name:      p2str
 **
 ** Descriptions:        send the string 
 **
 ** parameters:          Address of str
 ** Returned value:      None
 ** 
 ******************************************************************************/
void p2str(char *str)
{
    while (*str)
    {
    	p2char(*str);
    	str++;
        
    } 	
}

// Write character to Serial Port
s32_t p1char (s32_t ch)                                 
{ 

	u32_t timeout;

	timeout = set_timeout(1000);

	while (!(U1LSR & 0x20))
	{
		if (chk_timeout(timeout))
			return 0;
	}

	U1THR = ch;
	return 1;

}

/*
fa 00 00 00 0c   
fa 09 81 03 00 09 81 03 04 00 00 00 00 90 00  
fa 81 04 00 08 00 22 f7 01 00 00 00 00 
fa 06 00 00 02 00 00     
fa 81 00 00 08 03 05 00 20 20 3a 0d 14 
fa 81 01 00 08 00 14 14 18 05 05 40 c0  
fa 81 02 00 08 00 98 00 00 00 00 00 00   
fa 81 03 00 08 00 00 00 00 00 00 00 00  
fa 81 04 00 08 00 22 f7 01 00 00 00 00 
fa 02 8f 00 00                                	

*/


void send_bledata(char *str)
{
    while (*str)
    {
    	p1char(*str);
    	str++;        
    } 	
}

void p1str(char *str)
{
    while (*str)
    {
    	p1char(*str);
    	str++;        
    } 	
}

/******************************************************************************
 ** Function name:      pchar
 **
 ** Descriptions:       send a single charector through serial port
 **
 ** parameters:          ch
 ** Returned value:      ch
 ** 
 ******************************************************************************/
// Write character to Serial Port
s32_t pchar (s32_t ch)                                  
{ 

    u32_t timeout;
   
    timeout = set_timeout(1000);

    while (!(U0LSR & 0x20))
    {
        if (chk_timeout(timeout))
            return 0;
    }

    U0THR = ch;

    return 1;
}

/******************************************************************************
 ** Function name:      pstr
 **
 ** Descriptions:       send the string 
 **
 ** parameters:          address of str
 ** Returned value:      none
 ** 
 ******************************************************************************/
void pstr(char *str)
{
    while (*str)
    {
    	pchar(*str);
    	str++;        
    }	
}

void teststr(char *str)
{
   while (*str)
    {
    	pchar(*str);
  	str++;        
   }	
}

void handle_protocol_uart0(s32_t ch)
{
	gprotocol_packetconfig_in_out[lumi_count]=ch;
	lumi_count++;
}
/******************************************************************************
 ** Function name:      UART0_Handler
 **
 ** Descriptions:       access the UART0 serial for handle protocol GSM
 **
 ** parameters:          None
 ** Returned value:      None
 ** 
 ******************************************************************************/
void UART0_Handler(void)__irq
{
   u8_t IIRValue, LSRValue;
    u8_t ch;
    IIRValue = U0IIR;

    IIRValue >>= 1;                                 // skip pending bit in IIR
    IIRValue &= 0x07;                               // check bit 1~3, interrupt identification

    if ( IIRValue == IIR_RLS )                      // Receive Line Status
    {
        LSRValue = U0LSR;

        if ( LSRValue & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI) )
        {
            ch = U0RBR;    
					handle_protocol_uart0(0x00);
						// Dummy read on RX to clear
            VICVectAddr = 0;                        // Acknowledge Interrupt
            return;
        }
        if ( LSRValue & LSR_RDR )                   // Receive Data Ready          
        {
            ch = U0RBR;		
						handle_protocol_uart0(ch);
        }
    }
    else if ( IIRValue == IIR_RDA )                 // Receive Data Available
    {
        ch = U0RBR;
				handle_protocol_uart0(ch);
    }
    else if ( IIRValue == IIR_CTI )                 // Character timeout indicator
    {
        ch = U0RBR;
				handle_protocol_uart0(ch);
    }
    else
    {
    
    }
    VICVectAddr = 0;                               // Acknowledge Interrupt
}

/******************************************************************************
 ** Function name:      UART0_init
 **
 ** Descriptions:       initialization functon for UART0.
 **                     baudrate calculation
 **						baud rate =  pclok/(16 X (256 X UDLM+UDLL)) for UART1
 **						Baud rate =pclk/(16 X (256 X UDLM+UDLL) X(1+DIVADDVAL/MULVAL))
 **
 ** parameters:          None
 ** Returned value:      None
 ** 
 ******************************************************************************/
void UART0_init(u8_t flag)
{
	if(flag ==UART_BAUDRATE_9600)
	{
		U0LCR = 0x83;
		U0DLM=0;
		U0DLL=52;
		U0FDR=0x21;
		U0LCR = 0x03;
    U0FCR = 0x07;                                   // Enable and reset TX and RX FIFO.
    U0IER = IER_RBR | IER_RLS;                      // Enable UART0 interrupt	
	}		
	else if(flag ==UART_BAUDRATE_115200)
	{
    U0LCR = 0x83;                                   // 8 bits, no Parity, 2 Stop bits
    U0DLL = 4;                                      // 115200 Baud Rate @ 12MHz VPB Clock
    U0DLM = 0;
    U0FDR = 0x85;                                   // MUL = 8, DIV = 5
    U0LCR = 0x03;                                   // DLAB = 0

    U0FCR = 0x07;                                   // Enable and reset TX and RX FIFO.
    U0IER = IER_RBR | IER_RLS;                      // Enable UART0 interrupt
	}
	else if(flag ==UART_BAUDRATE_38400)
	{
		U0LCR = 0x83;
		U0DLM=0;
		U0DLL=13;
		U0FDR=0x21;
		U0LCR = 0x03;
    U0FCR = 0x07;                                   // Enable and reset TX and RX FIFO.
    U0IER = IER_RBR | IER_RLS;                      // Enable UART0 interrupt	
	}
}


void handle_protocol_ble(u8_t ch)
{
		if(ble_count>=2 && (ch=='#'))
		{
			if(!strncmp(ble_protocol_packet_in,"+DCLOSE",7))
			{	
			}
			else if(!strncmp(ble_protocol_packet_in,"+KEYNO",6))
			{
					lock_flg2=1;
			}
			else if(!strncmp(ble_protocol_packet_in,"+LOGIN",6))
			{
				lock_flg=1;
			}
			else if(!strncmp(ble_protocol_packet_in,"+CONN",5))
			{
				conn_flg=1;
			}	
			else if(!strncmp(ble_protocol_packet_in,"+DISCO",5))
			{
				disconn_flg=1;
			}
			else if(!strncmp(ble_protocol_packet_in,"+TASK",5))
			{
				ble_task_flg=1;
			}
			ble_count=0;
		}
		else if(ch !='#')
		{
			ble_protocol_packet_in[ble_count++]=ch;
		}
}

/******************************************************************************
 ** Function name:      UART1_Handler
 **
 ** Descriptions:       access the UART1 serial for handle RFID
 **
 ** parameters:          None
 ** Returned value:      None
 ** 
 ******************************************************************************/
void UART1_Handler(void)__irq
{
    u8_t IIRValue, LSRValue;
    u8_t ch;
    IIRValue = U1IIR;

    IIRValue >>= 1;                                 // skip pending bit in IIR
    IIRValue &= 0x07;                               // check bit 1~3, interrupt identification

    if ( IIRValue == IIR_RLS )                      // Receive Line Status
    {
        LSRValue = U1LSR;

        if ( LSRValue & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI) )
        {
            ch = U1RBR;                             // Dummy read on RX to clear
						handle_protocol_ble(0x00);
            VICVectAddr = 0;                        // Acknowledge Interrupt
            return;
        }
        if ( LSRValue & LSR_RDR )                   // Receive Data Ready          
        {
            ch = U1RBR;
					handle_protocol_ble((s32_t)ch);
        }
    }
    else if ( IIRValue == IIR_RDA )                 // Receive Data Available
    {
        ch = U1RBR;
			handle_protocol_ble((s32_t)ch);
    }
    else if ( IIRValue == IIR_CTI )                 // Character timeout indicator
    {
        ch = U1RBR;
			handle_protocol_ble((s32_t)ch);
    }
    else
    {
    }
    VICVectAddr = 0;                                    // Acknowledge Interrupt
}
/******************************************************************************
 ** Function name:      UART1_init
 **
 ** Descriptions:       initialization functon for UART1
 **
 ** parameters:          None
 ** Returned value:      None
 ** 
 ******************************************************************************/
void UART1_init(u8_t flag)
{
	if(flag==UART_BAUDRATE_9600)
	{
		U1LCR = 0x83;
		U1DLM=0;
		U1DLL=52;
		U1FDR=0x21;
		U1LCR = 0x03;
    U1FCR = 0x07;                                   // Enable and reset TX and RX FIFO.
    U1IER = IER_RBR | IER_RLS;                      // Enable UART0 interrupt	
	}
	else if(flag==UART_BAUDRATE_115200)
	{
	  U1LCR = 0x83;                                   // 8 bits, no Parity, 2 Stop bit
    U1DLL = 4;                                     // 115200 Baud Rate @ 12MHz VPB Clock
    U1DLM = 0;
		U1FDR =0x85;
    U1LCR = 0x03;                                   // DLAB = 0
    
    U1FCR = 0x07;                                   // Enable and reset TX and RX FIFO.
    U1IER = IER_RBR | IER_RLS;                      // Enable UART1 interrupt
	}
	else if(flag==UART_BAUDRATE_38400)
	{
		U1LCR = 0x83;
		U1DLM=0;
		U1DLL=13;
		U1FDR=0x21;
		U1LCR = 0x03;
    U1FCR = 0x07;                                   // Enable and reset TX and RX FIFO.
    U1IER = IER_RBR | IER_RLS;                      // Enable UART0 interrupt	
	}
}
/******************************************************************************
 ** Function name:      UART2_Handler
 **
 ** Descriptions:       access the UART2 serial for handle protocol suprema
 **
 ** parameters:          None
 ** Returned value:      None
 ** 
 ******************************************************************************/
void UART2_Handler(void)__irq
{
    u8_t IIRValue, LSRValue;
    u8_t ch;
    IIRValue = U2IIR;
    IIRValue >>= 1;                                 // skip pending bit in IIR
    IIRValue &= 0x07;                               // check bit 1~3, interrupt identification
    if ( IIRValue == IIR_RLS )                      // Receive Line Status
    {
        LSRValue = U2LSR;
        if ( LSRValue & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI) )
        {
            ch = U2RBR;                             // Dummy read on RX to clear
            VICVectAddr = 0;                        // Acknowledge Interrupt
            return;
        }
        if ( LSRValue & LSR_RDR )                   // Receive Data Ready          
        {
            ch = U2RBR;
        }
    }
    else if ( IIRValue == IIR_RDA )                 // Receive Data Available
    {
        ch = U2RBR;
    }
    else if ( IIRValue == IIR_CTI )                 // Character timeout indicator
    {
        ch = U2RBR;
    }
    else
    {

    }
    VICVectAddr = 0;                                // Acknowledge Interrupt
}

/******************************************************************************
 ** Function name:      UART2_init
 **
 ** Descriptions:       initialization functon for UART2
 **
 ** parameters:          None
 ** Returned value:      None
 ** 
 ******************************************************************************/
void UART2_init(void)								// for biometric reader
{
		U0LCR = 0x83;                                   // 8 bits, no Parity, 2 Stop bits
		U0DLL = 4;                                      // 115200 Baud Rate @ 12MHz VPB Clock
    U0DLM = 0;
    U0FDR = 0x85;                                   // MUL = 8, DIV = 5
    U0LCR = 0x03;                                   // DLAB = 0

    U0FCR = 0x07;                                   // Enable and reset TX and RX FIFO.
    U0IER = IER_RBR | IER_RLS;   
}
/*-----------------------------------------end of file-------------------------------------------*/
