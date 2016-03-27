/*
 * lcd.h
 *
 *  Created on: 24 Mar 2015
 *      Author: jtreurn
 *      Edited: Daniel Robinson
 */

#ifndef LCD_H_
#define LCD_H_

	// Local LCD Port 7 bit allocation
#define LCD_RS              P7_bit.no6		// Register select
#define LCD_RW              P7_bit.no5		// Read/Write line
#define LCD_E               P7_bit.no4		// Enable Strobe
#define LCD_D0              P7_bit.no0		// 4 Nibble data lines
#define LCD_D1              P7_bit.no1
#define LCD_D2              P7_bit.no2
#define LCD_D3              P7_bit.no3

	// Other LCD defines
#define LCD_DATA_WR 		1				// Data write
#define LCD_CTRL_WR 		0				// Command
#define LCD_LINE1 			0				// Line 1 offset
#define LCD_LINE2 			16				// Line 1 offset

	// LCD Commands
#define LCD_CLEAR        	0x01			// Clear LCD display and home cursor
#define LCD_HOME_L1      	0x80			// Move cursor to line 1
#define LCD_HOME_L2      	0xC0			// Move cursor to line 2
#define CURSOR_MODE_DEC  	0x04			// Cursor auto decrement after R/W
#define CURSOR_MODE_INC  	0x06			// Cursor auto increment after R/W
#define FUNCTION_SET     	0x28			// Setup, 4 bits,2 lines, 5X7
#define LCD_CURSOR_ON    	0x0E			// Display ON with Cursor
#define LCD_CURSOR_OFF   	0x0C			// Display ON with Cursor off
#define LCD_CURSOR_BLINK 	0x0D			// Display on with blinking cursor
#define LCD_CURSOR_LEFT  	0x10			// Move Cursor Left One Position
#define LCD_CURSOR_RIGHT 	0x14			// Move Cursor Right One Position
#define LCD_DISPLAY_ON   	0x04			// Turn display ON
#define LCD_TWO_LINE     	0x08			// Set cursor to start of line 2

	// Enable Strobe macros
#define	SET_LCD_EN_HIGH		LCD_E = 1 		//asm("SET1 0xFFF0E.6")
#define	SET_LCD_EN_LOW		LCD_E = 0 		//asm("CLR1 0xFFF0E.6")

#include "../r_cg_macrodriver.h"
#include <string.h>

	// Prototypes
void initLcd(void);
void writeNibbleLcd(uint8_t reg, uint8_t nibble);
void writeByteLcd(uint8_t reg, uint8_t value);

void lcd_clear();
void print_lcd(char *message);
void print_long_message(char * message);

#endif /* LCD_H_ */
