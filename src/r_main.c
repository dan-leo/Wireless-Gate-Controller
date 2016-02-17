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
* Creation Date: 2016-02-17
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
#include "stdlib.h"
#include "stdio.h"
#include <string.h>
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
uint8_t uart1RxBuf[RX_BUF_LEN];
uint16_t uart1RxCnt;
uint8_t uart1RxFlag;
uint8_t uart1TxBuf[TX_BUF_LEN];
uint16_t uart1TxCnt;
uint8_t uart1TxFlag;
uint8_t	uart1RxErrFlag;	// UART2 Receive Error Flag
uint8_t uart1RxOvrFlag;	// UART2 Receive Overrun Flag
MD_STATUS uart1Status;

//this variable gets modified every 1ms by the timer interrupt
extern timer2_interrupt;
extern rx_flag;
extern tx_flag;
extern soft_timer;
// flag to check if a 't' has been received
uint8_t t_received;
uint8_t send;
uint8_t lcd_message_max = 32;

void toggle_led();
void delay(uint16_t delay);
void print_lcd(char *message, uint8_t len);
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
	uart1RxBuf[0] = 'a';
	while (1U)
	{
		//		if (timer0_interrupt)
		//		{
		//			timer0_interrupt = 0;
		//			uart1Status = R_UART1_Send(&uart1TxBuf[0], 1);
		//		}
		if (rx_flag){
			rx_flag = 0U;

			DI();
			uart1TxBuf[0] = uart1RxBuf[0];
			send = 1;
			uart1Status = R_UART1_Receive(&uart1RxBuf[0],1);
			uart1Status = R_UART1_Send(&uart1TxBuf[0],1);
			EI();
		}

		//		if (timer2_interrupt && tx_flag && send){
		//			PM7 = 0x7F;
		//			P7 ^= 0x80;
		//			timer2_interrupt = 0;
		//			tx_flag = 0;
		//			send--;
		//			uart1Status = R_UART1_Send(&uart1TxBuf[0],1);
		//		}
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

	R_TAU0_Create();

	R_UART1_Start();
	initLcd();
	// writeByteLcd(0U, 0x1);
	// writeByteLcd(0U, 0x2);

	//	timer2_interrupt = 1;
	//	tx_flag = 1;
	//	send = 1;
	char lcd_message[20] = "Robinson DL 18361137";
	print_lcd(lcd_message, 20);
	R_TAU0_Channel2_Start();
	soft_timer = 0;
	while (soft_timer < 9);

	lcd_clear();
	delayNoInt(100);
	char lcd_messege[20] = "Fobinson DL 18361137";
	print_lcd(lcd_messege, 20);

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

void print_lcd(char *message, uint8_t len){
	// lcd_clear();

	int i;
	if (len > lcd_message_max) len = lcd_message_max;
	for (i = 0; i < len; i++)
	{
		writeByteLcd(1U, message[i]);
		delay(100);
		// writeByteLcd(0U, LCD_CURSOR_RIGHT);
		// for (j = 0 ; j < 100; j++);
		// writeByteLcd(1U, lcd_message[i]);
		if (i == 7){
			int k;
			for (k = 0; k < 32; k++)
			{
				writeByteLcd(0U, LCD_CURSOR_RIGHT);
				delay(100);
			}
		}
	}

	// scroll one right
	//writeByteLcd(0U, 0x1C);
//	delay(100);
}
/* End user code. Do not edit comment generated here */
