cmake_minimum_required(VERSION 2.8)
project(trafficLight)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(OpenCV_DIR /home/ubuntu/opencv-2.4.13.5/build)
set(OpenCV_FOUND 1)
find_package(OpenCV REQUIRED)

#include_directories(${OpenCV_INCLUDE_DIRS} )

add_executable(trafficLightDetection
	trafficLightDetection.cpp
	FrameLoad.cpp
)

target_link_libraries(trafficLightDetection
	pthread
	${OpenCV_LIBS}
)
