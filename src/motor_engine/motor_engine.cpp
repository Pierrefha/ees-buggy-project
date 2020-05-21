//
// Created by leonhard on 20.05.20.
//

#include <motor_engine/motor_engine.h>
#include <iostream>
#include <unistd.h>
#include <thread>
#include <motor_engine/constants.h>
#include <wiringPiI2C.h>

void motor_engine::decrease_speed() {
    //When decreasing speed we dont do a check like in increase_speed
    //So that decreasing speed will always stop the robot
    //No matter if the robot drives a curve or not
    left.decrease_speed();
    right.decrease_speed();
}

void motor_engine::increase_speed() {
    //When increasing speed we check if already a wheel has max speed
    //And if so we dont speed up the other to prevent the robot go
    //from a curve motion into a straight motion by increasing_speed
    if(left.is_at_max_speed() || right.is_at_max_speed()){
        return;
    }
    left.increase_speed();
    right.increase_speed();
}

motor_engine::motor_engine(int fd, const wheel &left, const wheel &right) : fd(fd), left(left), right(right) {}

int motor_engine::release_engine() {
    return close(fd);
}

void motor_engine::forward() {
    right.forward();
    left.forward();
    cur_direction = direction::FORWARD;
}

void motor_engine::backwards() {
    right.backwards();
    left.backwards();
    cur_direction = direction::BACKWARDS;
}

void motor_engine::smooth_stop() {
    //Decrease speed slowly
    while(!(left.stands_still() && right.stands_still())){
        left.decrease_speed();
        right.decrease_speed();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    //set wheels into stop mode
    left.stop();
    right.stop();

    cur_direction = direction::STOP;
}

void motor_engine::emergency_stop() {
    left.stop();
    right.stop();

    cur_direction = direction::STOP;
}

void motor_engine::turn_right() {
    left.increase_speed();
}

void motor_engine::turn_left() {
    right.increase_speed();
}


int motor_engine::device_fd() {
    return fd;
}

void motor_engine::set_speed(uint16_t speed_value) {
    left.set_speed(speed_value);
    right.set_speed(speed_value);
}

void motor_engine::turn_in_place_right() {
    left.forward();
    right.backwards();

    cur_direction = direction::IN_PLACE_TURN_RIGHT;
}

void motor_engine::turn_in_place_left() {
    left.backwards();
    right.forward();

    cur_direction = direction::IN_PLACE_TURN_LEFT;
}

direction motor_engine::direction() {
    return cur_direction;
}


motor_engine make_motor_engine(uint32_t motor_hat_addr) {
    int fd = wiringPiI2CSetup (motor_hat_addr) ;
    if(fd == -1){
        std::cout << "setup didnt work" << std::endl;
        return motor_engine{-1, wheel(), wheel()};
    }
    return motor_engine{fd, make_left_wheel(fd), make_right_wheel(fd)};
}
