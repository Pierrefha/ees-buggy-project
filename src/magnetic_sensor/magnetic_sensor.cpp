#include <magnetic_sensor/magnetic_sensor.h>
#include <iostream>
#include <wiringPiI2C.h>
#include <cmath>

#define PI		3.1415926535

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
		//Empfolende Werte für das SET/RESET Register
		wiringPiI2CWriteReg8(this->fd, SET_RESET_REG, 0x01);
		wiringPiI2CWriteReg8(this->fd, SETUP_REG, 0x1d);
	}
}

/**
 * @return gibt bei neuen Daten 1 zurück ansonsten 0
 */
int magnetic_sensor::check(){
	int tmp = wiringPiI2CReadReg8(this->fd, 0x06);
	// erstes Bit des Statusregisters zeigt an ob neue Daten zur Verfügung stehen
	//Modulo hier also um einfach nachzuverfolgen ob dieses Bit gesetzt ist.
	if((tmp % 2) != 0){
		//Einlese der Register
		int data[6];
		data[0] = wiringPiI2CReadReg8(this->fd, X_LSB);
		data[1] = wiringPiI2CReadReg8(this->fd, X_MSB);
		data[2] = wiringPiI2CReadReg8(this->fd, Y_LSB);
		data[3] = wiringPiI2CReadReg8(this->fd, Y_MSB);
		data[4] = wiringPiI2CReadReg8(this->fd, Z_LSB);
		data[5] = wiringPiI2CReadReg8(this->fd, Z_MSB);
		
		//Kombinieren der Daten (x, y und z sind 16 Bit signed Integers, dadurch wird das zweier Komplement
		//ohne größere Umstände angewandt
		this->x = data[0] | (data[1] << 8);
		this->y = data[2] | (data[3] << 8);
		this->z = data[4] | (data[5] << 8);
		
		return 1;
	}

	return 0;
}

int magnetic_sensor::getX(){
	return this->x;
}
int magnetic_sensor::getY(){
	return this->y;
}
int magnetic_sensor::getZ(){
	return this->z;
}


/**
 * @return The rotation of the buggy relative to north
 */
degree<float> magnetic_sensor::get_rotation() {
    return vertex2D<float>{-1060., -245.}.angle_to(get_direction());
}

vertex2D<float> magnetic_sensor::get_direction() {
//The given data is a ellipse
//To transform it the following parameters are needed
//They are calculated in magnetic_sensor_analysis.py
    const vertex2D<float> ellipse_center{-576.9119840046621, -738.0477416737527};
    const float width = 804.4517031652929;
    const float height = 549.1679710793408;
    const float phi = -0.721345733186768;
    const float c = std::cos(phi);
    const float s = std::sin(phi);

    //TODO this could be one matrix * vector operation
    vertex2D<float> measurement{static_cast<float>(x), static_cast<float>(y)};
    measurement = measurement - ellipse_center;
    measurement.x = measurement.x * c - measurement.y * s;
    measurement.y = measurement.y * c + measurement.x * s;
    measurement.x = measurement.x * width / height;

    return measurement;
}
