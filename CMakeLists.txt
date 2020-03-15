cmake_minimum_required (VERSION 3.11)

add_subdirectory(third_party)

project(smkflow
  LANGUAGES CXX
  VERSION 1.0.0
)

add_library(smkflow STATIC
  include/smkflow/Model.hpp
  include/smkflow/Board.hpp
  src/smkflow/BoardImpl.cpp
  src/smkflow/BoardImpl.hpp
  src/smkflow/Connector.cpp
  src/smkflow/Connector.hpp
  src/smkflow/Node.cpp
  src/smkflow/Node.hpp
  src/smkflow/Slot.cpp
  src/smkflow/Slot.hpp
)

target_include_directories(smkflow
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
    src/
)

target_compile_options(smkflow
 PRIVATE
  -Wall
  -Werror
  -pedantic-errors
  -Wextra
)

target_link_libraries(smkflow PUBLIC smk)
set_target_properties(smkflow PROPERTIES CXX_STANDARD 17)

add_subdirectory(examples)
