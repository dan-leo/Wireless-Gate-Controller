# Wireless-Gate-Controller
This is a project for E-design 314, a course hosted by Stellenbsch University. 

What does it do?
It opens a gate, quite simply.

It is built on a prototype board developed by Stellenbosch University. The routing is minimal, however, and the idea
is that students develop their own wiring/designs.

Of the main hardware, it has a Renesas RL78/G14 as the brains, a Powertip 16x1 lcd for user feedback, external FT230XS uart chip to communicate with an external test program at 9600 baud, 
a DRV8801 motor driver (40kHz PWM), a TSOP348X infra-red receiver, and a 2kHz buzzer for warning conditions.

There also exists an infra-red remote to control the gate (open/close/emergency stop). The heart is a Renesas RL78/G12 1026A processor. 
I call it the heart rather affectionately since I had a watchdog timer going off which gave it a pretty heartbeat.
I have also attached a 3.7V 1100mAH lithium battery, which might have been a little bit of an overkill.

Before I built the remote, I soldered in a transmitter/receiver led pair and a button,
for testing purposes and so that coding is faster.


* Variable motor speed works
* ADC current reading + display reading works.
* Infrared tx (manchester encoding) works on G12. (tx also on G14 -- just for debugging/testing purposes).
* Receive also works and displays bits on lcd.

TODO:
* Add 3G. (Open with a cellphone anywhere in the world).
* Possibly add a card reader.

Debugging: have a look at the debug.h folder and 
change variables in core_setup() to enable certain serial/lcd debugging outputs

All buttons and switches are connected with pull-up resistors as standard.

When a new event occurs, the lcd will update with the latest event (with timestamp) accordingly

One can cycle through the events using up/down menu keys (buttons).

Events include:
* Opening
* Opened
* Closing
* Closed
* Autoclosing
* Autoclosed
* Mechanical Interference Event

## Pinout
### G14 Gate controller

#### CN2
Pin | Signal Name | RL78/G14 Pin Name | Function
:---: | :---: | --- | ---
1 | `GND` | VSS / EVSS | Ground
2 | `VDD` | VDD / EVDD | 5V external input
8 | `P15` | P15_SCK20_Z_SCL20_TRDIOD0 | This is the 40kHz pwm enable for the motor (directed to the enable pin of the DRV8801)
9 | `P16` | P16_TIO1_TO01_INTP5_TRDIOC0_IVREF0 | This interrupt pin is triggered by data from the IR receiver 
10 | `P17` | P17_TI01_TO02_TRDIOA0_TRDCLK0_IVCMP0 | This is connected to a button which closes gate upon pressing
11 | `P55` | P16_TIO1_TO01_INTP5_TRDIOC0_IVREF0 | This is connected to a button which shows the previous event upon pressing
12| `P54` | P16_TIO1_TO01_INTP5_TRDIOC0_IVREF0 | This is connected to a button which shows the next event upon pressing
13 | `P53` | P16_TIO1_TO01_INTP5_TRDIOC0_IVREF0 | This is connected to a button which will stop the gate from moving upon pressing (as if an emergency stop)
14 | `P52` | P16_TIO1_TO01_INTP5_TRDIOC0_IVREF0 | This is connected to a button which opens gate upon pressing
15| `P51` | P51_INTP2_SO00_TXD0_TOOLTXD_TRGIOB | *For testing.* A button triggers this hardware interrupt which in turn sends a test manchestor string.
23| `P76` | P76_KR6_INTP10_RXD2 | LCD Register select
24| `P75` | P75_KR5_INTP9_SCK01_SCL01 | LCD Read /Write
25| `P74` | P74_KR4_INTP8_SI01_SDA01 | LCD Enable
26| `P73` | P73_KR3_SO01 | LCD Data channel 3
27| `P72` | P72_KR2_SO21 | LCD Data channel 2
28| `P71` | P71_KR1_SI21_SDA21 | LCD Data channel 1
29| `P70` | P70_KR0_SCK21_SCL21 | LCD Data channel 0

#### CN3
Pin | Signal Name | RL78/G14 Pin Name | Function
:---: | :---: | --- | ---
1 | `GND` | VSS / EVSS | Ground
2 | `VDD` | VDD / EVDD | 5V external input
3 | `P140`| P140_PCLBUZ0_INTP6 | 2kHZ buzzer output
7 | `P02` | P02_ANI17_SO10_TxD1 | Serial output to external test program
8 | `P03` | P03_ANI16_SI10_RxD1_SDA10 | Serial input from external test program
10| `P130`| P130 | *For testing.* Here is where a manchestor string is output to a built-in IR led.
11| `P20` | P20_ANI0_AVREFP | *Unused* ADC input
12| `P21` | P21_ANI0_AVREFM | *Unused* ADC input TODO: use potentiometer to cycle through menu.
13| `P22` | P22_ANI2_ANO0 | This reads a potentiometer built-in the RL78 G14. It controls the speed of the gate motor.
14| `P23` | P23_ANI3_ANO1 | This reads in a current value from 0 - 100mA, converted to a value between 0 - 5 V, with 10 bit accuracy.
22| `P42` | P42 | This digital output controls the phase/direction that the gate motor is turning.
23| `P41` | P41 | This digital output can sleep the motor driver (nSleep).
27| `P06` | P06 | Digital in which listens for when the gate closes/is closed (port is polled ~ debouncing).
28| `P05` | P05 | Digital in which listens for when the gate opens/is opened (port is polled ~ debouncing).
29| `P30` | P30/INTP3/RTC1HZ/SCK00/SCL00/TRJO0 | *Unused.* Hardware interrupt which listens for when the gate opens/closes.


### G12 Infra-red transmitter

Pin | Signal Name | RL78/G14 Pin Name | Function
:---: | :---: | --- | ---
3 | `P41`| P41_ANI22_SO01_SDA01_TI02_TO02_INTP1 | Open gate button
9 | `GND` | VSS | Ground
10 | `VDD` | VDD | 3.3V external input
14 | `P13` | P13_ANI19_TI00_TO00_INTP2 | Close gate button
17 | `P10` | P10_ANI16_PCLBUZ0_SCK00_SCL00 | IR Led output
