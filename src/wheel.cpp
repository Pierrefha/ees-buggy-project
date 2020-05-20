//
// Created by leonhard on 20.05.20.
//

#include <wheel.h>
#include <constants.h>

void wheel::setSpeed(uint16_t speedValue) {
    wiringPiI2CWriteReg16(fd, ON_register_addr_of(pwmPin) , 0);
    wiringPiI2CWriteReg16(fd, OFF_register_addr_off(pwmPin) , speedValue);
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
                                                                       fd(fd) {}

wheel make_right_wheel(int fd) {
    return {fd, 13, 11, 12};
}

wheel make_left_wheel(int fd) {
    return {fd, 8, 8, 9};
}
