#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <signal.h>
#include <iostream>
#include <thread>

int fd = -1;
/// Interrupt Routine for STRG-C
void signalHandler(int signum)
{
    std::cout << "Strg-C Programmende" << std::endl;
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

enum Registers
{
    kMode1       = 0x00,
    kMode2       = 0x01,
    kSubAddress1 = 0x02,
    kSubAddress2 = 0x03,
    kSubAddress3 = 0x04,
    kPreScale    = 0xFE,
    kLed0OnL     = 0x06,
    kLed0OnH     = 0x07,
    kLed0OffL    = 0x08,
    kLed0OffH    = 0x09,
    kAllLedOnL   = 0xFA,
    kAllLedOnH   = 0xFB,
    kAllLedOffL  = 0xFC,
    kAllLedOffH  = 0xFD,
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


    fd = wiringPiI2CSetup (0x60) ;
    if(fd == -1){
        std::cout << "setup didnt work" << std::endl;
        return -1;
    }

    //Setup
    setAll (0, 0);

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

    int pwm = 8;
    int pin1 = 9;
    int pin2 = 10;
    setChannel (pwm, 0, 100 * 16);
    setPin (pin1, true);
    setPin (pin2, false);

    std::this_thread::sleep_for(std::chrono::milliseconds (1000));

    // release the motor after use
    setPin (pin1, false);
    setPin (pin2, false);
}
