//
// Created by leonhard on 20.05.20.
//

#ifndef EES_BUGGY_CONSTANTS_H
#define EES_BUGGY_CONSTANTS_H

#include <cmath>
#include <cstdint>

//Max reg value is 12 bits set
const uint16_t MAX_REGISTER_VALUE = 4096;
const uint16_t MIN_REGISTER_VALUE = 0;

const uint16_t MIN_SPEED_VALUE = 300;

enum ModeBits
{
    kRestart     = 0x80,
    kSleep       = 0x10,
    kAllCall     = 0x01,
    kInvert      = 0x10,
    //kOutDrive is bit 2 in mode2 reg. bit 4 is invrt reg. But this is example code so better dont touch it
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

inline constexpr uint32_t ON_L_register_addr_of(uint8_t pin){
    return Registers::kLed0OnL + 4 * pin;
}

inline constexpr uint32_t ON_H_register_addr_of(uint8_t pin){
    return Registers::kLed0OnH + 4 * pin;
}

inline constexpr uint32_t OFF_L_register_addr_off(uint8_t pin){
    return Registers::kLed0OffL + 4 * pin;
}

inline constexpr uint32_t OFF_H_register_addr_off(uint8_t pin){
    return Registers::kLed0OffH + 4 * pin;
}
#endif //EES_BUGGY_CONSTANTS_H
