cmake_minimum_required(VERSION 3.8)
project(traffic_transportain)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp data_analysis.cpp traffic.h data_read.cpp debug.cpp initialize.cpp)
add_executable(traffic_transportain ${SOURCE_FILES})