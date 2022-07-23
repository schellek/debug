target_sources(${MAIN_TARGET}
  PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/Src/stdout.cpp
  ${CMAKE_CURRENT_LIST_DIR}/Src/ostream.cpp
  ${CMAKE_CURRENT_LIST_DIR}/Src/string_conv.cpp
)

target_include_directories(${MAIN_TARGET}
  PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/Inc
)

if(DEFINED FMT_ENDL)
  target_compile_definitions(${MAIN_TARGET} PRIVATE "FMT_ENDL=\"${FMT_ENDL}\"")
endif()
