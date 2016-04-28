# Wireless-Gate-Controller
This is a project for E-design 314, a course hosted by Stellenbsch University. 

What does it do?
It opens a gate, quite simply.

It is built on a prototype board developed by Stellenbosch University. The routing is minimal, however, and the idea
is that students develop their own wiring/designs.

Of the main hardware, it has a renesas RL78/G14 as the brains, a Powertip 16x1 lcd for user feedback, external FT230XS uart chip to communicate with an external test program at 9600 baud, 
a DRV8801 motor driver (40kHz PWM), a TSOP348X infra-red receiver, and a 1kHz buzzer for warning conditions.

There also exists an infra-red remote to control the gate (open/close/emergency stop). The heart is a Renesas RL78/G12 1026A processor. 
I call it the heart rather affectionately since I had a watchdog timer going off which gave it a pretty heartbeat.
I have also attached a 3.7V 1100mAH lithium battery, which might have been a little bit of an overkill.

Before I built the remote, I soldered in a transmitter/receiver led pair and a button,
for testing purposes and so that coding is faster.



ADC current reading + display reading works.
<br>
Infrared tx (manchester encoding) works on G12. (tx also on G14 -- just for debugging purposes).
<br>
Receive also works and displays bits on lcd.

Full marks again for the third demo.

TODO: 
add pinout here<br>
make this all more user friendly!! :)

Meet specs for fourth demo.
Possibly add a card reader.

Debugging: have a look at the debug.h folder and 
change variables to enable certain serial/lcd debugging outputs


CN2 | Signal Name | RL78/G14 Pin Name | Function
:---: | :---: | --- | ---
1 | `GND` | VSS / EVSS | Ground
2 | `VDD` | VDD / EVDD | 5V external input
8 | `P15` | P15_SCK20_Z_SCL20_TRDIOD0 | This is the 40kHz pwm enable for the motor (directed to the enable pin of the DRV8801)
9 | `P16` | P16_TIO1_TO01_INTP5_TRDIOC0_IVREF0 | This interrupt pin is triggered by data from the IR receiver 
15| `P51` | P51_INTP2_SO00_TXD0_TOOLTXD_TRGIOB | A button triggers this hardware interrupt which in turn sends a test manchestor string.


