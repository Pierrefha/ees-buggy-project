//
// Created by leonhard on 20.06.20.
//

#ifndef EES_BUGGY_AUTOMATIC_MOVEMENT_H
#define EES_BUGGY_AUTOMATIC_MOVEMENT_H

#include <motor_engine/motor_engine.h>
#include <ultrasonic_sensor/ultrasonic_sensor.h>
#include <util/strong_type_defs.h>
#include <magnetic_sensor/compass.h>

class automatic_movement{
private:
    compass* cmpass = nullptr;
    motor_engine* engine = nullptr;
    ultrasonic_sensor* dist_sensor = nullptr;


public:

    automatic_movement(compass *cmpass, motor_engine *engine, ultrasonic_sensor *distSensor);

    /**
     * Turn in a curve by the specified angle.
     * @param angle angle the buggy shall rotate
     */
    void rotate_by(degree<float> angle, uint16_t speed_diff = 0);
    /**
     * Turn in place by the specified angle
     * @param angle angle the buggy shall rotate
     */
    void rotate_in_place_by(degree<float> angle);

    /**
     * Turn in place to the specified angle
     * @param angle angle the buggy shall rotate
     */
    void rotate_in_place_to(vertex2D<float> direction);


    /**
     * Moves the buggy forward by distance_cm
     * If a wall appears, the buggy stops.
     * @param distance_cm
     * @return distance the buggy could move
     */
    cm move_forward(cm forward_dist);

    /**
     *
     * @param finish_point where to go to
     * @return the current point
     */
    vertex2D<float> move_to_point_with_retry(vertex2D<float> finish_point);

    vertex2D<float> move_to_point_with_retry(vertex2D<float> current_point, vertex2D<float> finish_point);

    vertex2D<float> move_to_point_if_possible(vertex2D<float> start, vertex2D<float> finish, bool direction_correction = false);

};


#endif //EES_BUGGY_AUTOMATIC_MOVEMENT_H
