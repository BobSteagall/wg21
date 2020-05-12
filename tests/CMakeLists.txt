add_executable(la_test "")

target_precompile_headers(la_test
    PRIVATE
        <linear_algebra.hpp>
        <gtest/gtest.h>
        <cassert>
)

target_sources(la_test
    PRIVATE
        test_common.hpp
        test_new_arithmetic.hpp
        test_new_engine.hpp
        test_new_number.hpp
        test_dynamic_engines.cpp
        test_fixed_size_engines.cpp
        test_obj_matrix.cpp
        test_op_add_traits.cpp
        test_op_mul_traits.cpp
        test_op_neg_traits.cpp
        test_op_sub_traits.cpp
 #       test_01.cpp
 #       test_02.cpp
        test_main.cpp  
)

target_link_libraries(la_test
    PRIVATE
        wg21_linear_algebra
        gtest_main
        gtest
)

set_target_properties(la_test PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_compile_options(la_test
    PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:GNU>>:-Wall -pedantic -Wextra  -Wno-unused-function -Wno-unused-variable -W -Wno-unused-but-set-variable -Wno-unused-local-typedefs -fmax-errors=10>
        $<$<OR:$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:Clang>>:-Wall -pedantic -Wextra -Wno-unused-parameter -Wno-unused-function -Wno-unused-local-typedefs>
)


if (LA_ENABLE_SANITIZERS)
    set(SANITIZER_FLAGS_ASAN "-fsanitize=address -fno-omit-frame-pointer")
    set(SANITIZER_FLAGS_UBSAN "-fsanitize=undefined")

    include(CheckCXXCompilerFlag)
    check_cxx_compiler_flag("${SANITIZER_FLAGS_ASAN}" COMPILER_SUPPORTS_ASAN)
    check_cxx_compiler_flag("${SANITIZER_FLAGS_UBSAN}" COMPILER_SUPPORTS_UBSAN)

    if (COMPILER_SUPPORTS_ASAN)
        add_library(asan INTERFACE IMPORTED)
        set_target_properties(asan PROPERTIES
            INTERFACE_COMPILE_OPTIONS "${SANITIZER_FLAGS_ASAN}"
            INTERFACE_LINK_OPTIONS "${SANITIZER_FLAGS_ASAN}"
        )
        target_link_libraries(la_test
            PRIVATE
                asan
        )
    endif(COMPILER_SUPPORTS_ASAN)

    if (COMPILER_SUPPORTS_UBSAN)
        add_library(ubsan INTERFACE IMPORTED)
        set_target_properties(ubsan PROPERTIES
            INTERFACE_COMPILE_OPTIONS "${SANITIZER_FLAGS_UBSAN}"
            INTERFACE_LINK_OPTIONS "${SANITIZER_FLAGS_UBSAN}"
        )
        target_link_libraries(la_test
            PRIVATE
                ubsan
        )
    endif(COMPILER_SUPPORTS_UBSAN)

endif(LA_ENABLE_SANITIZERS)

enable_testing()
include(GoogleTest)
gtest_discover_tests(la_test WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} DISCOVERY_TIMEOUT 30)
