EESchema Schematic File Version 2
LIBS:H_Bridge-rescue
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:H_Bridge
LIBS:w_analog
LIBS:H_Bridge-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L L298-RESCUE-H_Bridge U1
U 1 1 5698B733
P 5650 4500
F 0 "U1" H 4700 4850 60  0000 C CNN
F 1 "L298" H 6400 4850 60  0000 C CNN
F 2 "H_Bridge:L298" H 6400 4850 60  0001 C CNN
F 3 "" H 6400 4850 60  0000 C CNN
	1    5650 4500
	1    0    0    -1  
$EndComp
$Comp
L D D4
U 1 1 5699AD7D
P 3250 4700
F 0 "D4" H 3250 4800 50  0000 C CNN
F 1 "D" H 3250 4600 50  0000 C CNN
F 2 "Diodes_ThroughHole:Diode_TO-220_Horizontal" H 3250 4700 50  0001 C CNN
F 3 "" H 3250 4700 50  0000 C CNN
	1    3250 4700
	0    -1   1    0   
$EndComp
$Comp
L D D5
U 1 1 5699E224
P 3550 4700
F 0 "D5" H 3550 4800 50  0000 C CNN
F 1 "D" H 3550 4600 50  0000 C CNN
F 2 "Diodes_ThroughHole:Diode_TO-220_Horizontal" H 3550 4700 50  0001 C CNN
F 3 "" H 3550 4700 50  0000 C CNN
	1    3550 4700
	0    -1   1    0   
$EndComp
$Comp
L D D6
U 1 1 5699E29D
P 3250 5700
F 0 "D6" H 3250 5800 50  0000 C CNN
F 1 "D" V 3250 5600 50  0000 C CNN
F 2 "Diodes_ThroughHole:Diode_TO-220_Horizontal" H 3250 5700 50  0001 C CNN
F 3 "" H 3250 5700 50  0000 C CNN
	1    3250 5700
	0    -1   1    0   
$EndComp
$Comp
L D D7
U 1 1 5699E377
P 3550 5700
F 0 "D7" H 3550 5800 50  0000 C CNN
F 1 "D" H 3550 5600 50  0000 C CNN
F 2 "Diodes_ThroughHole:Diode_TO-220_Horizontal" H 3550 5700 50  0001 C CNN
F 3 "" H 3550 5700 50  0000 C CNN
	1    3550 5700
	0    -1   1    0   
$EndComp
$Comp
L PF1262 R1
U 1 1 569C510F
P 6050 5950
F 0 "R1" H 6150 6000 60  0000 C CNN
F 1 "PF1262" V 5950 5950 60  0000 C CNN
F 2 "H_Bridge:PF1262" V 5950 5950 60  0001 C CNN
F 3 "" V 5950 5950 60  0000 C CNN
	1    6050 5950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR01
U 1 1 569F124E
P 5750 3550
F 0 "#PWR01" H 5750 3300 50  0001 C CNN
F 1 "GND" H 5750 3400 50  0000 C CNN
F 2 "" H 5750 3550 50  0000 C CNN
F 3 "" H 5750 3550 50  0000 C CNN
	1    5750 3550
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X03 P1
U 1 1 569F1348
P 5100 2350
F 0 "P1" H 5100 2550 50  0000 C CNN
F 1 "CONN_01X03" V 5200 2350 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x03" H 5100 2350 50  0001 C CNN
F 3 "" H 5100 2350 50  0000 C CNN
	1    5100 2350
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR02
U 1 1 569F211E
P 6050 6100
F 0 "#PWR02" H 6050 5850 50  0001 C CNN
F 1 "GND" H 6050 5950 50  0000 C CNN
F 2 "" H 6050 6100 50  0000 C CNN
F 3 "" H 6050 6100 50  0000 C CNN
	1    6050 6100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR03
