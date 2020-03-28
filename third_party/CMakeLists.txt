include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED false)
FetchContent_Declare(smk
  GIT_REPOSITORY https://github.com/ArthurSonzogni/smk
  GIT_TAG 90ff3a630c06eef586c653a6ac20381c914a755e
)

FetchContent_GetProperties(smk)
if(NOT smk_POPULATED)
  FetchContent_Populate(smk)
  add_subdirectory(${smk_SOURCE_DIR} ${smk_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
