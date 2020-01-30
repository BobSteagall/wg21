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

using namespace STD_LA;
using STD_LA::detail::v_cmp_eq;
using STD_LA::detail::m_cmp_eq;

struct dummy_type {};

#if 1
    #undef PRINT_TYPE
    #define PRINT_TYPE(...)
#endif

template<typename = void>
struct test_expr : public std::false_type {};

#define TEST_EXPR(X)    \
  template<> struct test_expr<void_t<decltype(X)>> : public std:true_type {}

#endif  //- LA_TEST_COMMON_HPP_DEFINED
