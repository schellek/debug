project(Example NONE C CXX)
cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)

set(MAIN_TARGET "${CMAKE_PROJECT_NAME}")
add_executable(${MAIN_TARGET} main.cpp)

if (${CMAKE_BUILD_TYPE} MATCHES Debug)
  add_definitions(-DDEBUG)
endif()

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/.. fmt)
fmt_include_directories(${MAIN_TARGET})
fmt_compile_definitions(${MAIN_TARGET})
target_link_libraries(${MAIN_TARGET} fmt)
