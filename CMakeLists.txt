cmake_minimum_required (VERSION 3.11)
add_subdirectory(third_party)

project(smkflow
  LANGUAGES CXX
  VERSION 1.0.0
)

option(SMKFLOW_BUILD_EXAMPLES "Set to ON to build examples" ON)
option(SMKFLOW_BUILD_TESTS "Set to ON to build tests" OFF)

add_library(smkflow STATIC
  include/smkflow/Constants.hpp
  include/smkflow/CursorCapture.hpp
  include/smkflow/Elements.hpp
  include/smkflow/Model.hpp
  include/smkflow/widget/Action.hpp
  include/smkflow/widget/Box.hpp
  include/smkflow/widget/Input.hpp
  include/smkflow/widget/Label.hpp
  include/smkflow/widget/Menu.hpp
  include/smkflow/widget/Slider.hpp
  include/smkflow/widget/Widget.hpp
  src/smkflow/BoardImpl.cpp
  src/smkflow/BoardImpl.hpp
  src/smkflow/ConnectorImpl.cpp
  src/smkflow/ConnectorImpl.hpp
  src/smkflow/CursorCapture.cpp
  src/smkflow/NodeImpl.cpp
  src/smkflow/NodeImpl.hpp
  src/smkflow/SlotImpl.cpp
  src/smkflow/SlotImpl.hpp
  src/smkflow/widget/BoxImpl.cpp
  src/smkflow/widget/BoxImpl.cpp
  src/smkflow/widget/InputImpl.cpp
  src/smkflow/widget/LabelImpl.cpp
  src/smkflow/widget/LabelImpl.hpp
  src/smkflow/widget/MenuEntryImpl.cpp
  src/smkflow/widget/MenuImpl.cpp
  src/smkflow/widget/SliderImpl.cpp
  src/smkflow/widget/Widget.cpp
)
add_library(smkflow::smkflow ALIAS smkflow)

target_include_directories(smkflow
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
    src/
)

# Add as many warning as possible:
if (MSVC)
  target_compile_options(smkflow PRIVATE "/wd4244")
  target_compile_options(smkflow PRIVATE "/wd4267")
  target_compile_options(smkflow PRIVATE "/wd4996")
  target_compile_options(smkflow PRIVATE "/wd4305")
else()
  target_compile_options(smkflow PRIVATE "-Wall")
  target_compile_options(smkflow PRIVATE "-Werror")
  target_compile_options(smkflow PRIVATE "-pedantic-errors")
  target_compile_options(smkflow PRIVATE "-Wextra")
  target_compile_options(smkflow PRIVATE "-Wno-sign-compare")
endif()

target_link_libraries(smkflow PUBLIC smk::smk)
target_link_libraries(smkflow PRIVATE fmt::fmt)
target_link_libraries(smkflow PUBLIC nlohmann_json::nlohmann_json)

set_target_properties(smkflow PROPERTIES CXX_STANDARD 17)

if(SMKFLOW_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

if (SMKFLOW_BUILD_TESTS AND ${CMAKE_VERSION} VERSION_GREATER "3.11.4") 
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

  set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
  include(FetchContent)

  FetchContent_Declare( googletest
    GIT_REPOSITORY "https://github.com/google/googletest"
    GIT_TAG        release-1.10.0
  )

  FetchContent_GetProperties(googletest)
  if(NOT googletest_POPULATED)
    message(STATUS "Fetching googletest...")
    FetchContent_Populate(googletest)
    message(STATUS "... done")
    add_subdirectory(
      ${googletest_SOURCE_DIR}
      ${googletest_BINARY_DIR}
      EXCLUDE_FROM_ALL)
  endif()

  add_executable(tests
    src/smkflow/test.hpp
    src/smkflow/serialization_test.cpp
  )

  target_link_libraries(tests
    PRIVATE smkflow::smkflow 
    PRIVATE gtest
    PRIVATE gmock
    PRIVATE gtest_main
    PRIVATE asset
  )

  set_property(TARGET tests PROPERTY CXX_STANDARD 17)
endif()
