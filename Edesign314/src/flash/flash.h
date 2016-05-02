/*
 * flash.h
 *
 *  Created on: 02 May 2016
 *      Author: Daniel
 */

#ifndef FLASH_H_
#define FLASH_H_


#include "../iodefine.h"
#include "../iodefine_ext.h"
#include "../r_cg_macrodriver.h"

//#define FLASH_START_ADDRESS     (0xF1000U)                              /* Start address of data flash               */
#define FLASH_START_ADDRESS     (0xFEA0U)                              /* Start address of data flash               */
#define TARGET_BLOCK            (0U)                                    /* Block number of target(0 ~ 3)             */
#define BLOCK_SIZE              (0x400U)                                /* Block size                                */
#define WRITE_SIZE              (1U)                                    /* Size of data to be written at once        */
#define MAX_VALUE               (0xFFU)                                 /* Maximum value of writing                  */
#define MAX_ADDRESS             ((TARGET_BLOCK + 1U) * BLOCK_SIZE - 1U) /* Maximum address of writing                */
#define PFDL_NG                 (1U)                                    /* Failure to Data Flash                     */
#define FDL_FRQ                 (16U)                                   /* Setting frequency (MHz)                   */
#define FDL_VOL                 (0x00U)                                 /* Voltage mode                              */

uint8_t  g_read_value;
uint8_t  g_write_value;
uint16_t g_write_address;

void    R_FDL_Init(void);
uint8_t R_FDL_BlankCheck(void);
uint8_t R_FDL_Erase(void);
uint8_t R_FDL_Verify(void);
void    R_FDL_Read(void);
uint8_t R_FDL_Write(void);
void    R_FDL_ChangeAddress(void);
uint8_t R_FDL_ExecuteWrite(void);
uint8_t R_FDL_ClearDataFlash(void);

#if    TARGET_BLOCK > 3U
#error target_block error!!
#endif

#endif /* FLASH_H_ */
