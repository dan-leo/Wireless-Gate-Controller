/*
 * pins.h
 *
 *  Created on: 27 Mar 2016
 *      Author: Daniel
 */

#ifndef PINS_H_
#define PINS_H_

#define PHASE P4_bit.no3
#define nSLEEP P4_bit.no2

#define SW_OPENED P0_bit.no5
#define SW_CLOSED P0_bit.no6

#define IR_LED_TX P13_bit.no0
#define IR_BUTTON_TX P5_bit.no1
#define IR_RX P1_bit.no6

#define BT_OPEN P5_bit.no2
#define BT_CLOSE P1_bit.no7
#define BT_EMER_STOP P5_bit.no3
#define BT_MENU_UP P5_bit.no4
#define BT_MENU_DOWN P5_bit.no5

typedef enum buttons {
	BT_OPEN_enum,
	BT_CLOSE_enum,
	BT_EMER_STOP_enum,
	BT_MENU_UP_enum,
	BT_MENU_DOWN_enum
} buttons;

#endif /* PINS_H_ */
