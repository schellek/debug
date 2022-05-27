target_sources(${EXECUTABLE}
  PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/Src/stdout.cpp
  ${CMAKE_CURRENT_LIST_DIR}/Src/ostream.cpp
)

target_include_directories(${EXECUTABLE}
  PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/Inc
)
