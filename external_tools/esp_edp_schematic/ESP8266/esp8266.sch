EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:switches
LIBS:relays
LIBS:motors
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
LIBS:ESP8266
LIBS:esp8266-cache
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
L ESP-12E U1
U 1 1 59FAD28F
P 3050 2200
F 0 "U1" H 3050 2100 50  0000 C CNN
F 1 "ESP-12E" H 3050 2300 50  0000 C CNN
F 2 "ESP8266:ESP-12E" H 3050 2200 50  0001 C CNN
F 3 "" H 3050 2200 50  0001 C CNN
	1    3050 2200
	1    0    0    -1  
$EndComp
$Comp
L Conn_01x24_Female J1
U 1 1 59FAD333
P 7950 4550
F 0 "J1" H 7950 5750 50  0000 C CNN
F 1 "EDP_CON" H 7950 3250 50  0000 C CNN
F 2 "ESP8266_proj:FPC-24pin-0.5mm" H 7950 4550 50  0001 C CNN
F 3 "" H 7950 4550 50  0001 C CNN
	1    7950 4550
	1    0    0    1   
$EndComp
$Comp
L C C5
U 1 1 59FAEACC
P 5750 5850
F 0 "C5" H 5775 5950 50  0000 L CNN
F 1 "105" H 5775 5750 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 5788 5700 50  0001 C CNN
F 3 "" H 5750 5850 50  0001 C CNN
	1    5750 5850
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR01
U 1 1 59FAEC34
P 5750 6150
F 0 "#PWR01" H 5750 5900 50  0001 C CNN
F 1 "GND" H 5750 6000 50  0000 C CNN
F 2 "" H 5750 6150 50  0001 C CNN
F 3 "" H 5750 6150 50  0001 C CNN
	1    5750 6150
	1    0    0    -1  
$EndComp
$Comp
L C C6
U 1 1 59FAED01
P 6000 5850
F 0 "C6" H 6025 5950 50  0000 L CNN
F 1 "105" H 6025 5750 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 6038 5700 50  0001 C CNN
F 3 "" H 6000 5850 50  0001 C CNN
	1    6000 5850
	1    0    0    -1  
$EndComp
$Comp
L C C7
U 1 1 59FAED33
P 6250 5850
F 0 "C7" H 6275 5950 50  0000 L CNN
F 1 "105" H 6275 5750 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 6288 5700 50  0001 C CNN
F 3 "" H 6250 5850 50  0001 C CNN
	1    6250 5850
	1    0    0    -1  
$EndComp
$Comp
L C C8
U 1 1 59FAED39
P 6500 5850
F 0 "C8" H 6525 5950 50  0000 L CNN
F 1 "105" H 6525 5750 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 6538 5700 50  0001 C CNN
F 3 "" H 6500 5850 50  0001 C CNN
	1    6500 5850
	1    0    0    -1  
$EndComp
$Comp
L C C9
U 1 1 59FAEDA1
P 6750 5850
F 0 "C9" H 6775 5950 50  0000 L CNN
F 1 "105" H 6775 5750 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 6788 5700 50  0001 C CNN
F 3 "" H 6750 5850 50  0001 C CNN
	1    6750 5850
	1    0    0    -1  
$EndComp
$Comp
L C C10
U 1 1 59FAEDA7
P 7000 5850
F 0 "C10" H 7025 5950 50  0000 L CNN
F 1 "105" H 7025 5750 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 7038 5700 50  0001 C CNN
F 3 "" H 7000 5850 50  0001 C CNN
	1    7000 5850
	1    0    0    -1  
$EndComp
$Comp
L C C11
U 1 1 59FAEDAD
P 7250 5850
F 0 "C11" H 7275 5950 50  0000 L CNN
F 1 "105" H 7275 5750 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 7288 5700 50  0001 C CNN
F 3 "" H 7250 5850 50  0001 C CNN
	1    7250 5850
	1    0    0    -1  
$EndComp
$Comp
L C C12
U 1 1 59FAEDB3
P 7500 5850
F 0 "C12" H 7525 5950 50  0000 L CNN
F 1 "105" H 7525 5750 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 7538 5700 50  0001 C CNN
F 3 "" H 7500 5850 50  0001 C CNN
	1    7500 5850
	1    0    0    -1  
