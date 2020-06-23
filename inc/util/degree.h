//
// Created by leonhard on 20.06.20.
//

#ifndef EES_BUGGY_DEGREE_H
#define EES_BUGGY_DEGREE_H

#include <stdlib.h>

template<typename T>
struct degree{
public:
    T value;

    constexpr explicit degree(T value){
        this->value = check_value(value);
    }

    T to_radian()const{
        return value * M_PI / 180.;
    }
    /**
     * @return this rotation with positive value
     */
    degree<T> to_positive()const{
        if(value < 0){
            return degree<T>{180 + (180 - std::abs(value))};
        }else{
            return *this;
        }
    }

    degree<T> operator-(const degree &rhs) const {
        return degree{value - rhs.value};
    }

    degree<T> operator+(const degree &rhs) const {
        return degree{value + rhs.value};
    }

    bool operator==(const degree &rhs) const {
        return value == rhs.value;
    }

    bool operator!=(const degree &rhs) const {
        return !(rhs == *this);
    }

    bool operator<(const degree &rhs) const {
        return std::abs(value) < std::abs(rhs.value);
    }

    bool operator>(const degree &rhs) const {
        return rhs < *this;
    }

    bool operator<=(const degree &rhs) const {
        return !(rhs < *this);
    }

    bool operator>=(const degree &rhs) const {
        return !(*this < rhs);
    }

private:
    constexpr T check_value(T value){
        while(value >= 360){
            value -= 360;
        }
        while(value <= -360){
            value += 360;
        }
        return value;
    }
};

#endif //EES_BUGGY_DEGREE_H
