cmake_minimum_required(VERSION 3.11)

set(CMAKE_VERBOSE_MAKEFILE off)

project(Example NONE C CXX)

set(EXECUTABLE "${CMAKE_PROJECT_NAME}")
add_executable(${EXECUTABLE} main.cpp)
set_target_properties(${EXECUTABLE} PROPERTIES CXX_STANDARD 17)

IF (${CMAKE_BUILD_TYPE} MATCHES Debug)
  add_definitions(-DDEBUG -g)
ENDIF()

add_subdirectory(.. debug)