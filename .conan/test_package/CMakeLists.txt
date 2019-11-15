project(test_package LANGUAGES CXX)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_VERBOSE_MAKEFILE TRUE)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

find_package(wg21_linear_algebra REQUIRED)

add_executable(test_linear_algebra
        ../../linear_algebra/code/test/test_new_arithmetic.hpp
        ../../linear_algebra/code/test/test_new_engine.hpp
        ../../linear_algebra/code/test/test_new_number.hpp
        ../../linear_algebra/code/test/test_obj_matrix.cpp
        ../../linear_algebra/code/test/test_op_add.cpp
        ../../linear_algebra/code/test/test_op_mul.cpp
        ../../linear_algebra/code/test/test_op_neg.cpp
        ../../linear_algebra/code/test/test_op_sub.cpp
        #       test/test_01.cpp
        #       test/test_02.cpp
        ../../linear_algebra/code/test/test_main.cpp
)

target_link_libraries(test_linear_algebra PRIVATE wg21_linear_algebra::wg21_linear_algebra)
