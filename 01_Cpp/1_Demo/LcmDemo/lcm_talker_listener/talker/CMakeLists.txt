cmake_minimum_required(VERSION 3.0.0)
project(Talker VERSION 0.1.0)

find_package(lcm)

add_executable(Talker main.cpp)

target_link_libraries(Talker lcm)


