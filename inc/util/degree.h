//
// Created by leonhard on 20.06.20.
//

#ifndef EES_BUGGY_DEGREE_H
#define EES_BUGGY_DEGREE_H

#include <stdlib.h>
#include <math.h>

template<typename T>
struct degree{
public:
    T value;

    degree() : value{T()}{}

    constexpr explicit degree(T value){
        this->value = check_value(value);
    }

    T to_radian()const{
        return T(value * M_PI / 180.);
    }

    std::tuple<T, T> to_vec()const{
        const auto radians = to_radian();
        return std::tuple<T, T>{T(std::cos(radians)), T(std::sin(radians))};
    }
    /**
     * @return this rotation with positive value
     */
    degree<T> to_positive()const{
        if(value < 0){
            return degree<T>{T(360. + value)};
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
