cmake_minimum_required(VERSION 3.5)

project(pbconf)

add_compile_options(-std=c++11)

find_package(Protobuf REQUIRED)

if(PROTOBUF_FOUND)
    message(STATUS "protobuf library found")
else()
    message(FATAL_ERROR "protobuf library is needed but cant be found")
endif()

include_directories(./
    )

set(SRC_FILE
    main.cpp
    proto/dp_poly_path_config.pb.cc
    proto/navi_path_decider_config.pb.cc
    proto/planner.pb.cc
    )


add_executable(${PROJECT_NAME} ${SRC_FILE})

target_link_libraries(${PROJECT_NAME} ${PROTOBUF_LIBRARIES})
