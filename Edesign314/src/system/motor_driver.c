/*
 * motor_driver.c
 *
 *  Created on: 27 Mar 2016
 *      Author: Daniel
 */

#include "motor_driver.h"

/**
 * change duty cycle of pwm signal for motor.
 * @param: uint8_t percentage
 */
void pwm_change_duty_cycle(uint8_t percentage){
	volatile uint16_t max_motor_steps = TRDGRA0;
	volatile uint16_t falling_edge_of_decrementing_steps;
	if (percentage > 100) percentage = 100;
	if (percentage < -100) percentage = -100;
	falling_edge_of_decrementing_steps = (100*max_motor_steps - percentage*max_motor_steps)/100;
	TRDGRB0 = falling_edge_of_decrementing_steps;
}

/**
 * change power of motor.
 * motor pwm % limited to specified value.
 * @param: 0-100%
 */
void motor_power(uint8_t percentage){
	volatile uint8_t max_duty_cycle_percentage = 44;
	volatile uint16_t falling_edge_of_decrementing_steps;
	volatile uint16_t scaled_steps;
	scaled_steps = scale_down(TRDGRA0,max_duty_cycle_percentage);
	scaled_steps = scale_down(scaled_steps,percentage);
	falling_edge_of_decrementing_steps = TRDGRA0 - scaled_steps;
	TRDGRB0 = falling_edge_of_decrementing_steps;
}

/**
 * update power of motor according ratio
 * motor pwm % limited to specified value.
 *
 * be careful of overflow!!
 *
 * @param: fraction
 */
void motor_power_ratio(uint16_t ratio_numerator, uint16_t ratio_denominator){
	volatile uint8_t max_duty_cycle_percentage = 44;
	volatile uint16_t falling_edge_of_decrementing_steps;
	volatile uint16_t scaled_steps;
	scaled_steps = scale_down(TRDGRA0,max_duty_cycle_percentage);
	scaled_steps = scale_down_ratio(scaled_steps, ratio_numerator, ratio_denominator);
	falling_edge_of_decrementing_steps = TRDGRA0 - scaled_steps;
	if (TRDGRA0 == falling_edge_of_decrementing_steps){
		falling_edge_of_decrementing_steps = TRDGRA0 - 1;
	}
	TRDGRB0 = falling_edge_of_decrementing_steps;
}

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
			//					R_INTC4_Start();
			print_lcd("opened");
		}
	}
	if (SW_CLOSED && !SW_OPENED){
		if (last_gate_position != CLOSED){
			P7^=0x80;
			nSLEEP = 0;
			gate_is_moving = 0;
			gate_position = CLOSED;
			last_gate_position = CLOSED;
			//					R_INTC3_Start();
			print_lcd("closed");
		}
	}
	if (!(SW_OPENED || SW_CLOSED)){
		if (last_gate_position != UNKNOWN){
			last_gate_position = UNKNOWN;
			gate_position = UNKNOWN;
			print_lcd("unknown");
		}
	}
}