$EndComp
Wire Wire Line
	7750 5650 7500 5650
Wire Wire Line
	7500 5650 7500 5700
Wire Wire Line
	7200 5550 7750 5550
Wire Wire Line
	7250 5550 7250 5700
Wire Wire Line
	7750 5450 7000 5450
Wire Wire Line
	7000 5450 7000 5700
Wire Wire Line
	6700 5350 7750 5350
Wire Wire Line
	6750 5350 6750 5700
Wire Wire Line
	7750 5250 6500 5250
Wire Wire Line
	6500 5250 6500 5700
Wire Wire Line
	7750 5150 6250 5150
Wire Wire Line
	6250 5150 6250 5700
Wire Wire Line
	7750 5050 6000 5050
Wire Wire Line
	6000 5050 6000 5700
Wire Wire Line
	7750 4950 7300 4950
Wire Wire Line
	7750 4850 7300 4850
Wire Wire Line
	7750 4750 5750 4750
Wire Wire Line
	5750 4750 5750 5700
Wire Wire Line
	5750 6000 5750 6150
Wire Wire Line
	6000 6000 6000 6050
Wire Wire Line
	5750 6050 7500 6050
Connection ~ 5750 6050
Wire Wire Line
	6250 6050 6250 6000
Connection ~ 6000 6050
Wire Wire Line
	6500 6050 6500 6000
Connection ~ 6250 6050
Wire Wire Line
	6750 6050 6750 6000
Connection ~ 6500 6050
Wire Wire Line
	7000 6050 7000 6000
Connection ~ 6750 6050
Wire Wire Line
	7250 6050 7250 6000
Connection ~ 7000 6050
Wire Wire Line
	7500 6050 7500 6000
Connection ~ 7250 6050
Text Label 7300 4850 0    60   ~ 0
3v3
Text Label 7300 4950 0    60   ~ 0
GND
Text Label 7300 4750 0    60   ~ 0
3v3
Wire Wire Line
	7750 4650 7300 4650
Wire Wire Line
	7750 4550 7300 4550
Text Label 7300 4550 0    60   ~ 0
SPI_SCK
Text Label 7300 4650 0    60   ~ 0
SPI_MOSI
Wire Wire Line
	7750 4450 7300 4450
Wire Wire Line
	7750 4350 7300 4350
Text Label 7300 4350 0    60   ~ 0
D/C
Text Label 7300 4450 0    60   ~ 0
SPI_CE0
Wire Wire Line
	7750 4250 7300 4250
Wire Wire Line
	7750 4150 7300 4150
Text Label 7300 4150 0    60   ~ 0
BUSY
Text Label 7300 4250 0    60   ~ 0
RST
Wire Wire Line
	7750 4050 7300 4050
Wire Wire Line
	7750 3950 7300 3950
Text Label 7300 4050 0    60   ~ 0
BS
NoConn ~ 7300 3950
Wire Wire Line
	7750 3850 7300 3850
Wire Wire Line
	6800 3750 7750 3750
Wire Wire Line
	6800 3650 7750 3650
Wire Wire Line
	7750 3550 7300 3550
Text Label 7300 3550 0    60   ~ 0
RESEN
Wire Wire Line
	7750 3450 7300 3450
Wire Wire Line
	7750 3350 7300 3350
NoConn ~ 7300 3350
NoConn ~ 7300 3850
Text Label 7300 3450 0    60   ~ 0
GDR
$Comp
L C C2
U 1 1 59FB184F
P 6350 3800
F 0 "C2" H 6375 3900 50  0000 L CNN
F 1 "105" H 6375 3700 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 6388 3650 50  0001 C CNN
F 3 "" H 6350 3800 50  0001 C CNN
	1    6350 3800
	0    1    1    0   
$EndComp
$Comp
L C C1
U 1 1 59FB18B3
P 6350 3550
F 0 "C1" H 6375 3650 50  0000 L CNN
F 1 "105" H 6375 3450 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 6388 3400 50  0001 C CNN
F 3 "" H 6350 3550 50  0001 C CNN
	1    6350 3550
	0    1    1    0   
