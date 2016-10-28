cmake_minimum_required(VERSION 3.3)
project(triangles)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/")

SET(CMAKE_C_COMPILER mpicc)
SET(CMAKE_CXX_COMPILER mpicxx)

set(SOURCE_FILES main.cpp auxiliary.cpp auxiliary.h constants.h)
add_executable(triangles ${SOURCE_FILES})
add_definitions(-std=c++11)
