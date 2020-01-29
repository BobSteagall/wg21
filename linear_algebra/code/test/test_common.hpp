#ifndef LA_TEST_COMMON_HPP_DEFINED
#define LA_TEST_COMMON_HPP_DEFINED

#include <iostream>
#include <iomanip>
#include <string_view>

#include <gtest/gtest.h>

#include "linear_algebra.hpp"
#include "test_new_number.hpp"
#include "test_new_engine.hpp"
#include "test_new_arithmetic.hpp"

using cx_float  = std::complex<float>;
using cx_double = std::complex<double>;

using std::cout;
using std::endl;
using STD_LA::get_type_name;

struct dummy_type {};

#if 1
    #undef PRINT_TYPE
    #define PRINT_TYPE(...)
#endif

#endif  //- LA_TEST_COMMON_HPP_DEFINED
