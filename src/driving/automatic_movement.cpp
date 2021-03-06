//
// Created by leonhard on 20.06.20.
//

#include <driving/automatic_movement.h>
#include <cassert>
#include <chrono>
#include <util/time_util.h>
#include <magnetic_sensor/magnetic_sensor.h>
#include <iostream>

const auto rotation_epsilon = degree<float>{3.};
void automatic_movement::rotate_in_place_by(degree<float> angle) {
    auto current_rot = cmpass->get_rotation_360();
    auto end_rot = (current_rot + angle).to_positive();
    if(current_rot - end_rot <= rotation_epsilon){
        return;
    }

//    std::cout << "Rotating in place" << std::endl;
//    std::cout << "Current rot: " << current_rot.value << std::endl;
//    std::cout << "End rot: " << end_rot.value << std::endl;
    if(angle.value >= 0){
        engine->turn_in_place_left();
    }else{
        engine->turn_in_place_right();
    }
    engine->set_speed(MIN_SPEED_VALUE);
//    int i = 0;
    while(current_rot - end_rot > rotation_epsilon){
        current_rot = cmpass->get_rotation_360();
//        if(i == 100){
//            std::cout << current_rot.value << std::endl;
//            i = 0;
//        }else{
//            i++;
//        }
    }
    engine->smooth_stop();
}

automatic_movement::automatic_movement(compass *compass, motor_engine *engine, ultrasonic_sensor *distSensor)
        : cmpass(compass), engine(engine), dist_sensor(distSensor) {}

vertex2D<float> automatic_movement::move_to_point_if_possible(vertex2D<float> start, vertex2D<float> finish, bool direction_correction) {
    //Rotate buggy to point to finish_point
    const auto init_dir = (finish - start).normalize();
    rotate_in_place_to(init_dir);

    const auto distance = cm{start.distance_to(finish)};
    const auto travel_time_needed = std::chrono::milliseconds{long(distance.get() / MIN_SPEED_IN_CM_PER_SEC * 1000)};

    engine->forward();
    engine->set_speed(MIN_SPEED_VALUE);
    auto time_moved = exec_functor_for([&](auto dt) -> bool{
        //Check if obstacle in front
        auto obstacle_dist = dist_sensor->calc_distance();
        if(!obstacle_dist){
            return true;
        }
        //There is an obstacle
        auto obst_dist = *obstacle_dist;
        //This leads to the buggy not driving closer to the obstacle and staying in place
//        if(current_point.distance_to(finish) >= obst_dist.get()){
        if(obst_dist.get() < 10){
            engine->smooth_stop();
            //Stop execution
            return false;
        }

        //Check buggy is still going correct direction
        if(direction_correction){
            const auto rot_delta = cmpass->get_direction().angle_to(init_dir);
            if(rot_delta > rotation_epsilon){
                if(rot_delta.value < 0){
                    engine->turn_right();
                }else{
                    engine->turn_left();
                }
            }else{
                //If we have init_dir / if we have correct direction
                //Check that both wheels have same speed, and if not
                //Set both wheels to same speed
                auto[left_speed, right_speed] = engine->get_speed_perc();
                //This float check is okay, because both floats gets computed same way
                if(left_speed != right_speed){
                    std::cout << "on track again" << std::endl;
                    engine->set_speed(MIN_SPEED_VALUE);
                }
            }
        }

        return true;

    }, travel_time_needed);
    //Make sure engine is stopped
    engine->smooth_stop();

    return start + init_dir *
                   (std::chrono::duration_cast<std::chrono::milliseconds>(time_moved).count()
             * MIN_SPEED_IN_CM_PER_SEC / 1000.0);
}

vertex2D<float> automatic_movement::move_to_point_with_retry(vertex2D<float> finish_point) {
    return move_to_point_with_retry(vertex2D<float>{0., 0.}, finish_point);
}

