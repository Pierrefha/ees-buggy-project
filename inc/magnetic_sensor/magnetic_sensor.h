#ifndef EES_MAGNETIC_SENSOR_H
#define EES_MAGNETIC_SENSOR_H

#include <cstdint>

namespace magnetic_sensor_defs{
    enum class MODE{
        STANDBY = 0,
        CONTINOUS = 1
    };
    enum class ODR{
        _10Hz = 0,
        _50Hz = 1,
        _100Hz = 2,
        _200Hz = 3
    };
    enum class RNG{
        _2G = 0,
        _8G = 1,
    };
    enum class OSR{
        _512 = 0,
        _256 = 1,
        _128 = 2,
        _64 = 3
    };
}


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

    magnetic_sensor();
	//Konstruktor für eigene Einstellungen
	magnetic_sensor(magnetic_sensor_defs::MODE mode,
	        magnetic_sensor_defs::ODR odr,
	        magnetic_sensor_defs::RNG rng,
	        magnetic_sensor_defs::OSR osr);

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
