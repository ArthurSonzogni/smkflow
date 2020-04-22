[smk]: https://github.com/ArthurSonzogni/smk
[smkflow]: https://github.com/ArthurSonzogni/smkflow
[logo]: ./logo.png

![logo][logo]

[![travis][badge.travis]][travis]
[![issues][badge.issues]][issues]
[![license][badge.license]][license]
[![contributors][badge.contributors]][contributors]

[badge.travis]: https://img.shields.io/travis/com/arthursonzogni/smkflow
[badge.issues]: https://img.shields.io/github/issues-raw/arthursonzogni/smkflow
[badge.license]: https://img.shields.io/github/license/arthursonzogni/smkflow?color=black
[badge.contributors]: https://img.shields.io/github/contributors/arthursonzogni/smkflow?color=blue

[travis]: https://travis-ci.com/ArthurSonzogni/smkflow
[issues]: https://github.com/ArthurSonzogni/smkflow/issues
[license]: http://opensource.org/licenses/MIT
[contributors]: https://github.com/ArthurSonzogni/smkflow/graphs/contributors

SMKFLOW is a node editor in C++ compatible with WebAssembly using [SMK][smk]

**Features**:
- Compatible with WebAssembly.
- The permissive MIT license.
- Use CMake FetchContent. No download is needed. You use the cmake snippet below
  to depends on smkflow.
  
**Warning**. This is an active project. Please do not expect the API to be stable for the next few months.

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
  GIT_TAG master # Please choose a fixed commit hash here.
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
