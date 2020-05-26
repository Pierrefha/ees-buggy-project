//
// Created by leonhard on 21.05.20.
//

#ifndef EES_BUGGY_DIRECTION_H
#define EES_BUGGY_DIRECTION_H

#include <cstdint>

enum class direction : uint8_t{
    IN_PLACE_TURN_RIGHT,
    IN_PLACE_TURN_LEFT,
    FORWARD,
    BACKWARDS,
    STOP,

};

#endif //EES_BUGGY_DIRECTION_H
