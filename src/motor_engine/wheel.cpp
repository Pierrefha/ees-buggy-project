#include <motor_engine/wheel.h>
#include <motor_engine/constants.h>
#include <cassert>
#include <wiringPiI2C.h>

void set_reg_value(int fd, uint8_t pin, uint16_t on_value, uint16_t off_value){
    wiringPiI2CWriteReg8(fd, ON_L_register_addr_of(pin) , on_value & 0xFF);
    wiringPiI2CWriteReg8(fd, ON_H_register_addr_of(pin) , on_value >> 8);
    wiringPiI2CWriteReg8(fd, OFF_L_register_addr_off(pin) , off_value & 0xFF);
    wiringPiI2CWriteReg8(fd, OFF_H_register_addr_off(pin) , off_value >> 8);
}

void wheel::set_speed(uint16_t speed_value) {
    assert(speed_value <= MAX_REGISTER_VALUE);
    if(speed_value < MIN_SPEED_VALUE){
        speed_value = 0;
    }
    set_reg_value(fd, pwmPin, 0, speed_value);
    current_speed = speed_value;
}

void wheel::forward() {
    set_reg_value(fd, in1Pin, MIN_REGISTER_VALUE, MAX_REGISTER_VALUE);
    set_reg_value(fd, in2Pin, MAX_REGISTER_VALUE, MIN_REGISTER_VALUE);
}

void wheel::backwards(){
    set_reg_value(fd, in1Pin, MAX_REGISTER_VALUE, MIN_REGISTER_VALUE);
    set_reg_value(fd, in2Pin, MIN_REGISTER_VALUE, MAX_REGISTER_VALUE);
}

void wheel::stop() {
    set_speed(MIN_REGISTER_VALUE);
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

void wheel::decrease_speed(uint16_t speed_change) {
    //Check current_speed >= speed_change to prevent underflow
    uint16_t new_speed = current_speed >= speed_change ? current_speed - speed_change : 0;
    set_speed(new_speed);
}

void wheel::increase_speed(uint16_t speed_change) {
    if(stands_still()){
        //setting cur_speed to min speed will call set_speed with MIN_SPEED_VAL + speed_change
        //But if speed_change > MIN_SPEED_VALUE, MIN_SPEED_VALUE - speed_change can underflow
        //So we accept the higher start speed_value in favour of less checks
        current_speed = MIN_SPEED_VALUE;
    }
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
    //CAUTION: We change the in1Pin with in2Pin so that every CCW motion becomes a CW motion and vice versa (180 deg rot)
    //As the left wheel is in model world rotated by 180 deg around the up-axis (y-axis) this pin change makes a 360 deg
    //rot.
    //This makes the left wheel forward / backwards function behaves as expected, but not behave (strictly speaking) correctly
    return {fd, 8, 9, 10};
}
