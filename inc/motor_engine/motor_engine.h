#ifndef EES_BUGGY_MOTOR_ENGINE_H
#define EES_BUGGY_MOTOR_ENGINE_H

#include <tuple>
#include <motor_engine/wheel.h>
#include <motor_engine/direction.h>
#include <magnetic_sensor/magnetic_sensor.h>
#include <util/degree.h>

class motor_engine{
private:
    int fd;
    wheel left, right;
    direction cur_direction = direction::STOP;

    //The value by which the wheel slows down or speeds up
    //by increase / decrease speed
    uint16_t speed_change = 20;
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

    void turn_in_place_right();
    void turn_in_place_left();

    void smooth_stop();
    void emergency_stop();

    void set_frequency(double frequency);
    void set_speed_change(uint16_t speed_change);

    int release_engine();

    // TODO if we turn left is left wheel full speed and right wheel on zero?
    //  or how does it work?

    // returns speed of right wheel. 
    // This is just to test speed when driving in a straight line
    uint16_t get_speed_right();
    uint16_t get_speed_left();
    /**
     * @return Speed of left and right wheels in percent
     */
    std::tuple<float, float> get_speed_perc();
    direction get_direction();
    int device_fd();
};

motor_engine make_motor_engine(uint32_t motor_hat_addr = 0x60);

#endif //EES_BUGGY_MOTOR_ENGINE_H
