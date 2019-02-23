cmake_minimum_required(VERSION 3.1)
project(wg21_linear_algebra)

set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Set build type")
SET_PROPERTY(
  CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
  "Debug" "Release" "MinSizeRel" "RelWithDebInfo"
)
# If CMAKE_BUILD_TYPE is not already set...
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

include(CTest)

set(Includes
    linear_algebra.hpp
    matrix.hpp
    matrix_arithmetic_traits.hpp
    matrix_element_traits.hpp
    matrix_engine_traits.hpp
    matrix_engines.hpp
    matrix_fwd.hpp
    matrix_operator_traits.hpp
    matrix_operators.hpp
    matrix_test_impl.hpp
)

set(Sources
    test_01.cpp
    test_02.cpp
)

include_directories(.)

add_executable(la_test ${Sources})

set_target_properties(la_test PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED TRUE
  CXX_EXTENSIONS FALSE
)
target_compile_options(la_test 
  PUBLIC -Wall
  PUBLIC -Wextra
  PUBLIC -pedantic
  PUBLIC -march=westmere
  PUBLIC -Wno-unused-variable
  PUBLIC -Wno-unused-but-set-variable
  PUBLIC -Wno-unused-parameter
  PUBLIC -Wno-unused-function
)

set(CMAKE_VERBOSE_MAKEFILE 1)

#if(CMAKE_CXX_COMPILER MATCHES ".*clang\\+\\+.*")
# clang specific stuff
#elseif(CMAKE_CXX_COMPILER MATCHES ".*g\\+\\+.*")
# gcc specific stuff
#else()
# otherwise
#endif()

