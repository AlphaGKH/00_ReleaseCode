cmake_minimum_required(VERSION 3.0.0)
project(singleton_demo2 VERSION 0.1.0)

enable_testing()

add_executable(singleton_demo2 main.cpp)
target_link_libraries(singleton_demo2 pthread)
