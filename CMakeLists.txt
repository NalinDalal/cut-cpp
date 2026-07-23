cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    cut-cpp
    VERSION 0.1.0
    DESCRIPTION "Short description"
    HOMEPAGE_URL "https://example.com/"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

add_library(
    cut-cpp_cut-cpp
    source/cut-cpp.cpp
    source/cut.cpp
)
add_library(cut-cpp::cut-cpp ALIAS cut-cpp_cut-cpp)

include(GenerateExportHeader)
generate_export_header(
    cut-cpp_cut-cpp
    BASE_NAME cut-cpp
    EXPORT_FILE_NAME export/cut-cpp/cut-cpp_export.hpp
    CUSTOM_CONTENT_FROM_VARIABLE pragma_suppress_c4251
)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(cut-cpp_cut-cpp PUBLIC CUT_CPP_STATIC_DEFINE)
endif()

set_target_properties(
    cut-cpp_cut-cpp PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"
    EXPORT_NAME cut-cpp
    OUTPUT_NAME cut-cpp
)

target_include_directories(
    cut-cpp_cut-cpp ${warning_guard}
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_include_directories(
    cut-cpp_cut-cpp SYSTEM
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/export>"
)

target_compile_features(cut-cpp_cut-cpp PUBLIC cxx_std_20)

# ---- Declare executable ----

add_executable(
    cut-cpp
    source/main.cpp
)
target_link_libraries(cut-cpp PRIVATE cut-cpp::cut-cpp)
set_target_properties(cut-cpp PROPERTIES OUTPUT_NAME cut-cpp)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Examples ----

if(PROJECT_IS_TOP_LEVEL)
  option(BUILD_EXAMPLES "Build examples tree." "${cut-cpp_DEVELOPER_MODE}")
  if(BUILD_EXAMPLES)
    add_subdirectory(example)
  endif()
endif()

# ---- Developer mode ----

if(NOT cut-cpp_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of cut-cpp"
  )
endif()

include(cmake/dev-mode.cmake)
