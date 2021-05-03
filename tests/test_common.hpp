#ifndef LA_TEST_COMMON_HPP_DEFINED
#define LA_TEST_COMMON_HPP_DEFINED

#include <iostream>
#include <iomanip>
#include <string_view>

#include <gtest/gtest.h>

#include "matrix.hpp"
#include "test_new_number.hpp"
#include "test_new_engine.hpp"
#include "test_new_arithmetic.hpp"

using cx_float  = std::complex<float>;
using cx_double = std::complex<double>;

using std::cout;
using std::endl;

using namespace STD_LA;
//using STD_LA::detail::v_cmp_eq;
//using STD_LA::detail::m_cmp_eq;

struct dummy_type {};

#ifndef ENABLE_TEST_PRINTING
    #undef  PRINT_TYPE
    #define PRINT_TYPE(...)
    #undef  PRINT_FNAME
    #define PRINT_FNAME()
#endif

#if 0
using index_tuple_type = std::experimental::extents<std::experimental::dynamic_extent,
                                               std::experimental::dynamic_extent>;

namespace std::experimental
{
    inline bool
    operator >=(index_tuple_type lhs, index_tuple_type rhs)
    {
        return lhs.extent(0) >= rhs.extent(0)  &&  lhs.extent(1) >= rhs.extent(1);
    }
}
#else
using index_tuple_type = std::tuple<ptrdiff_t, ptrdiff_t>;

namespace STD_LA
{
    inline bool
    operator >=(index_tuple_type lhs, index_tuple_type rhs)
    {
        return !(lhs < rhs);
    }
}
#endif
using itup_t = index_tuple_type;


using vi_init_list = std::initializer_list<int>;
using vf_init_list = std::initializer_list<float>;

#define LST_4_0     {  0,   0,   0,   0}
#define LST_4_1     {  1,   2,   3,   4}
#define LST_4_2     { 11,  12,  13,  14}
#define LST_4_3     {101, 202, 303, 404}

#define LST_5_2     {  1,   2,   3,   4,  5}

static const vi_init_list   il_4_0 = LST_4_0;
static const vf_init_list   fl_4_0 = LST_4_0;

static const vi_init_list   il_4_1 = LST_4_1;
static const vf_init_list   fl_4_1 = LST_4_1;

static const vi_init_list   il_4_2 = LST_4_2;
static const vf_init_list   fl_4_2 = LST_4_2;

static const vi_init_list   il_4_3 = LST_4_3;
static const vf_init_list   fl_4_3 = LST_4_3;


inline constexpr index_tuple_type     st_33(3, 3);
inline constexpr index_tuple_type     st_34(3, 4);
inline constexpr index_tuple_type     st_43(4, 3);
inline constexpr index_tuple_type     st_44(4, 4);
inline constexpr index_tuple_type     st_55(5, 5);

#define LST_14_0       {{0, 0, 0, 0}}
#define LST_14_1       {{1, 2, 3, 4}}
#define LST_14_2       {{11, 12, 13, 14}}
#define LST_15_2       {{1, 2, 3, 4, 5}}

#define LST_41_0       {{0}, {0}, {0}, {0}}
#define LST_41_1       {{1}, {2}, {3}, {4}}
#define LST_41_2       {{11}, {12}, {13}, {14}}
#define LST_51_2       {{1}, {2}, {3}, {4}, {5}}

#define LST_33_0       {{ 0, 0, 0 },        \
                        { 0, 0, 0 },        \
                        { 0, 0, 0 }}

#define LST_33_1       {{ 10, 20, 30 },     \
                        { 40, 50, 60 },     \
                        { 70, 80, 90 }}

#define LST_33_1_T     {{ 10, 40, 70 },     \
                        { 20, 50, 80 },     \
                        { 30, 60, 90 }}

#define LST_33_2       {{ 110, 120, 130 },  \
                        { 210, 220, 230 },  \
                        { 310, 320, 330 }}

#define LST_33_3       {{ 11, 12, 13 },     \
                        { 21, 22, 23 },     \
                        { 31, 32, 33 }}

