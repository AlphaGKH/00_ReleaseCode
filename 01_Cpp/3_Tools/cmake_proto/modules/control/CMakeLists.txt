project(control)

include_directories(${cmake_proto_SOURCE_DIR})
include_directories(${cmake_proto_BINARY_DIR})

file (GLOB PLANNING_PROTO
    #  ${cmake_proto_SOURCE_DIR}/modules/planning/proto/planning_config.proto
     ${cmake_proto_SOURCE_DIR}/modules/control/proto/*.proto
     ${cmake_proto_SOURCE_DIR}/modules/common/proto/*.proto
)

PROTOC_CPP(PB_SRCS PB_HDRS PROTO_ROOT_PATH ${cmake_proto_SOURCE_DIR} ${PLANNING_PROTO})



add_executable(control main.cpp ${PB_SRCS})
target_link_libraries(control protobuf)
