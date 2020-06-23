#include <motor_engine/motor_engine.h>
#include <ultrasonic_sensor/ultrasonic_sensor.h>
#include <magnetic_sensor/magnetic_sensor.h>
#include <iostream>
#include <driving/automatic_movement.h>
#include "../../test_inc/test_util.h"

//
// Created by leonhard on 23.06.20.
//
void test_turns(motor_engine* engine, ultrasonic_sensor* dist_sensor, magnetic_sensor* compass){
    std::cout << "Testing turning" << std::endl;
    std::cout << "Turning 45 deg" << std::endl;

    automatic_movement auto_control{compass, engine, dist_sensor};
    auto_control.rotate_in_place_by(degree<float>{45});
    continue_on_click();

    std::cout << "Turning -45 deg" << std::endl;
    auto_control.rotate_in_place_by(degree<float>{-45});
    continue_on_click();

    std::cout << "Turning 360 deg" << std::endl;
    auto_control.rotate_in_place_by(degree<float>{360});
    continue_on_click();

    std::cout << "Turning -180 deg" << std::endl;
    auto_control.rotate_in_place_by(degree<float>{-180});
    continue_on_click();

    std::cout << "Turning -90 deg" << std::endl;
    auto_control.rotate_in_place_by(degree<float>{-90});
    continue_on_click();
}
