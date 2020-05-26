//
// Created by leonhard on 20.05.20.
//

#ifndef EES_BUGGY_WHEEL_H
#define EES_BUGGY_WHEEL_H

#include <cstdint>
#include "constants.h"

class wheel{
private:
    uint8_t pwmPin;
    uint8_t in1Pin;
    uint8_t in2Pin;
    int fd;

    uint16_t current_speed = MIN_REGISTER_VALUE;



public:
    wheel();

    wheel(int fd, uint8_t pwmPin, uint8_t in1Pin, uint8_t in2Pin);

    bool is_at_max_speed();
    bool stands_still();

    uint16_t get_speed();
    void set_speed(uint16_t speed_value);

    void forward();
    void backwards();
    void stop();

    void decrease_speed(uint16_t speed_change);
    void increase_speed(uint16_t speed_change);
};

wheel make_right_wheel(int fd);
wheel make_left_wheel(int fd);

#endif //EES_BUGGY_WHEEL_H
