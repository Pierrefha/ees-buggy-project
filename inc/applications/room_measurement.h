//
// Created by leonhard on 19.06.20.
//

#ifndef EES_BUGGY_ROOM_MEASUREMENT_H
#define EES_BUGGY_ROOM_MEASUREMENT_H

#include <motor_engine/motor_engine.h>
#include <ultrasonic_sensor/ultrasonic_sensor.h>

/**
 *
 * @param engine
 * @param dist_sensor
 */
void analyse_room(motor_engine& engine, ultrasonic_sensor dist_sensor);

#endif //EES_BUGGY_ROOM_MEASUREMENT_H
