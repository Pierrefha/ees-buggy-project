#ifndef EES_MAGNETIC_SENSOR_H
#define EES_MAGNETIC_SENSOR_H

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
#define UPDATE_RATE_IN_MILLIS 5


class magnetic_sensor{
private:
    /**
     * Default base_dir is north
     */
    //Filehandle für das Benutzen der WiringI2CLibrary
	int fd;
	int16_t x;
	int16_t y;
	int16_t z;
		
public:
	//Standardkonstruktor, Default Einstellung: 0x1d
    magnetic_sensor();
	//Konstruktor für eigene Einstellungen
	magnetic_sensor(int mode, int odr, int rng, int osr);

	//Getter für die Daten
	int getX();
	int getY();
	int getZ();
	
	//void setup();
	//Überprüft nach neuen Daten und fügt sie, wenn gegeben, in die Attribute ein
	int check();


	double testDirection();

	int release_resources();
};

#endif //EES_MAGNETIC_SENSOR_H