U 1 1 569F2E41
P 4050 6100
F 0 "#PWR03" H 4050 5850 50  0001 C CNN
F 1 "GND" H 4050 5950 50  0000 C CNN
F 2 "" H 4050 6100 50  0000 C CNN
F 3 "" H 4050 6100 50  0000 C CNN
	1    4050 6100
	1    0    0    -1  
$EndComp
$Comp
L LM75A-RESCUE-H_Bridge U2
U 1 1 569F3E76
P 8550 4650
F 0 "U2" H 8350 4950 60  0000 C CNN
F 1 "LM75A" H 8450 4200 60  0000 C CNN
F 2 "H_Bridge:LM75A" H 8450 4200 60  0001 C CNN
F 3 "" H 8450 4200 60  0000 C CNN
	1    8550 4650
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR04
U 1 1 569F3F51
P 8500 4100
F 0 "#PWR04" H 8500 3850 50  0001 C CNN
F 1 "GND" H 8500 3950 50  0000 C CNN
F 2 "" H 8500 4100 50  0000 C CNN
F 3 "" H 8500 4100 50  0000 C CNN
	1    8500 4100
	1    0    0    -1  
$EndComp
$Comp
L R R2
U 1 1 569F4573
P 8500 5350
F 0 "R2" V 8580 5350 50  0000 C CNN
F 1 "10k" V 8500 5350 50  0000 C CNN
F 2 "Resistors_SMD:R_0201" V 8430 5350 50  0001 C CNN
F 3 "" H 8500 5350 50  0000 C CNN
	1    8500 5350
	0    -1   -1   0   
$EndComp
$Comp
L R R3
U 1 1 569F48A3
P 8500 5600
F 0 "R3" V 8580 5600 50  0000 C CNN
F 1 "10k" V 8500 5600 50  0000 C CNN
F 2 "Resistors_SMD:R_0201" V 8430 5600 50  0001 C CNN
F 3 "" H 8500 5600 50  0000 C CNN
	1    8500 5600
	0    -1   -1   0   
$EndComp
$Comp
L R R4
U 1 1 569F48F3
P 8500 5900
F 0 "R4" V 8580 5900 50  0000 C CNN
F 1 "10k" V 8500 5900 50  0000 C CNN
F 2 "Resistors_SMD:R_0201" V 8430 5900 50  0001 C CNN
F 3 "" H 8500 5900 50  0000 C CNN
	1    8500 5900
	0    -1   -1   0   
$EndComp
$Comp
L PWR_FLAG #FLG05
U 1 1 569F52B0
P 6700 2750
F 0 "#FLG05" H 6700 2845 50  0001 C CNN
F 1 "PWR_FLAG" H 6700 2930 50  0000 C CNN
F 2 "" H 6700 2750 50  0000 C CNN
F 3 "" H 6700 2750 50  0000 C CNN
	1    6700 2750
	0    -1   -1   0   
$EndComp
$Comp
L PWR_FLAG #FLG06
U 1 1 569F52E0
P 8150 3400
F 0 "#FLG06" H 8150 3495 50  0001 C CNN
F 1 "PWR_FLAG" H 8150 3580 50  0000 C CNN
F 2 "" H 8150 3400 50  0000 C CNN
F 3 "" H 8150 3400 50  0000 C CNN
	1    8150 3400
	-1   0    0    1   
$EndComp
$Comp
L PWR_FLAG #FLG07
U 1 1 569F5310
P 8750 3500
F 0 "#FLG07" H 8750 3595 50  0001 C CNN
F 1 "PWR_FLAG" H 8750 3680 50  0000 C CNN
F 2 "" H 8750 3500 50  0000 C CNN
F 3 "" H 8750 3500 50  0000 C CNN
	1    8750 3500
	-1   0    0    1   
$EndComp
Text Notes 9150 3050 0    60   ~ 0
Pin 1: +5V Logic Power\nPin 2: +3.3V Logic Power\nPin 3: Ground (Microcontroller Power)
$Comp
L LED D1
U 1 1 569FAE61
P 4600 3250
F 0 "D1" H 4600 3350 50  0000 C CNN
F 1 "LED" H 4600 3150 50  0000 C CNN
F 2 "LEDs:LED-3MM" H 4600 3250 50  0001 C CNN
F 3 "" H 4600 3250 50  0000 C CNN
	1    4600 3250
	0    -1   -1   0   
