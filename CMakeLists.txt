cmake_minimum_required(VERSION 3.0.2)
project(stereo_dfm27uro135ml)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# configure build types and build c-flags
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif()

message(WARNING "CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")

set(WITH_TONEMAPPING_FLAG OFF)

if(WITH_TONEMAPPING_FLAG)
  message(WARNING "Build with Tonemapping")
  add_definitions( -DTONEMAPPING)
  set(WITH_TONEMAPPING cvl)
else()
  message(WARNING "Build WITHOUT Tonemapping")
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_subdirectory(tiscamera_interface)
find_package(Threads REQUIRED)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  # msg
  std_msgs
  sensor_msgs
)

catkin_package(
  INCLUDE_DIRS include

  CATKIN_DEPENDS 
  std_msgs
  sensor_msgs 
)

# include directories
include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

# libraries
link_libraries(
  ${catkin_LIBRARIES}
  tiscamera_interface::tiscamera_interface
  Threads::Threads
  ${WITH_TONEMAPPING}
  yaml-cpp
)

add_executable(ListDevices src/ListDevices.cpp)
target_link_libraries(ListDevices ${catkin_LIBRARIES})

add_executable(ListFormats src/ListFormats.cpp)
target_link_libraries(ListFormats ${catkin_LIBRARIES})

add_executable(ListProperties src/ListProperties.cpp)
target_link_libraries(ListProperties ${catkin_LIBRARIES})

add_executable(LiveStreamCam0 src/LiveStreamCam0.cpp)
target_link_libraries(LiveStreamCam0 ${catkin_LIBRARIES})

add_executable(LiveStreamCam1 src/LiveStreamCam1.cpp)
target_link_libraries(LiveStreamCam1 ${catkin_LIBRARIES})

add_executable(StereoImages src/StereoImages.cpp)
target_link_libraries(StereoImages ${catkin_LIBRARIES})