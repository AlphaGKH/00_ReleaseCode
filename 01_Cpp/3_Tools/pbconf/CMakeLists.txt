cmake_minimum_required(VERSION 3.10)
project(pbconf)
add_compile_options(-std=c++11)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIR})
link_directories(${PROTOBUF_LIB_DIR})

file(GLOB CONF_PROTO_SRCS proto/*.proto)
PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS ${CONF_PROTO_SRCS})

set(SRC_FILE
    main.cpp
    ${PROTO_SRCS}
    )

add_executable(${PROJECT_NAME} ${SRC_FILE})

target_link_libraries(${PROJECT_NAME} ${PROTOBUF_LIBRARIES})