$EndComp
$Comp
L LED D2
U 1 1 569FAF14
P 4950 3250
F 0 "D2" H 4950 3350 50  0000 C CNN
F 1 "LED" H 4950 3150 50  0000 C CNN
F 2 "LEDs:LED-3MM" H 4950 3250 50  0001 C CNN
F 3 "" H 4950 3250 50  0000 C CNN
	1    4950 3250
	0    -1   -1   0   
$EndComp
$Comp
L LED D3
U 1 1 569FAF59
P 5250 3250
F 0 "D3" H 5250 3350 50  0000 C CNN
F 1 "LED" H 5250 3150 50  0000 C CNN
F 2 "LEDs:LED-3MM" H 5250 3250 50  0001 C CNN
F 3 "" H 5250 3250 50  0000 C CNN
	1    5250 3250
	0    -1   -1   0   
$EndComp
Text Notes 7350 7500 0    60   ~ 0
H Bridge
Text Notes 8150 7650 0    60   ~ 0
1/19/16
Text Notes 7000 6850 0    60   ~ 0
Max Hansen\nu0629365\nLab Section 005
$Comp
L R R6
U 1 1 56A16746
P 5250 2800
F 0 "R6" V 5330 2800 50  0000 C CNN
F 1 "10k" V 5250 2800 50  0000 C CNN
F 2 "Resistors_SMD:R_0201" V 5180 2800 50  0001 C CNN
F 3 "" H 5250 2800 50  0000 C CNN
	1    5250 2800
	1    0    0    -1  
$EndComp
$Comp
L R R7
U 1 1 56A16847
P 4950 2800
F 0 "R7" V 5030 2800 50  0000 C CNN
F 1 "10k" V 4950 2800 50  0000 C CNN
F 2 "Resistors_SMD:R_0201" V 4880 2800 50  0001 C CNN
F 3 "" H 4950 2800 50  0000 C CNN
	1    4950 2800
	1    0    0    -1  
$EndComp
$Comp
L R R8
U 1 1 56A1689B
P 4600 2800
F 0 "R8" V 4680 2800 50  0000 C CNN
F 1 "10k" V 4600 2800 50  0000 C CNN
F 2 "Resistors_SMD:R_0201" V 4530 2800 50  0001 C CNN
F 3 "" H 4600 2800 50  0000 C CNN
	1    4600 2800
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR08
U 1 1 56A173D2
P 5250 3550
F 0 "#PWR08" H 5250 3300 50  0001 C CNN
F 1 "GND" H 5250 3400 50  0000 C CNN
F 2 "" H 5250 3550 50  0000 C CNN
F 3 "" H 5250 3550 50  0000 C CNN
	1    5250 3550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR09
U 1 1 56A17410
P 4950 3550
F 0 "#PWR09" H 4950 3300 50  0001 C CNN
F 1 "GND" H 4950 3400 50  0000 C CNN
F 2 "" H 4950 3550 50  0000 C CNN
F 3 "" H 4950 3550 50  0000 C CNN
	1    4950 3550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR010
U 1 1 56A1744E
P 4600 3550
F 0 "#PWR010" H 4600 3300 50  0001 C CNN
F 1 "GND" H 4600 3400 50  0000 C CNN
F 2 "" H 4600 3550 50  0000 C CNN
F 3 "" H 4600 3550 50  0000 C CNN
	1    4600 3550
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X03 P3
U 1 1 56A1AE30
P 9300 3300
F 0 "P3" H 9300 3500 50  0000 C CNN
F 1 "CONN_01X03" V 9400 3300 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Angled_1x03" H 9300 3300 50  0001 C CNN
F 3 "" H 9300 3300 50  0000 C CNN
	1    9300 3300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR011
