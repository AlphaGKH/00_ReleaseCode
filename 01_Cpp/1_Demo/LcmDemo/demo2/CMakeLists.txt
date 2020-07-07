cmake_minimum_required(VERSION 3.5)

project(demo2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(./)

set (SRCS
    main.cpp
    time/duration.cpp
    time/time.cpp
    time/rate.cpp
    )

add_executable(demo2 ${SRCS})
target_link_libraries(demo2
    lcm
    pthread
    )
