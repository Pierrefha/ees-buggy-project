#include <magnetic_sensor/magnetic_sensor.h>
#include <iostream>
#include <wiringPiI2C.h>

magnetic_sensor::magnetic_sensor(){
	//Setup für den Filehandle für die Funktionen von wiringPiI2C
	this->fd = wiringPiI2CSetup(sensor_address);
	
	//Werte erstmals auf Null setzten
	this->x = 0;
	this->y = 0;
	this->z = 0;
	
	if(this->fd == -1){
		std::cout << "I2CSetup for Magnetic Sensor didn't work" << std::endl;
	}
	else {
		//Testweise einstellen des Sensors für erste Werte
		//Empfolende Werte für das SET/RESET Register
		wiringPiI2CWriteReg8(this->fd, 0x0b, 0x01);
		//Hier erstmal ein Beispiel Setup aus dem Datenblatt
		wiringPiI2CWriteReg8(this->fd, 0x09, 0x1d);
	}
}

int magnetic_sensor::check(){
	int tmp = wiringPiI2CReadReg8(this->fd, 0x06);
	// erstes Bit des Statusregisters zeigt an ob neue Daten zur Verfügung stehen
	//Modulo hier also um einfach nachzuverfolgen ob dieses Bit gesetzt ist.
	if((tmp % 1) != 0){
		//TO-DO Werte einlesen, für die 3 Achsen kombinieren und dann in die Klasse-attribute übertragen
		//gerade nur simples auslesen der Daten zum testen
		std::cout << "Data LBS X" << wiringPiI2CReadReg8(this->fd, X_LSB) << std::endl;
		std::cout << "Data MBS X" << wiringPiI2CReadReg8(this->fd, X_MSB) << std::endl;
		std::cout << "Data LBS Y" << wiringPiI2CReadReg8(this->fd, Y_LSB) << std::endl;
		std::cout << "Data MBS Y" << wiringPiI2CReadReg8(this->fd, Y_MSB) << std::endl;
		std::cout << "Data LBS Z" << wiringPiI2CReadReg8(this->fd, Z_LSB) << std::endl;
		std::cout << "Data MBS Z" << wiringPiI2CReadReg8(this->fd, Z_MSB) << std::endl;
	}

	return tmp;
}