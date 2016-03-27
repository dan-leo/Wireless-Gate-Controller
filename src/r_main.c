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
 * Creation Date: 2016-03-11
 ***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
 ***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_serial.h"
#include "r_cg_adc.h"
#include "r_cg_timer.h"
#include "r_cg_pclbuz.h"
/* Start user code for include. Do not edit comment generated here */
#include "r_cg_userdefine.h"
#include "lcd.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
 ***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
uint8_t uart1RxBuf[RX_BUF_LEN];
volatile uint16_t uart1RxCnt;
volatile uint8_t uart1RxFlag;
volatile uint8_t uart1TxBuf[TX_BUF_LEN];
volatile uint16_t uart1TxCnt;
volatile uint8_t uart1TxFlag;
volatile uint8_t uart1RxErrFlag;	// UART2 Receive Error Flag
volatile uint8_t uart1RxOvrFlag;	// UART2 Receive Overrun Flag
volatile MD_STATUS uart1Status;
volatile uint8_t gate_position;
volatile uint8_t last_gate_position;

//this variable gets modified every 1ms by the timer interrupt
extern volatile timer1_interrupt;
extern volatile timer2_interrupt;
extern volatile rx_flag;
extern volatile adc_ready;
extern volatile gate_opened;
extern volatile gate_closed;

uint8_t rx;
// uart fix
uint8_t rx_char_main;

volatile uint8_t lcd_message_max = 32;

volatile uint8_t ADC_done = 0; /* Flag: conversion complete */
volatile uint16_t ADC_value[NUM_CHANNELS];
volatile uint8_t INTAD_FSM_state = AD_IDLE;

