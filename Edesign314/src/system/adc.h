/*
 * adc.h
 *
 *  Created on: 27 Mar 2016
 *      Author: Daniel
 */

#ifndef ADC_H_
#define ADC_H_

#include "../iodefine.h"
#include "../iodefine_ext.h"
#include "../r_cg_macrodriver.h"

// adc
#define NUM_CHANNELS (4)
#define AD_SAMPLING (1)
#define AD_IDLE (0)

volatile uint8_t ADC_done; /* Flag: conversion complete */
volatile uint16_t ADC_value[NUM_CHANNELS];
volatile uint8_t INTAD_FSM_state;
volatile uint8_t adc_ready;

volatile uint8_t latest_current_reading;

uint16_t adc_get_reading();
void adc_multi_channel_interrupt_service_routine();
char *adc_ascii_current(uint16_t *adc_val);

#endif /* ADC_H_ */
