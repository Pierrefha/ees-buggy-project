# EES-Buggy-Project

## Prerequisites
ncurses:  sudo apt-get install libncurses5-dev libncursesw5-dev
wiringPi: sudo apt-get install wiringpi

## Build Command
in the directory with the CMakeLists.txt<br>
```shell
cmake . && make
```
It will build a binary named: "ees_buggy". To start it type:
```shell script
./ees_buggy <argument>
```



## Available Commandline Arguments
GeneralTest - Runs distance measurements after a few seconds. Then drives forwards for 1 second and backwards for 1 second<br>
Rotation - Turns in place<br>
Drehungen - Turns in place by 45, -45, -180 and -90 degrees<br>
Rechteck - Moves rectangle shape like<br>
Punktbewegung - Tries to move to a user specified point<br>
Punktbewegung_Rechteck - Moves rectangle shape like based on vector arithmetik<br>
UnterstuetzteBewegung - Moves forward holding direction<br>
Drehen - Drives curves

When the test finishes or no commandline argument is supplied the wasd-control starts.

## Pinconfig
Brackets () show position on the Breadboard<br>
##### Motorhat/PI			Magnetic-sensor<br>
VCC 3V (C30)	->	VCC (B 30)<br>
GND	(C29)		->	GND (B 29)<br>
SCL	(C28)		->	SCL (B 28)<br>
SDA	(C27)		->	SDA (B 27)

curved brackets {} shows pin number for wiringPI<br>
##### Motorhat/PI			Ultrasonic-sensor<br>
GND (G18)		->	GND (F18)<br>
Pin 18 (I22) {1}->	ECHO (F17)	::Pin 18 at (I22) connected to GND via a 470 Ohm Resistance (J22 to GND)<br>
:: and connected to (G17) via a 330 Ohm Resistance (H22 to G17)<br>								
Pin 27 (I16) {2}->	Trigger (F16)<br>
VCC 5V (J15)	->	VCC (F15)

##### Motorhat/PI			LED<br>
Pin 17 {0}