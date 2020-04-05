smkflow
-------

A dataflow node editor in C++ compatible with WebAssembly using
[SMK](https://github.com/ArthurSonzogni/smk)

Features:
--------
- Compatible with WebAssembly.
- Use CMake FetchContent. You don't have to download anything. Just use the
  CMake snippet below:

Examples:
---------

Use the [./examples/](./examples/) directory. This produce the following
webassembly [demo](http://arthursonzogni.github.io/smkflow/examples/index.html)

API
---
The API consists of 4 files:

[Model.hpp](./include/smkflow/Model.hpp) Defines your own nodes. This is the
main file you need to look at for starting. Please try the
[./examples/minimal.cpp](./examples/minimal.cpp) file.

[Elements.hpp](./include/smkflow/Elements.hpp) Contains the public definition of the
elements composing the view. Use it to query the current state or to apply
modifications. See [./examples/algebra.cpp](./examples/algebra.cpp) file.

[Widget.hpp](./include/smkflow/Widget.hpp). Each node can contains arbitrary
widget. The base library contains InputBox and Sliders. You can also define
yours using this file.

[Constants.hpp](./include/smkflow/Constants.hpp) Contains the default sizes and
colors the library is using for staying consistant. Feel free to fork and modify
this file to make smkflow suits your needs.

CMake
-----
Include the following CMake:
~~~cmake
include(FetchContent)

FetchContent_Declare(smkflow
  GIT_REPOSITORY https://github.com/ArthurSonzogni/smkflow
  GIT_REPOSITORY git@github.com:ArthurSonzogni/smkflow
  # GIT_TAG to-be-choosen, either a commit or master
)

FetchContent_GetProperties(smkflow)
if(NOT smkflow_POPULATED)
  FetchContent_Populate(smkflow)
  add_subdirectory( ${smkflow_SOURCE_DIR} ${smkflow_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
~~~

Then link your application with smkflow:
~~~cmake
target_link_library(my_applcation smkflow::smkflow)
~~~

Want to contribute?
-------------------
Feel free to post issues, ask questions or submit any work to this repository.
