function(fmt_include_directories TARGET)
  target_include_directories(${TARGET}
    PUBLIC
    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/Inc
  )
endfunction()

function(fmt_compile_definitions TARGET)
  if(DEFINED FMT_ENDL)
    target_compile_definitions(${TARGET} PUBLIC "FMT_ENDL=\"${FMT_ENDL}\"")
  endif()
endfunction()

function(fmt_link TARGET)
  target_link_libraries(${TARGET} "${CMAKE_STATIC_LIBRARY_PREFIX}fmt${CMAKE_STATIC_LIBRARY_SUFFIX}")
endfunction()
