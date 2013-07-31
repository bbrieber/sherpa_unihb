
find_package(Protobuf REQUIRED)

set(msgs sherpa_animation_pose.proto sherpa_position.proto)
PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS ${msgs})
add_library(sherpa_gazebo_internal_msgs SHARED ${PROTO_SRCS})
target_link_libraries(sherpa_gazebo_internal_msgs ${PROTOBUF_LIBRARY})