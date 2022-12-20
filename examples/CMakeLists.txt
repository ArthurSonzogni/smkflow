add_subdirectory(assets)

if(EMSCRIPTEN)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/index.html
    ${CMAKE_CURRENT_BINARY_DIR}/index.html
    COPYONLY
  )

  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/run_webassembly.py
    ${CMAKE_CURRENT_BINARY_DIR}/run_webassembly.py
    COPYONLY
  )
endif()

function(add_example target input)
  set(ns_target smkflow_example_${target})
  add_executable(${ns_target} ${input})
  target_link_libraries(${ns_target} PRIVATE smkflow::smkflow fmt::fmt asset)
  set_target_properties(${ns_target} PROPERTIES CXX_STANDARD 11)
  set_target_properties(${ns_target} PROPERTIES OUTPUT_NAME ${target})

  # Detect emscripten is used.
  if(EMSCRIPTEN)
    # Release flags
    # Release flags
    set_property(TARGET ${ns_target} APPEND_STRING PROPERTY LINK_FLAGS
      " -sALLOW_MEMORY_GROWTH"
      " -sDEMANGLE_SUPPORT"
      " -s EXPORT_NAME='main' -s MODULARIZE=1"
    )

    # Allow some files to be fetched.
    file(GLOB_RECURSE files "./assets/*")
    foreach(file ${files})
      file(RELATIVE_PATH relative_file ${CMAKE_SOURCE_DIR} "${file}")
      set_property(TARGET ${ns_target} APPEND_STRING PROPERTY LINK_FLAGS " --preload-file \"${file}@/${relative_file}\"")
    endforeach()
  endif()
endfunction(add_example)

add_example(algebra algebra.cpp)
add_example(widget_gallery widget_gallery.cpp)
add_example(minimal minimal.cpp)
add_example(perftest perftest.cpp)
