[smk]: https://github.com/ArthurSonzogni/smk
[smkflow]: https://github.com/ArthurSonzogni/smkflow
[logo]: ./logo.png

![logo][logo]

SMKFLOW is a node editor in C++ compatible with WebAssembly using [SMK][smk]

**status**: experimental. Expect regular change in the API for the next few months.
If you need new features, please ask on Github.

**Features**:
- Compatible with WebAssembly.
- Use CMake FetchContent. You don't have to download anything. Just use the
  CMake snippet below:

Examples/Demo:
---------

Use the [./examples/](./examples/) directory. This produce the following
webassembly [demo](http://arthursonzogni.github.io/smkflow/examples/index.html)

API
---
The API consists of 4 files:

- [Model.hpp](./include/smkflow/Model.hpp) Let you define how your nodes will look like. Please try example:
[./examples/minimal.cpp](./examples/minimal.cpp).

- [Elements.hpp](./include/smkflow/Elements.hpp) Contains the public definition elements composing the view at runtime. You can use them to query/update the view. See [./examples/algebra.cpp](./examples/algebra.cpp) file.

- [Constants.hpp](./include/smkflow/Constants.hpp) Contains the default sizes and
colors the library is using for staying consistant. Feel free to fork and modify
this file to make smkflow suits your needs.

- [Widget/](./include/smkflow/widget). Every nodes can display some GUI inside. This directory contains the GUI public interface. You can also easily define your own components if needed.

CMake
-----
Include the following lines in your CMake and you are ready to go.
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
target_link_library(my_applcation PRIVATE smkflow::smkflow)
~~~

Want to contribute?
-------------------
Feel free to post issues, ask questions or submit any work to this repository.
