#include <magnetic_sensor/compass.h>
#include <util/time_util.h>

//
// Created by leonhard on 23.06.20.
//
degree<float> compass::get_rotation_360() {
    return get_rotation().to_positive();
}

degree<float> compass::get_current_rotation() {
    return get_current_direction().angle_to(base_dir);
}

vertex2D<float> compass::get_current_direction() {
    sensor.check();
    //update measurements

    //The given data is a ellipse
    //To transform it the following parameters are needed
    //They are calculated in compass_analysis.py
    const vertex2D<float> ellipse_center{-576.9119840046621, -738.0477416737527};
    const float width = 804.4517031652929;
    const float height = 549.1679710793408;
    const float phi = -0.721345733186768;

    vertex2D<float> measurement{static_cast<float>(sensor.getX()), static_cast<float>(sensor.getY())};
    measurement = measurement - ellipse_center;
    measurement = measurement.rotate_by(phi);
    measurement.x = measurement.x * width / height;
    measurement = measurement.rotate_by(base_rot_angle_radians);
    return measurement;
}


void compass::set_current_dir_as(vertex2D<float> as_dir) {
    this->base_dir = as_dir;
    base_rot_angle_radians = get_current_direction().angle_to(as_dir).to_radian();
}

void compass::set_current_dir_as_base_dir() {
    this->base_dir = get_current_direction();
    base_rot_angle_radians = get_current_direction().angle_to(base_dir).to_radian();
}

compass::compass() {
    for (int i = 0; i < COMPASS_FLOATING_AVERAGE_SIZE; i++){
        saved_rotations[i] = get_current_rotation();
    }
    oldest_rot_value = 0;
    calc_floating_average();

    update_thread = std::thread{[&](){
        while(true){
            //End execution when the thread shall join
            if(join_update_thread){
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds{UPDATE_RATE_IN_MILLIS});
            saved_rotations[oldest_rot_value] = get_current_rotation();
            //Advance oldest value pointer
            if(oldest_rot_value == COMPASS_FLOATING_AVERAGE_SIZE - 1){
                oldest_rot_value = 0;
            }else{
                ++oldest_rot_value;
            }

            calc_floating_average();
        }
    }};
}

degree<float> compass::get_rotation() {
    return calculated_rot_averg;
}

void compass::calc_floating_average() {
    float total_rot = 0.;
    for(auto rot : saved_rotations){
        total_rot += rot.value;
    }
    //Add newest trice so it has more influence
    total_rot += 4 * get_newest_rot_value().value;

    calculated_rot_averg = degree<float>{total_rot / 9};
}

degree<float> compass::get_newest_rot_value() {
    if(oldest_rot_value == 0){
        return saved_rotations[4];
    }else{
        return saved_rotations[oldest_rot_value - 1];
    }
}

int compass::release_resources() {
    join_update_thread = true;
    if(update_thread.joinable()){
        update_thread.join();
    }
    return sensor.release_resources();
}

vertex2D<float> compass::get_direction() {
    return base_dir.rotate_by(get_rotation().to_radian()).normalize();
}