#define LST_33_4       {{ 1, 2, 3 },        \
                        { 2, 3, 4 },        \
                        { 3, 4, 5 }}

#define LST_33_5       {{ 1, -2, 3 },       \
                        { 2, -3, -4 },      \
                        { -3, 4, 5 }}

#define LST_34_0       {{ 0, 0, 0, 0 },     \
                        { 0, 0, 0, 0 },     \
                        { 0, 0, 0, 0 }}

#define LST_34_1       {{ 11, 12, 13, 14 }, \
                        { 21, 22, 23, 24 }, \
                        { 31, 32, 33, 34 }}

#define LST_34_1_N     {{ -11, -12, -13, -14 }, \
                        { -21, -22, -23, -24 }, \
                        { -31, -32, -33, -34 }}

#define LST_34_1_T     {{ 11, 21, 31 },         \
                        { 12, 22, 32 },         \
                        { 13, 23, 33 },         \
                        { 14, 24, 34 }}

#define LST_34_1_T_N   {{ -11, -21, -31 },      \
                        { -12, -22, -32 },      \
                        { -13, -23, -33 },      \
                        { -14, -24, -34 }}

#define LST_34_2       {{ 110, 120, 130, 140 }, \
                        { 210, 220, 230, 240 }, \
                        { 310, 320, 330, 340 }}

#define LST_34_2_N     {{ -110, -120, -130, -140 }, \
                        { -210, -220, -230, -240 }, \
                        { -310, -320, -330, -340 }}

#define LST_34_2_T     {{ 110, 210, 310 },      \
                        { 120, 220, 320 },      \
                        { 130, 230, 330 },      \
                        { 140, 240, 340 }}

#define LST_34_2_T_N   {{ -110, -210, -310 },   \
                        { -120, -220, -320 },   \
                        { -130, -230, -330 },   \
                        { -140, -240, -340 }}

#define LST_43_1       {{  1,  2,  3 },         \
                        {  5,  6,  7 },         \
                        {  1,  3,  2 },         \
                        {  0,  2,  1 }}

#define LST_44_0       {{ 0, 0, 0, 0 },         \
                        { 0, 0, 0, 0 },         \
                        { 0, 0, 0, 0 },         \
                        { 0, 0, 0, 0 }}

#define LST_44_1       {{  1,  2,  3,  4 },     \
                        {  5,  6,  7,  8 },     \
                        {  9, 10, 11, 12 },     \
                        { 13, 14, 15, 16 }}

#define LST_44_2       {{ 11, 12, 13, 14 },     \
                        { 21, 22, 23, 24 },     \
                        { 31, 32, 33, 34 },     \
                        { 41, 42, 43, 44 }}

#define LST_44_2_CR1   {{ 11, 12 },             \
                        { 21, 22 },             \
                        { 31, 32 },             \
                        { 41, 42 }}

#define LST_44_2_CR2   {{ 11, 12,  0,  0 },     \
                        { 21, 22,  0,  0 },     \
                        { 31, 32,  0,  0 },     \
                        { 41, 42,  0,  0 }}

#define LST_44_2_CR3   {{ 11, 12, 13, 14,  0,  0 }, \
                        { 21, 22, 23, 24,  0,  0 }, \
                        { 31, 32, 33, 34,  0,  0 }, \
                        { 41, 42, 43, 44,  0,  0 }}

#define LST_44_2_RR1   {{ 11, 12, 13, 14 },     \
                        { 21, 22, 23, 24 }}

#define LST_44_2_RR2   {{ 11, 12, 13, 14 },     \
                        { 21, 22, 23, 24 },     \
                        {  0,  0,  0,  0 },     \
                        {  0,  0,  0,  0 }}

#define LST_44_2_RR3   {{ 11, 12, 13, 14 },     \
                        { 21, 22, 23, 24 },     \
                        { 31, 32, 33, 34 },     \
                        { 41, 42, 43, 44 },     \
                        {  0,  0,  0,  0 },     \
                        {  0,  0,  0,  0 }}

