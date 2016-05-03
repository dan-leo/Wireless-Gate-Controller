/*
 * event.h
 *
 *  Created on: 02 May 2016
 *      Author: Daniel
 */

#ifndef EVENT_H_
#define EVENT_H_

#define max_events 40

#include "../iodefine.h"
#include "../iodefine_ext.h"
#include "../r_cg_macrodriver.h"

#include "../r_cg_rtc.h"

#include "../system/globals.h"
#include "../flash/flash.h"

// index increments as events are added
volatile int8_t event_index;
volatile datalog_t event_datalogs[max_events];

// new event buffer
datalog_t new_event;

void eventAdd(datalog_t event);
void eventPrint(datalog_t event);

#endif /* EVENT_H_ */
