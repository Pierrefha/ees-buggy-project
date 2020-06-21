//
// Created by leonhard on 20.06.20.
//

#ifndef EES_BUGGY_DEGREE_H
#define EES_BUGGY_DEGREE_H

template<typename T>
struct degree{
public:
    T value;

    constexpr explicit degree(T value){
        this->value = check_value(value);
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
    }
};

#endif //EES_BUGGY_DEGREE_H
