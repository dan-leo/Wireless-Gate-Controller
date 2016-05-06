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


extern volatile uint8_t timer_adc_reader_10Hz_interrupt;

//this variable gets modified every 1ms by the timer interrupt
extern volatile uint8_t timer_button_checker_1kHz_interrupt;

extern volatile uint8_t rx_flag;

#define max_overcurrent_counter 3 // checks n times before autoclose
volatile uint8_t nReady_overcurrent_counter;

void core_setup(){
	mode = NORMAL_MODE;
	ADC_done = 0;
	INTAD_FSM_state = AD_IDLE;

	debug_ir_lcd_request = 0;
	debug_adc_serial = 0;
	debug_lcd = 0;
	debug_adc_lcd = 0;

	scrolling = 0;

	nReady_overcurrent_counter = 0;

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
	eventInit();

	//	print_long_message("Robinson DL 18361137");
	delay(100);

	R_UART1_Receive(&serial_rx,1);	// Prime UART1 Rx

	echo(0x80);
	print_lcd("normal_mode");
	/* Peripheral start function calls */
	R_ADC_Set_OperationOn(); /* Enable ADC voltage comparator */

	char *s  = PFDL_GetVersionString(); // DRL78T04U1301GV105

	uint8_t ret;

	flash_setup();
	/*g_read_value = 9;

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

	//this all works. Just not going to implement it. :)

	motor_power(28);

	gate_close();
	//	auto_close();
}

void core_main(){

	while (1U){
		if (rx_flag){
			rx_flag = 0;
			serial_handler(latest_current_reading);
		}

		// 1kHz button checker
		if (timer_button_checker_1kHz_interrupt){
			timer_button_checker_1kHz_interrupt = 0;
			static buttons last_keypad_button_press = 0;
			gate_stop_handler();
			eventButtonHandler();
			if (!BT_EMER_STOP){
				//				if (last_keypad_button_press != BT_EMER_STOP_enum){
				//					last_keypad_button_press = BT_EMER_STOP_enum;
				if (event_datalogs[event_index].event != event_emergency_stopped){
					gate_stop();
					scrolling = 0;
					new_event.cmd = cmd_ESTOP_pressed;
					new_event.event = event_emergency_stopped;
					new_event.status = status_emergency_stopped;
					eventAdd(new_event);
					eventPrint(event_datalogs[event_index]);
					beep(5);
				}
			}
			if (!BT_OPEN){
				if (event_datalogs[event_index].event != event_open){
					//				if (last_keypad_button_press != BT_OPEN_enum){
					//					last_keypad_button_press = BT_OPEN_enum;
					gate_open();
					scrolling = 0;
					new_event.cmd = cmd_remote_opening;
					new_event.event = event_open;
					new_event.status = status_base;
					eventAdd(new_event);
					eventPrint(event_datalogs[event_index]);
					beep(2);
				}
			}
			if (!BT_CLOSE){
				//				if (last_keypad_button_press != BT_CLOSE_enum){
				//					last_keypad_button_press = BT_CLOSE_enum;
				if (event_datalogs[event_index].event != event_close){
					gate_close();
					scrolling = 0;
					new_event.cmd = cmd_remote_closing;
					new_event.event = event_close;
					new_event.status = status_base;
					eventAdd(new_event);
					eventPrint(event_datalogs[event_index]);
					beep(2);
				}
			}
		}


		// 10 Hz adc reader
		if (timer_adc_reader_10Hz_interrupt){
			timer_adc_reader_10Hz_interrupt = 0;
			adc_get_multiple_channels();

			// process data
			uint8_t current_plus_status[16];
			//			current_plus_status[5] = '\0';
			uint8_t current_hex;
			current_hex = ADC_value[3] / 10;
			latest_current_reading = current_hex;
			updateSerialCurrentReading(current_hex);

			g_write_value = current_hex;
			R_FDL_ExecuteWrite();

			current_plus_status[0] = current_hex/10 + '0';
			current_plus_status[1] = (current_hex % 10) + '0';
			current_plus_status[2] = ' ';
			current_plus_status[3] = 'm';
			current_plus_status[4] = 'A';
			current_plus_status[5] = ' ';
			current_plus_status[6] = ' ';
			current_plus_status[7] = ' ';

			uint8_t max_current_before_obstruction = 80; //mA
			uint8_t * lcd_status_string = " unknown";
			switch (event_datalogs[event_index].event){
			case event_open:
				if (latest_current_reading > max_current_before_obstruction){
					nReady_overcurrent_counter++;
					if (nReady_overcurrent_counter > max_overcurrent_counter){
						nReady_overcurrent_counter=max_overcurrent_counter;
						gate_close();
						new_event.cmd = cmd_mechanical_interference;
						new_event.event = event_autoclose;
						new_event.status = status_auto_close;
						eventAdd(new_event);
						eventPrint(event_datalogs[event_index]);
					}
				}
				lcd_status_string = "    open";
				break;
			case event_close:
				if (latest_current_reading > max_current_before_obstruction){
					nReady_overcurrent_counter++;
					if (nReady_overcurrent_counter > max_overcurrent_counter){
						nReady_overcurrent_counter=max_overcurrent_counter;
						gate_open();
						new_event.cmd = cmd_mechanical_interference;
						new_event.event = event_autoclose;
						new_event.status = status_auto_close;
						eventAdd(new_event);
						eventPrint(event_datalogs[event_index]);
					}
				}
				lcd_status_string = "   close";
				break;
			case event_opened:
				lcd_status_string = "  opened";
				break;
			case event_closed:
				lcd_status_string = "  closed";
				break;
			case event_emergency_stopped:
				lcd_status_string = "Estopped";
				break;
			case event_autoclose:
				lcd_status_string = "autclose";
				if (latest_current_reading > max_current_before_obstruction){
					nReady_overcurrent_counter++;
					if (nReady_overcurrent_counter > max_overcurrent_counter){
						nReady_overcurrent_counter=max_overcurrent_counter;
						PHASE = !PHASE;
					}
				}
				break;
			case event_autoclosed:
				lcd_status_string = "auclosed";
				break;
			case event_mech_interference:
				lcd_status_string = "  object";
				break;
			}

			uint8_t i;
			for (i = 8; i < 16; i++){
				current_plus_status[i] = lcd_status_string[i - 8];
			}

			if ((mode == NORMAL_MODE) && !scrolling) print_lcd(current_plus_status);
		}


		if (ir_new_command_interrupt){
			ir_new_command_interrupt = 0;
			if (ir_rxMessage == IR_GATE_OPEN){
				if (mode == NORMAL_MODE) gate_open();
				new_event.cmd = cmd_remote_opening;
				new_event.event = event_open;
				new_event.status = status_remote_open;
				eventAdd(new_event);
				eventPrint(event_datalogs[event_index]);
				beep(2);
			}
			if (ir_rxMessage == IR_GATE_CLOSE){
				if (mode == NORMAL_MODE) gate_close();
				new_event.cmd = cmd_remote_closing;
				new_event.event = event_close;
				new_event.status = status_remote_close;
				eventAdd(new_event);
				eventPrint(event_datalogs[event_index]);
				beep(2);
			}
			if (ir_rxMessage == IR_GATE_E_STOP){
				gate_stop();
				new_event.cmd = cmd_ESTOP_pressed;
				new_event.event = event_emergency_stopped;
				new_event.status = status_emergency_stopped;
				eventAdd(new_event);
				eventPrint(event_datalogs[event_index]);
				beep(5);
			}
			if (debug_ir_lcd_request){
				uint8_t ascii_word[16];
				word_to_ascii(ir_rxMessage, ascii_word);
				print_lcd(ascii_word);
			}
		}
	}
}
