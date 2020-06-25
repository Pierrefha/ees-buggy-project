#include "../../test_inc/driving/test_automatic_movement.h"
#include <magnetic_sensor/magnetic_sensor.h>
#include <iostream>
#include <driving/automatic_movement.h>
#include <magnetic_sensor/compass.h>
#include "../../test_inc/test_util.h"

//
// Created by leonhard on 23.06.20.
//
void test_turns(motor_engine* engine, ultrasonic_sensor* dist_sensor, compass* compass){
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

void test_rectangle(motor_engine* engine, ultrasonic_sensor* dist_sensor, compass* cmpass){
    automatic_movement auto_control{cmpass, engine, dist_sensor};
    std::cout << "Driving Rectangle !" << std::endl;
    auto_control.move_forward(cm{20});
    auto_control.rotate_in_place_by(degree<float>{90});
    auto_control.move_forward(cm{20});
    auto_control.rotate_in_place_by(degree<float>{90});
    auto_control.move_forward(cm{20});
    auto_control.rotate_in_place_by(degree<float>{90});
    auto_control.move_forward(cm{20});
    auto_control.rotate_in_place_by(degree<float>{90});
    continue_on_click();
}

void test_move_to_point(motor_engine* engine, ultrasonic_sensor* dist_sensor, compass* compass){
    automatic_movement auto_control{compass, engine, dist_sensor};
    std::cout << "Driving Rectangle !" << std::endl;
    std::cout << "Setting current dir as (0, 1)" << std::endl;
    std::cout << "input finish pos as: <x> <y>"<< std::endl;
    std::cout << "values in cm"<< std::endl;
    std::string x, y;
    std::cin >> x;
    std::cin >> y;
    std::cout << "read: " << x << " " << y << std::endl;
    vertex2D<float> finish{std::stof(x), std::stof(y)};
    compass->set_current_dir_as(vertex2D<float>{0,1});
    //Give compass small time to adjust
    std::this_thread::sleep_for(std::chrono::milliseconds{100});
    std::cout << "rotation: " << compass->get_rotation().value << " | Direction" << compass->get_direction() << std::endl;
    auto_control.move_to_point_with_retry(finish);
    continue_on_click();
}

void test_move_rectangle_with_points(motor_engine* engine, ultrasonic_sensor* dist_sensor, compass* compass){
    automatic_movement auto_control{compass, engine, dist_sensor};
    compass->set_current_dir_as(vertex2D<float>{0,1});
    vertex2D<float> finish{0, 20};
    //Give compass small time to adjust
    std::this_thread::sleep_for(std::chrono::milliseconds{100});
    std::cout << "rotation: " << compass->get_rotation().value << " | Direction" << compass->get_direction() << std::endl;
    auto current_point = auto_control.move_to_point_with_retry(finish);
    auto_control.rotate_in_place_by(degree<float>{90});
    current_point = auto_control.move_to_point_with_retry(current_point, vertex2D<float>{-20, 20});
    auto_control.rotate_in_place_by(degree<float>{90});
    current_point = auto_control.move_to_point_with_retry(current_point, vertex2D<float>{-20, 0});
    auto_control.rotate_in_place_by(degree<float>{90});
    current_point = auto_control.move_to_point_with_retry(current_point, vertex2D<float>{0, 0});
    auto_control.rotate_in_place_by(degree<float>{90});


    continue_on_click();
}

void test_move_with_direction_control(motor_engine* engine, ultrasonic_sensor* dist_sensor, compass* cmpass){
    automatic_movement auto_control{cmpass, engine, dist_sensor};
    //distract buggy and check manualy that buggy tries to hold direction
    cmpass->set_current_dir_as({0,1});
    auto_control.move_to_point_if_possible({0,0}, {0,200}, true);
    continue_on_click();
}

void test_turn_by(motor_engine* engine, ultrasonic_sensor* dist_sensor, compass* compass){
    automatic_movement auto_control{compass, engine, dist_sensor};
    //distract buggy and check manualy that buggy tries to hold direction
    auto_control.rotate_by(degree<float>(180), 200);
    continue_on_click();
    auto_control.rotate_by(degree<float>(180), 200);
    continue_on_click();
    auto_control.rotate_by(degree<float>(-90), 200);
    continue_on_click();
    auto_control.rotate_by(degree<float>(-90), 200);
    continue_on_click();
}
