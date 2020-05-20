cmake_minimum_required(VERSION 3.15)
project(ees_buggy C)

set(CMAKE_C_STANDARD 99)

file(GLOB_RECURSE buggy_src
        "src/*.c"
)
file(GLOB_RECURSE buggy_inc
        "inc/*.c"
)

add_executable(ees_buggy main.c ${buggy_src} ${buggy_inc})

#make include from inc possible without inc/ at begin
target_include_directories(ees_buggy PRIVATE ${PROJECT_SOURCE_DIR}/inc)

target_compile_options(ees_buggy PUBLIC -Wall)
target_link_options(ees_buggy PUBLIC -lwiringPi)
