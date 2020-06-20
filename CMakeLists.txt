cmake_minimum_required(VERSION 3.13)
set (CMAKE_CXX_STANDARD 17)
project(ees_buggy)

file(GLOB_RECURSE buggy_src
        "src/*.cpp"
)
file(GLOB_RECURSE buggy_inc
        "inc/*.h"
)

add_executable(ees_buggy main.cpp ${buggy_src} ${buggy_inc})

#make include from inc possible without inc/ at begin
target_include_directories(ees_buggy PRIVATE ${PROJECT_SOURCE_DIR}/inc)

target_compile_options(ees_buggy PUBLIC -Wall)
target_link_options(ees_buggy PUBLIC -lwiringPi -pthread -lncurses)
