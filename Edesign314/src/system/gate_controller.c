/*
 * gate_controller.c
 *
 *  Created on: 13 Apr 2016
 *      Author: Daniel
 */


#include "gate_controller.h"
#include "../event/event.h"


/**
 * this reads the switches at either end of the rotating gate,
 * and stops the gate when the switches change state
 */
void gate_stop_handler(){
	if (SW_OPENED && !SW_CLOSED){
		if (last_gate_position != OPEN){
			P7^=0x80;
			nSLEEP = 0;
			gate_is_moving = 0;
			gate_position = OPEN;
			last_gate_position = OPEN;
			//	R_INTC4_Start();
			// print_lcd("opened");
			new_event.cmd = cmd_unspecified;
			new_event.event = event_opened;
			new_event.status = status_gate_opened;
			eventAdd(new_event);
			eventPrint(event_datalogs[event_index]);
		}
	}
	if (SW_CLOSED && !SW_OPENED){
		if (last_gate_position != CLOSED){
			P7^=0x80;
			nSLEEP = 0;
			gate_is_moving = 0;
			gate_position = CLOSED;
			last_gate_position = CLOSED;
			//	R_INTC3_Start();
			// print_lcd("closed");
			new_event.cmd = cmd_unspecified;
			new_event.event = event_closed;
			new_event.status = status_gate_closed;
			eventAdd(new_event);
			eventPrint(event_datalogs[event_index]);
		}
	}
	if (!(SW_OPENED || SW_CLOSED)){
		if (last_gate_position != UNKNOWN){
			last_gate_position = UNKNOWN;
			gate_position = UNKNOWN;
			// print_lcd("unknown");
		}
	}
}

void gate_close(){
	PHASE = 1;
	nSLEEP = 1;
	gate_position = UNKNOWN;
	gate_is_moving = 1;
}

void gate_open(){
	PHASE = 0;
	nSLEEP = 1;
	gate_position = UNKNOWN;
	gate_is_moving = 1;
}

void gate_stop(){
	nSLEEP = 0;
	gate_is_moving = 0;
}
