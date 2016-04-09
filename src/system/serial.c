/*
 * serial.c
 *
 *  Created on: 27 Mar 2016
 *      Author: Daniel
 */

#include "serial.h"
#include "motor_driver.h"
#include "debug.h"
#include "lcd.h"

#define RX_BUF_LEN 16
#define TX_BUF_LEN 16

uint8_t uart1RxBuf[RX_BUF_LEN];
volatile uint16_t uart1RxCnt;
volatile uint8_t uart1RxFlag;
volatile uint8_t uart1TxBuf[TX_BUF_LEN];
volatile uint16_t uart1TxCnt;
volatile uint8_t uart1TxFlag;
volatile uint8_t uart1RxErrFlag;	// UART2 Receive Error Flag
volatile uint8_t uart1RxOvrFlag;	// UART2 Receive Overrun Flag

uint8_t rx_tail;

extern volatile uint8_t rx_flag;


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
 * echo char to serial
 * @param: uint8_t
 */
void echo(uint8_t c){
	R_UART1_Send(&c,1);
}

/**
 * handle all serial comms
 */
void serial_handler(){
	PM7^=0x80;
	//DI();
	R_UART1_Receive(&serial_rx,1);
	// rx = rx_char_main;
	uart1RxBuf[rx_tail] = serial_rx;

	switch (serial_rx){
	case 0x81:
		echo(0x81);
		serial_test_mode = 1;
		rx_tail=0;
		print_lcd("test_mode");
		break;
	case 0x80:
		echo(0x80);
		serial_test_mode = 0;
		rx_tail=0;
		print_lcd("normal_mode");
		break;
	case '*':
		// debug output to serial
		debug_adc_serial ^= 1;

		// send dummy chars to get a column of 2 chars in RealTerm
		echo('*');
		echo('{');
		break;
	case '/': // send a command byte
		while (!rx_flag);
		rx_flag = 0;
		R_UART1_Receive(&serial_rx, 1);
		writeByteLcd(0U, serial_rx);
		echo(serial_rx);
		return;
	case '\\': // send a data byte
		while (!rx_flag);
		rx_flag = 0;
		R_UART1_Receive(&serial_rx, 1);
		writeByteLcd(1U, serial_rx);
		echo(serial_rx);
		return;
	case '&': // clear lcd
		lcd_clear();
		echo('&');
		return;
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

	rx_tail++;
	if (debug_lcd){
		print_lcd(uart1RxBuf);
		rx_tail %= RX_BUF_LEN;
	}
	//EI();
}
