//
// Created by leonhard on 20.05.20.
//

#ifndef EES_BUGGY_WHEEL_H
#define EES_BUGGY_WHEEL_H

#include <cstdint>

class wheel{
private:
    uint8_t pwmPin;
    uint8_t in1Pin;
    uint8_t in2Pin;
    int fd;

public:
    wheel(int fd, uint8_t pwmPin, uint8_t in1Pin, uint8_t in2Pin);

    void setSpeed(uint16_t speedValue);

    void forward();
    void backwards();
    void stop();

};

wheel make_right_wheel(int fd);
wheel make_left_wheel(int fd);

#endif //EES_BUGGY_WHEEL_H
