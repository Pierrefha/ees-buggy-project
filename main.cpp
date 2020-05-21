#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <signal.h>
#include <iostream>
#include <thread>
#include <motor_engine/constants.h>
#include <motor_engine/motor_engine.h>

int fd = -1;
motor_engine* engine = nullptr;
/// Interrupt Routine for STRG-C
void signalHandler(int signum)
{
    std::cout << "Strg-C Programmende" << std::endl;
    if(engine){
        engine->emergency_stop();
        engine->release_engine();
    }
    // Beenden Sie hier bitte alle Verbindung zu den Sensoren etc.
    exit(signum);
}

enum Bits
{
    kRestart     = 0x80,
    kSleep       = 0x10,
    kAllCall     = 0x01,
    kInvert      = 0x10,
    kOutDrive    = 0x04,
};


void setChannel (int channel, int on, int off)
{
    wiringPiI2CWriteReg8(fd, Registers::kLed0OnL + 4 * channel , on & 0xFF);
    wiringPiI2CWriteReg8(fd, Registers::kLed0OnH + 4 * channel, on >> 8);
    wiringPiI2CWriteReg8(fd, Registers::kLed0OffL + 4 * channel, off & 0xFF);
    wiringPiI2CWriteReg8(fd, Registers::kLed0OffH + 4 * channel, off >> 8);
}

void setPin (int pin, bool enabled)
{
//    if (pin < 0 || pin > 15)
//    {
//        log::error ("Failed to set PWM pin " + std::to_string (pin) + ". Must be between 0 and 15 inclusive.");
//        return;
//    }

    setChannel (pin, enabled ? 4096 : 0, enabled ? 0 : 4096);
}

void setAll (int on, int off)
{
    wiringPiI2CWriteReg8(fd, Registers::kAllLedOnL, on & 0xFF);
    wiringPiI2CWriteReg8(fd, Registers::kAllLedOnH, on >> 8);
    wiringPiI2CWriteReg8(fd, Registers::kAllLedOffL, off & 0xFF);
    wiringPiI2CWriteReg8(fd, Registers::kAllLedOffH, off >> 8);
}
int main ()
{
    // Csignal für Abbruch über STRG-C
    signal(SIGINT, signalHandler);
    wiringPiSetup () ;
    pinMode (0, OUTPUT) ;
    do
    {
        digitalWrite (0, HIGH) ; delay (500) ;
        digitalWrite (0,  LOW) ; delay (500) ;
    }while(false);



    engine = new motor_engine{make_motor_engine()};
    fd = engine->device_fd();


//    //Setup
    setAll (0, 0);
//
//    wiringPiI2CWriteReg8(fd, Registers::kAllLedOnH, on >> 8);
    wiringPiI2CWriteReg8(fd, Registers::kMode2, Bits::kOutDrive);
    wiringPiI2CWriteReg8(fd, Registers::kMode1, Bits::kAllCall);

    // wait for oscillator
    std::this_thread::sleep_for(std::chrono::milliseconds (5));
    int mode = wiringPiI2CReadReg8(fd, Registers::kMode1);
    // reset sleep
    mode = mode & ~Bits::kSleep;
    wiringPiI2CWriteReg8(fd, Registers::kMode1, mode);

    // wait for oscillator
    std::this_thread::sleep_for (std::chrono::milliseconds (5));

    std::cout << "driving forward" << std::endl;
    engine->set_speed(255);
    engine->forward();
    std::this_thread::sleep_for (std::chrono::milliseconds (500));
    engine->smooth_stop();

    std::cout << "driving backwards" << std::endl;
    engine->set_speed(500);
    engine->backwards();
    std::this_thread::sleep_for (std::chrono::milliseconds (500));
    engine->smooth_stop();

    engine->release_engine();
    delete engine;
//
//    //LEFT WHEEL
//    int pwm = 8;
//    int pin1 = 9;
//    int pin2 = 10;
//    //RIGHT WHEEl
//    int pwmPin = 13;
//    int in2Pin = 12;
//    int in1Pin = 11;
//
//    setChannel (pwm, 0, 100 * 16);
//    setPin (pin1, true);
//    setPin (pin2, false);
//
//    std::this_thread::sleep_for(std::chrono::milliseconds (1000));
//
//    // release the motor after use
//    setPin (pin1, false);
//    setPin (pin2, false);
}
