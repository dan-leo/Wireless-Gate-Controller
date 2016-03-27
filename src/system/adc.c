/*
 * adc.c
 *
 *  Created on: 27 Mar 2016
 *      Author: Daniel
 */

#include "adc.h"
#include "debug.h"

/**
 * get adc reading (single-shot reading of 1 channel)
 * @return: 10-bit adc result
 */
uint16_t adc_get_reading(){
	uint16_t adc_result;
	P7^=0x80;
	R_ADC_Start();
	while(!adc_ready);
	adc_ready=0;
	R_ADC_Get_Result(&adc_result);
	R_ADC_Stop();
	return adc_result;
}

/**
 * service the adc interrupt and get multiple readings
 */
void adc_multi_channel_interrupt_service_routine(){
	static uint8_t cur_channel = 0;
	switch (INTAD_FSM_state) {
	case AD_SAMPLING:
		R_ADC_Get_Result((&(ADC_value[cur_channel])));
		cur_channel++;
		if (cur_channel == NUM_CHANNELS) {
			INTAD_FSM_state = AD_IDLE;
			cur_channel = 0;
			ADC_done = 1;
		}
		break;
	case AD_IDLE:
	default:
		INTAD_FSM_state = AD_IDLE;
		break;
	}
}

/**
 * get multiple channels from the adc
 */
void adc_get_multiple_channels(){
	INTAD_FSM_state = AD_SAMPLING;
	ADC_done = 0;
	R_ADC_Start(); /* Start ADC (ADCS = 0n) */
	while (!ADC_done)
		;

	if (debug_adc_serial){
		serial_print_adc(ADC_value[2]);
		serial_print_adc(ADC_value[3]);
		//				lcd_clear();
		//				uint8_t a = ADC_value[2] >> 8;
		//				writeByteLcd(1,a);
		//				a = (uint8_t)ADC_value[2];
		//				writeByteLcd(1,a);
	}

	/*			adc_last_reading = adc_get_reading();*/
	volatile tiny_adc_reading = ADC_value[2] >> 3;
	motor_power_ratio(tiny_adc_reading,128);
	/*if (adc_enable){
				serial_print_adc(adc_last_reading);
			}*/


	/*			pwm_counter++;
			pwm_counter%=2*1000/30;
			if (pwm_counter < 1000/30 ){
				//				P7|=0x80;
			}
			else {
				//				P7&=0x7F;
			}*/
	//			PM7=0x7F;
	//			P7^=0x80;
	/*			if (soft_timer < 9){
				PM7=0x7F;
				P7^=0x80;
				soft_timer++;
			}
			else if (soft_timer >= 9)
			{
				lcd_clear();
				EI();
//				delayNoInt(100);
//				char lcd_messege[20] = "abcdefghijklmnopqrst";
//				print_lcd(lcd_messege, 20);
				soft_timer++;
			}*/
}
