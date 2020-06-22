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
    void print_info(std::tuple<float, float> speed_perc, std::optional<cm> obst_dist, degree<float> rot, vertex2D<float> dir);
public:

    void run(motor_engine* engine, ultrasonic_sensor* dist_sensor, magnetic_sensor* compass);
    void release_resources();

    void print_warning();

    void print_no_forward_movement();

    bool check_forward_movement_possible(std::optional<cm> obst_dist);

    void clear_obstacle_warnings();

    void print_user_input(int cmd);
};

#endif //EES_BUGGY_WASD_CONTROL_H
