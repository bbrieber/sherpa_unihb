cmake_minimum_required(VERSION 2.8.3)
project(sherpa_map_description)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS gazebo_msgs gazebo_plugins gazebo_ros roscpp rospy std_msgs image_transport cv_bridge)


# Find gazebo
include (FindPkgConfig)
if (PKG_CONFIG_FOUND)
  pkg_check_modules(GAZEBO gazebo)
endif()

link_directories(${GAZEBO_LIBRARY_DIRS})
include_directories(${Boost_INCLUDE_DIR} ${catkin_INCLUDE_DIRS} ${GAZEBO_INCLUDE_DIRS})

include_directories(include
  ${catkin_INCLUDE_DIRS}
)

add_library(sherpa_map_plugin src/SherpaMapPlugin.cpp)
target_link_libraries(sherpa_map_plugin ${catkin_LIBRARIES})

catkin_package(DEPENDS roscpp gazebo_ros)