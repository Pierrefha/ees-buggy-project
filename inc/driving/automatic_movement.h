//
// Created by leonhard on 20.06.20.
//

#ifndef EES_BUGGY_AUTOMATIC_MOVEMENT_H
#define EES_BUGGY_AUTOMATIC_MOVEMENT_H

#include <motor_engine/motor_engine.h>
#include <ultrasonic_sensor/ultrasonic_sensor.h>
#include <util/strong_type_defs.h>

class automatic_movement{
private:
    magnetic_sensor* compass = nullptr;
    motor_engine* engine = nullptr;
    ultrasonic_sensor* dist_sensor = nullptr;


public:

    automatic_movement(magnetic_sensor *compass, motor_engine *engine, ultrasonic_sensor *distSensor);

    /**
     * Turn in place by the specified angle
     * @param dir either turn_in_place_left or turn_in_place_right
     * @param angle angle the buggy shall rotate
     */
    void rotate_in_place_by(direction dir, degree<float> angle);

    /**
     * Turn in place to the specified angle
     * @param dir either turn_in_place_left or turn_in_place_right
     * @param angle angle the buggy shall rotate
     */
    void rotate_in_place_to(vertex2D<float> direction);

    /**
     * Moves the buggy forward by distance_cm
     * If a wall appears, the buggy stops.
     * @param distance_cm
     */
    void move_forward(cm forward_dist);

    void move_to_point(vertex2D<float> finish);

    vertex2D<float> move_to_point_if_possible(vertex2D<float> start, vertex2D<float> finish);

};


#endif //EES_BUGGY_AUTOMATIC_MOVEMENT_H
