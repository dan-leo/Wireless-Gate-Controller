/*
 * event.c
 *
 *  Created on: 02 May 2016
 *      Author: Daniel
 */

#include "event.h"
#include "../system/pins.h"
#include "../system/utility.h"
#include "../system/serial.h"
#include "../r_cg_serial.h"
#include "../system/delay.h"
#include "../system/buzzer.h"


//typedef enum menu_buttons_e {
//	MENU_UP,
//	MENU_DOWN
//} menu_buttons_e;

volatile uint8_t ready = 0;
volatile uint8_t event_index_on_lcd = 0;
volatile uint8_t events = 0;

void eventInit(){
	event_index=-1;
	ready = true;
}

void eventAdd(datalog_t event){
	event_index++;
	event_index%=max_events;
	event_index_on_lcd = event_index - 1;
	rtc_counter_value_t time_now;
	R_RTC_Get_CounterValue(&time_now);
	event.time = time_now;
	event.used = 1;
	events++;
	event_datalogs[event_index] = event;
}

void eventPrint(datalog_t event){
	if (event.used){
		uint8_t lcd_event[16];
		//	lcd_event[0] = (event.time.month >> 4) + '0';
		lcd_event[0] = (event.time.month & 0x0F) + '0';
		//	lcd_event[2] = (event.time.day >> 4) + '0';
		lcd_event[1] = (event.time.day & 0x0F) + '0';
		lcd_event[2] = (event.time.hour >> 4) + '0';
		lcd_event[3] = (event.time.hour & 0x0F) + '0';
		lcd_event[4] = (event.time.min >> 4) + '0';
		lcd_event[5] = (event.time.min & 0x0F) + '0';
		lcd_event[6] = (event.time.sec >> 4) + '0';
		lcd_event[7] = (event.time.sec & 0x0F) + '0';

		uint8_t * lcd_event_string = " unknown";
		switch (event.event){
		case event_open:
			lcd_event_string = "    open";
			break;
		case event_close:
			lcd_event_string = "   close";
			break;
		case event_opened:
			lcd_event_string = "  opened";
			break;
		case event_closed:
			lcd_event_string = "  closed";
			break;
		case event_emergency_stopped:
			lcd_event_string = "Estopped";
			break;
		case event_autoclose:
			lcd_event_string = "autclose";
			break;
		case event_autoclosed:
			lcd_event_string = "auclosed";
			break;
		case event_mech_interference:
			lcd_event_string = "  object";
			break;
		}

		uint8_t i;
		for (i = 8; i < 16; i++){
			lcd_event[i] = lcd_event_string[i - 8];
		}

		print_lcd(lcd_event);
	}
	else{
		print_lcd("erased");
	}
}

void eventButtonHandler(){
		if (!BT_MENU_DOWN && ready){
			ready = false;
			scrolling = 1;
			event_index_on_lcd--;
			event_index_on_lcd%=events;
			if (events) eventPrint(event_datalogs[event_index_on_lcd]);
			beep(1);
		}
		else if (!BT_MENU_UP && ready){
			ready = false;
			scrolling = 1;
			event_index_on_lcd++;
			event_index_on_lcd%=events;
			if (events) eventPrint(event_datalogs[event_index_on_lcd]);
			beep(1);
		}
		if (BT_MENU_DOWN && BT_MENU_UP){
			ready = true;
		}
}

void eventSerialRead(){

	int i;
	uint8_t serial_datalog_buffer[10];
	serial_datalog_buffer[0] = 0xF6;
	for (i = 0; i < events; i++){
		serial_datalog_buffer[1] = event_datalogs[i].used;
		serial_datalog_buffer[2] = event_datalogs[i].cmd;
		serial_datalog_buffer[3] = toHex(event_datalogs[i].time.month);
		serial_datalog_buffer[4] = toHex(event_datalogs[i].time.day);
		serial_datalog_buffer[5] = toHex(event_datalogs[i].time.hour);
		serial_datalog_buffer[6] = toHex(event_datalogs[i].time.min);
		serial_datalog_buffer[7] = toHex(event_datalogs[i].time.sec);
		serial_datalog_buffer[8] = event_datalogs[i].status;
		serial_datalog_buffer[9] = i;
		if (events) R_UART1_Send(serial_datalog_buffer, 10);
		else echo(0xF6);
		delayMillis(100);
	}

}
