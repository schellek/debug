target_sources(${EXECUTABLE}
  PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/Src/debugio.cpp
  ${CMAKE_CURRENT_LIST_DIR}/Src/ostream.cpp
  ${CMAKE_CURRENT_LIST_DIR}/Src/string_conv.cpp
)

target_include_directories(${EXECUTABLE}
  PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/Inc
)