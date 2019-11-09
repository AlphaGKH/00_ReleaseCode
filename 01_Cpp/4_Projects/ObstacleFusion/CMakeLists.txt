cmake_minimum_required(VERSION 2.8)

project(ObstacleFusion)

set(CMAKE_CXX_FLAGS "-std=c++17")

find_package(Eigen3 REQUIRED)

include_directories(
    ./
    )

set(SOURCESFILES
    main.cpp
    kalman/unscented/unscented_kalman.cpp
    kalman/common/system_model.cpp
    kalman/common/measurement_model.cpp
    example/obstacle.cpp
    )



add_executable(ObstacleFusion ${SOURCESFILES})
