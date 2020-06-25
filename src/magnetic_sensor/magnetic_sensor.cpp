#include <magnetic_sensor/magnetic_sensor.h>
#include <iostream>
#include <wiringPiI2C.h>
#include <cmath>
#include <unistd.h>
#include <chrono>
#include <util/time_util.h>

#define PI		3.1415926535
#define sensor_address	0x0d
#define STATUS_REG 		0x06
#define SET_RESET_REG	0x0b
#define CONTROL_REG_1	0x09
#define CONTROL_REG_2   0x0a
#define CONTROL_REG_1_MODE_BIT 0
#define CONTROL_REG_1_ODR_BIT 2
#define CONTROL_REG_1_RNG_BIT 4
#define CONTROL_REG_1_OSR_BIT 6
#define X_LSB			0x00
#define X_MSB			0x01
#define Y_LSB			0x02
#define Y_MSB			0x03
#define Z_LSB			0x04
#define Z_MSB			0x05


#define TEMPERATURE_REG_LSB 0x07
#define TEMPERATURE_REG_MSB 0x08

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
        wiringPiI2CWriteReg8(this->fd, CONTROL_REG_2, 0x80);
        busy_wait(std::chrono::milliseconds{1});
		//Testweise einstellen des Sensors für erste Werte
		//Empfolende Werte für das SET/RESET Register
		wiringPiI2CWriteReg8(this->fd, SET_RESET_REG, 0x01);
		//Hier erstmal ein Beispiel Setup aus dem Datenblatt
		wiringPiI2CWriteReg8(this->fd, CONTROL_REG_1, 0x1d);
        busy_wait(std::chrono::milliseconds{1});
	}
}

/**
 * @param mode Modus des Sensors, 1 für Continuos, 0 für Standby
 * @param odr Output Data Rate, 0: 10Hz, 1: 50Hz, 2: 100Hz, 3: 200Hz
 * @param rng Field range, 0: 2G, 1: 8G
 * @param osr Over sample Rate, 0: 512, 1: 256, 2: 128, 3: 64
 * Der Kompass ist auf rng = 1 (8G) und osr = 0 (512) kalibriert, ein Setup mit anderen Werten
 * könnte zu Fehlern in der Gradberechnung führen
 * Bsp. vom Standardkonstruktor (0x1d): mode = 1, odr = 3, rng = 1, osr = 0
 */
magnetic_sensor::magnetic_sensor(magnetic_sensor_defs::MODE mode, magnetic_sensor_defs::ODR odr,
                                 magnetic_sensor_defs::RNG rng, magnetic_sensor_defs::OSR osr) {
	//Setup für den Filehandle für die Funktionen von wiringPiI2C
	this->fd = wiringPiI2CSetup(sensor_address);
	
	//Werte erstmals auf Null setzten
	this->x = 0;
	this->y = 0;
	this->z = 0;
	
	// Settings zusammenfügen
	int settings = static_cast<uint>(mode) |
	        (static_cast<uint>(odr) << 2) |
	        (static_cast<uint>(rng) << 4) |
	        (static_cast<uint>(osr) << 6);
	
	if(this->fd == -1){
		std::cout << "I2CSetup for Magnetic Sensor didn't work" << std::endl;
	}
	else {
        wiringPiI2CWriteReg8(this->fd, CONTROL_REG_2, 0x80);
        busy_wait(std::chrono::milliseconds{1});
		//Empfolende Werte für das SET/RESET Register
		wiringPiI2CWriteReg8(this->fd, SET_RESET_REG, 0x01);
		//Settings in das Control Register schreiben
		wiringPiI2CWriteReg8(this->fd, CONTROL_REG_1, settings);
        busy_wait(std::chrono::milliseconds{1});
	}
}

