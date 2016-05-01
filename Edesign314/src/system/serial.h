/*
 * serial.h
 *
 *  Created on: 27 Mar 2016
 *      Author: Daniel
 */

#ifndef SERIAL_H_
#define SERIAL_H_

#include "../iodefine.h"
#include "../iodefine_ext.h"
#include "../r_cg_macrodriver.h"
#include "debug.h"
#include "pins.h"
#include "time.h"

uint8_t serial_rx;
uint8_t serial_test_mode;

void serial_print_adc(uint16_t reading);
void echo(uint8_t c);
void serial_handler();

#endif /* SERIAL_H_ */
