#ifndef EES_ULTRASONIC_SENSOR_H
#define EES_ULTRASONIC_SENSOR_H

//TODO probably move to constants.h
#define SPEED_OF_SOUND_IN_CM_PER_SECOND 34300
#define SLEEP_TIME_IN_MILLISECONDS 1
#define ON 1
#define OFF 0

#include <cstdint>
/*
 * Class used to calculate distance with the ultrasonic sensor.
 */
class ultrasonic_sensor{

    int8_t trigger_pin;
    int8_t echo_pin;
    int8_t brake_light_pin;

    public:
        ultrasonic_sensor(int8_t trigger_pin,int8_t echo_pin, int8_t brake_light_pin);

        /*
         * core functions
         */
        void init();
        int32_t get_distance();
        void set_brake_light(int8_t mode);
        
        /*
         * additional functions
         */
        bool turn_around();
    
}








#endif // EES_ULTRASONIC_SENSOR_H
