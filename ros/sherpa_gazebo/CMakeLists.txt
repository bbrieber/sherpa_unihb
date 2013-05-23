cmake_minimum_required(VERSION 2.4.6)
include($ENV{ROS_ROOT}/core/rosbuild/rosbuild.cmake)

# Set the build type.  Options are:
#  Coverage       : w/ debug symbols, w/o optimization, w/ code-coverage
#  Debug          : w/ debug symbols, w/o optimization
#  Release        : w/o debug symbols, w/ optimization
#  RelWithDebInfo : w/ debug symbols, w/ optimization
#  MinSizeRel     : w/o debug symbols, w/ optimization, stripped binaries
#set(ROS_BUILD_TYPE RelWithDebInfo)

rosbuild_init()


#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

include(FindPkgConfig)
if(PKG_CONFIG_FOUND)
  pkg_check_modules(GAZEBO gazebo)
  pkg_check_modules(OGRE OGRE)
  pkg_check_modules(GAZEBO_ODE gazebo_ode)
endif()



include_directories(${GAZEBO_INCLUDE_DIRS} ${GAZEBO_ODE_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR}/msgs ${OGRE_INCLUDE_DIRS})
link_directories(${GAZEBO_LIBRARY_DIRS} ${GAZEBO_ODE_LIBRARY_DIRS} ${CMAKE_CURRENT_BINARY_DIR}/msgs ${OGRE_LIBRARY_DIRS})
add_subdirectory(msgs)


#uncomment if you have defined messages
#rosbuild_genmsg()
#uncomment if you have defined services
#rosbuild_gensrv()

#common commands for building c++ executables and libraries
#rosbuild_add_library(${PROJECT_NAME} src/example.cpp)
#target_link_libraries(${PROJECT_NAME} another_library)
#rosbuild_add_boost_directories()
#rosbuild_link_boost(${PROJECT_NAME} thread)
#rosbuild_add_executable(example examples/example.cpp)
#target_link_libraries(example ${PROJECT_NAME})
#rosbuild_add_library(sherpa_actor src/ActorPluginJoints.cpp)
#rosbuild_add_library(sherpa_actor_factory src/ActorFactoryPluginJoints.cpp)

rosbuild_add_library(sherpa_human_model_plugin SHARED src/SherpaHuman.cpp)
target_link_libraries(sherpa_human_model_plugin sherpa_gazebo_msgs ${Boost_LIBRARIES} ${GAZEBO_LIBRARIES} ${OGRE_LIBRARIES} ${GAZEBO_ODE_INCLUDE_LIBRARIES})
add_dependencies(sherpa_human_model_plugin sherpa_gazebo_msgs)

rosbuild_add_library(sherpa_human_gui_plugin SHARED src/SherpaGuiPlugin.cpp)
target_link_libraries(sherpa_human_gui_plugin sherpa_gazebo_msgs ${Boost_LIBRARIES} ${GAZEBO_LIBRARIES} ${OGRE_LIBRARIES} ${GAZEBO_ODE_INCLUDE_LIBRARIES})
add_dependencies(sherpa_human_gui_plugin sherpa_gazebo_msgs)



rosbuild_add_library(sherpa_factory_plugin SHARED src/ActorFactoryPluginJoints.cpp)
target_link_libraries(sherpa_factory_plugin ${Boost_LIBRARIES} ${GAZEBO_LIBRARIES} ${OGRE_LIBRARIES} ${GAZEBO_ODE_INCLUDE_LIBRARIES})
