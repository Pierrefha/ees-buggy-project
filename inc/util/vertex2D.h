//
// Created by leonhard on 20.06.20.
//

#ifndef EES_BUGGY_VERTEX2D_H
#define EES_BUGGY_VERTEX2D_H

#include <util/degree.h>
#include <math.h>

template<typename T>
struct vertex2D{
    T x = 0.,y = 0.;

    vertex2D(){}
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

    double squared_length()const{
        return x * x + y * y;
    }

    double length()const{
        return std::sqrt(x * x + y * y);
    }

    double distance_to(const vertex2D<float>& point)const{
        return (*this - point).length();
    }

    vertex2D<T> rotate_by(T radians){
        const float c = std::cos(radians);
        const float s = std::sin(radians);
        const float x_copy = x;
        const float y_copy = y;
        vertex2D<T> result;
        result.x = x_copy * c - y_copy * s;
        result.y = y_copy * c + x_copy * s;
        return result;
    }

    degree<float> angle_to(const vertex2D<float>& other)const{
//        https://math.stackexchange.com/questions/2584451/how-to-get-the-direction-of-the-angle-from-a-dot-product-of-two-vectors
        int min = 1;
        if((x * other.y - y * other.x) < 0){
            min = -1;
        }else{
            min = 1;
        }
        auto inside = ((*this) * other) / (length() * other.length());
        inside = std::min(1., inside);
        inside = std::max(-1., inside);
        return degree<float>{float(min * std::acos(inside) * 180 / M_PI)};
    }

};
#endif //EES_BUGGY_VERTEX2D_H