$EndComp
Wire Wire Line
	6800 3650 6800 3550
Wire Wire Line
	6800 3550 6500 3550
Wire Wire Line
	6800 3750 6800 3800
Wire Wire Line
	6800 3800 6500 3800
Wire Wire Line
	6200 3550 5950 3550
Wire Wire Line
	5950 3550 5950 3850
Wire Wire Line
	6200 3800 5950 3800
Connection ~ 5950 3800
$Comp
L GND #PWR02
U 1 1 59FB1CA0
P 5950 3850
F 0 "#PWR02" H 5950 3600 50  0001 C CNN
F 1 "GND" H 5950 3700 50  0000 C CNN
F 2 "" H 5950 3850 50  0001 C CNN
F 3 "" H 5950 3850 50  0001 C CNN
	1    5950 3850
	1    0    0    -1  
$EndComp
Connection ~ 7250 5550
Text Label 7200 5550 0    60   ~ 0
PREVGL
Connection ~ 6750 5350
Text Label 6700 5350 0    60   ~ 0
PREVGH
$Comp
L C C4
U 1 1 59FB41CF
P 1950 4800
F 0 "C4" H 1975 4900 50  0000 L CNN
F 1 "475" H 1975 4700 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 1988 4650 50  0001 C CNN
F 3 "" H 1950 4800 50  0001 C CNN
	1    1950 4800
	1    0    0    -1  
$EndComp
$Comp
L L L1
U 1 1 59FB4226
P 2500 4550
F 0 "L1" V 2450 4550 50  0000 C CNN
F 1 "68uH" V 2575 4550 50  0000 C CNN
F 2 "Inductors_SMD:L_1206_HandSoldering" H 2500 4550 50  0001 C CNN
F 3 "" H 2500 4550 50  0001 C CNN
	1    2500 4550
	0    -1   -1   0   
$EndComp
$Comp
L C C3
U 1 1 59FB44D7
P 3950 5100
F 0 "C3" H 3975 5200 50  0000 L CNN
F 1 "104" H 3975 5000 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 3988 4950 50  0001 C CNN
F 3 "" H 3950 5100 50  0001 C CNN
	1    3950 5100
	1    0    0    -1  
$EndComp
$Comp
L D D3
U 1 1 59FB4524
P 3250 4550
F 0 "D3" H 3250 4650 50  0000 C CNN
F 1 "MBR0530" H 3250 4450 50  0000 C CNN
F 2 "Diodes_SMD:D_SOD-123" H 3250 4550 50  0001 C CNN
F 3 "" H 3250 4550 50  0001 C CNN
	1    3250 4550
	-1   0    0    1   
$EndComp
$Comp
L D D2
U 1 1 59FB459C
P 4250 5350
F 0 "D2" H 4250 5450 50  0000 C CNN
F 1 "MBR0530" H 4250 5250 50  0000 C CNN
F 2 "Diodes_SMD:D_SOD-123" H 4250 5350 50  0001 C CNN
F 3 "" H 4250 5350 50  0001 C CNN
	1    4250 5350
	1    0    0    -1  
$EndComp
$Comp
L D D1
U 1 1 59FB45F7
P 3950 5600
F 0 "D1" H 3950 5700 50  0000 C CNN
F 1 "MBR0530" H 3950 5500 50  0000 C CNN
F 2 "Diodes_SMD:D_SOD-123" H 3950 5600 50  0001 C CNN
F 3 "" H 3950 5600 50  0001 C CNN
	1    3950 5600
	0    -1   -1   0   
$EndComp
$Comp
L Q_NMOS_DGS Q1
U 1 1 59FB464C
P 2800 5150
F 0 "Q1" H 3000 5200 50  0000 L CNN
F 1 "Si1304BDL" H 3000 5100 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 3000 5250 50  0001 C CNN
F 3 "" H 2800 5150 50  0001 C CNN
	1    2800 5150
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 59FB4876
P 2550 5700
F 0 "R1" V 2630 5700 50  0000 C CNN
F 1 "10K" V 2550 5700 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 2480 5700 50  0001 C CNN
F 3 "" H 2550 5700 50  0001 C CNN
	1    2550 5700
	1    0    0    -1  
