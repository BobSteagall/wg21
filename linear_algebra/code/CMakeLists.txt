cmake_minimum_required(VERSION 3.14)
project(wg21_linear_algebra VERSION 0.0.1)

set(CMAKE_VERBOSE_MAKEFILE ON)

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
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/column_engine.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/debug_helpers.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/dynamic_vector_engine.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/dynamic_matrix_engine.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/fixed_size_vector_engine.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/fixed_size_matrix_engine.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/forward_declarations.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/library_aliases.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/matrix.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/multiplication_traits.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/multiplication_traits_impl.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/negation_traits.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/negation_traits_impl.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/operation_traits.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/private_support.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/public_support.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/row_engine.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/submatrix_engine.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/subvector_engine.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/subtraction_traits.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/subtraction_traits_impl.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/transpose_engine.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/vector.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra/addition_traits.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra/addition_traits_impl.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra/arithmetic_operators.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra/column_engine.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra/debug_helpers.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra/dynamic_vector_engine.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra/dynamic_matrix_engine.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra/fixed_size_vector_engine.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra/fixed_size_matrix_engine.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra/forward_declarations.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra/library_aliases.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra/matrix.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra/multiplication_traits.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra/multiplication_traits_impl.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra/negation_traits.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra/negation_traits_impl.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra/operation_traits.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra/private_support.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra/public_support.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra/row_engine.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra/subvector_engine.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra/submatrix_engine.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra/subtraction_traits.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra/subtraction_traits_impl.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra/transpose_engine.hpp>
        $<INSTALL_INTERFACE:include/linear_algebra/vector.hpp>
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
    option(gtest_force_shared_crt "" ON)
    if(NOT gtest_POPULATED)
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
            test/test_new_arithmetic.hpp
            test/test_new_engine.hpp
            test/test_new_number.hpp
            test/test_obj_matrix.cpp
            test/test_op_add.cpp
            test/test_op_mul.cpp
            test/test_op_neg.cpp
            test/test_op_sub.cpp
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
            $<$<OR:$<CXX_COMPILER_ID:GNU>>:-Wall -pedantic -Wextra  -Wno-unused-function -Wno-unused-variable -W -Wno-unused-but-set-variable -fmax-errors=10>
            $<$<OR:$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:Clang>>:-Wall -pedantic -Wextra -Wno-unused-parameter -Wno-unused-function>
    )

endif(${BUILD_TESTING})

# This should be passed in via the commmand line ("cmake --build ./ -v" or "cmake --build ./ -DCMAKE_VERBOSE_MAKEFILE=1")
#set(CMAKE_VERBOSE_MAKEFILE 1)

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
    #FILE wg21_linear_algebra-target.cmake
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
