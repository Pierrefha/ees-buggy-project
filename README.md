# EES-Buggy-Project

## Build Command
in the directory with the CMakeLists.txt
cmake . && make && ./ees_buggy

## Pinconfig
Brackets () show position on the Breadboard
Motorhat/PI			Magnetic-sensor
VCC 3V (C30)	->	VCC (B 30)
GND	(C29)		->	GND (B 29)
SCL	(C28)		->	SCL (B 28)
SDA	(C27)		->	SDA (B 27)

Curved brackets {} shows pin number for wiringPI
Motorhat/PI			Ultrasonic-sensor
GND (G18)		->	GND (F18)
Pin 18 (I22) {1}->	ECHO (F17)	::Pin 18 at (I22) connected to GND via a 470 Ohm Resistance (J22 to GND)
								:: and connected to (G17) via a 330 Ohm Resistance (H22 to G17)
Pin 27 (I16) {2}->	Trigger (F16)
VCC 5V (J15)	->	VCC (F15)

Motorhat/PI			LED
Pin 17 {0}