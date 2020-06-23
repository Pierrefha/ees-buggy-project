//
// Created by leonhard on 20.06.20.
//

#include <driving/automatic_movement.h>
#include <cassert>
#include <chrono>
#include <util/time_util.h>
#include <magnetic_sensor/magnetic_sensor.h>
#include <iostream>

void automatic_movement::rotate_in_place_by(degree<float> angle) {
    const auto epsilon = degree<float>{10.};
    auto current_rot = compass->get_rotation_360();
    auto end_rot = (current_rot + angle).to_positive();
    std::cout << "Current rot: " << current_rot.value << std::endl;
    std::cout << "End rot: " << end_rot.value << std::endl;
    if(current_rot - end_rot <= epsilon){
        return;
    }

    if(angle.value >= 0){
        engine->turn_in_place_left();
    }else{
        engine->turn_in_place_right();
    }
    engine->set_speed(MIN_SPEED_VALUE);
    int i = 0;
    while(current_rot - end_rot > epsilon){
        current_rot = compass->get_rotation_360();
        if(i == 100){
            std::cout << current_rot.value << std::endl;
            i = 0;
        }else{
            i++;
        }
    }
    engine->smooth_stop();
}

automatic_movement::automatic_movement(magnetic_sensor *compass, motor_engine *engine, ultrasonic_sensor *distSensor)
        : compass(compass), engine(engine), dist_sensor(distSensor) {}

vertex2D<float> automatic_movement::move_to_point_if_possible(vertex2D<float> start, vertex2D<float> finish) {
    //Rotate buggy to point to finish_point
    const auto direction_normalized = (finish - start).normalize();
    rotate_in_place_to(direction_normalized);

    cm distance = cm{start.distance_to(finish)};
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
        auto current_point =
                direction_normalized *
                        (std::chrono::duration_cast<std::chrono::milliseconds>(dt).count()
                         * MIN_SPEED_IN_CM_PER_SEC / 1000.0);
        if(current_point.distance_to(finish) >= obst_dist.get()){
            engine->smooth_stop();
            //Stop execution
            return false;
        }

        return true;

        //TODO
        //Check buggy is still going correct direction
//        auto current_dir = compass->get_direction();
//        auto epsilon = degree<float>{3};
//        if(current_dir.angle_to() < epsilon){
//
//        }
    }, travel_time_needed);
    //Make sure engine is stopped
    engine->smooth_stop();

    return direction_normalized *
            (std::chrono::duration_cast<std::chrono::milliseconds>(time_moved).count()
             * MIN_SPEED_IN_CM_PER_SEC / 1000.0);
}

void automatic_movement::move_to_point(vertex2D<float> finish_point) {

    auto current_point = vertex2D<float>{0., 0.};

    //While we are not closer than 5 cm to the goal
    while((current_point - finish_point).length() < 3) {
        current_point = move_to_point_if_possible(current_point, finish_point);
        if (current_point.distance_to(finish_point) < 3) {
            //We are at the finish
            return;
        }

        //An obstacle was in front
        //Tactic: Move left, every 20 cm check if we can go right.
        // if we can go right, do so for 20 cm. Then move to point again
        // if we hit a wall before finding path to the right, we turn 180 deg and try on left side.
        // if no open wall before wall in front end unsuccessfull
        bool open_area_found = false;
        int checks_done = 0;
        rotate_in_place_by(degree<float>{90});
        auto obst_dist = dist_sensor->calc_distance();
        while(!obst_dist || obst_dist->get() >= 30){
            ++checks_done;
            this->move_forward(cm{20});
            rotate_in_place_by(degree<float>{90});
            obst_dist = dist_sensor->calc_distance();
            if(!obst_dist || obst_dist->get() < 40){
                //if no obstacle or obstacle so far away there might be path on right
                open_area_found = true;
                move_forward(cm{30});
                break;
            }
            rotate_in_place_by(degree<float>{90});
            obst_dist = dist_sensor->calc_distance();
        }
        if(open_area_found){
            continue;
        }

        //no open wall on left side of obstacle, now try on right side
        rotate_in_place_by(degree<float>{180.});
        move_forward(cm{checks_done * 20});

        obst_dist = dist_sensor->calc_distance();
        while(!obst_dist || obst_dist->get() >= 30){
            this->move_forward(cm{20});
            rotate_in_place_by(degree<float>{90});
            obst_dist = dist_sensor->calc_distance();
            if(!obst_dist || obst_dist->get() < 40){
                //if no obstacle or obstacle so far away there might be path on right
                open_area_found = true;
                move_forward(cm{30});
                break;
            }
            rotate_in_place_by(degree<float>{90});
            obst_dist = dist_sensor->calc_distance();
        }

        if(!open_area_found){
            //neither on left nor right side open area, cant reach point. return
            return;
        }
    }
}

void automatic_movement::move_forward(cm forward_dist) {
    const auto dir = compass->get_direction().normalize();
    move_to_point_if_possible({0.,0.}, dir * forward_dist.get());
}


void automatic_movement::rotate_in_place_to(vertex2D<float> direction) {
    const auto epsilon = degree<float>{1};
    auto current_dir = compass->get_direction();

    auto angle = current_dir.angle_to(direction);
    if(angle < epsilon){
        return;
    }

    if(angle.value > 0){
        engine->turn_in_place_left();
    }else{
        engine->turn_in_place_right();
    }
    engine->set_speed(MIN_SPEED_VALUE);

    while(current_dir.angle_to(direction) > epsilon){
        current_dir = compass->get_direction();
    }

    engine->smooth_stop();
}

