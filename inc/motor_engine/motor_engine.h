//
// Created by leonhard on 20.05.20.
//

#ifndef EES_BUGGY_MOTOR_ENGINE_H
#define EES_BUGGY_MOTOR_ENGINE_H

#include <motor_engine/wheel.h>

class motor_engine{
private:
    int fd;
    wheel left, right;
public:
    motor_engine(int fd, const wheel &left, const wheel &right);

public:
    void decrease_speed();
    void increase_speed();
    void set_speed(uint16_t speed_value);
    void forward();
    void backwards();

    void turn_right();
    void turn_left();

    void smooth_stop();
    void emergency_stop();

    int release_engine();

    int device_fd();
};

motor_engine make_motor_engine(uint32_t motor_hat_addr = 0x60);

#endif //EES_BUGGY_MOTOR_ENGINE_H
