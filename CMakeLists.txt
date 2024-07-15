cmake_minimum_required(VERSION 3.16)

if(NOT DEFINED PROJECT_NAME)
    set(LA_IS_PRIMARY_PROJECT ON)
endif()

project(wg21_linear_algebra VERSION 0.7.2)

include(CTest)
include(FetchContent)
include(GNUInstallDirs)
include(CMakeDependentOption)
include(CMakePackageConfigHelpers)

option(USE_EXTERNAL_MDSPAN "Use external copy of mdspan" OFF)

if(${USE_EXTERNAL_MDSPAN})
    find_package(mdspan CONFIG REQUIRED)
elseif(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo_multi.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo_multi.cmake)
    conan_basic_setup()
    find_package(mdspan CONFIG REQUIRED)
elseif(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
    find_package(mdspan CONFIG REQUIRED)
else()
    FetchContent_Declare(mdspan GIT_REPOSITORY https://github.com/kokkos/mdspan.git GIT_TAG stable) #5694f21c39f3b948d06a0c63b9c219bf802e28a8)
    FetchContent_MakeAvailable(mdspan)
endif()

option(LA_BUILD_USING_PCH "Build using precompiled headers" OFF)
option(LA_VERBOSE_TEST_OUTPUT "Write verbose test results" OFF)
option(LA_BUILD_PACKAGE "Build package files" ON)
option(LA_ENABLE_SANITIZERS "Enable Address Sanitizer and Undefined Behaviour Sanitizer if available" OFF)
cmake_dependent_option(LA_INSTALL "Install WG Linear Algebra library" ON "LA_IS_PRIMARY_PROJECT" OFF)
cmake_dependent_option(LA_ENABLE_TESTS "Enable std::math:linear_algebra tests" ON "BUILD_TESTING" OFF)
cmake_dependent_option(LA_BUILD_DEB "Create a DEB" ON "LA_BUILD_PACKAGE" OFF)
cmake_dependent_option(LA_BUILD_RPM "Create a RPM" ON "LA_BUILD_PACKAGE" OFF)

option(MDSPAN_ENABLE_CONCEPTS "" OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

add_library(wg21_linear_algebra INTERFACE)

target_include_directories(wg21_linear_algebra
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_sources(wg21_linear_algebra
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/matrix>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/arithmetic_operators.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/debug_helpers.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/engine_support.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/matrix.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/matrix_storage_engine.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/matrix_view_engine.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/mdspan_support.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/op_traits_addition.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/op_traits_division.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/op_traits_multiplication.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/op_traits_subtraction.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/op_traits_support.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/operation_traits.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/matrix>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/linear_algebra/arithmetic_operators.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/linear_algebra/debug_helpers.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/linear_algebra/engine_support.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/linear_algebra/matrix.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/linear_algebra/matrix_storage_engine.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/linear_algebra/matrix_view_engine.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/linear_algebra/mdspan_support.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/linear_algebra/opt_traits_addition.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/linear_algebra/opt_traits_division.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/linear_algebra/opt_traits_multiplication.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/linear_algebra/opt_traits_subtraction.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/linear_algebra/opt_traits_support.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/linear_algebra/operation_traits.hpp>
)

target_compile_features(wg21_linear_algebra
    INTERFACE
        cxx_std_20
)

#target_compile_definitions(wg21_linear_algebra
#    INTERFACE
#        LA_USE_MDSPAN
#)

target_link_libraries(wg21_linear_algebra
    INTERFACE
        std::mdspan
)

add_library(wg21_linear_algebra::wg21_linear_algebra ALIAS wg21_linear_algebra)

# message("VAR la_enable_tests is: " ${LA_ENABLE_TESTS})

if (${LA_ENABLE_TESTS})
    add_subdirectory(tests)
endif()

if (${LA_INSTALL})
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

    configure_package_config_file(
        ${CMAKE_CURRENT_LIST_DIR}/cmake/wg21_linear_algebra-config.cmake.in
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


    if (NOT LA_BUILD_PACKAGE)
        return()
    endif()

    list(APPEND source-generators TBZ2 TGZ TXZ ZIP)

    if (CMAKE_HOST_WIN32)
        list(APPEND binary-generators "WIX")
    endif()

    if (LA_PACKAGE_DEB AND CMAKE_HOST_LINUX)
        list(APPEND binary-generators "DEB")
    endif()

    if (LA_BUILD_RPM AND CMAKE_HOST_LINUX)
        list(APPEND binary-generators "RPM")
    endif()


    set(CPACK_SOURCE_GENERATOR ${source-generators})
    set(CPACK_GENERATOR ${binary-generators})

    set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}")
    set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CPACK_PACKAGE_FILE_NAME}")

    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Bob Steagall")

    list(APPEND CPACK_SOURCE_IGNORE_FILES /.git/ /build*/ .gitignore .DS_Store)

    include(CPack)
endif()