U 1 1 56A1B452
P 2450 5450
F 0 "#PWR011" H 2450 5200 50  0001 C CNN
F 1 "GND" H 2450 5300 50  0000 C CNN
F 2 "" H 2450 5450 50  0000 C CNN
F 3 "" H 2450 5450 50  0000 C CNN
	1    2450 5450
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X02 P2
U 1 1 56A1B9D0
P 6850 2300
F 0 "P2" H 6850 2450 50  0000 C CNN
F 1 "CONN_01X02" V 6950 2300 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Angled_1x02" H 6850 2300 50  0001 C CNN
F 3 "" H 6850 2300 50  0000 C CNN
	1    6850 2300
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR012
U 1 1 56A1BB58
P 7050 2850
F 0 "#PWR012" H 7050 2600 50  0001 C CNN
F 1 "GND" H 7050 2700 50  0000 C CNN
F 2 "" H 7050 2850 50  0000 C CNN
F 3 "" H 7050 2850 50  0000 C CNN
	1    7050 2850
	1    0    0    -1  
$EndComp
Text Notes 4800 2150 0    60   ~ 0
Pin 1: Direction 1\nPin 2: Direction 2\nPin 3: Enable
Text Notes 6600 2150 0    60   ~ 0
Pin 1: Motor Power\nPin 2: Ground (External Power)
Text Notes 9550 4550 0    60   ~ 0
Pin 1: Serial Clock\nPin 2: Serial Data\nPin 3: Temp. Sensor Interrupt line\nPin 4: Encoder Data Output 1\nPin 5: Encoder Data Output 2
Text Notes 9700 5800 0    60   ~ 0
Pin 1: Current Sensor Voltage
Text Notes 1550 5050 0    60   ~ 0
Pin 1: Motor Power Input 1\nPin 2: Motor Power Input 2\nPin 3: Encoder Ground\nPin 4: Encoder Logic Power\nPin 5: Encoder Data Input 1\nPin 6: Encoder Data Input 2\n
$Comp
L PWR_FLAG #FLG013
U 1 1 56A1665B
P 8900 3100
F 0 "#FLG013" H 8900 3195 50  0001 C CNN
F 1 "PWR_FLAG" H 8900 3280 50  0000 C CNN
F 2 "" H 8900 3100 50  0000 C CNN
F 3 "" H 8900 3100 50  0000 C CNN
	1    8900 3100
	1    0    0    -1  
