cmake_minimum_required(VERSION 3.5)
project(la_test)

include(CTest)

##- This is the set of all headers comprising the Hades public API.
##
set(Includes
    linear_algebra.hpp
    matrix_element_promotions.hpp
    matrix_element_traits.hpp
    matrix_engines.hpp
    matrix_engine_traits.hpp
    matrix.hpp
    matrix_operation_traits.hpp
    matrix_operators.hpp
)

##- This is the set of all private source and header files used to build Hades.
##
set(Sources
    alt_test.cpp
)

include_directories(api_bob)

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

