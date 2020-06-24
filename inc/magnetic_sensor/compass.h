//
// Created by leonhard on 23.06.20.
//

#ifndef EES_BUGGY_COMPASS_H
#define EES_BUGGY_COMPASS_H

#include "magnetic_sensor.h"
#include <util/vertex2D.h>
#include <util/degree.h>
#include <thread>

#define COMPASS_FLOATING_AVERAGE_SIZE 5
class compass{
private:
    magnetic_sensor sensor;
    std::thread update_thread;
    bool join_update_thread = false;

    vertex2D<float> base_dir{-54.42, 394.77};
    float base_rot_angle_radians{0.};

    degree<float> saved_rotations[COMPASS_FLOATING_AVERAGE_SIZE];
    int oldest_rot_value = 0;
    degree<float> calculated_rot_averg{};

    degree<float> get_newest_rot_value();
    degree<float> get_current_rotation();
    vertex2D<float> get_current_direction();

    void calc_floating_average();
public:
    compass();
    /**
     * @return The rotation of the buggy relative to base_dir from [-180; 180] deg
     */
    degree<float> get_rotation();
    /**
     * @return The rotation of the buggy relative to base_dir from [0; 360[ deg
     */
    degree<float> get_rotation_360();

    /**
     * @return The rotation as normalized vector
     */
    vertex2D<float> get_direction();

    void set_current_dir_as(vertex2D<float> as_dir);
    void set_current_dir_as_base_dir();

    int release_resources();
};
#endif //EES_BUGGY_COMPASS_H
