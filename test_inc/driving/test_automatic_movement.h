//
// Created by leonhard on 23.06.20.
//

#ifndef EES_BUGGY_TEST_AUTOMATIC_MOVEMENT_H
#define EES_BUGGY_TEST_AUTOMATIC_MOVEMENT_H

#include <motor_engine/motor_engine.h>
#include <ultrasonic_sensor/ultrasonic_sensor.h>
#include <magnetic_sensor/compass.h>

void test_turns(motor_engine* engine, ultrasonic_sensor* dist_sensor, compass* compass);

void test_rectangle(motor_engine* engine, ultrasonic_sensor* dist_sensor, compass* compass);

void test_move_to_point(motor_engine* engine, ultrasonic_sensor* dist_sensor, compass* compass);

void test_move_rectangle_with_points(motor_engine* engine, ultrasonic_sensor* dist_sensor, compass* compass);

void test_move_with_direction_control(motor_engine* engine, ultrasonic_sensor* dist_sensor, compass* cmpass);

void test_turn_by(motor_engine* engine, ultrasonic_sensor* dist_sensor, compass* compass);
#endif //EES_BUGGY_TEST_AUTOMATIC_MOVEMENT_H
