/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under 
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING 
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT 
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR 
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE 
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software 
* and to discontinue the availability of this software.  By using this software, 
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011, 2013 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_intc_user.c
* Version      : CodeGenerator for RL78/G14 V2.02.00.01 [25 Dec 2013]
* Device(s)    : R5F104LE
* Tool-Chain   : GCCRL78
* Description  : This file implements device driver for INTC module.
* Creation Date: 2016-05-02
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_intc.h"
/* Start user code for include. Do not edit comment generated here */
#include "system/infrared.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
//volatile uint8_t gate_opened = 0, gate_closed = 0;s
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_intc2_interrupt
* Description  : This function is INTP2 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void r_intc2_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
	ir_txMessage |= 0x1;
	R_TAU0_Channel3_Start();
	R_INTC2_Stop();
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_intc3_interrupt
* Description  : This function is INTP3 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void r_intc3_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
	// opened
//	gate_opened = 1;
	P7^=0x80;
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_intc5_interrupt
* Description  : This function is INTP5 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void r_intc5_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
	P7_bit.no7=P1_bit.no6;
	// ir_start_of_rx_interrupt = 1;
	R_INTC5_Stop();
	R_TMR_RJ0_Start();
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