$EndComp
$Comp
L R R2
U 1 1 59FB48CF
P 2900 5700
F 0 "R2" V 2980 5700 50  0000 C CNN
F 1 "3R" V 2900 5700 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 2830 5700 50  0001 C CNN
F 3 "" H 2900 5700 50  0001 C CNN
	1    2900 5700
	1    0    0    -1  
$EndComp
Wire Wire Line
	1600 4550 2350 4550
Wire Wire Line
	1950 4650 1950 4550
Connection ~ 1950 4550
Wire Wire Line
	2650 4550 3100 4550
Wire Wire Line
	2900 4950 2900 4550
Connection ~ 2900 4550
Wire Wire Line
	2600 5150 2550 5150
Wire Wire Line
	2550 5150 2550 5550
Wire Wire Line
	2900 5350 2900 5550
Wire Wire Line
	2900 4800 3950 4800
Wire Wire Line
	3950 4800 3950 4950
Connection ~ 2900 4800
Wire Wire Line
	3950 5250 3950 5450
Wire Wire Line
	4100 5350 3950 5350
Connection ~ 3950 5350
Text Label 1600 4550 0    60   ~ 0
3v3
$Comp
L GND #PWR03
U 1 1 59FB7981
P 1950 4950
F 0 "#PWR03" H 1950 4700 50  0001 C CNN
F 1 "GND" H 1950 4800 50  0000 C CNN
F 2 "" H 1950 4950 50  0001 C CNN
F 3 "" H 1950 4950 50  0001 C CNN
	1    1950 4950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR04
U 1 1 59FB7C20
P 2550 5850
F 0 "#PWR04" H 2550 5600 50  0001 C CNN
F 1 "GND" H 2550 5700 50  0000 C CNN
F 2 "" H 2550 5850 50  0001 C CNN
F 3 "" H 2550 5850 50  0001 C CNN
	1    2550 5850
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR05
U 1 1 59FB7C5E
P 2900 5850
F 0 "#PWR05" H 2900 5600 50  0001 C CNN
F 1 "GND" H 2900 5700 50  0000 C CNN
F 2 "" H 2900 5850 50  0001 C CNN
F 3 "" H 2900 5850 50  0001 C CNN
	1    2900 5850
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR06
U 1 1 59FB7E6A
P 3950 5800
F 0 "#PWR06" H 3950 5550 50  0001 C CNN
F 1 "GND" H 3950 5650 50  0000 C CNN
F 2 "" H 3950 5800 50  0001 C CNN
F 3 "" H 3950 5800 50  0001 C CNN
	1    3950 5800
	1    0    0    -1  
$EndComp
Wire Wire Line
	3400 4550 4500 4550
Wire Wire Line
	4400 5350 4500 5350
Text Label 4500 4550 0    60   ~ 0
PREVGH
Text Label 4500 5350 0    60   ~ 0
PREVGL
Wire Wire Line
	3950 5750 3950 5800
Text Label 3300 3400 3    60   ~ 0
SPI_SCK
Text Label 3200 3400 3    60   ~ 0
SPI_MOSI
Text Label 2800 3400 3    60   ~ 0
SPI_CE0
Wire Wire Line
	3300 3100 3300 3400
Wire Wire Line
	3200 3100 3200 3400
Wire Wire Line
	3100 3100 3100 3400
Wire Wire Line
	3000 3100 3000 3400
Wire Wire Line
	2900 3100 2900 3400
Wire Wire Line
	2800 3100 2800 3400
Text Label 2900 3400 3    60   ~ 0
D/C
Text Label 3100 3400 3    60   ~ 0
BUSY
Text Label 3000 3400 3    60   ~ 0
RST
Connection ~ 2900 5400
Connection ~ 2550 5500
Wire Wire Line
	2900 5400 3150 5400
Wire Wire Line
	2550 5500 3150 5500
Text Label 3150 5400 0    60   ~ 0
RESEN
Text Label 3150 5500 0    60   ~ 0
GDR
$EndSCHEMATC
