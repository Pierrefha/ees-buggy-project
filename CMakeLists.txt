cmake_minimum_required(VERSION 3.13)
set (CMAKE_CXX_STANDARD 17)
project(ees_buggy)

option(test "test" OFF)
set(test ON)

file(GLOB_RECURSE buggy_src
        "src/*.cpp"
)
file(GLOB_RECURSE buggy_inc
        "inc/*.h"
)

if(${test} STREQUAL ON)
    add_compile_definitions(TEST_ON)
    file(GLOB_RECURSE buggy_test_src
            "test_src/*.cpp")
    file(GLOB_RECURSE buggy_test_inc
            "test_inc/*.h")
endif()

add_executable(ees_buggy main.cpp ${buggy_src} ${buggy_inc} ${buggy_test_src} ${buggy_test_inc})

#make include from inc possible without inc/ at begin
target_include_directories(ees_buggy PRIVATE ${PROJECT_SOURCE_DIR}/inc)

target_compile_options(ees_buggy PUBLIC -Wall)
target_link_options(ees_buggy PUBLIC -lwiringPi -pthread -lncurses)
