#ifndef EES_ULTRASONIC_SENSOR_H
#define EES_ULTRASONIC_SENSOR_H

//TODO probably move to constants.h
#define SPEED_OF_SOUND_IN_CM_PER_MILLISECOND 34.3
#define SLEEP_TIME_IN_MICROSECONDS 10
#define ON 1
#define OFF 0

#include <cstdint>
#include <util/strong_type_defs.h>
#include <optional>

/*
 * Class used to calculate distance with the ultrasonic sensor.
 */
class ultrasonic_sensor{

    int8_t trigger_pin;
    int8_t echo_pin;
    int8_t brake_light_pin;
    std::optional<double> time_diff;
    std::optional<double> distance;

public:
    ultrasonic_sensor(int8_t trigger_pin,int8_t echo_pin, int8_t brake_light_pin);

    /*
     * core functions
     */
    void init();
    // measures time taken between trigger and echo
    std::optional<double> measure_time_diff();
    // calculates distance from time difference
    std::optional<cm> calc_distance();
    // sets brake light
    void set_brake_light(int8_t mode);
    void release_resources();

    /*
     * additional functions
     */
    bool turn_around();

};
#endif // EES_ULTRASONIC_SENSOR_H
