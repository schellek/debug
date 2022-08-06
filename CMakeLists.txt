project(FMT C CXX)
cmake_minimum_required(VERSION 3.13)

set(FMT_LIB fmt)

add_library(${FMT_LIB} STATIC)

set_property(TARGET ${FMT_LIB}
  PROPERTY CXX_STANDARD 17
)

target_sources(${FMT_LIB}
  PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/Src/stdout.cpp
  ${CMAKE_CURRENT_LIST_DIR}/Src/ostream.cpp
  ${CMAKE_CURRENT_LIST_DIR}/Src/format_options.cpp
  ${CMAKE_CURRENT_LIST_DIR}/Src/formatted_writer.cpp
  ${CMAKE_CURRENT_LIST_DIR}/Src/string_conv.cpp
)

target_include_directories(${FMT_LIB}
  PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/Inc
)

if(DEFINED FMT_ENDL)
  target_compile_definitions(${FMT_LIB} PRIVATE "FMT_ENDL=\"${FMT_ENDL}\"")
endif()

if(DEFINED MAIN_TARGET)
  target_link_libraries(${MAIN_TARGET} ${FMT_LIB})
  target_include_directories(${MAIN_TARGET} PUBLIC $<TARGET_PROPERTY:${FMT_LIB},INCLUDE_DIRECTORIES>)
  target_compile_definitions(${MAIN_TARGET} PUBLIC $<TARGET_PROPERTY:${FMT_LIB},COMPILE_DEFINITIONS>)
endif()
