#ifndef EES_MAGNETIC_SENSOR_H
#define EES_MAGNETIC_SENSOR_H

#include <util/vertex2D.h>
#include <util/degree.h>
#include <cstdint>

#define sensor_address	0x0d
#define SET_RESET_REG	0x0b
#define SETUP_REG		0x09
#define X_LSB			0x00
#define X_MSB			0x01
#define Y_LSB			0x02
#define Y_MSB			0x03
#define Z_LSB			0x04
#define Z_MSB			0x05


class magnetic_sensor{
private:
	//Filehandle für das Benutzen der WiringI2CLibrary
	int fd;
	int16_t x;
	int16_t y;
	int16_t z;
		
public:
	/*
	get_rotation() und get_direction() arbeiten mit den Werten die mit check()
	gesetzt werden. Also vor der Benutzung sicherstellen das es Werte / neue Werte gibt Bsp:
	if(magneticSensor.check()==1){
			std::cout << magneticSensor.get_rotation().value << "\n";
		}
	*/
    degree<float> get_rotation();
    vertex2D<float> get_direction();
	
	//Konstruktor
	magnetic_sensor();
	//Getter für die Daten
	int getX();
	int getY();
	int getZ();
	
	//Überprüft nach neuen Daten und fügt sie, wenn gegeben, in die Attribute ein
	int check();
	
};

#endif //EES_MAGNETIC_SENSOR_H
