//
// Created by leonhard on 20.05.20.
//

#include <motor_engine/wheel.h>
#include <motor_engine/constants.h>
#include <cassert>
#include <wiringPiI2C.h>

//See the header for more information
const uint16_t wheel::speed_change = 5;

void set_reg_value(int fd, uint8_t pin, uint16_t on_value, uint16_t off_value){
    wiringPiI2CWriteReg8(fd, ON_L_register_addr_of(pin) , on_value & 0xFF);
    wiringPiI2CWriteReg8(fd, ON_H_register_addr_of(pin) , on_value >> 8);
    wiringPiI2CWriteReg8(fd, OFF_L_register_addr_off(pin) , off_value & 0xFF);
    wiringPiI2CWriteReg8(fd, ON_H_register_addr_of(pin) , off_value >> 8);
}

void wheel::set_speed(uint16_t speed_value) {
    assert(speed_value <= MAX_REGISTER_VALUE);
    set_reg_value(fd, pwmPin, 0, speed_value);
    current_speed = speed_value;
}

void wheel::forward() {
    set_reg_value(fd, in1Pin, MAX_REGISTER_VALUE, MIN_REGISTER_VALUE);
    set_reg_value(fd, in2Pin, MIN_REGISTER_VALUE, MAX_REGISTER_VALUE);
}

void wheel::backwards(){
    set_reg_value(fd, in1Pin, MIN_REGISTER_VALUE, MAX_REGISTER_VALUE);
    set_reg_value(fd, in2Pin, MAX_REGISTER_VALUE, MIN_REGISTER_VALUE);
}

void wheel::stop() {
    set_reg_value(fd, in1Pin, MIN_REGISTER_VALUE, MAX_REGISTER_VALUE);
    set_reg_value(fd, in2Pin, MIN_REGISTER_VALUE, MAX_REGISTER_VALUE);
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
