/*
 * core.c
 *
 *  Created on: 27 Mar 2016
 *      Author: Daniel
 */

#include "core.h"

//volatile uint8_t g_read_value	= 0x00;
//volatile uint8_t g_write_value   = 0x00;
//volatile uint8_t g_write_address = TARGET_BLOCK * BLOCK_SIZE; // zero


extern volatile uint8_t timer1_interrupt;

//this variable gets modified every 1ms by the timer interrupt
extern volatile uint8_t timer2_interrupt;

extern volatile uint8_t rx_flag;

void core_setup(){
	ADC_done = 0;
	INTAD_FSM_state = AD_IDLE;

	debug_ir_lcd_request = 0;
	debug_adc_serial = 0;
	debug_lcd = 0;
	debug_adc_lcd = 0;

	// idle mode
	IR_LED_TX = 1;

	nSLEEP = 0;

	R_RTC_Start();
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

	char *s  = PFDL_GetVersionString(); // DRL78T04U1301GV105

	uint8_t ret;

	/*flash_setup();
	g_read_value = 9;

	g_write_value = 'z';
	R_FDL_ExecuteWrite();

	R_FDL_Init();
	R_FDL_Read();
	uint8_t read = g_read_value;
	PFDL_Close();

	R_FDL_ClearDataFlash();

	R_FDL_Init();
	R_FDL_Read();
	uint8_t read1 = g_read_value;
	PFDL_Close();

	R_FDL_ChangeAddress();
	R_FDL_ChangeAddress();
	uint8_t write_addr1 = g_write_address;
	ret = R_FDL_BlankCheck();
	uint8_t write_addr = g_write_address;
	PFDL_Close();*/

	motor_power(28);
}

void core_main(){

	while (1U){
		if (rx_flag){
			rx_flag = 0;
			serial_handler();
		}

		// 1kHz button checker
		if (timer2_interrupt){
			timer2_interrupt = 0;
			static buttons last_keypad_button_press = 0;
			gate_stop_handler();
			if (!BT_EMER_STOP){
				if (last_keypad_button_press != BT_EMER_STOP_enum){
					last_keypad_button_press = BT_EMER_STOP_enum;
					gate_stop();
					new_event.cmd = cmd_ESTOP_pressed;
					new_event.event = event_emergency_stopped;
					new_event.status = status_emergency_stopped;
					eventAdd(new_event);
					eventPrint(event_datalogs[event_index]);
				}
			}
			if (!BT_OPEN){
				if (last_keypad_button_press != BT_OPEN_enum){
					last_keypad_button_press = BT_OPEN_enum;
					gate_open();
					new_event.cmd = cmd_remote_opening;
					new_event.event = event_open;
					new_event.status = status_base;
					eventAdd(new_event);
					eventPrint(event_datalogs[event_index]);
				}
			}
			if (!BT_CLOSE){
				if (last_keypad_button_press != BT_CLOSE_enum){
					last_keypad_button_press = BT_CLOSE_enum;
					gate_close();
					new_event.cmd = cmd_remote_closing;
					new_event.event = event_close;
					new_event.status = status_base;
					eventAdd(new_event);
					eventPrint(event_datalogs[event_index]);
				}
			}
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
				new_event.cmd = cmd_remote_opening;
				new_event.event = event_open;
				new_event.status = status_remote_open;
				eventAdd(new_event);
				eventPrint(event_datalogs[event_index]);
			}
			if (ir_rxMessage == IR_GATE_CLOSE){
				gate_close();
				new_event.cmd = cmd_remote_closing;
				new_event.event = event_close;
				new_event.status = status_remote_close;
				eventAdd(new_event);
				eventPrint(event_datalogs[event_index]);
			}
			if (ir_rxMessage == IR_GATE_E_STOP){
				gate_stop();
				new_event.cmd = cmd_ESTOP_pressed;
				new_event.event = event_emergency_stopped;
				new_event.status = status_emergency_stopped;
				eventAdd(new_event);
				eventPrint(event_datalogs[event_index]);
			}
			if (debug_ir_lcd_request){
				uint8_t ascii_word[16];
				word_to_ascii(ir_rxMessage, ascii_word);
				print_lcd(ascii_word);
			}
		}
	}
}
