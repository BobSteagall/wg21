cmake_minimum_required(VERSION 3.14)
project(wg21_linear_algebra VERSION 0.0.1)

set(CMAKE_VERBOSE_MAKEFILE ON)

option(ENABLE_SANITIZERS "Enable Address Sanitizer and Undefined Behaviour Sanitizer if available" OFF)

include(FetchContent)
FetchContent_Declare(mdspan GIT_REPOSITORY https://github.com/kokkos/mdspan.git)
FetchContent_GetProperties(mdspan)
option(MDSPAN_ENABLE_CONCEPTS "" OFF)
if(NOT mdspan_POPULATED)
    FetchContent_Populate(mdspan)
    add_subdirectory(${mdspan_SOURCE_DIR} ${mdspan_BINARY_DIR})
endif()

add_library(wg21_linear_algebra INTERFACE)

target_include_directories(wg21_linear_algebra
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_sources(wg21_linear_algebra
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/addition_traits.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/addition_traits_impl.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/arithmetic_operators.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/debug_helpers.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/dynamic_vector_engine.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/dynamic_matrix_engine.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/fixed_size_vector_engine.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/fixed_size_matrix_engine.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/forward_declarations.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/matrix.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/matrix_view_engine.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/multiplication_traits.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/multiplication_traits_impl.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/negation_traits.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/negation_traits_impl.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/operation_traits.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/private_support.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/public_support.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/subtraction_traits.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/subtraction_traits_impl.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/vector.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/vector_view_engine.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra/addition_traits.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra/addition_traits_impl.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra/arithmetic_operators.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra/debug_helpers.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra/dynamic_vector_engine.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra/dynamic_matrix_engine.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra/fixed_size_vector_engine.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra/fixed_size_matrix_engine.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra/forward_declarations.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra/matrix.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra/matrix_view_engine.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra/multiplication_traits.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra/multiplication_traits_impl.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra/negation_traits.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra/negation_traits_impl.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra/operation_traits.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra/private_support.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra/public_support.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra/subtraction_traits.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra/subtraction_traits_impl.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra/vector.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra/vector_view_engine.hpp>
)

target_compile_features(wg21_linear_algebra
    INTERFACE
        cxx_std_17
)

target_compile_definitions(wg21_linear_algebra
    INTERFACE
        LA_USE_MDSPAN
)

target_link_libraries(wg21_linear_algebra
    INTERFACE
        std::mdspan
)

include(CTest)
if (${BUILD_TESTING})

    FetchContent_Declare(gtest GIT_REPOSITORY https://github.com/google/googletest.git)
    FetchContent_GetProperties(gtest)
    option(BUILD_GMOCK "" OFF)
    option(INSTALL_GTEST "" OFF)
    option(gtest_force_shared_crt "" ON)
    if(NOT gtest_POPULATED)
        set(CMAKE_CXX_STANDARD 11)
        set(CMAKE_CXX_STANDARD_REQUIRED ON)
        FetchContent_Populate(gtest)
        add_subdirectory(${gtest_SOURCE_DIR} ${gtest_BINARY_DIR})
    endif()

    add_library(wg21_linear_algebra::wg21_linear_algebra ALIAS wg21_linear_algebra)
    add_executable(la_test "")

    target_include_directories(la_test
        PRIVATE
            test
    )

    target_sources(la_test
        PRIVATE
            test/test_common.hpp
            test/test_new_arithmetic.hpp
            test/test_new_engine.hpp
            test/test_new_number.hpp
            test/test_dynamic_engines.cpp
            test/test_fixed_size_engines.cpp
            test/test_obj_matrix.cpp
            test/test_op_add_traits.cpp
            test/test_op_mul_traits.cpp
            test/test_op_neg_traits.cpp
            test/test_op_sub_traits.cpp
     #       test/test_01.cpp
     #       test/test_02.cpp
            test/test_main.cpp
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
            $<$<OR:$<CXX_COMPILER_ID:GNU>>:-Wall -pedantic -Wextra  -Wno-unused-function -Wno-unused-variable -W -Wno-unused-but-set-variable -Wno-unused-local-typedefs -fmax-errors=10 ${ASAN_FLAGS}>
            $<$<OR:$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:Clang>>:-Wall -pedantic -Wextra -Wno-unused-parameter -Wno-unused-function -Wno-unused-local-typedefs ${ASAN_FLAGS}>
    )

    if (ENABLE_SANITIZERS)
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
    endif(ENABLE_SANITIZERS)

    include(CTest)
    enable_testing()
    include(GoogleTest)
    gtest_discover_tests(la_test WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} DISCOVERY_TIMEOUT 30)
endif(${BUILD_TESTING})

include(GNUInstallDirs)

# Hierarchically copy headers to the install dir
install (
    DIRECTORY
        "${CMAKE_CURRENT_SOURCE_DIR}/include/"
    DESTINATION
        ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN
        "*.hpp"
)

install(
    TARGETS wg21_linear_algebra
    DESTINATION lib/cmake/wg21_linear_algebra
    EXPORT wg21_linear_algebra-target
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
    EXPORT wg21_linear_algebra-target
    NAMESPACE wg21_linear_algebra::
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/wg21_linear_algebra"
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
    ${CMAKE_CURRENT_LIST_DIR}/wg21_linear_algebra-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/wg21_linear_algebra-config.cmake
    INSTALL_DESTINATION
        "${CMAKE_INSTALL_LIBDIR}/cmake/wg21_linear_algebra"
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/wg21_linear_algebra-version.cmake
    VERSION ${PROPAGATE_CONST_VERSION}
    COMPATIBILITY SameMajorVersion
    ARCH_INDEPENDENT
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/wg21_linear_algebra-config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/wg21_linear_algebra-version.cmake
    DESTINATION
        "${CMAKE_INSTALL_LIBDIR}/cmake/wg21_linear_algebra"
)