int magnetic_sensor::check_and_update(){
	int tmp = wiringPiI2CReadReg8(this->fd, 0x06);
	// erstes Bit des Statusregisters zeigt an ob neue Daten zur Verfügung stehen
	//Modulo hier also um einfach nachzuverfolgen ob dieses Bit gesetzt ist.
#ifdef DEBUG_MAGNETIC_SENSOR
	std::cout << "handle: " << fd << std::endl;
	std::cout << "Update: " << (tmp % 1) << std::endl;
#endif
	if((tmp % 2) != 0){
		//TO-DO Werte einlesen, für die 3 Achsen kombinieren und dann in die Klasse-attribute übertragen
		//gerade nur simples auslesen der Daten zum testen
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

//Unused
//double magnetic_sensor::testDirection(){
//	double dX = this->x;
//	double dY = this->y;
//	std::cout << atan((dX/dY)) << std::endl;
//	std::cout << atan(5)*180/PI << std::endl;
//	if(this->y > 0){
//		return 90 - atan(dX/dY)*180/PI;
//	} else if(this->y < 0){
//		return 270 - atan(dX/dY)*180/PI;
//	} else{
//		if(this->x < 0){
//			return 180;
//		} else {
//			return 0;
//		}
//	}
//}

int magnetic_sensor::release_resources() {
    if(fd != -1){
        wiringPiI2CWriteReg8(this->fd, CONTROL_REG_1, 0x00);
        return close(fd);
    }
    return 0;
}

bool magnetic_sensor::has_new_data() {
	const int tmp = wiringPiI2CReadReg8(this->fd, STATUS_REG);
	return bool(tmp & 1);
}

bool magnetic_sensor::data_overflow() {
	const int tmp = wiringPiI2CReadReg8(this->fd, STATUS_REG);
	return bool(tmp & 0b10);
}

bool magnetic_sensor::data_skipped() {
	const int tmp = wiringPiI2CReadReg8(this->fd, STATUS_REG);
	return bool(tmp & 0b100);
}

void magnetic_sensor::set_mode(magnetic_sensor_defs::MODE new_mode) {
	const int current_state = wiringPiI2CReadReg8(this->fd, CONTROL_REG_1);
	wiringPiI2CWriteReg8(this->fd, CONTROL_REG_1, (current_state & 0xFC) | (static_cast<uint>(new_mode) << CONTROL_REG_1_MODE_BIT));
}

void magnetic_sensor::set_odr(magnetic_sensor_defs::ODR new_odr) {
	const int current_state = wiringPiI2CReadReg8(this->fd, CONTROL_REG_1);
	wiringPiI2CWriteReg8(this->fd, CONTROL_REG_1, (current_state & 0xF3) | (static_cast<uint>(new_odr) << CONTROL_REG_1_ODR_BIT));
}

void magnetic_sensor::set_rng(magnetic_sensor_defs::RNG new_rng) {
	const int current_state = wiringPiI2CReadReg8(this->fd, CONTROL_REG_1);
	wiringPiI2CWriteReg8(this->fd, CONTROL_REG_1, (current_state & 0xCF) | (static_cast<uint>(new_rng) << CONTROL_REG_1_RNG_BIT));
}

void magnetic_sensor::set_osr(magnetic_sensor_defs::OSR new_osr) {
	const int current_state = wiringPiI2CReadReg8(this->fd, CONTROL_REG_1);
	wiringPiI2CWriteReg8(this->fd, CONTROL_REG_1, (current_state & 0x3F) | (static_cast<uint>(new_osr) << CONTROL_REG_1_OSR_BIT));
}

void magnetic_sensor::soft_reset() {
    //No need to keep other bits set in control_reg_2, as they get 0 by reset anyways
    wiringPiI2CWriteReg8(this->fd, CONTROL_REG_2, 1 << 7);
}

void magnetic_sensor::enable_update_ready_bit() {
    const int current_state = wiringPiI2CReadReg8(this->fd, CONTROL_REG_2);
    wiringPiI2CWriteReg8(this->fd, CONTROL_REG_2, current_state & 0xFE);
}

void magnetic_sensor::disable_update_ready_bit() {
    const int current_state = wiringPiI2CReadReg8(this->fd, CONTROL_REG_2);
    wiringPiI2CWriteReg8(this->fd, CONTROL_REG_2, current_state | 0b1);
}

int magnetic_sensor::get_temperature() {
    uint lsb, msb;
    lsb = static_cast<uint>(wiringPiI2CReadReg8(this->fd, TEMPERATURE_REG_LSB));
    msb = static_cast<uint>(wiringPiI2CReadReg8(this->fd, TEMPERATURE_REG_MSB));
    return static_cast<int>(
            //msb << 8 is uint << int, and so the return value is int
            //clang tidy isn't happy about: int | uint
            //Interesting one :)
            msb << uint(8) | lsb
            );
}


