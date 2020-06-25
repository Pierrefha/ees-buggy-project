#ifndef EES_BUGGY_MOVEMENT_TYPE_H
#define EES_BUGGY_MOVEMENT_TYPE_H

#include <cstdint>

enum class movement_type : uint8_t{
    IN_PLACE_TURN_RIGHT,
    IN_PLACE_TURN_LEFT,
    FORWARD,
    BACKWARDS,
    STOP,

};

#endif //EES_BUGGY_MOVEMENT_TYPE_H
