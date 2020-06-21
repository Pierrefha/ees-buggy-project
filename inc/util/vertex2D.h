//
// Created by leonhard on 20.06.20.
//

#ifndef EES_BUGGY_VERTEX2D_H
#define EES_BUGGY_VERTEX2D_H

#include <util/degree.h>
#include <math.h>

template<typename T>
struct vertex2D{
    T x,y;

    vertex2D(T x, T y) : x(x), y(y) {}

    vertex2D<T> operator+(const vertex2D<T>& rhs)const{
        return vertex2D(x + rhs.x, y + rhs.y);
    }

    vertex2D<T> operator-(const vertex2D<T>& rhs)const{
        return vertex2D(x - rhs.x, y - rhs.y);
    }

    vertex2D<T> operator*(double scalar)const{
        return vertex2D<T>(x * scalar, y * scalar);
    }

    double operator*(const vertex2D<T>& rhs)const{
        return x * rhs.x + y * rhs.y;
    }

    vertex2D<T> normalize()const{
        const auto v_length = length();
        return vertex2D<T>(x / v_length, y / v_length);
    }

    double length()const{
        return std::sqrt(x * x + y * y);
    }

    double distance_to(const vertex2D<float>& point)const{
        return (*this - point).length();
    }

    degree<float> angle_to(const vertex2D<float>& other)const{
//        https://math.stackexchange.com/questions/2584451/how-to-get-the-direction-of-the-angle-from-a-dot-product-of-two-vectors
        return degree<float>{
            std::asin(
                    (x * other.y - other.x * y) / std::sqrt((length() * other.length()))
                    ) * 180 / M_PI
        };
    }

};
#endif //EES_BUGGY_VERTEX2D_H
