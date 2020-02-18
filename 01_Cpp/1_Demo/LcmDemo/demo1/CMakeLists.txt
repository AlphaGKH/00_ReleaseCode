cmake_minimum_required(VERSION 3.5)

project(demo1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(./)

set (SRCS
    main.cpp
    )

add_executable(demo1 ${SRCS})
target_link_libraries(demo1 lcm)