vertex2D<float> automatic_movement::move_to_point_with_retry(vertex2D<float> current_point, vertex2D<float> finish_point) {
    std::cout << "Moving to point" << std::endl;
    std::cout << "Current: " << current_point
        << "Finish: " << finish_point << std::endl;

    //While we are not closer than 3 cm to the goal
    while(true) {
	    std::cout << "Current point: " << current_point << std::endl;
        current_point = move_to_point_if_possible(current_point, finish_point);
        if (current_point.distance_to(finish_point) < 3) {
            //We are at the finish
            std::cout << "Finish reached" << std::endl;
            return current_point;
        }

        //An obstacle was in front
        //Tactic: Move left, every 20 cm check if we can go right.
        // if we can go right, do so for 20 cm. Then move to point again
        // if we hit a wall before finding path to the right, we turn 180 deg and try on left side.
        // if no open wall before wall in front end unsuccessfull
        std::cout << "Checking leftside for a way around the obstacle" << std::endl;
        bool open_area_found = false;
        rotate_in_place_by(degree<float>{90});
        auto obst_dist = dist_sensor->calc_distance();
        cm total_moved_dist{0.};
        while(!obst_dist || obst_dist->get() >= 30){
            cm moved_dist = this->move_forward(cm{20});
            current_point = current_point + (cmpass->get_direction() * moved_dist.get());
            total_moved_dist = cm{total_moved_dist.get() + moved_dist.get()};
            rotate_in_place_by(degree<float>{-90});
            obst_dist = dist_sensor->calc_distance();
            if(!obst_dist || obst_dist->get() >= 40){
                //if no obstacle or obstacle so far away there might be path on right
                std::cout << "Found a way around on left side! Retrying:" << std::endl;
                open_area_found = true;
                moved_dist = move_forward(cm{30});
                current_point = current_point + (cmpass->get_direction() * moved_dist.get());
                break;
            }
            rotate_in_place_by(degree<float>{90});
            obst_dist = dist_sensor->calc_distance();
        }
        if(open_area_found){
            continue;
        }

        std::cout << "Found no way on left side. Retrying on right side" << std::endl;
        //no open wall on left side of obstacle, now try on right side
        rotate_in_place_by(degree<float>{180.});
        move_forward(total_moved_dist);
        current_point = current_point + (cmpass->get_direction() * total_moved_dist.get());

        obst_dist = dist_sensor->calc_distance();
        while(!obst_dist || obst_dist->get() >= 30){
            auto moved_dist = this->move_forward(cm{20});
            current_point = current_point + (cmpass->get_direction() * moved_dist.get());
            rotate_in_place_by(degree<float>{90});
            obst_dist = dist_sensor->calc_distance();
            if(!obst_dist || obst_dist->get() >= 40){
                //if no obstacle or obstacle so far away there might be path on right
                std::cout << "Found a way around on right side! Retrying:" << std::endl;
                open_area_found = true;
                moved_dist = move_forward(cm{30});
                current_point = current_point + (cmpass->get_direction() * moved_dist.get());
                break;
            }
            rotate_in_place_by(degree<float>{-90});
            obst_dist = dist_sensor->calc_distance();
        }

        if(!open_area_found){
            //neither on left nor right side open area, cant reach point. return
            std::cout << "No way left or right. Ending unsuccessfull" << std::endl;
            return current_point;
        }

    }
}

cm automatic_movement::move_forward(cm forward_dist) {
    const auto dir = cmpass->get_direction().normalize();
    return cm{move_to_point_if_possible({0.,0.}, dir * forward_dist.get()).length()};
}


void automatic_movement::rotate_in_place_to(vertex2D<float> direction) {
    rotate_in_place_by(cmpass->get_direction().angle_to(direction));
}

void automatic_movement::rotate_by(degree<float> rotate_by, uint16_t speed_diff) {
    if(speed_diff == 0){
        speed_diff = engine->get_speed_change();
    }
    auto end_rot = cmpass->get_rotation_360() + rotate_by;
    end_rot = end_rot.to_positive();
    engine->forward();
    engine->set_speed(MIN_SPEED_VALUE);
    if(rotate_by.value < 0){
        engine->turn_right(speed_diff);
    }else{
        engine->turn_left(speed_diff);
    }

    busy_wait_until([&](){return cmpass->get_rotation_360() - end_rot <= rotation_epsilon;});
    engine->emergency_stop();
}
