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
public:
    /**
     * Default base_dir is north
     */
	vertex2D<float> base_dir{-54.42, 394.77};
	float base_rot_angle_radians{0.};
    //Filehandle für das Benutzen der WiringI2CLibrary
	int fd;
	int16_t x;
	int16_t y;
	int16_t z;
		
public:

    magnetic_sensor();
    /**
     * @return The rotation of the buggy relative to base_dir from [-180; 180] deg
     */
    degree<float> get_rotation();

    /**
     * @return The rotation of the buggy relative to base_dir from [0; 360[ deg
     */
    degree<float> get_rotation_360();

    vertex2D<float> get_direction();
	//Getter für die Daten
	int getX();
	int getY();
	int getZ();
	
	//void setup();
	//Überprüft nach neuen Daten und fügt sie, wenn gegeben, in die Attribute ein
	int check();

	void set_current_dir_as(vertex2D<float> as_dir);
	void set_current_dir_as_base_dir();
	
	double testDirection();

	int release_resources();
};

#endif //EES_MAGNETIC_SENSOR_H
