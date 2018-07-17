cmake_minimum_required(VERSION 2.8)
project(trafficLight)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(OpenCV_DIR /home/ubuntu/opencv-2.4.13.5/build)
set(OpenCV_FOUND 1)
find_package(OpenCV REQUIRED)

SET(CMAKE_BUILD_TYPE "Debug")  
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")  
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall") 

#include_directories(${OpenCV_INCLUDE_DIRS} )


include_directories(${PROJECT_SOURCE_DIR}/..)

add_executable(trafficLightDetection
    #trafficLightDetection.cpp
    #lightSaver.cpp
    pylonCameraTest.cpp
    trafficLight.cpp
	FrameLoad.cpp
    receiver.cpp
)

target_link_libraries(trafficLightDetection
    lcm
    pthread
	${OpenCV_LIBS}
)
