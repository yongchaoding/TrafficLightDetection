cmake_minimum_required(VERSION 2.8)
project(trafficLight)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(OpenCV_DIR /home/ubuntu/opencv-2.4.13.5/build)
set(OpenCV_FOUND 1)
find_package(OpenCV REQUIRED)

set(PYLON_ROOT "/opt/pylon5")
EXEC_PROGRAM("${PYLON_ROOT}/bin/pylon-config" ARGS --cflags OUTPUT_VARIABLE CPPFLAGS)
SET(CPPFLAGS "${CPPFLAGS}" CACHE STRING "CPPFLAGS")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CPPFLAGS}")

EXEC_PROGRAM("${PYLON_ROOT}/bin/pylon-config" ARGS --libs-rpath OUTPUT_VARIABLE LDFLAGS)
#SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LDFLAGS}" CACHE STRING "LDFLAGS")
SET(LDFLAGS "${LDFLAGS}" CACHE STRING "LDFLAGS")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LDFLAGS}")

EXEC_PROGRAM("${PYLON_ROOT}/bin/pylon-config" ARGS --libs OUTPUT_VARIABLE LDLIBS)
SET(LDLIBS "${LDLIBS}" CACHE STRING "LDLIBS")
#SET(CMAKE_BUILD_TYPE "Debug")  
#SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")  
#SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall") 

#include_directories(${OpenCV_INCLUDE_DIRS} )

include_directories(${PROJECT_SOURCE_DIR}/..)
include_directories(/opt/pylon5/include)
add_executable(trafficLightDetection
    #trafficLightDetection.cpp
    #lightSaver.cpp
    pylonCameraTest.cpp
    trafficLight.cpp
    FrameLoad.cpp
    receiver.cpp
    pylonCamera.cpp
)

#link_directories(trafficLightDetection /opt/pylon5/lib64)
##link_libraries("/opt/pylon5/lib64/libuxapi.so")
message(${OpenCV_DIR})
message(${OpenCV_LIBS})

target_link_libraries(trafficLightDetection
    lcm
    pthread
    ${OpenCV_LIBS}
    ${LDLIBS}
)
