//
// Created by leonhard on 20.05.20.
//

#include <motor_engine/wheel.h>
#include <motor_engine/constants.h>
#include <cassert>

//See the header for more information
const uint16_t wheel::speed_change = 5;

void wheel::set_speed(uint16_t speed_value) {
    assert(speed_value <= MAX_REGISTER_VALUE);
    wiringPiI2CWriteReg16(fd, ON_register_addr_of(pwmPin) , 0);
    wiringPiI2CWriteReg16(fd, OFF_register_addr_off(pwmPin) , speed_value);
    current_speed = speed_value;
}

void wheel::forward() {
    wiringPiI2CWriteReg16(fd, ON_register_addr_of(in1Pin) , MAX_REGISTER_VALUE);
    wiringPiI2CWriteReg16(fd, OFF_register_addr_off(in1Pin) , MIN_REGISTER_VALUE);

    wiringPiI2CWriteReg16(fd, ON_register_addr_of(in2Pin) , MIN_REGISTER_VALUE);
    wiringPiI2CWriteReg16(fd, OFF_register_addr_off(in2Pin) , MAX_REGISTER_VALUE);
}

void wheel::backwards(){
    wiringPiI2CWriteReg16(fd, ON_register_addr_of(in1Pin) , MIN_REGISTER_VALUE);
    wiringPiI2CWriteReg16(fd, OFF_register_addr_off(in1Pin) , MAX_REGISTER_VALUE);

    wiringPiI2CWriteReg16(fd, ON_register_addr_of(in2Pin) , MAX_REGISTER_VALUE);
    wiringPiI2CWriteReg16(fd, OFF_register_addr_off(in2Pin) , MIN_REGISTER_VALUE);
}

void wheel::stop() {
    wiringPiI2CWriteReg16(fd, ON_register_addr_of(in1Pin) , MIN_REGISTER_VALUE);
    wiringPiI2CWriteReg16(fd, OFF_register_addr_off(in1Pin) , MAX_REGISTER_VALUE);

    wiringPiI2CWriteReg16(fd, ON_register_addr_of(in2Pin) , MIN_REGISTER_VALUE);
    wiringPiI2CWriteReg16(fd, OFF_register_addr_off(in2Pin) , MAX_REGISTER_VALUE);
}

wheel::wheel(int fd, uint8_t pwmPin, uint8_t in1Pin, uint8_t in2Pin) : pwmPin(pwmPin), in1Pin(in1Pin), in2Pin(in2Pin),
                                                                       fd(fd), current_speed{0} {}

uint16_t wheel::get_speed() {
    return current_speed;
}

wheel::wheel() {}

bool wheel::is_at_max_speed() {
    return current_speed == MAX_REGISTER_VALUE;
}

void wheel::decrease_speed() {
    //Check current_speed >= speed_change to prevent underflow
    uint16_t new_speed = current_speed >= speed_change ? current_speed - speed_change : 0;
    set_speed(new_speed);
}

void wheel::increase_speed() {
    //check current_speed <= MAX_REG_VALUE to prevent overflow
    uint16_t new_speed = current_speed <= MAX_REGISTER_VALUE - speed_change ? current_speed + speed_change : MAX_REGISTER_VALUE;
    set_speed(new_speed);
}

bool wheel::stands_still() {
    return current_speed == MIN_REGISTER_VALUE;
}

wheel make_right_wheel(int fd) {
    return {fd, 13, 11, 12};
}

wheel make_left_wheel(int fd) {
    return {fd, 8, 10, 9};
}
