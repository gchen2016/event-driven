# Copyright: (C) 2010 RobotCub Consortium
# Authors: Francesco Rea
# CopyPolicy: Released under the terms of the GNU GPL v2.0.
cmake_minimum_required(VERSION 2.6)

set(PROJECT_NAME "vFramer")
project(${PROJECT_NAME})


include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${YARP_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
    ${EVENTDRIVENLIBS_INCLUDE_DIRS}
)

# Search for source code.
file(GLOB folder_source src/vFramer.cpp src/vDraw.cpp src/main.cpp)
file(GLOB folder_header include/vFramer.h include/vDraw.h)
source_group("Source Files" FILES ${folder_source})
source_group("Header Files" FILES ${folder_header})


add_executable(${PROJECT_NAME} ${folder_source} ${folder_header})

target_link_libraries(${PROJECT_NAME} ${YARP_LIBRARIES} ${OpenCV_LIBRARIES} ${EVENTDRIVEN_LIBRARIES})

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

