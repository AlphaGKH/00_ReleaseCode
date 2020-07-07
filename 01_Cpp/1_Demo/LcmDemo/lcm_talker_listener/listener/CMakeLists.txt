cmake_minimum_required(VERSION 3.0.0)
project(Listener VERSION 0.1.0)

find_package(lcm)

add_executable(Listener main.cpp)
target_link_libraries(Listener lcm pthread)
