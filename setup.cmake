set(FMT_DIR       ${CMAKE_CURRENT_LIST_DIR})
set(FMT_BUILD_DIR ${CMAKE_BINARY_DIR}/fmt)

include(ExternalProject)

if(${MSVC})
  set(FMT_BUILD_TYPE $<CONFIG>)
elseif(DEFINED DEBUG_FMT)
  set(FMT_BUILD_TYPE "Debug")
else()
  set(FMT_BUILD_TYPE "Release")
endif()

set(CMAKE_ARGS "-DCMAKE_BUILD_TYPE=${FMT_BUILD_TYPE}" "--no-warn-unused-cli")

foreach(CMAKE_VAR FMT_ENDL CMAKE_TOOLCHAIN_FILE CMAKE_C_STANDARD CMAKE_CXX_STANDARD CMAKE_EXPORT_COMPILE_COMMANDS ARM_GCC_PATH GCC_PATH LLVM_PATH)
  if (DEFINED ${CMAKE_VAR})
    list(APPEND CMAKE_ARGS "-D${CMAKE_VAR}=${${CMAKE_VAR}}")
  endif()
endforeach()

ExternalProject_Add(fmt
  PREFIX            fmt
  SOURCE_DIR        ${FMT_DIR}
  BINARY_DIR        ${FMT_BUILD_DIR}
  CMAKE_ARGS        ${CMAKE_ARGS}
  BUILD_COMMAND     ${CMAKE_COMMAND} --build <BINARY_DIR> --config ${FMT_BUILD_TYPE}
  INSTALL_COMMAND   ${CMAKE_COMMAND} -E make_directory <BINARY_DIR>/${FMT_BUILD_TYPE}
)

function(fmt_setup TARGET)
  add_dependencies(${TARGET} fmt)

  target_include_directories(${TARGET}
    PUBLIC
    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/Inc/public
  )

  if(DEFINED FMT_ENDL)
    target_compile_definitions(${TARGET}
      PUBLIC
      "FMT_ENDL=\"${FMT_ENDL}\""
    )
  endif()

  target_link_directories(${TARGET}
    PUBLIC
    ${FMT_BUILD_DIR}
    ${FMT_BUILD_DIR}/${FMT_BUILD_TYPE}
  )

  target_link_libraries(${TARGET}
    "${CMAKE_STATIC_LIBRARY_PREFIX}fmt${CMAKE_STATIC_LIBRARY_SUFFIX}"
  )
endfunction()
