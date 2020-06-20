//
// Created by leonhard on 20.06.20.
//

#include <driving/automatic_movement.h>
#include <cassert>
#include <chrono>
#include <util/time_util.h>

void automatic_movement::rotate_in_place_by(direction dir, degree<float> angle) {
    assert(dir == direction::IN_PLACE_TURN_LEFT || dir == direction::IN_PLACE_TURN_RIGHT);
    auto current_rot = compass->get_rotation();
    auto end_rot = current_rot + angle;

    if(dir == direction::IN_PLACE_TURN_LEFT){
        engine->turn_in_place_left();
    }else{
        engine->turn_in_place_right();
    }
    engine->set_speed(MIN_SPEED_VALUE);
    const auto epsilon = degree<float>{1.};
    while(current_rot - end_rot > epsilon){
        current_rot = compass->get_rotation();
    }
    engine->smooth_stop();
}

automatic_movement::automatic_movement(magnetic_sensor *compass, motor_engine *engine, ultrasonic_sensor *distSensor)
        : compass(compass), engine(engine), dist_sensor(distSensor) {}

void automatic_movement::move_to_point(vertex2D<float> finish_point) {
    auto obstacle_dist_cm = dist_sensor->calc_distance();

    auto current_point = vertex2D<float>{0., 0.};

    //Rotate buggy to point to finish_point
    rotate_in_place_to(finish_point);

    //While we are not closer than 5 cm to the goal
    while((current_point - finish_point).length() < 3){
        cm distance = cm{current_point.distance(finish_point)};
        const auto travel_time_needed = std::chrono::microseconds{long(distance.get() / MIN_SPEED_IN_CM_PER_SEC * 1000)};

        engine->set_speed(MIN_SPEED_VALUE);
        auto time_moved = exec_functor_for([&]() -> bool{
            //Check if obstacle in front
            if(dist_sensor->calc_distance().get() < 15){
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

        if(time_moved >= travel_time_needed){
            //we are at the finish
            return;
        }

        //We have an obstacle in front not allowing us to move further
        

    }



}

void automatic_movement::rotate_in_place_to(vertex2D<float> direction) {
    auto current_dir = compass->get_direction();

    auto angle = current_dir.angle_to(direction);
    if(angle.value > 0){
        engine->turn_in_place_left();
    }else{
        engine->turn_in_place_right();
    }
    engine->set_speed(MIN_SPEED_VALUE);

    auto epsilon = degree<float>{1};
    while(current_dir.angle_to(direction) > epsilon){
        current_dir = compass->get_direction();
    }

    engine->smooth_stop();
}
