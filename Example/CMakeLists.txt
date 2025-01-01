cmake_minimum_required(VERSION 3.10)
project(Example NONE C CXX)

set(CMAKE_CXX_STANDARD 20)
set(FMT_DIR ${CMAKE_SOURCE_DIR}/..)

set(MAIN_TARGET "${CMAKE_PROJECT_NAME}")
add_executable(${MAIN_TARGET} main.cpp)

if (${CMAKE_BUILD_TYPE} MATCHES Debug)
  add_definitions(-DDEBUG)
endif()

include(${FMT_DIR}/setup.cmake)
fmt_setup(${MAIN_TARGET})
