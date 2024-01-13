/*
 * File name: target.c
 * 	 	This file contains related to the functions of microcontroller's
 * 		 PLL and clock configuration.
 * 		And also explains about the interrupt function. 		
 * 
 * Version V 02.00.00 -- code restructure
 * 
 * Copyright (c) 2004-2011 Sterna Security.
 * All rights reserved.
 * Author:sterna security.
 */ 
#include "lpc23xx.h"
#include "ticker_itrp.h"
#include "menu_main.h"
#include "adc.h"
#include "uart.h"
#include "timer.h"
#include "LCD.h"
#include "DOOR.h"

/* Initialize the target board before running the main() 
 * function; User may change it as needed, but may not 
 * deleted it.
PLL=(2*M*F)/N=(2*12*12)/1=288MHz
CPU Clock= PLL/(divider+1)=288/(5+1)=48MHz 
 */

void TargetResetInit(void)
{
    MEMMAP = 0x1;                                   /* remap to internal flash */
    SCS |= 1<<5;                                  /* Start main oscillator */

    while ((SCS & (1<<6)) == 0)
    {
    	//
    }

    CLKSRCSEL = 0x01;                               /* Select PLL as source */
    PLLCFG    = 0x0000000B;                         /* Start PLL M = 12 */
    PLLFEED   = 0xAA;
    PLLFEED   = 0x55;
    PLLCON    = 0x01;
    PLLFEED   = 0xAA;
    PLLFEED   = 0x55;

    while ((PLLSTAT & (1<<26)) == 0)                /* Wait until locked */
    {
    	//
    }

    while ((PLLSTAT & 0xFF7FFF) != 0x0000000B)      /* Wait until M and N are correct */
    {
    	//
    }

    CCLKCFG = 5;                                    /* Setup CPU clock divider 48 MHz, see Errata v1.3 Flash.1 */
    USBCLKCFG = 5;                                  /* Setup USB clock divider */
    PCLKSEL0 = 0;                                   /* Setup Peripheral clock */
    PCLKSEL1 = 0;
    PLLCON   = 0x03;                                /* Switch to PLL clock */
    PLLFEED  = 0xAA;
    PLLFEED  = 0x55;

    while ((PLLSTAT & (1<<25))==0)                  /* Make sure we are connected to PLL */
    {
    	//
    }
        
    
    MAMCR  = 0x00;                                  /* Turn first off to be able to change settings */
    MAMTIM = 0x03;                                  /* 3 CCLK cycles for > 40 MHz */
    MAMCR  = 0x01;                                  /* Partially enabled, see Errata v1.3 MAM.1 */
    MEMMAP = 0x01;                                  /* Uses interrupt vectors in flash */
    
    PINSEL0  = PINSEL0VAL;
    PINSEL1  = PINSEL1VAL;
    PINSEL2  = PINSEL2VAL;
    PINSEL3  = PINSEL3VAL;
    PINSEL4  = PINSEL4VAL;
    PINSEL5  = PINSEL5VAL;
    PINSEL6  = PINSEL6VAL;
    PINSEL7  = PINSEL7VAL;
    PINSEL8  = PINSEL8VAL;
    PINSEL9  = PINSEL9VAL;
    PINSEL10 = 0;

    FIO0PIN  = 0x00200464;                          // All MCI signals low
    FIO1PIN  = 0x00280000;
    FIO2PIN  = 0x00000089;
    FIO3PIN  = 0x04000000;
    FIO4PIN  = 0x10000000;
    
    FIO0DIR  = 0x9C7DF7E5;	//0x9C0877E5;                          // MCI Signals are output
    FIO1DIR  = 0x0FFC0000;
    FIO2DIR  = 0x00003B89;	//0x00002B89;                          // Also MCI signals output
    FIO3DIR  = 0x06000000;
    FIO4DIR  = 0x10000000;
    
    SCS |= 1<<0;                                  /* Enable FAST mode for PORT0 and PORT1 */
    
    VICVectAddr4  = (u32_t) TICKER_Handler; /* Timer0 Used for ticker */
    VICVectCntl4  = 1;
    TICKER_Init();

    VICVectAddr5  = (u32_t) TICKER_25us_Handler; /* Timer1 Used for ticker */
    VICVectCntl5  = 6;
    TICKER_100ms_Init();
	
    VICVectAddr6  = (u32_t) UART0_Handler;  /* UART0 Used for host protocol */
    VICVectCntl6  = 5;
    UART0_init(UART_BAUDRATE_9600);
	 
    VICVectAddr7  = (u32_t) UART1_Handler;  /* UART1 Used for RFID card */
    VICVectCntl7  = 3;
    UART1_init(UART_BAUDRATE_115200);
    rtc_init();

    adc_init();
   
    PWM1TCR = 1 << 1;                             /* Counter reset */
    PWM1PR  = 0;                                    /* No prescaler */
    PWM1MR0 = 95;                                   /* 125 kHz */
    PWM1MR3 = 47;
    PWM1MCR = 1 << 1;                             /* Clear timer when MR0 matches */
    PWM1LER = 9;                                    /* Enable MR0 and MR3 */
    PWM1PCR = 1 << 11;                            /* PWM 3 enabled */
    PWM1TCR = 9;                                    /* Counter and PWM enable */

   VICIntEnable  = INT_MCI | INT_TIMER0 | INT_UART0 | INT_UART1;
    VICIntEnable  |= INT_TIMER1;
		
			lock_on(0);
	  lcd_dis(1);	
}

