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
    include/linear_algebra/row_column_views.hpp
    include/linear_algebra/subtraction_traits.hpp
    include/linear_algebra/subtraction_traits_impl.hpp
    include/linear_algebra/transpose_views.hpp
    include/linear_algebra/vector.hpp

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

set_target_properties(la_test PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_compile_options(la_test
    PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:Clang>>:-Werror -Wall -Wextra -pedantic>
        $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CXX_COMPILER_ID:GNU>>:-Wno-unused-but-set-variable>
        $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:Clang>>>:-Wno-unused-function -Wno-unused-variable>
)

set(CMAKE_VERBOSE_MAKEFILE 1)
