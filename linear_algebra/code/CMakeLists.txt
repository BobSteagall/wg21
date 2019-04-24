cmake_minimum_required(VERSION 3.5)
project(wg21_linear_algebra)

include(CTest)

set(Includes
    include/linear_algebra.hpp
    include/linear_algebra/addition_traits.hpp
    include/linear_algebra/addition_traits_impl.hpp
    include/linear_algebra/arithmetic_operators.hpp
    include/linear_algebra/arithmetic_traits.hpp
    include/linear_algebra/debug_helpers.hpp
    include/linear_algebra/dynamic_engines.hpp
    include/linear_algebra/engine_traits.hpp
    include/linear_algebra/fixed_size_engines.hpp
    include/linear_algebra/forward_declarations.hpp
    include/linear_algebra/library_aliases.hpp
    include/linear_algebra/matrix.hpp
    include/linear_algebra/multiplication_traits.hpp
    include/linear_algebra/multiplication_traits_impl.hpp
    include/linear_algebra/negation_traits.hpp
    include/linear_algebra/negation_traits_impl.hpp
    include/linear_algebra/number_traits.hpp
    include/linear_algebra/operation_traits.hpp
    include/linear_algebra/subtraction_traits.hpp
    include/linear_algebra/subtraction_traits_impl.hpp
    include/linear_algebra/vector.hpp
    include/linear_algebra/view_based_engines.hpp

    test/test_new_arithmetic.hpp
    test/test_new_engine.hpp
    test/test_new_number.hpp
)

set(Sources
    test/test_obj_matrix.cpp
    test/test_op_add.cpp
    test/test_op_mul.cpp
    test/test_op_neg.cpp
    test/test_op_sub.cpp
#    test/test_01.cpp
#    test/test_02.cpp
    test/test_main.cpp
)

include_directories(include test .)

add_executable(la_test ${Sources} ${Includes})

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
    set(CMAKE_CXX_FLAGS_DEBUG "-g -std=c++17 -Wall -pedantic -Wextra -Wno-unused-parameter -Wno-unused-function")

    set(CMAKE_C_FLAGS_RELEASE   "-O3 -march=westmere -Wall -pedantic -Wextra")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=westmere -std=c++17 -stdlib=libc++ -Wall -pedantic -Wextra -Wno-unused-parameter -Wno-unused-function")

elseif(CMAKE_CXX_COMPILER STREQUAL g++)

    set(CMAKE_C_FLAGS_DEBUG   "-g -Wall -pedantic")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -std=c++17 -Wall -pedantic -Wextra  -Wno-unused-function -Wno-unused-variable -W -Wno-unused-but-set-variable -fmax-errors=10")

    set(CMAKE_C_FLAGS_RELEASE   "-O3 -march=westmere -Wall -pedantic")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=westmere -std=c++17 -Wall -pedantic -Wextra -Wno-unused-function -Wno-unused-variable -W -Wno-unused-but-set-variable -fmax-errors=10")

endif()
