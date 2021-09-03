project(test_package LANGUAGES CXX)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_VERBOSE_MAKEFILE TRUE)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

find_package(wg21_linear_algebra REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

add_executable(test_linear_algebra
    test_package.cpp
)

target_compile_options(test_linear_algebra
    PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:GNU>>:-fconcepts -Wall -pedantic -Wextra  -Wno-unused-function -Wno-unused-variable -W -Wno-unused-but-set-variable -Wno-unused-local-typedefs -fmax-errors=10>
        $<$<OR:$<CXX_COMPILER_ID:Clang>>:-stdlib=libc++ -Wall -pedantic -Wextra -Wno-unused-parameter -Wno-unused-function -Wno-unused-local-typedefs>
        $<$<OR:$<CXX_COMPILER_ID:MSVC>>:/permissive->
        $<$<OR:$<CXX_COMPILER_ID:AppleClang>>:-Wall -pedantic -Wextra -Wno-unused-parameter -Wno-unused-function -Wno-unused-local-typedefs>
)

target_compile_options(test_linear_algebra
    PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:Clang>>:-stdlib=libc++>
)

target_link_libraries(test_linear_algebra PRIVATE wg21_linear_algebra::wg21_linear_algebra)

set_target_properties(test_linear_algebra
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

enable_testing()
add_test(
        NAME test_linear_algebra
        COMMAND test_linear_algebra
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

