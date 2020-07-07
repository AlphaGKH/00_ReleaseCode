cmake_minimum_required(VERSION 3.0.0)
project(Listener VERSION 0.1.0)

find_package(lcm)
find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIR})
link_directories(${PROTOBUF_LIB_DIR})

add_executable(Listener main.cpp exlcm/expb.pb.cc)
target_link_libraries(Listener lcm protobuf)
