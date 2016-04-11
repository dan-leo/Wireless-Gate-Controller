/*
 * core.c
 *
 *  Created on: 11 Apr 2016
 *      Author: Daniel
 */

#include "core.h"

void core_setup(){
//    R_TAU0_Channel0_Start();
//    R_TAU0_Channel3_Start();
    R_INTC1_Start();
    R_INTC2_Start();

    ir_enable_pulse_train = 0;
}
