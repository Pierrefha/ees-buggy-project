# EES-Buggy-Project

## Build Command
in the directory with the CMakeLists.txt
cmake . && make && ./ees_buggy

## Available Commandline Arguments
GeneralTest<br>
Rotation<br>
Drehungen<br>
Rechteck<br>
Punktbewegung<br>
Punktbewegung_Rechteck<br>
UnterstuetzteBewegung<br>
Drehen

## Pinconfig
Brackets () show position on the Breadboard<br>
Motorhat/PI			Magnetic-sensor<br>
VCC 3V (C30)	->	VCC (B 30)<br>
GND	(C29)		->	GND (B 29)<br>
SCL	(C28)		->	SCL (B 28)<br>
SDA	(C27)		->	SDA (B 27)

curved brackets {} shows pin number for wiringPI<br>
Motorhat/PI			Ultrasonic-sensor<br>
GND (G18)		->	GND (F18)<br>
Pin 18 (I22) {1}->	ECHO (F17)	::Pin 18 at (I22) connected to GND via a 470 Ohm Resistance (J22 to GND)<br>
:: and connected to (G17) via a 330 Ohm Resistance (H22 to G17)<br>								
Pin 27 (I16) {2}->	Trigger (F16)<br>
VCC 5V (J15)	->	VCC (F15)

Motorhat/PI			LED<br>
Pin 17 {0}