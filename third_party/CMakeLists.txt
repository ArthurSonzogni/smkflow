include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED false)

# ------------------------------------------------------------------------------

FetchContent_Declare(smk
  GIT_REPOSITORY https://github.com/ArthurSonzogni/smk
  GIT_TAG 9e04a9c46af123a890244c4e2f5350a11150f25e
)

FetchContent_Declare(fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG 2cac8a9
)

FetchContent_Declare(json
  #GIT_REPOSITORY https://github.com/nlohmann/json
  GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
  GIT_TAG master)

# ------------------------------------------------------------------------------

FetchContent_GetProperties(smk)
FetchContent_GetProperties(fmt)
FetchContent_GetProperties(json)

if(NOT smk_POPULATED)
  message(STATUS "populate SMK > Start")
  FetchContent_Populate(smk)
  message(STATUS "populate SMK > Done")
  add_subdirectory(${smk_SOURCE_DIR} ${smk_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

if(NOT fmt_POPULATED)
  message(STATUS "populate fmt > Start")
  FetchContent_Populate(fmt)
  set(BUILD_SHARED_LIBS FALSE)
  message(STATUS "populate fmt > Done")
  add_subdirectory(
    ${fmt_SOURCE_DIR}
    ${fmt_BINARY_DIR}
    EXCLUDE_FROM_ALL)
endif()

FetchContent_Declare(json
  GIT_REPOSITORY https://github.com/nlohmann/json
  GIT_TAG v3.7.3)
FetchContent_GetProperties(json)
if(NOT json_POPULATED)
  message(STATUS "populate json > Start")
  FetchContent_Populate(json)
  message(STATUS "populate json > Done")
  add_subdirectory( ${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
