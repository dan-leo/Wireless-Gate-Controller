/*
 * core.c
 *
 *  Created on: 27 Mar 2016
 *      Author: Daniel
 */

#include "core.h"

extern volatile uint8_t timer1_interrupt;

//this variable gets modified every 1ms by the timer interrupt
extern volatile uint8_t timer2_interrupt;

extern volatile uint8_t rx_flag;

void core_setup(){
	ADC_done = 0;
	INTAD_FSM_state = AD_IDLE;

	debug_ir_lcd_request = 1;
	debug_adc_serial = 0;
	debug_lcd = 0;
	debug_adc_lcd = 0;

	// idle mode
	IR_LED_TX = 1;

	nSLEEP = 0;

	R_TAU0_Channel0_Start();
	R_TAU0_Channel1_Lower8bits_Start();
	R_TAU0_Channel2_Start();  // 1kHz gate stop listener
	// R_TAU0_Channel3_Start();

	R_UART1_Start();
	delay(100);
	//	R_INTC3_Start();
	R_INTC5_Start(); // listen for beginning of ir data stream
	R_INTC2_Start(); // listen for ir button


	R_TMR_RD0_Start();
	// R_TMR_RJ0_Start();

	initLcd();

	//	print_long_message("Robinson DL 18361137");
	delay(100);

	R_UART1_Receive(&serial_rx,1);	// Prime UART1 Rx

	echo(0x80);
	print_lcd("normal_mode");
	/* Peripheral start function calls */
	R_ADC_Set_OperationOn(); /* Enable ADC voltage comparator */

	motor_power(28);
}

void core_main(){

	while (1U){
		if (rx_flag){
			rx_flag = 0;
			serial_handler();
		}

		// 1kHz gate stop button checker
		if (timer2_interrupt){
			timer2_interrupt = 0;
			gate_stop_handler();
		}

		// 10 Hz adc reader
		if (timer1_interrupt){
			timer1_interrupt = 0;
			adc_get_multiple_channels();
		}

		if (ir_new_command_interrupt){
			ir_new_command_interrupt = 0;
			if (ir_rxMessage == IR_GATE_OPEN){
				gate_open();
			}
			if (ir_rxMessage == IR_GATE_CLOSE){
				gate_close();
			}
			if (ir_rxMessage == IR_GATE_E_STOP){
				gate_stop();
			}
			if (debug_ir_lcd_request){
				uint8_t ascii_word[16];
				word_to_ascii(ir_rxMessage, ascii_word);
				print_lcd(ascii_word);
			}
		}
	}
}