$EndComp
Text Label 3850 4400 0    60   ~ 0
+6V
Text Notes 550  4150 0    60   ~ 0
R1 (PF1262): A high power (20W) resistor. The value would be set at 0.5 Ohms. This is\nbecause the max current that should be going through it is 5A (2.5A repetetive  [80% \non per 10ms] through each channel). The ideal value would be 0.72 Ohms. Since that is\nnot a standard value, 0.5 Ohms is probably best since it captures the all current values.\n\nOn the current sensor output, may need a large resistor to limit current into the STMF0\nbut the STMF0 has high impedance input, so it may be fine as is.\n\nChanged the diodes from being in series with the L298 to being in parallel (and with a \n10k resistor to limit current). These are used to indicate the state of the motor inputs.\n\nFixed the flyback diodes (and changed their direction to try and make it more \nreadable).\n\nOn the capacitors, they are at 200nF. This is because the L298 has a suggested \ncapacitor value of 100nF. With the LEDs and other components, I upped it up to 220nF.\n\nSeparated the analog and digital pin headers. Might be able to connect the +6V/GND \nheader to the current voltage sensor since they're all analog, but I'm not sure on that, \nso I'm keeping them separate, just in case.\n\nOn the temperature sensor, the inputs/outputs should be tied to the pullup resistor \ncorrectly. Using the O.S. (interrupt pin) so I can use it if I can/want to for when the\ncurrent sensor resistor or L298 get too hot.\n\nOriginally had a BJT and op amp for a current limiter. Remembered about TVS \ndiodes today and realized that it would probably work better. It would also be \neasier to test and take out of the final circuit since it's just in parallel with the \nsense resistor. Would need to find a TVS with a break down voltage of about \n2.5 or 3 volts and simulate it in a program such as PSpice. There is also a picture \nof the voltage and current waveform from testing a TVS is just found really \nquick. The red signal is the input to the circuit, blue is the voltage across the TVS, \nand green is the current through the TVS. This solution would allow current to \ncontinue to flow once the cut off is reached, as well.\n(TVS = transient voltage suppressor)\n\nI also cleared up some of the power wires to try and make the schematic more \nreadable. The connections are now made by labelling the nodes and putting\nthe power symbol. (Not sure which would be better, or if they're the same in \nKiCad [not sure if node labelling works the same as in other simulation \nprograms. Would assume that it does], so I put both methods, just in case).
$Comp
L CP C1
U 1 1 56A19184
P 7300 3550
F 0 "C1" H 7325 3650 50  0000 L CNN
F 1 "0.22uF" H 7325 3450 50  0000 L CNN
F 2 "Capacitors_SMD:c_elec_4x4.5" H 7338 3400 50  0001 C CNN
F 3 "" H 7300 3550 50  0000 C CNN
	1    7300 3550
	1    0    0    -1  
$EndComp
$Comp
L CP C2
U 1 1 56A19243
P 7650 3550
F 0 "C2" H 7675 3650 50  0000 L CNN
F 1 "0.22uF" H 7675 3450 50  0000 L CNN
F 2 "Capacitors_SMD:c_elec_4x4.5" H 7688 3400 50  0001 C CNN
F 3 "" H 7650 3550 50  0000 C CNN
	1    7650 3550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR014
U 1 1 56A19739
P 7300 3850
F 0 "#PWR014" H 7300 3600 50  0001 C CNN
F 1 "GND" H 7300 3700 50  0000 C CNN
F 2 "" H 7300 3850 50  0000 C CNN
F 3 "" H 7300 3850 50  0000 C CNN
	1    7300 3850
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR015
U 1 1 56A1977F
P 7650 3850
F 0 "#PWR015" H 7650 3600 50  0001 C CNN
F 1 "GND" H 7650 3700 50  0000 C CNN
F 2 "" H 7650 3850 50  0000 C CNN
F 3 "" H 7650 3850 50  0000 C CNN
	1    7650 3850
	1    0    0    -1  
$EndComp
$Comp
L TVS D8
U 1 1 56A1B668
P 6350 6000
F 0 "D8" H 6350 6150 50  0000 C CNN
F 1 "TVS" H 6350 5850 50  0000 C CNN
F 2 "Diodes_ThroughHole:Diode_P600_Horizontal" H 6350 6000 50  0001 C CNN
F 3 "" H 6350 6000 50  0000 C CNN
	1    6350 6000
	0    1    1    0   
$EndComp
$Comp
L GND #PWR016
U 1 1 56A1B9EC
P 6350 6500
F 0 "#PWR016" H 6350 6250 50  0001 C CNN
F 1 "GND" H 6350 6350 50  0000 C CNN
F 2 "" H 6350 6500 50  0000 C CNN
F 3 "" H 6350 6500 50  0000 C CNN
	1    6350 6500
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR017
U 1 1 56A1DC05
P 7650 3000
F 0 "#PWR017" H 7650 2850 50  0001 C CNN
F 1 "+3.3V" H 7650 3140 50  0000 C CNN
F 2 "" H 7650 3000 50  0000 C CNN
F 3 "" H 7650 3000 50  0000 C CNN
	1    7650 3000
	1    0    0    -1  
$EndComp
$Comp
L +6V #PWR018
U 1 1 56A1DC4D
P 6250 3100
F 0 "#PWR018" H 6250 2950 50  0001 C CNN
F 1 "+6V" H 6250 3240 50  0000 C CNN
F 2 "" H 6250 3100 50  0000 C CNN
F 3 "" H 6250 3100 50  0000 C CNN
	1    6250 3100
	1    0    0    -1  
$EndComp
$Comp
L +6V #PWR019
U 1 1 56A1DDD4
P 3850 4350
F 0 "#PWR019" H 3850 4200 50  0001 C CNN
F 1 "+6V" H 3850 4490 50  0000 C CNN
F 2 "" H 3850 4350 50  0000 C CNN
F 3 "" H 3850 4350 50  0000 C CNN
	1    3850 4350
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR020
U 1 1 56A1E1EC
P 7650 4700
F 0 "#PWR020" H 7650 4550 50  0001 C CNN
F 1 "+3.3V" H 7650 4840 50  0000 C CNN
F 2 "" H 7650 4700 50  0000 C CNN
F 3 "" H 7650 4700 50  0000 C CNN
	1    7650 4700
	1    0    0    -1  
$EndComp
$Comp
L +6V #PWR021
U 1 1 56A1E36A
P 6200 5350
F 0 "#PWR021" H 6200 5200 50  0001 C CNN
F 1 "+6V" H 6200 5490 50  0000 C CNN
F 2 "" H 6200 5350 50  0000 C CNN
F 3 "" H 6200 5350 50  0000 C CNN
	1    6200 5350
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR022
U 1 1 56A1E44F
P 7300 3000
F 0 "#PWR022" H 7300 2850 50  0001 C CNN
F 1 "+5V" H 7300 3140 50  0000 C CNN
F 2 "" H 7300 3000 50  0000 C CNN
F 3 "" H 7300 3000 50  0000 C CNN
	1    7300 3000
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR023
U 1 1 56A1E497
P 6250 5000
F 0 "#PWR023" H 6250 4850 50  0001 C CNN
F 1 "+5V" H 6250 5140 50  0000 C CNN
F 2 "" H 6250 5000 50  0000 C CNN
F 3 "" H 6250 5000 50  0000 C CNN
	1    6250 5000
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR024
U 1 1 56A1E4DF
P 2750 5500
F 0 "#PWR024" H 2750 5350 50  0001 C CNN
F 1 "+5V" H 2750 5640 50  0000 C CNN
F 2 "" H 2750 5500 50  0000 C CNN
F 3 "" H 2750 5500 50  0000 C CNN
	1    2750 5500
	1    0    0    -1  
$EndComp
Wire Wire Line
	3250 4850 3250 5550
Wire Wire Line
	3550 4850 3550 5550
Wire Wire Line
	5600 4000 5600 3400
Wire Wire Line
	5600 3400 5750 3400
Wire Wire Line
	5750 3400 5750 3550
Wire Wire Line
	4800 5000 4800 5600
Wire Wire Line
	6400 5600 6400 5000
Wire Wire Line
	4050 5950 4050 6100
Wire Wire Line
	8000 4650 8100 4650
Wire Wire Line
	8000 3950 8000 4950
Wire Wire Line
	8000 3950 9050 3950
Wire Wire Line
	8100 4500 8000 4500
Connection ~ 8000 4500
Wire Wire Line
	8950 4500 9050 4500
Wire Wire Line
	9050 4500 9050 3950
Connection ~ 8500 3950
Wire Wire Line
	8500 3400 8500 4100
Wire Wire Line
	8000 4950 8100 4950
Connection ~ 8000 4650
Wire Wire Line
	8500 3400 9100 3400
Wire Wire Line
	5600 5100 5600 5000
Wire Wire Line
	7900 5350 8350 5350
Wire Wire Line
	7900 5600 8350 5600
Connection ~ 7900 5350
Wire Wire Line
	7900 5900 8350 5900
Connection ~ 7900 5600
Wire Wire Line
	8950 4950 9350 4950
Wire Wire Line
	9050 4950 9050 5350
Wire Wire Line
	9050 5350 8650 5350
Wire Wire Line
	8650 5600 9150 5600
Wire Wire Line
	9150 5600 9150 4800
Wire Wire Line
	8950 4800 9600 4800
Wire Wire Line
	9250 4650 8950 4650
Connection ~ 9150 4800
Connection ~ 9050 4950
Wire Wire Line
	9250 5900 8650 5900
Wire Wire Line
	9250 4650 9250 5900
Wire Wire Line
	9600 4700 9250 4700
Connection ~ 9250 4700
Connection ~ 6050 5600
Wire Wire Line
	8150 3400 8150 3300
Wire Wire Line
	8750 3500 8750 3400
Connection ~ 8750 3400
Wire Wire Line
	5200 2550 5400 2550
Wire Wire Line
	4800 2550 4800 3900
Wire Wire Line
	5100 2550 5100 4000
Wire Wire Line
	5400 2550 5400 4000
Wire Wire Line
	6050 5800 6050 5600
Wire Wire Line
	6200 5450 5400 5450
Connection ~ 6400 5600
Wire Wire Line
	4800 5600 7350 5600
Wire Wire Line
	4600 2550 5000 2550
Wire Wire Line
	5250 2550 5250 2650
Connection ~ 5250 2550
Wire Wire Line
	5250 2950 5250 3050
Wire Wire Line
	4950 2550 4950 2650
Connection ~ 4950 2550
Wire Wire Line
	4950 2950 4950 3050
Wire Wire Line
	4600 2550 4600 2650
Connection ~ 4800 2550
Wire Wire Line
	4600 2950 4600 3050
Wire Wire Line
	4600 3450 4600 3550
Wire Wire Line
	4950 3450 4950 3550
Wire Wire Line
	5250 3450 5250 3550
Wire Wire Line
	6050 6050 6050 6100
Wire Wire Line
	1850 5400 2450 5400
Wire Wire Line
	2450 5400 2450 5450
Wire Wire Line
	2250 6300 9350 6300
Wire Wire Line
	7350 5600 7350 6050
Wire Wire Line
	7350 6050 9600 6050
Wire Wire Line
	6900 2500 6900 2650
Wire Wire Line
	6900 2650 7050 2650
Wire Wire Line
	7050 2650 7050 2850
Wire Wire Line
	6700 2750 6800 2750
Connection ~ 6800 2750
Wire Wire Line
	7300 3200 9100 3200
Wire Wire Line
	8900 3100 8900 3200
Connection ~ 8900 3200
Wire Wire Line
	7650 3300 9100 3300
Wire Wire Line
	7650 4800 8100 4800
Wire Wire Line
	2350 5650 2750 5650
Wire Wire Line
	6250 5100 5600 5100
Wire Wire Line
	6800 2500 6800 3200
Wire Wire Line
	5400 5450 5400 5000
Wire Wire Line
	7900 4800 7900 5900
Connection ~ 7900 4800
Wire Wire Line
	3250 4550 3250 4400
Wire Wire Line
	3250 4400 3850 4400
Wire Wire Line
	3550 4550 3550 4400
Connection ~ 3550 4400
Wire Wire Line
	3250 5850 3250 5950
Wire Wire Line
	3250 5950 4050 5950
Wire Wire Line
	3550 5850 3550 5950
Connection ~ 3550 5950
Wire Wire Line
	7650 3000 7650 3400
Connection ~ 8150 3300
Wire Wire Line
	7650 3700 7650 3850
Wire Wire Line
	7300 3700 7300 3850
Wire Wire Line
	6350 5700 6350 5600
Connection ~ 6350 5600
Wire Wire Line
	6350 6300 6350 6500
Connection ~ 7650 3300
Connection ~ 7300 3200
Wire Wire Line
	6800 3200 6250 3200
Wire Wire Line
	6250 3200 6250 3100
Wire Wire Line
	7300 3000 7300 3400
Wire Wire Line
	7650 4700 7650 4800
Wire Wire Line
	6200 5350 6200 5450
Wire Wire Line
	2750 5650 2750 5500
Wire Wire Line
	6250 5000 6250 5100
Wire Wire Line
	3850 4400 3850 4350
Wire Wire Line
	6000 4000 6000 3800
Wire Wire Line
	6000 3800 5400 3800
Connection ~ 5400 3800
Wire Wire Line
	5000 4000 5000 3900
Wire Wire Line
	4800 3900 6200 3900
Wire Wire Line
	6200 3900 6200 4000
Wire Wire Line
	5100 4000 5800 4000
Connection ~ 5000 3900
Connection ~ 5200 4000
Wire Wire Line
	5000 5300 5000 5000
Wire Wire Line
	1850 5300 6000 5300
Wire Wire Line
	6000 5300 6000 5000
Wire Wire Line
	5200 5200 5200 5000
Wire Wire Line
	1850 5200 5800 5200
Wire Wire Line
	5800 5200 5800 5000
Connection ~ 3550 5200
Connection ~ 5200 5200
Connection ~ 3250 5300
Connection ~ 5000 5300
$Comp
L CONN_01X06 P4
U 1 1 56A228EB
P 1650 5450
F 0 "P4" H 1650 5800 50  0000 C CNN
F 1 "CONN_01X06" V 1750 5450 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Angled_1x06" H 1650 5450 50  0001 C CNN
F 3 "" H 1650 5450 50  0000 C CNN
	1    1650 5450
	-1   0    0    -1  
$EndComp
Wire Wire Line
	1850 5500 2350 5500
Wire Wire Line
	2350 5500 2350 5650
Wire Wire Line
	1850 5600 2250 5600
Wire Wire Line
	2250 5600 2250 6300
$Comp
L CONN_01X05 P6
U 1 1 56A23FDF
P 9800 4900
F 0 "P6" H 9800 5200 50  0000 C CNN
F 1 "CONN_01X05" V 9900 4900 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Angled_1x05" H 9800 4900 50  0001 C CNN
F 3 "" H 9800 4900 50  0000 C CNN
	1    9800 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	9350 4950 9350 4900
Wire Wire Line
	9350 4900 9600 4900
Wire Wire Line
	9350 6300 9350 5000
Wire Wire Line
	9350 5000 9600 5000
Wire Wire Line
	1850 5700 2150 5700
Wire Wire Line
	2150 5700 2150 6400
Wire Wire Line
	2150 6400 9450 6400
Wire Wire Line
	9450 6400 9450 5100
Wire Wire Line
	9450 5100 9600 5100
Text Notes 550  7650 0    60   ~ 0
PIN HEADERS:\nP1: Digital control for motor.\nP2: Motor power input.\nP3: Power from STMF0 (5V, 3.3V, and ground)\nP4: Connections from the motor.\nP5: Current sensor value.\nP6: Digital sensor data inputs/outputs.
Entry Bus Bus
	3050 5050 3150 5150
$Comp
L CONN_01X03 P5
U 1 1 56A19109
P 9800 6050
F 0 "P5" H 9800 6250 50  0000 C CNN
F 1 "CONN_01X03" V 9900 6050 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Angled_1x03" H 9800 6050 50  0001 C CNN
F 3 "" H 9800 6050 50  0000 C CNN
	1    9800 6050
	1    0    0    -1  
$EndComp
Wire Wire Line
	9600 5950 9550 5950
Wire Wire Line
	9550 5950 9550 6300
Wire Wire Line
	9550 6150 9600 6150
$Comp
L GND #PWR025
U 1 1 56A19252
P 9550 6300
F 0 "#PWR025" H 9550 6050 50  0001 C CNN
F 1 "GND" H 9550 6150 50  0000 C CNN
F 2 "" H 9550 6300 50  0000 C CNN
F 3 "" H 9550 6300 50  0000 C CNN
	1    9550 6300
	1    0    0    -1  
$EndComp
Connection ~ 9550 6150
$EndSCHEMATC
