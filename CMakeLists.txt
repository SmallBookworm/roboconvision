cmake_minimum_required(VERSION 3.5)
project(roboconVision)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)

set(SOURCE_FILES main.cpp serial.cpp
        LineTracker.cpp LineFinder.cpp Info.cpp Info.h
        ball_tracker.cpp cv-helpers.hpp ring_watcher.cpp deviationPosition.cpp
        lineTest.cpp lineInfo.cpp linesOption.cpp rtlFinder.cpp rtlFinder.h rtlInfo.cpp rtlInfo.h)

add_executable(roboconVision ${SOURCE_FILES})

target_link_libraries(roboconVision ${OpenCV_LIBS} realsense2 pthread)