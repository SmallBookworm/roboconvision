cmake_minimum_required(VERSION 3.8)
project(roboconVision)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)

set(SOURCE_FILES main.cpp serial.cpp signal_change.cpp
        LineTracker.cpp ball_yn.cpp LineFinder.cpp)

add_executable(roboconVision ${SOURCE_FILES})

target_link_libraries(roboconVision ${OpenCV_LIBS})