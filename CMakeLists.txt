cmake_minimum_required(VERSION 3.21)
project(OS)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(OS lab10_1.cpp lab10_2.cpp)
