/*
 * serial.c
 *
 *  Created on: 27 Mar 2016
 *      Author: Daniel
 */

#include "serial.h"
#include "motor_driver.h"
#include "gate_controller.h"
#include "debug.h"
#include "lcd.h"
#include "adc.h"
#include "infrared.h"

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

	uint8_t address = (uint8_t)(ir_rxMessage >> 3);
	uint8_t toggle = (uint8_t)((ir_rxMessage >> 11) & 0x1);
	uint8_t data = (uint8_t)(ir_rxMessage & 0x3);
	uint8_t data_byte = data | (toggle << 7);

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
	case '/': // send a command byte to lcd
		while (!rx_flag);
		rx_flag = 0;
		R_UART1_Receive(&serial_rx, 1);
		writeByteLcd(0U, serial_rx);
		echo(serial_rx);
		return;
	case '\\': // send a data byte to lcd
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
		print_lcd(uart1RxBuf);
		int k;
		for (k = 0; k < rx_tail; k++){
			uart1RxBuf[k] = 0;
		}
		rx_tail = 0;
		break;
	case 0xF7:
		// read current
		echo(0xF7);
		serial_print_adc(latest_current_reading);
		break;
	case 0xF8:
		// close gate
		echo(0xF8);
		gate_close();
		//if (gate_position == OPEN) R_INTC3_Stop();
		print_lcd("unknown");
		break;
	case 0xF9:
		// open gate
		echo(0xF9);
		gate_open();
		//if (gate_position == CLOSED) R_INTC4_Stop();
		print_lcd("unknown");
		break;
	case 0xFC:
		// read IR cmd
//		echo(0xFC);
//		echo(0xFC);
//		volatile uint16_t address = 0x3141;
//		R_UART1_Send(&address, 2);

        STMK1 = 1U;    /* disable INTST1 interrupt */
        TXD1 = 0xFC;
//        STMK1 = 0U;    /* enable INTST1 interrupt */
//        STMK1 = 1U;    /* disable INTST1 interrupt */
        delay(1000);
        TXD1 = address;
//        STMK1 = 0U;    /* enable INTST1 interrupt */
//        STMK1 = 1U;    /* disable INTST1 interrupt */
//        NOP();
//        TXD1 = address;
        delayNoInt(16000U);
        TXD1 = data_byte;
//        NOP();
//        TXD1 = data_byte;
        delayNoInt(16000U);
        STMK1 = 0U;    /* enable INTST1 interrupt */

//		echo(0x41);
//		echo(0x31);
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