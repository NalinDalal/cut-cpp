cmake_minimum_required(VERSION 3.14)

project(cut-cppTests LANGUAGES CXX)

include(../cmake/project-is-top-level.cmake)
include(../cmake/folders.cmake)

# ---- Dependencies ----

if(PROJECT_IS_TOP_LEVEL)
  find_package(cut-cpp REQUIRED)
  enable_testing()
endif()

# ---- Tests ----

add_executable(cut-cpp_test source/cut-cpp_test.cpp)
target_link_libraries(cut-cpp_test PRIVATE cut-cpp::cut-cpp)
target_compile_features(cut-cpp_test PRIVATE cxx_std_20)

add_test(NAME cut-cpp_test COMMAND cut-cpp_test)

# ---- End-of-file commands ----

add_folders(Test)
