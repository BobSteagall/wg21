cmake_minimum_required(VERSION 3.5)
project(wg21_linear_algebra)

include(CTest)

set(Includes
    linear_algebra.hpp
    linear_algebra_fwd.hpp
    matrix.hpp
    matrix_arithmetic_traits.hpp
    matrix_element_traits.hpp
    matrix_engine_traits.hpp
    matrix_engines.hpp
    matrix_operator_traits.hpp
    matrix_operators.hpp
    matrix_test_impl.hpp
)

set(Sources
    interface_test_01.cpp
    # interface_test_02.cpp
)

include_directories(.)

add_executable(la_test ${Sources})

set(CMAKE_VERBOSE_MAKEFILE 1)

if(CXX_COMPILER STREQUAL clang++)
    set(CMAKE_C_COMPILER   clang)
    set(CMAKE_CXX_COMPILER clang++)
else()
    set(CMAKE_C_COMPILER   gcc)
    set(CMAKE_CXX_COMPILER g++)
endif()

if(CMAKE_CXX_COMPILER STREQUAL clang++)

    set(CMAKE_C_FLAGS_DEBUG   "-g -Wall -pedantic -Wextra")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -std=c++17 -Wall -pedantic -Wextra -Wno-unused-parameter")

    set(CMAKE_C_FLAGS_RELEASE   "-O3 -march=westmere -Wall -pedantic -Wextra")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=westmere -std=c++17 -stdlib=libc++ -Wall -pedantic -Wextra -Wno-unused-parameter")

elseif(CMAKE_CXX_COMPILER STREQUAL g++)

    set(CMAKE_C_FLAGS_DEBUG   "-g -Wall -pedantic")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -std=c++17 -Wall -pedantic -Wextra -Wno-unused-variable -W -Wno-unused-but-set-variable")

    set(CMAKE_C_FLAGS_RELEASE   "-O3 -march=westmere -Wall -pedantic")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=westmere -std=c++17 -Wall -pedantic -Wextra -Wno-unused-variable -W -Wno-unused-but-set-variable")

endif()

