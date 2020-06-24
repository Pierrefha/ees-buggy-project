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
    auto old_rot = degree<float>(0);
	while(true){
		auto rot = compass->get_rotation_360() ;

		if(rot != old_rot){
			now = std::chrono::steady_clock::now();
			old_rot = rot;
        std::cout << std::chrono::duration_cast<std::chrono::microseconds>(now - start).count() << " " << rot.value << std::endl;
		}
	}
	engine->smooth_stop();
}
