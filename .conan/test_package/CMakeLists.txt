project(test_package LANGUAGES CXX)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_VERBOSE_MAKEFILE TRUE)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

find_package(wg21_linear_algebra REQUIRED)

add_executable(test_linear_algebra
    test_package.cpp
)

target_link_libraries(test_linear_algebra PRIVATE wg21_linear_algebra::wg21_linear_algebra)
