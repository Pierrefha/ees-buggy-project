//
// Created by leonhard on 24.06.20.
//

#include <motor_engine/motor_engine.h>
#include <magnetic_sensor/compass.h>
#include <iostream>

void test_rotation_over_time(motor_engine* engine, compass* compass){
    auto start = std::chrono::steady_clock::now();
    auto now = std::chrono::steady_clock::now();
    engine->turn_in_place_left();
    engine->set_speed(MIN_SPEED_VALUE);
    for (int i = 0; i < 10000; i++) {
        std::cout << std::chrono::duration_cast<std::chrono::microseconds>(start - now).count() << " " << compass->get_rotation().value << std::endl;
    }
    engine->smooth_stop();
}