void echo(uint8_t c);
void toggle_led();
void delay(uint16_t delay);
void print_lcd(char *message);
void print_long_message(char * message);
void main_service();
uint16_t scale_down(uint16_t value, uint8_t percentage);
uint16_t scale_down_ratio(uint16_t value, uint16_t ratio_numerator, uint16_t ratio_denominator);
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
	main_service();
	while (1U)
	{
		//#
		;
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
	nSLEEP = 0;
	R_TAU0_Channel0_Start();
	R_TAU0_Channel1_Lower8bits_Start();

	R_UART1_Start();
	delay(100);
	//	R_INTC4_Start();
	//	R_INTC3_Start();

	R_TMR_RD0_Start();

	initLcd();

	//	timer2_interrupt = 1;
	print_long_message("Robinson DL 18361137");
	// print_lcd(lcd_message);
	delay(100);


	uart1Status = R_UART1_Receive(&rx,1);	// Prime UART1 Rx
	/* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */

void delay(uint16_t delay){
	// DI();
	for ( ; delay > 0 ; delay--);
	// EI();
}

void print_lcd(char *message){
	uint8_t len = strlen(message);
	lcd_clear();
	delay(10000);

	int i;
	if (len > lcd_message_max) len = lcd_message_max;
	for (i = 0; i < len; i++)
	{
		if (message[i] <= 0x7F){
			writeByteLcd(1U, message[i]);
		}
		delay(100);
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
	EI();
}

void print_long_message(char * message)
{
	uint8_t head;
	uint8_t tail;
	uint8_t lcd_display[16];
	uint16_t k;
	for (tail = 0; tail < strlen(message) - 15; tail++)
	{
		for(head = 0; head < 16; head++)
		{
			lcd_display[head] = message[head + tail];
		}
		print_lcd(lcd_display);
		for (k = 0; k < 100; k++)
		{
			delay(10000);
		}
	}
}

/**
 * Print adc result to serial terminal
 * @param: adc result
 */
void serial_print_adc(uint16_t reading){
	uint8_t a;
	a = reading >> 8;
	R_UART1_Send(&a, 1);
	delay(10000U);
	a = (uint8_t)reading;
	R_UART1_Send(&a, 1);
}

/**
 * get adc reading (single-shot reading of 1 channel)
 * @return: 10-bit adc result
 */
uint16_t adc_get_reading(){
	uint16_t adc_result;
	P7^=0x80;
	R_ADC_Start();
	while(!adc_ready);
	adc_ready=0;
	R_ADC_Get_Result(&adc_result);
	R_ADC_Stop();
	return adc_result;
}

/**
 * change duty cycle of pwm signal for motor.
 * @param: uint8_t percentage
 */
void pwm_change_duty_cycle(uint8_t percentage){
	volatile uint16_t max_motor_steps = TRDGRA0;
	volatile uint16_t falling_edge_of_decrementing_steps;
	if (percentage > 100) percentage = 100;
	if (percentage < -100) percentage = -100;
	falling_edge_of_decrementing_steps = (100*max_motor_steps - percentage*max_motor_steps)/100;
	TRDGRB0 = falling_edge_of_decrementing_steps;
}

/**
 * change power of motor.
 * motor pwm % limited to specified value.
 * @param: 0-100%
 */
void motor_power(uint8_t percentage){
	volatile uint8_t max_duty_cycle_percentage = 44;
	volatile uint16_t falling_edge_of_decrementing_steps;
	volatile uint16_t scaled_steps;
	scaled_steps = scale_down(TRDGRA0,max_duty_cycle_percentage);
	scaled_steps = scale_down(scaled_steps,percentage);
	falling_edge_of_decrementing_steps = TRDGRA0 - scaled_steps;
	TRDGRB0 = falling_edge_of_decrementing_steps;
}

/**
 * update power of motor according ratio
 * motor pwm % limited to specified value.
 *
 * be careful of overflow!!
 *
 * @param: fraction
 */
void motor_power_ratio(uint16_t ratio_numerator, uint16_t ratio_denominator){
	volatile uint8_t max_duty_cycle_percentage = 44;
	volatile uint16_t falling_edge_of_decrementing_steps;
	volatile uint16_t scaled_steps;
	scaled_steps = scale_down(TRDGRA0,max_duty_cycle_percentage);
	scaled_steps = scale_down_ratio(scaled_steps, ratio_numerator, ratio_denominator);
	falling_edge_of_decrementing_steps = TRDGRA0 - scaled_steps;
	if (TRDGRA0 == falling_edge_of_decrementing_steps){
		falling_edge_of_decrementing_steps = TRDGRA0 - 1;
	}
	TRDGRB0 = falling_edge_of_decrementing_steps;
}

uint16_t scale_down(uint16_t value, uint8_t percentage){
	return ((value * percentage) / 100);
}

/**
 * a way to scale down without using floats
 * @param: the value to be scaled, and a fraction
 */
uint16_t scale_down_ratio(uint16_t value, uint16_t ratio_numerator, uint16_t ratio_denominator){
	return ((value * ratio_numerator) / ratio_denominator);
}

/**
 * echo char to serial
 * @param: uint8_t
 */
void echo(uint8_t c){
	R_UART1_Send(&c,1);
}

void main_service(){
	volatile uint8_t rx_tail, tx_tail, test_mode = 0;
	volatile uint16_t pwm_counter;
	volatile uint16_t adc_last_reading;
	volatile uint8_t adc_enable = 0;
	volatile uint8_t gate_is_moving = 0;

	echo(0x80);
	print_lcd("normal_mode");
	/* Peripheral start function calls */
	R_ADC_Set_OperationOn(); /* Enable ADC voltage comparator */

	motor_power(28);

	while (1U)
	{
		if (rx_flag){
			rx_flag = 0;
			PM7^=0x80;
			//DI();
			uart1Status = R_UART1_Receive(&rx,1);
			// rx = rx_char_main;
			uart1RxBuf[rx_tail] = rx;
			//			uart1TxBuf[tx_tail] = rx;
			//			R_UART1_Send(&rx,1);
			//			echo(rx);

			//lcd_clear();
			switch (rx){
			case 0x81:
				echo(0x81);
				test_mode = 1;
				rx_tail=0;
				print_lcd("test_mode");
				break;
			case 0x80:
				echo(0x80);
				test_mode = 0;
				rx_tail=0;
				print_lcd("normal_mode");
				break;
			case '*':
				// debug output to serial
				adc_enable ^= 1;

				// send dummy chars to get a column of 2 chars in RealTerm
				echo('*');
				echo('{');
				break;
			case 0xF0:
				// start buzzer
				echo(0xF0);
				R_PCLBUZ0_Start();
				break;
			case 0xF1:
				// stop buzzer
				echo(0xF1);
				R_PCLBUZ0_Stop();
				break;
			case 0xF4:
				//				if (rx_tail < 16){
				//					rx_tail = 0;
				//					print_lcd(uart1RxBuf);
				//				}
				//				else{
				//					rx_tail = 0;
				//					print_long_message(uart1RxBuf);
				//				}
				lcd_clear();
				delay(100);
				print_long_message(uart1RxBuf);
				int k;
				for (k = 0; k < rx_tail; k++){
					uart1RxBuf[k] = 0;
				}
				rx_tail = 0;
				break;
			case 0xF7:
				// read current
				echo(0xF7);
				if (gate_is_moving){
					echo(0x0A);
				}
				else{
					echo(0x0);
				}
				break;
			case 0xF8:
				// close gate
				echo(0xF8);
				PHASE = 1;
				nSLEEP = 1;
				gate_position = UNKNOWN;
				gate_is_moving = 1;
				//if (gate_position == OPEN) R_INTC3_Stop();
				print_lcd("unknown");
				break;
			case 0xF9:
				// open gate
				echo(0xF9);
				PHASE = 0;
				nSLEEP = 1;
				gate_position = UNKNOWN;
				gate_is_moving = 1;
				//if (gate_position == CLOSED) R_INTC4_Stop();
				print_lcd("unknown");
				break;
			case 0xFF:
				//read status
				echo(0xFF);
				echo(gate_position);
				break;
			}

			//			if (rx_tail < 16){
			//				print_lcd(uart1RxBuf);
			//			}

			//			if (test_mode == 1){
			//				if ((*rx == 0xF4) /*|| (*rx == 0x0D)*/) print_lcd(uart1RxBuf, strlen(uart1RxBuf));
			//			}

			//			if (rx_tail < 17){
			//				rx_tail++;
			//			}


			//			if (rx_tail < 17){
			//				rx_tail++;
			//			}
			//			else
			//			{
			//				for (k = rx_tail; k > 1; k--){
			//					uart1RxBuf[k-1] = uart1RxBuf[k];
			//				}
			//			}
			rx_tail++;
			//			rx_tail %= RX_BUF_LEN;
			//			tx_tail++;
			//			tx_tail %= TX_BUF_LEN;
			//EI();
		}

		if (timer2_interrupt){
			timer2_interrupt = 0;
			if (SW_OPENED && !SW_CLOSED){
				if (last_gate_position != OPEN){
					P7^=0x80;
					nSLEEP = 0;
					gate_is_moving = 0;
					gate_position = OPEN;
					last_gate_position = OPEN;
					R_INTC4_Start();
					print_lcd("opened");
				}
			}
			if (SW_CLOSED && !SW_OPENED){
				if (last_gate_position != CLOSED){
					P7^=0x80;
					nSLEEP = 0;
					gate_is_moving = 0;
					gate_position = CLOSED;
					last_gate_position = CLOSED;
					R_INTC3_Start();
					print_lcd("closed");
				}
			}
			if (!(SW_OPENED || SW_CLOSED)){
				if (last_gate_position != UNKNOWN){
					last_gate_position = UNKNOWN;
					gate_position = UNKNOWN;
					print_lcd("unknown");
				}
			}
		}

		//		if (gate_opened){
		//			gate_opened=0;
		//
		//		}
		//
		//		if (gate_closed){
		//			gate_closed=0;
		//
		//		}


		if (timer1_interrupt){
			timer1_interrupt = 0;

			INTAD_FSM_state = AD_SAMPLING;
			ADC_done = 0;
			R_ADC_Start(); /* Start ADC (ADCS = 0n) */
			while (!ADC_done)
				;

			if (adc_enable){
				serial_print_adc(ADC_value[2]);
				serial_print_adc(ADC_value[3]);
				//				lcd_clear();
				//				uint8_t a = ADC_value[2] >> 8;
				//				writeByteLcd(1,a);
				//				a = (uint8_t)ADC_value[2];
				//				writeByteLcd(1,a);
			}

			/*			adc_last_reading = adc_get_reading();*/
			volatile tiny_adc_reading = ADC_value[2] >> 3;
			motor_power_ratio(tiny_adc_reading,128);
			/*if (adc_enable){
						serial_print_adc(adc_last_reading);
					}*/


			/*			pwm_counter++;
					pwm_counter%=2*1000/30;
					if (pwm_counter < 1000/30 ){
						//				P7|=0x80;
					}
					else {
						//				P7&=0x7F;
					}*/
			//			PM7=0x7F;
			//			P7^=0x80;
			/*			if (soft_timer < 9){
						PM7=0x7F;
						P7^=0x80;
						soft_timer++;
					}
					else if (soft_timer >= 9)
					{
						lcd_clear();
						EI();
		//				delayNoInt(100);
		//				char lcd_messege[20] = "abcdefghijklmnopqrst";
		//				print_lcd(lcd_messege, 20);
						soft_timer++;
					}*/
		}
		/*		if (adc_ready){
			adc_ready = 0;
//			P7&=0x7F;
			P7^=0x80;
//			R_ADC_Get_Result(&adc_result);
		}*/

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
