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


	void set_mode(magnetic_sensor_defs::MODE new_mode);
    void set_odr(magnetic_sensor_defs::ODR new_odr);
    void set_rng(magnetic_sensor_defs::RNG new_rng);
    void set_osr(magnetic_sensor_defs::OSR new_osr);

	/**
	 * @return true if any x, y, z value had an overflow; false otherwise
	 */
	bool data_overflow();
	/**
	 * @return true if new data is available; false otherwise
	 */
	bool has_new_data();

	/**
	 * @return true if new data is available but has not yet been read in
	 */
	bool data_skipped();

	/**
	 * Checks for new data and updates x,y,z coordinates when new data is available
	 * @return 1 if data updated, 0 otherwise
	 */
	int check_and_update();

	/**
	 * Resets all registers to default values.
	 */
	void soft_reset();

	/**
	 * Disable ability to check for new data
	 * Default: Update ready bit is enabled
	 */
	void disable_update_ready_bit();
	/**
	 * Enables ability to check for new data
	 * Default: Update ready bit is enabled
	 */
	void enable_update_ready_bit();

	int release_resources();

	/**
	 * The temperature is factory calibrated, but the offset is not compensated
	 * Only uses these values for relative calculations
	 * e.G. (new_temp - old_temp) > 0 == temperature_increased
	 * The coefficient is ca. 100 LSB/Celsius
	 * @return relative temperature value
	 */
	int get_temperature();
};

#endif //EES_MAGNETIC_SENSOR_H