#define LST_44_2CS     {{ 13, 12, 11, 14 },     \
                        { 23, 22, 21, 24 },     \
                        { 33, 32, 31, 34 },     \
                        { 43, 42, 41, 44 }}

#define LST_44_2RS     {{ 11, 12, 13, 14 },     \
                        { 31, 32, 33, 34 },     \
                        { 21, 22, 23, 24 },     \
                        { 41, 42, 43, 44 }}

#define LST_44_3       {{ 11, 12, 13,  0 },     \
                        { 21, 22, 23,  0 },     \
                        { 31, 32, 33,  0 },     \
                        {  0,  0,  0,  0 }}

#define LST_55_2       {{ 11, 12, 13, 14, 15 }, \
                        { 21, 22, 23, 24, 25 }, \
                        { 31, 32, 33, 34, 35 }, \
                        { 41, 42, 43, 44, 45 }, \
                        { 51, 52, 53, 54, 55 }}

#define LST_55_3       {{ 11, 12, 13,  0,  0 }, \
                        { 21, 22, 23,  0,  0 }, \
                        { 31, 32, 33,  0,  0 }, \
                        {  0,  0,  0,  0,  0 }, \
                        {  0,  0,  0,  0,  0 }}


using mi_init_list = std::initializer_list<std::initializer_list<int>>;
using mf_init_list = std::initializer_list<std::initializer_list<float>>;

static const mi_init_list   il_14_0 = LST_14_0;
static const mf_init_list   fl_14_0 = LST_14_0;
static const mi_init_list   il_14_1 = LST_14_1;
static const mf_init_list   fl_14_1 = LST_14_1;
static const mi_init_list   il_14_2 = LST_14_2;
static const mf_init_list   fl_14_2 = LST_14_2;
static const mf_init_list   fl_15_2 = LST_15_2;

static const mi_init_list   il_41_0 = LST_41_0;
static const mf_init_list   fl_41_0 = LST_41_0;
static const mi_init_list   il_41_1 = LST_41_1;
static const mf_init_list   fl_41_1 = LST_41_1;
static const mi_init_list   il_41_2 = LST_41_2;
static const mf_init_list   fl_41_2 = LST_41_2;
static const mf_init_list   fl_51_2 = LST_51_2;

static const mi_init_list   il_33_0 = LST_33_0;
static const mf_init_list   fl_33_0 = LST_33_0;

static const mi_init_list   il_33_1   = LST_33_1;
static const mf_init_list   fl_33_1   = LST_33_1;
static const mf_init_list   fl_33_1_t = LST_33_1_T;

static const mi_init_list   il_33_2 = LST_33_2;
static const mf_init_list   fl_33_2 = LST_33_2;

static const mf_init_list   fl_34_0     = LST_34_0;
static const mf_init_list   fl_34_1     = LST_34_1;
static const mf_init_list   fl_34_1_n   = LST_34_1_N;
static const mf_init_list   fl_34_1_t   = LST_34_1_T;
static const mf_init_list   fl_34_1_t_n = LST_34_1_T_N;

static const mf_init_list   fl_34_2     = LST_34_2;
static const mf_init_list   fl_34_2_n   = LST_34_2_N;
static const mf_init_list   fl_34_2_t   = LST_34_2_T;
static const mf_init_list   fl_34_2_t_n = LST_34_2_T_N;

static const mi_init_list   il_44_0 = LST_44_0;
static const mf_init_list   fl_44_0 = LST_44_0;

static const mi_init_list   il_44_1 = LST_44_1;
static const mf_init_list   fl_44_1 = LST_44_1;

static const mi_init_list   il_44_2 = LST_44_2;
static const mf_init_list   fl_44_2 = LST_44_2;

static const mi_init_list   il_55_2 = LST_55_2;
static const mf_init_list   fl_55_2 = LST_55_2;

static const mi_init_list   il_55_3 = LST_55_3;
static const mf_init_list   fl_55_3 = LST_55_3;

#endif  //- LA_TEST_COMMON_HPP_DEFINED
