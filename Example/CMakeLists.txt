project(Example NONE C CXX) 
cmake_minimum_required(VERSION 3.11)
set(CMAKE_CXX_STANDARD 17)

set(EXECUTABLE "${CMAKE_PROJECT_NAME}")
add_executable(${EXECUTABLE} main.cpp)

IF (${CMAKE_BUILD_TYPE} MATCHES Debug)
  add_definitions(-DDEBUG -g)
ENDIF()

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/.. debug)