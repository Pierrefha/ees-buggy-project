//
// Created by leonhard on 25.06.20.
//

#ifndef EES_BUGGY_TEST_H
#define EES_BUGGY_TEST_H

#include <magnetic_sensor/compass.h>
#include <ultrasonic_sensor/ultrasonic_sensor.h>
#include <motor_engine/motor_engine.h>

void run_tests(motor_engine* engine, ultrasonic_sensor* dist_sensor, compass* compass);


#endif //EES_BUGGY_TEST_H
