//
// Created by leonhard on 19.06.20.
//

#ifndef EES_BUGGY_TIME_UTIL_H
#define EES_BUGGY_TIME_UTIL_H

#include <chrono>

//std::this_thread::sleep_for may sleep longer than passed time.
//To be more accurate in exchange of battery we busy_wait
template<typename chrono_time>
void busy_wait(chrono_time delay){
    auto start = std::chrono::steady_clock::now();
    auto now = std::chrono::steady_clock::now();
    while(now - start < delay){
        now = std::chrono::steady_clock::now();
    }
}

/**
 * busy waits until the given predicate p is true or timeout happend
 * @tparam Rep
 * @tparam Period
 * @tparam predicate
 * @tparam chrono_time
 * @param p
 * @param timeout
 * @return duration representing how long the function ran
 */
template<typename predicate, typename chrono_time = std::chrono::hours>
auto busy_wait_until(predicate p, chrono_time timeout = std::chrono::hours{100'000}){
    auto start = std::chrono::steady_clock::now();
    auto now = std::chrono::steady_clock::now();
    while (now - start < timeout && !p()) {
        now = std::chrono::steady_clock::now();
    }
    return now - start;
}

/**
 * Executes functor f repeatedly until time t is over
 * To stop the execution before the time t is over,
 * the functor must return anything convertible to false;
 * To continue execution until the time t is over,
 * the functor must return anything convertible to true;
 *
 * In each iteration the functor gets the passed time
 * since start of this function
 *
 * @return time the function ran
 */
template<typename functor, typename chrono_time>
auto exec_functor_for(functor f, chrono_time t){
    auto start = std::chrono::steady_clock::now();
    auto now = std::chrono::steady_clock::now();
    while(now - start < t){
        auto shall_continue = f(now - start);
        if(!shall_continue) return now - start;
        now = std::chrono::steady_clock::now();
    }
    return now - start;
}

#endif //EES_BUGGY_TIME_UTIL_H
