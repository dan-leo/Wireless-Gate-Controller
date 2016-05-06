/*
 * serial.c
 *
 *  Created on: 27 Mar 2016
 *      Author: Daniel
 */

#include "serial.h"
//#include "globals.h"
#include "../event/event.h"
#include "../flash/flash.h"

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

volatile uint8_t serial_latest_current_reading;

extern volatile uint8_t rx_flag;

void serial_clear_tx_buf();
void serial_clear_tx_buf();

/**
 * Print adc result to serial terminal
 * @param: adc result
 */
void serial_print_adc(uint8_t reading){
	uint8_t buffer[2];
	buffer[0] = 0xF7;
	buffer[1] = latest_current_reading/5;
	R_UART1_Send(&buffer, 2);
}

/**
 * echo char to serial
 * @param: uint8_t
 */
void echo(uint8_t c){
	R_UART1_Send(&c,1);
}

/**
 * clear serial tx buffer
 */
void serial_clear_tx_buf(){
	uint8_t pos;
	for (pos = 0; pos < TX_BUF_LEN; pos++){
		uart1TxBuf[pos] = 0;
	}
}

/**
 * clear serial rx buffer
 */
void serial_clear_rx_buf(){
	int k;
	for (k = 0; k < RX_BUF_LEN; k++){
		uart1RxBuf[k] = 0;
	}
	rx_tail = 0;
}

/**
 * handle all serial comms
 */
void serial_handler(uint8_t current){
	//DI();
	R_UART1_Receive(&serial_rx,1);
	uart1RxBuf[rx_tail] = serial_rx;

	uint8_t address = (uint8_t)(ir_rxMessage >> 3);
	uint8_t toggle = (uint8_t)((ir_rxMessage >> 11) & 0x1);
	uint8_t data = (uint8_t)(ir_rxMessage & 0x3);
	uint8_t data_byte = data | (toggle << 7);

	volatile uint8_t buffer[2];
//	buffer[1] = serial_latest_current_reading;

	uint8_t read_current;

	if (serial_rx == 0xF7){
		buffer[0] = 0xF7;
		buffer[1] = current;
		R_UART1_Send_Daniel(buffer, 2);
	}

	switch (serial_rx){
	case 0x81:
		echo(0x81);
		mode = TEST_MODE;
		rx_tail=0;
		print_lcd("test_mode");
		R_TAU0_Channel1_Lower8bits_Stop();
		beep(3);

		break;
	case 0x80:
		echo(0x80);
		mode = NORMAL_MODE;
		rx_tail=0;
		print_lcd("normal_mode");
		R_TAU0_Channel1_Lower8bits_Start();
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
	case 0xF2:
		// set time
		echo(0xF2);
		time_now.sec = toBCD(uart1RxBuf[rx_tail - 1]);
		time_now.min = toBCD(uart1RxBuf[rx_tail - 2]);
		time_now.hour = toBCD(uart1RxBuf[rx_tail - 3]);
		time_now.day = toBCD(uart1RxBuf[rx_tail - 4]);
		time_now.month = toBCD(uart1RxBuf[rx_tail - 5]);
		R_RTC_Stop();
		R_RTC_Set_CounterValue(time_now);
		R_RTC_Start();
		serial_clear_rx_buf();
		break;
	case 0xF3:
		// read time
		// F3 > F3 04  10  08  01  05
		// Read_Clock = 16 Apr 8:1:5 (h:m:s)
		serial_clear_tx_buf();

		R_RTC_Get_CounterValue(&time_now);

		uart1TxBuf[0] = 0xF3;
		uart1TxBuf[1] = toHex(time_now.month);
		uart1TxBuf[2] = toHex(time_now.day);
		uart1TxBuf[3] = toHex(time_now.hour);
		uart1TxBuf[4] = toHex(time_now.min);
		uart1TxBuf[5] = toHex(time_now.sec);

		R_UART1_Send(uart1TxBuf, 6);
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
		uart1RxBuf[rx_tail + 1] = '\0';
		print_lcd(uart1RxBuf);
		serial_clear_rx_buf();
		return;
	case 0xF5:
		echo(0xF5);
		eventClearDatalog();
		break;
	case 0xF6:
		eventSerialRead();
		break;
	case 0xF7:
		// read current
		//echo(0xF7);
//		serial_print_adc(latest_current_reading);

//		latest_current_reading = 10;

		R_FDL_Init();
		R_FDL_Read();
		read_current = g_read_value;
		PFDL_Close();


		buffer[0] = 0xF7;
		buffer[1] = read_current;
		R_UART1_Send_Daniel(&buffer, 2);

//		latest_current_reading = 10;

//		STMK1 = 1U;    /* disable INTST1 interrupt */
//		TXD1 = 0xF7;
//		delay(1000);
//		delayNoInt(40U);
//		TXD1 = serial_latest_current_reading;
//		delayNoInt(16000U);
//		STMK1 = 0U;    /* enable INTST1 interrupt */
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
		STMK1 = 1U;    /* disable INTST1 interrupt */
		TXD1 = 0xFC;
		delay(1000);
		TXD1 = address;
		delayNoInt(16000U);
		TXD1 = data_byte;
		delayNoInt(16000U);
		STMK1 = 0U;    /* enable INTST1 interrupt */
		break;
	case 0xFF:
		//read status
		echo(0xFF);
		echo(event_datalogs[event_index].status);
		break;
	}

	rx_tail++;
	if (debug_lcd){
		print_lcd(uart1RxBuf);
		rx_tail %= RX_BUF_LEN;
	}
	//EI();
}

void updateSerialCurrentReading(uint8_t current){
	serial_latest_current_reading = current;
}
