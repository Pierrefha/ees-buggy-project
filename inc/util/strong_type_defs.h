//
// Created by leonhard on 20.06.20.
//

#ifndef EES_BUGGY_STRONG_TYPE_DEFS_H
#define EES_BUGGY_STRONG_TYPE_DEFS_H

#include <utility>

//Coppied from https://www.fluentcpp.com/2016/12/08/strong-types-for-strong-interfaces/
template <typename T, typename Parameter>
class NamedType
{
public:
    explicit NamedType(T const& value) : value_(value) {}
    explicit NamedType(T&& value) : value_(std::move(value)) {}
    T& get() { return value_; }
    T const& get() const {return value_; }
private:
    T value_;
};

using cm = NamedType<double, struct cm_struct>;

#endif //EES_BUGGY_STRONG_TYPE_DEFS_H
