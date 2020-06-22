//
// Created by leonhard on 22.06.20.
//

#ifndef EES_BUGGY_WASD_CONTROL_H
#define EES_BUGGY_WASD_CONTROL_H


#include <motor_engine/motor_engine.h>
#include <ultrasonic_sensor/ultrasonic_sensor.h>

class wasd_control{
private:
    bool init();
    void print_info(motor_engine* engine, ultrasonic_sensor* dist_sensor, magnetic_sensor* compass);
public:

    void run(motor_engine* engine, ultrasonic_sensor* dist_sensor, magnetic_sensor* compass);
    void release_resources();
};

#endif //EES_BUGGY_WASD_CONTROL_H
