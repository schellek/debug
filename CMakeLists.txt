cmake_minimum_required(VERSION 3.17)
project(FMT C CXX)

set(FMT_LIB fmt)

add_library(${FMT_LIB} STATIC)

target_compile_features(${FMT_LIB} PUBLIC cxx_std_17)

target_sources(${FMT_LIB}
  PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/Src/stdout.cpp
  ${CMAKE_CURRENT_LIST_DIR}/Src/ostream.cpp
  ${CMAKE_CURRENT_LIST_DIR}/Src/format_options.cpp
  ${CMAKE_CURRENT_LIST_DIR}/Src/formatted_writer.cpp
  ${CMAKE_CURRENT_LIST_DIR}/Src/string_conv_int.cpp
  ${CMAKE_CURRENT_LIST_DIR}/Src/string_conv_float.cpp
)

target_include_directories(${FMT_LIB}
  PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/Inc/private
  ${CMAKE_CURRENT_LIST_DIR}/Inc/public
)

if(DEFINED FMT_ENDL)
  target_compile_definitions(${FMT_LIB}
    PUBLIC
    "FMT_ENDL=\"${FMT_ENDL}\""
  )
endif()
