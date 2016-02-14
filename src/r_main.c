/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under 
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING 
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT 
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR 
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE 
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software 
* and to discontinue the availability of this software.  By using this software, 
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011, 2013 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_main.c
* Version      : CodeGenerator for RL78/G14 V2.02.00.01 [25 Dec 2013]
* Device(s)    : R5F104LE
* Tool-Chain   : GCCRL78
* Description  : This file implements main function.
* Creation Date: 2016-02-14
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_serial.h"
#include "r_cg_timer.h"
/* Start user code for include. Do not edit comment generated here */
#include "r_cg_userdefine.h"
#include "lcd.h"
uint8_t uart1RxBuf[RX_BUF_LEN];
uint16_t uart1RxCnt;
uint8_t uart1RxFlag;
uint8_t uart1TxBuf[TX_BUF_LEN];
uint16_t uart1TxCnt;
uint8_t uart1TxFlag;
uint8_t	uart1RxErrFlag;	// UART2 Receive Error Flag
uint8_t uart1RxOvrFlag;	// UART2 Receive Overrun Flag
MD_STATUS uart1Status;

//this variable gets modified every second by the timer interrupt
extern timer0_interrupt;
// flag to check if a 't' has been received
uint8_t t_received;

void toggle_led();
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
void delay(uint16_t delay);
/* End user code. Do not edit comment generated here */
void R_MAIN_UserInit(void);

/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void main(void)
{
    R_MAIN_UserInit();
    /* Start user code. Do not edit comment generated here */
    uart1TxBuf[0] = 'a';
    while (1U)
    {
    	if (timer0_interrupt)
    	{
    		timer0_interrupt = 0;
//    		toggle_led();
//    		P6 ^= 0xFF;
//    		P5 ^= 0xFF;
//    		P4 ^= 0xFF;
//    		P3 ^= 0xFF;
//    		P2 ^= 0xFF;
    		uart1Status = R_UART1_Send(&uart1TxBuf[0], 1);
    	}

//        delay(1000);
//        P6 ^= 0x1;
//        delay(1000);
    }
    /* End user code. Do not edit comment generated here */
}


/***********************************************************************************************************************
* Function Name: R_MAIN_UserInit
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MAIN_UserInit(void)
{
    /* Start user code. Do not edit comment generated here */
    EI();
    R_TAU0_Create();
    R_TAU0_Channel0_Start();

    // R_UART1_Create();
    R_UART1_Start();

    //PM7 = 0x7F;
    // P7 = 0x7F;

//    PM6 = 0x00;
//	PM5 = 0x00;
//	PM4 = 0x00;
//	PM3 = 0x00;
//	PM2 = 0x00;
//
    initLcd();

    int i;
    char lcd_message[11] = {'H','a','l','l','e','l','u','j','a','h','!'};
    for (i = 0; i < 11; i++)
    {
    	writeByteLcd(1U, lcd_message[i]);
    	writeByteLcd(1U, lcd_message[i]);
    }

    uart1Status = R_UART1_Receive(&uart1RxBuf[0],1);	// Prime UART1 Rx
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
void toggle_led(){
	//
	P7 ^= 0xFF;
}

void delay(uint16_t delay){
	// DI();
	for ( ; delay > 0 ; delay--);
	// EI();
}
/* End user code. Do not edit comment generated here */
