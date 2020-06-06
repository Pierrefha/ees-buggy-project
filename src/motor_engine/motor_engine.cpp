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
    left.decrease_speed(speed_change);
    right.decrease_speed(speed_change);

    if(left.stands_still() && right.stands_still()){
        cur_direction = direction::STOP;
    }
    //If only one of them stands still and the other drives,
    //we are turning_in_place. The decrase has no effect on the rotation direction
}

void motor_engine::increase_speed() {
    //When increasing speed we check if already a wheel has max speed
    //And if so we dont speed up the other to prevent the robot go
    //from a curve motion into a straight motion by increasing_speed
    if(left.is_at_max_speed() || right.is_at_max_speed()){
        return;
    }
    left.increase_speed(speed_change);
    right.increase_speed(speed_change);
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
        left.decrease_speed(speed_change);
        right.decrease_speed(speed_change);
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
    left.increase_speed(speed_change);
}

void motor_engine::turn_left() {
    right.increase_speed(speed_change);
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

direction motor_engine::get_direction() {
    return cur_direction;
}

void motor_engine::set_frequency(double frequency) {
    // 25Mhz
    double preScaleValue = 25000000.0;
    // to 12-bit
    preScaleValue /= 4096.0;
    preScaleValue /= frequency;
    preScaleValue -= 1.0;

    const int finalPreScale = static_cast<int> (preScaleValue + 0.5);

    int oldMode = wiringPiI2CReadReg8(fd, Registers::kMode1);
    const int newMode = (oldMode & 0x7F) | ModeBits::kSleep;

    // go to sleep
    wiringPiI2CWriteReg8(fd, Registers::kMode1, newMode);
    // set prescale
    wiringPiI2CWriteReg8(fd, Registers::kPreScale, finalPreScale);
    // wake up
    wiringPiI2CWriteReg8(fd, Registers::kMode1, oldMode);

    std::this_thread::sleep_for (std::chrono::milliseconds (5));

    // restart
    wiringPiI2CWriteReg8(fd, Registers::kMode1, oldMode | ModeBits::kRestart);
}

void setAll (int fd, int on, int off)
{
    wiringPiI2CWriteReg8(fd, Registers::kAllLedOnL, on & 0xFF);
    wiringPiI2CWriteReg8(fd, Registers::kAllLedOnH, on >> 8);
    wiringPiI2CWriteReg8(fd, Registers::kAllLedOffL, off & 0xFF);
    wiringPiI2CWriteReg8(fd, Registers::kAllLedOffH, off >> 8);
}

void motor_engine::set_speed_change(uint16_t speed_change) {
    this->speed_change = speed_change;
}

/*
 * @returns the current speed of the right wheel.
 */
uint16_t motor_engine::get_speed(){
	return left.get_speed();
}


motor_engine make_motor_engine(uint32_t motor_hat_addr) {
    int fd = wiringPiI2CSetup (motor_hat_addr) ;
    if(fd == -1){
        std::cout << "setup didnt work" << std::endl;
        return motor_engine{-1, wheel(), wheel()};
    }

    //reset??? mode reg
    setAll (fd, 0, 0);
    wiringPiI2CWriteReg8(fd, Registers::kMode2, ModeBits::kOutDrive);
    wiringPiI2CWriteReg8(fd, Registers::kMode1, ModeBits::kAllCall);

    // wait for oscillator
    std::this_thread::sleep_for (std::chrono::milliseconds (5));

    int mode = wiringPiI2CReadReg8(fd, Registers::kMode1);
    // reset sleep
    mode = mode & ~ModeBits::kSleep;
    wiringPiI2CWriteReg8(fd, Registers::kMode1, mode);

    // wait for oscillator
    std::this_thread::sleep_for (std::chrono::milliseconds (5));

    return motor_engine{fd, make_left_wheel(fd), make_right_wheel(fd)};
}


