#ifndef LA_TEST_COMMON_HPP_DEFINED
#define LA_TEST_COMMON_HPP_DEFINED

#if __has_include(<version>)
    #include <version>      //- Included first due to possible libc++ bug on Xcode 11
#endif

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

using size_tuple = std::tuple<size_t, size_t>;

inline constexpr size_tuple     st_33(3, 3);
inline constexpr size_tuple     st_44(4, 4);
inline constexpr size_tuple     st_55(5, 5);

#define LST_33_0   {{0, 0, 0},          \
                    {0, 0, 0},          \
                    {0, 0, 0}}

#define LST_33_1   {{ 10, 20, 30},      \
                    { 40, 50, 60},      \
                    { 70, 80, 90}}

#define LST_33_2   {{ 110, 120, 130},   \
                    { 210, 220, 230},   \
                    { 310, 320, 330}}

#define LST_33_3   {{ 11, 12, 13},      \
                    { 21, 22, 23},      \
                    { 31, 32, 33}}

#define LST_44_0   {{0, 0, 0, 0},       \
                    {0, 0, 0, 0},       \
                    {0, 0, 0, 0},       \
                    {0, 0, 0, 0}}

#define LST_44_1   {{  1,  2,  3,  4},  \
                    {  5,  6,  7,  8},  \
                    {  9, 10, 11, 12},  \
                    { 13, 14, 15, 16}}

#define LST_44_2   {{ 11, 12, 13, 14},  \
                    { 21, 22, 23, 24},  \
                    { 31, 32, 33, 34},  \
                    { 41, 42, 43, 44}}

#define LST_44_2CS {{ 13, 12, 11, 14},  \
                    { 23, 22, 21, 24},  \
                    { 33, 32, 31, 34},  \
                    { 43, 42, 41, 44}}

#define LST_44_2RS {{ 11, 12, 13, 14},  \
                    { 31, 32, 33, 34},  \
                    { 21, 22, 23, 24},  \
                    { 41, 42, 43, 44}}

#define LST_44_2   {{ 11, 12, 13, 14},  \
                    { 21, 22, 23, 24},  \
                    { 31, 32, 33, 34},  \
                    { 41, 42, 43, 44}}

#define LST_44_3   {{ 11, 12, 13,  0},  \
                    { 21, 22, 23,  0},  \
                    { 31, 32, 33,  0},  \
                    {  0,  0,  0,  0}}

#define LST_55_3   {{ 11, 12, 13,  0,  0},  \
                    { 21, 22, 23,  0,  0},  \
                    { 31, 32, 33,  0,  0},  \
                    {  0,  0,  0,  0,  0},  \
                    {  0,  0,  0,  0,  0}}


using mi_init_list = std::initializer_list<std::initializer_list<int>>;
using mf_init_list = std::initializer_list<std::initializer_list<float>>;

static const mi_init_list   il_33_0 = LST_33_0;
static const mf_init_list   fl_33_0 = LST_33_0;

static const mi_init_list   il_33_1 = LST_33_1;
static const mf_init_list   fl_33_1 = LST_33_1;

static const mi_init_list   il_33_2 = LST_33_2;
static const mf_init_list   fl_33_2 = LST_33_2;

static const mi_init_list   il_44_0 = LST_44_0;
static const mf_init_list   fl_44_0 = LST_44_0;

static const mi_init_list   il_44_1 = LST_44_1;
static const mf_init_list   fl_44_1 = LST_44_1;

static const mi_init_list   il_44_2 = LST_44_2;
static const mf_init_list   fl_44_2 = LST_44_2;


template<typename = void>
struct test_expr : public std::false_type {};

#define TEST_EXPR(X)    \
  template<> struct test_expr<void_t<decltype(X)>> : public std:true_type {}

#endif  //- LA_TEST_COMMON_HPP_DEFINED
