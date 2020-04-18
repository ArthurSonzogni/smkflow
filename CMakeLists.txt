cmake_minimum_required (VERSION 3.11)

add_subdirectory(third_party)

project(smkflow
  LANGUAGES CXX
  VERSION 1.0.0
)

add_library(smkflow STATIC
  include/smkflow/Constants.hpp
  include/smkflow/Elements.hpp
  include/smkflow/Model.hpp
  include/smkflow/widget/Box.hpp
  include/smkflow/widget/Input.hpp
  include/smkflow/widget/Label.hpp
  include/smkflow/widget/Menu.hpp
  include/smkflow/widget/Slider.hpp
  include/smkflow/widget/Widget.hpp
  include/smkflow/CursorCapture.hpp

  src/smkflow/CursorCapture.cpp
  src/smkflow/BoardImpl.cpp
  src/smkflow/BoardImpl.hpp
  src/smkflow/ConnectorImpl.cpp
  src/smkflow/ConnectorImpl.hpp
  src/smkflow/NodeImpl.cpp
  src/smkflow/NodeImpl.hpp
  src/smkflow/SlotImpl.cpp
  src/smkflow/SlotImpl.hpp
  src/smkflow/widget/BoxImpl.cpp
  src/smkflow/widget/BoxImpl.hpp
  src/smkflow/widget/InputImpl.cpp
  src/smkflow/widget/LabelImpl.cpp
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

target_compile_options(smkflow
 PRIVATE
  -Wall
  -Werror
  -pedantic-errors
  -Wextra
)

target_link_libraries(smkflow PUBLIC smk::smk)
target_link_libraries(smkflow PRIVATE fmt::fmt)
set_target_properties(smkflow PROPERTIES CXX_STANDARD 17)

add_subdirectory(examples)
