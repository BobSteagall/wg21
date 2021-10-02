#define ENABLE_TEST_PRINTING
#include "test_common.hpp"

using namespace STD_LA;
using namespace STD_LA::detail;
using namespace MDSPAN_NS;

//- A helper macro to assist in readability of test functions below.
//
#define ASSERT_A_MUL_B_EQ_C(A, B, C)    \
    static_assert(std::is_same_v<decltype(std::declval<A>() * std::declval<B>()), C>)


//--------------------------------------------------------------------------------------------------
//  The following are several traits types used to exercise the element, engine, and operation
//  type detection meta-functions in the private implementation.  See test function t400() below.
//--------------------------------------------------------------------------------------------------
//- This traits type is used to verify that default operations are selected when they are not
//  declared in the operations traits type.
//
struct test_mul_op_traits_empty {};

//- This operation traits type is analogous to STD_LA::matrix_operation_traits, where its nested
//  traits types for element/engine/arithmetic are alias templates.
//
//  Suffix "_nta" means "nested type alias"
//
template<class OT, class T1, class T2>
struct test_element_mul_traits_nta
{
    using element_type = dummy_type;
};

template<class OT, class T1, class T2>
struct test_engine_mul_traits_nta
{
    using engine_type = dummy_type;
};

template<class OT, class T1, class T2>
struct test_mul_traits_nta
{
    using result_type = dummy_type;
};

struct test_mul_op_traits_nta
{
    template<class OT, class T1, class T2>
    using multiplication_element_traits = test_element_mul_traits_nta<OT, T1, T2>;

    template<class OT, class ET1, class ET2>
    using multiplication_engine_traits = test_engine_mul_traits_nta<OT, ET1, ET2>;

    template<class OT, class OP1, class OP2>
    using multiplication_arithmetic_traits = test_mul_traits_nta<OT, OP1, OP2>;
};

//- This operation traits type has its element/engine/arithmetic traits as nested class templates.
//
//  Suffix "_nct" means "nested class type"
//
struct test_mul_op_traits_nct
{
    template<class OT, class T1, class T2>
    struct multiplication_element_traits
    {
        using element_type = dummy_type;
    };

    template<class OT, class ET1, class ET2>
    struct multiplication_engine_traits
    {
        using engine_type = dummy_type;
    };

    template<class OT, class OP1, class OP2>
    struct multiplication_arithmetic_traits
    {
        using result_type = dummy_type;
    };
};

//- The following are some invalid operation traits.
//
struct test_mul_op_traits_bad_00
{
    using multiplication_element_traits = int;
    using multiplication_engine_traits = int;
    using multiplication_arithmetic_traits = int;
};

struct test_mul_op_traits_bad_01
{
    template<class OT>
    using multiplication_element_traits = STD_LA::detail::multiplication_element_traits<OT, int, int>;

    template<class OT>
    using multiplication_engine_traits = STD_LA::detail::multiplication_engine_traits<OT, int, int>;

    template<class OT>
    using multiplication_arithmetic_traits = STD_LA::detail::multiplication_arithmetic_traits<OT, int, int>;
};

struct test_mul_op_traits_bad_02
{
    template<class OT, class T1>
    using multiplication_element_traits = STD_LA::detail::multiplication_element_traits<OT, T1, T1>;

    template<class OT, class T1>
    using multiplication_engine_traits = STD_LA::detail::multiplication_engine_traits<OT, T1, T1>;

    template<class OT, class T1>
    using multiplication_arithmetic_traits = STD_LA::detail::multiplication_arithmetic_traits<OT, T1, T1>;
};

//--------------------------------------------------------------------------------------------------
//  This test ensures that the nested traits associated with multiplication are properly validated.
//  It exercises only the validation meta-functions.
//
//  Note that the types for the second and third template parameters are unimportant for these
//  tests - it is the form of the nested template that is being validated (specifically, being
//  instantiable with three template arguments).
//--------------------------------------------------------------------------------------------------
//
TEST(MulTraits, Validation)
{
    static_assert(valid_multiplication_element_traits<test_mul_op_traits_empty, int, int>);
    static_assert(valid_multiplication_element_traits<test_mul_op_traits_nta, int, int>);
    static_assert(valid_multiplication_element_traits<test_mul_op_traits_nct, int, int>);

    static_assert(!valid_multiplication_element_traits<test_mul_op_traits_bad_00, int, int>);
    static_assert(!valid_multiplication_element_traits<test_mul_op_traits_bad_01, int, int>);
    static_assert(!valid_multiplication_element_traits<test_mul_op_traits_bad_02, int, int>);

    static_assert(valid_multiplication_engine_traits<test_mul_op_traits_empty, int, int>);
    static_assert(valid_multiplication_engine_traits<test_mul_op_traits_nta, int, int>);
    static_assert(valid_multiplication_engine_traits<test_mul_op_traits_nct, int, int>);

    static_assert(!valid_multiplication_engine_traits<test_mul_op_traits_bad_00, int, int>);
    static_assert(!valid_multiplication_engine_traits<test_mul_op_traits_bad_01, int, int>);
    static_assert(!valid_multiplication_engine_traits<test_mul_op_traits_bad_02, int, int>);

    static_assert(valid_multiplication_arithmetic_traits<test_mul_op_traits_empty, int, int>);
    static_assert(valid_multiplication_arithmetic_traits<test_mul_op_traits_nta, int, int>);
    static_assert(valid_multiplication_arithmetic_traits<test_mul_op_traits_nct, int, int>);

    static_assert(!valid_multiplication_arithmetic_traits<test_mul_op_traits_bad_00, int, int>);
    static_assert(!valid_multiplication_arithmetic_traits<test_mul_op_traits_bad_01, int, int>);
    static_assert(!valid_multiplication_arithmetic_traits<test_mul_op_traits_bad_02, int, int>);

    static_assert(valid_multiplication_traits<test_mul_op_traits_empty, int, int>);
    static_assert(valid_multiplication_traits<test_mul_op_traits_nta, int, int>);
    static_assert(valid_multiplication_traits<test_mul_op_traits_nct, int, int>);

    static_assert(!valid_multiplication_traits<test_mul_op_traits_bad_00, int, int>);
    static_assert(!valid_multiplication_traits<test_mul_op_traits_bad_01, int, int>);
    static_assert(!valid_multiplication_traits<test_mul_op_traits_bad_02, int, int>);
}

#if 0
//--------------------------------------------------------------------------------------------------
//  This test verifies that VECTOR*SCALAR multiplications return the correct result type.
//--------------------------------------------------------------------------------------------------
//
void t401()
{
    PRINT_FNAME();

    using fsv_float   = STD_LA::fs_vector<float, 3>;
    using fsv_double  = STD_LA::fs_vector<double, 3>;
    using fsv_new_num = STD_LA::fs_vector<new_num, 3>;

    using drv_float   = STD_LA::dyn_vector<float>;
    using drv_double  = STD_LA::dyn_vector<double>;
    using drv_new_num = STD_LA::dyn_vector<new_num>;

    ASSERT_A_MUL_B_EQ_C(fsv_float,  float,      fsv_float);
    ASSERT_A_MUL_B_EQ_C(fsv_float,  double,     fsv_double);
    ASSERT_A_MUL_B_EQ_C(fsv_float,  new_num,    fsv_new_num);

    ASSERT_A_MUL_B_EQ_C(fsv_double,  float,     fsv_double);
    ASSERT_A_MUL_B_EQ_C(fsv_double,  double,    fsv_double);
    ASSERT_A_MUL_B_EQ_C(fsv_double,  new_num,   fsv_new_num);

    ASSERT_A_MUL_B_EQ_C(fsv_new_num,  float,    fsv_new_num);
    ASSERT_A_MUL_B_EQ_C(fsv_new_num,  double,   fsv_new_num);
    ASSERT_A_MUL_B_EQ_C(fsv_new_num,  new_num,  fsv_new_num);

    //------
    //
    ASSERT_A_MUL_B_EQ_C(drv_float,  float,      drv_float);
    ASSERT_A_MUL_B_EQ_C(drv_float,  double,     drv_double);
    ASSERT_A_MUL_B_EQ_C(drv_float,  new_num,    drv_new_num);

    ASSERT_A_MUL_B_EQ_C(drv_double,  float,     drv_double);
    ASSERT_A_MUL_B_EQ_C(drv_double,  double,    drv_double);
    ASSERT_A_MUL_B_EQ_C(drv_double,  new_num,   drv_new_num);

    ASSERT_A_MUL_B_EQ_C(drv_new_num,  float,    drv_new_num);
    ASSERT_A_MUL_B_EQ_C(drv_new_num,  double,   drv_new_num);
    ASSERT_A_MUL_B_EQ_C(drv_new_num,  new_num,  drv_new_num);
}

//--------------------------------------------------------------------------------------------------
//  This test verifies that SCALAR*VECTOR multiplications return the correct result type.
//--------------------------------------------------------------------------------------------------
//
void t402()
{
    PRINT_FNAME();

    using fsv_float   = STD_LA::fs_vector<float, 3>;
    using fsv_double  = STD_LA::fs_vector<double, 3>;
    using fsv_new_num = STD_LA::fs_vector<new_num, 3>;

    using drv_float   = STD_LA::dyn_vector<float>;
    using drv_double  = STD_LA::dyn_vector<double>;
    using drv_new_num = STD_LA::dyn_vector<new_num>;

    ASSERT_A_MUL_B_EQ_C(float,  fsv_float,      fsv_float);
    ASSERT_A_MUL_B_EQ_C(float,  fsv_double,     fsv_double);
    ASSERT_A_MUL_B_EQ_C(float,  fsv_new_num,    fsv_new_num);

    ASSERT_A_MUL_B_EQ_C(double,  fsv_float,     fsv_double);
    ASSERT_A_MUL_B_EQ_C(double,  fsv_double,    fsv_double);
    ASSERT_A_MUL_B_EQ_C(double,  fsv_new_num,   fsv_new_num);

    ASSERT_A_MUL_B_EQ_C(new_num,  fsv_float,    fsv_new_num);
    ASSERT_A_MUL_B_EQ_C(new_num,  fsv_double,   fsv_new_num);
    ASSERT_A_MUL_B_EQ_C(new_num,  fsv_new_num,  fsv_new_num);

    //------
    //
    ASSERT_A_MUL_B_EQ_C(float,  drv_float,      drv_float);
    ASSERT_A_MUL_B_EQ_C(float,  drv_double,     drv_double);
    ASSERT_A_MUL_B_EQ_C(float,  drv_new_num,    drv_new_num);

    ASSERT_A_MUL_B_EQ_C(double,  drv_float,     drv_double);
    ASSERT_A_MUL_B_EQ_C(double,  drv_double,    drv_double);
    ASSERT_A_MUL_B_EQ_C(double,  drv_new_num,   drv_new_num);

    ASSERT_A_MUL_B_EQ_C(new_num,  drv_float,    drv_new_num);
    ASSERT_A_MUL_B_EQ_C(new_num,  drv_double,   drv_new_num);
    ASSERT_A_MUL_B_EQ_C(new_num,  drv_new_num,  drv_new_num);
}

//--------------------------------------------------------------------------------------------------
//  This test verifies that MATRIX*SCALAR multiplications return the correct result type.
//--------------------------------------------------------------------------------------------------
//
void t403()
{
    PRINT_FNAME();

    using fsm_float   = STD_LA::fs_matrix<float, 3, 4>;
    using fsm_double  = STD_LA::fs_matrix<double, 3, 4>;
    using fsm_new_num = STD_LA::fs_matrix<new_num, 3, 4>;

    using drm_float   = STD_LA::dyn_matrix<float>;
    using drm_double  = STD_LA::dyn_matrix<double>;
    using drm_new_num = STD_LA::dyn_matrix<new_num>;

    ASSERT_A_MUL_B_EQ_C(fsm_float,  float,      fsm_float);
    ASSERT_A_MUL_B_EQ_C(fsm_float,  double,     fsm_double);
    ASSERT_A_MUL_B_EQ_C(fsm_float,  new_num,    fsm_new_num);

    ASSERT_A_MUL_B_EQ_C(fsm_double,  float,     fsm_double);
    ASSERT_A_MUL_B_EQ_C(fsm_double,  double,    fsm_double);
    ASSERT_A_MUL_B_EQ_C(fsm_double,  new_num,   fsm_new_num);

    ASSERT_A_MUL_B_EQ_C(fsm_new_num,  float,    fsm_new_num);
    ASSERT_A_MUL_B_EQ_C(fsm_new_num,  double,   fsm_new_num);
    ASSERT_A_MUL_B_EQ_C(fsm_new_num,  new_num,  fsm_new_num);

    //------
    //
    ASSERT_A_MUL_B_EQ_C(drm_float,  float,      drm_float);
    ASSERT_A_MUL_B_EQ_C(drm_float,  double,     drm_double);
    ASSERT_A_MUL_B_EQ_C(drm_float,  new_num,    drm_new_num);

    ASSERT_A_MUL_B_EQ_C(drm_double,  float,     drm_double);
    ASSERT_A_MUL_B_EQ_C(drm_double,  double,    drm_double);
    ASSERT_A_MUL_B_EQ_C(drm_double,  new_num,   drm_new_num);

    ASSERT_A_MUL_B_EQ_C(drm_new_num,  float,    drm_new_num);
    ASSERT_A_MUL_B_EQ_C(drm_new_num,  double,   drm_new_num);
    ASSERT_A_MUL_B_EQ_C(drm_new_num,  new_num,  drm_new_num);
}

//--------------------------------------------------------------------------------------------------
//  This test verifies that SCALAR*MATRIX multiplications return the correct result type.
//--------------------------------------------------------------------------------------------------
//
void t404()
{
    PRINT_FNAME();

    using fsm_float   = STD_LA::fs_matrix<float, 3, 4>;
    using fsm_double  = STD_LA::fs_matrix<double, 3, 4>;
    using fsm_new_num = STD_LA::fs_matrix<new_num, 3, 4>;

    using drm_float   = STD_LA::dyn_matrix<float>;
    using drm_double  = STD_LA::dyn_matrix<double>;
    using drm_new_num = STD_LA::dyn_matrix<new_num>;

    ASSERT_A_MUL_B_EQ_C(float,  fsm_float,      fsm_float);
    ASSERT_A_MUL_B_EQ_C(float,  fsm_double,     fsm_double);
    ASSERT_A_MUL_B_EQ_C(float,  fsm_new_num,    fsm_new_num);

    ASSERT_A_MUL_B_EQ_C(double,  fsm_float,     fsm_double);
    ASSERT_A_MUL_B_EQ_C(double,  fsm_double,    fsm_double);
    ASSERT_A_MUL_B_EQ_C(double,  fsm_new_num,   fsm_new_num);

    ASSERT_A_MUL_B_EQ_C(new_num,  fsm_float,    fsm_new_num);
    ASSERT_A_MUL_B_EQ_C(new_num,  fsm_double,   fsm_new_num);
    ASSERT_A_MUL_B_EQ_C(new_num,  fsm_new_num,  fsm_new_num);

    //------
    //
    ASSERT_A_MUL_B_EQ_C(float,  drm_float,      drm_float);
    ASSERT_A_MUL_B_EQ_C(float,  drm_double,     drm_double);
    ASSERT_A_MUL_B_EQ_C(float,  drm_new_num,    drm_new_num);

    ASSERT_A_MUL_B_EQ_C(double,  drm_float,     drm_double);
    ASSERT_A_MUL_B_EQ_C(double,  drm_double,    drm_double);
    ASSERT_A_MUL_B_EQ_C(double,  drm_new_num,   drm_new_num);

    ASSERT_A_MUL_B_EQ_C(new_num,  drm_float,    drm_new_num);
    ASSERT_A_MUL_B_EQ_C(new_num,  drm_double,   drm_new_num);
    ASSERT_A_MUL_B_EQ_C(new_num,  drm_new_num,  drm_new_num);
}

//--------------------------------------------------------------------------------------------------
//  This test verifies that VECTOR*VECTOR multiplications return the correct result type.
//--------------------------------------------------------------------------------------------------
//
void t405()
{
    PRINT_FNAME();

    using fsv_float      = STD_LA::fs_vector<float, 3>;
    using fsv_double     = STD_LA::fs_vector<double, 3>;
    using fsv_new_num    = STD_LA::fs_vector<new_num, 3>;

    using drv_float      = STD_LA::dyn_vector<float>;
    using drv_double     = STD_LA::dyn_vector<double>;
    using drv_new_num    = STD_LA::dyn_vector<new_num>;

    ASSERT_A_MUL_B_EQ_C(fsv_float,  fsv_float,    float);
    ASSERT_A_MUL_B_EQ_C(fsv_float,  fsv_double,   double);
    ASSERT_A_MUL_B_EQ_C(fsv_float,  fsv_new_num,  new_num);
    ASSERT_A_MUL_B_EQ_C(fsv_float,  drv_float,    float);
    ASSERT_A_MUL_B_EQ_C(fsv_float,  drv_double,   double);
    ASSERT_A_MUL_B_EQ_C(fsv_float,  drv_new_num,  new_num);

    ASSERT_A_MUL_B_EQ_C(fsv_double,  fsv_float,    double);
    ASSERT_A_MUL_B_EQ_C(fsv_double,  fsv_double,   double);
    ASSERT_A_MUL_B_EQ_C(fsv_double,  fsv_new_num,  new_num);
    ASSERT_A_MUL_B_EQ_C(fsv_double,  drv_float,    double);
    ASSERT_A_MUL_B_EQ_C(fsv_double,  drv_double,   double);
    ASSERT_A_MUL_B_EQ_C(fsv_double,  drv_new_num,  new_num);

    ASSERT_A_MUL_B_EQ_C(fsv_new_num,  fsv_float,    new_num);
    ASSERT_A_MUL_B_EQ_C(fsv_new_num,  fsv_double,   new_num);
    ASSERT_A_MUL_B_EQ_C(fsv_new_num,  fsv_new_num,  new_num);
    ASSERT_A_MUL_B_EQ_C(fsv_new_num,  drv_float,    new_num);
    ASSERT_A_MUL_B_EQ_C(fsv_new_num,  drv_double,   new_num);
    ASSERT_A_MUL_B_EQ_C(fsv_new_num,  drv_new_num,  new_num);

    //------
    //
    ASSERT_A_MUL_B_EQ_C(drv_float,  fsv_float,    float);
    ASSERT_A_MUL_B_EQ_C(drv_float,  fsv_double,   double);
    ASSERT_A_MUL_B_EQ_C(drv_float,  fsv_new_num,  new_num);
    ASSERT_A_MUL_B_EQ_C(drv_float,  drv_float,    float);
    ASSERT_A_MUL_B_EQ_C(drv_float,  drv_double,   double);
    ASSERT_A_MUL_B_EQ_C(drv_float,  drv_new_num,  new_num);

    ASSERT_A_MUL_B_EQ_C(drv_double,  fsv_float,    double);
    ASSERT_A_MUL_B_EQ_C(drv_double,  fsv_double,   double);
    ASSERT_A_MUL_B_EQ_C(drv_double,  fsv_new_num,  new_num);
    ASSERT_A_MUL_B_EQ_C(drv_double,  drv_float,    double);
    ASSERT_A_MUL_B_EQ_C(drv_double,  drv_double,   double);
    ASSERT_A_MUL_B_EQ_C(drv_double,  drv_new_num,  new_num);

    ASSERT_A_MUL_B_EQ_C(drv_new_num,  fsv_float,    new_num);
    ASSERT_A_MUL_B_EQ_C(drv_new_num,  fsv_double,   new_num);
    ASSERT_A_MUL_B_EQ_C(drv_new_num,  fsv_new_num,  new_num);
    ASSERT_A_MUL_B_EQ_C(drv_new_num,  drv_float,    new_num);
    ASSERT_A_MUL_B_EQ_C(drv_new_num,  drv_double,   new_num);
    ASSERT_A_MUL_B_EQ_C(drv_new_num,  drv_new_num,  new_num);
}

//--------------------------------------------------------------------------------------------------
//  This test verifies that MATRIX*VECTOR multiplications return the correct result type.
//--------------------------------------------------------------------------------------------------
//
void t406()
{
    PRINT_FNAME();

    using fsm_float   = STD_LA::fs_matrix<float, 4, 3>;
    using fsm_double  = STD_LA::fs_matrix<double, 4, 3>;
    using fsm_new_num = STD_LA::fs_matrix<new_num, 4, 3>;
    using drm_float   = STD_LA::dyn_matrix<float>;
    using drm_double  = STD_LA::dyn_matrix<double>;
    using drm_new_num = STD_LA::dyn_matrix<new_num>;

    using fsv_float   = STD_LA::fs_vector<float, 3>;
    using fsv_double  = STD_LA::fs_vector<double, 3>;
    using fsv_new_num = STD_LA::fs_vector<new_num, 3>;
    using drv_float   = STD_LA::dyn_vector<float>;
    using drv_double  = STD_LA::dyn_vector<double>;
    using drv_new_num = STD_LA::dyn_vector<new_num>;

    ASSERT_A_MUL_B_EQ_C(fsm_float,  fsv_float,    fsv_float);
    ASSERT_A_MUL_B_EQ_C(fsm_float,  fsv_double,   fsv_double);
    ASSERT_A_MUL_B_EQ_C(fsm_float,  fsv_new_num,  fsv_new_num);
    ASSERT_A_MUL_B_EQ_C(fsm_float,  drv_float,    drv_float);
    ASSERT_A_MUL_B_EQ_C(fsm_float,  drv_double,   drv_double);
    ASSERT_A_MUL_B_EQ_C(fsm_float,  drv_new_num,  drv_new_num);

    ASSERT_A_MUL_B_EQ_C(fsm_double,  fsv_float,    fsv_double);
    ASSERT_A_MUL_B_EQ_C(fsm_double,  fsv_double,   fsv_double);
    ASSERT_A_MUL_B_EQ_C(fsm_double,  fsv_new_num,  fsv_new_num);
    ASSERT_A_MUL_B_EQ_C(fsm_double,  drv_float,    drv_double);
    ASSERT_A_MUL_B_EQ_C(fsm_double,  drv_double,   drv_double);
    ASSERT_A_MUL_B_EQ_C(fsm_double,  drv_new_num,  drv_new_num);

    ASSERT_A_MUL_B_EQ_C(fsm_new_num,  fsv_float,    fsv_new_num);
    ASSERT_A_MUL_B_EQ_C(fsm_new_num,  fsv_double,   fsv_new_num);
    ASSERT_A_MUL_B_EQ_C(fsm_new_num,  fsv_new_num,  fsv_new_num);
    ASSERT_A_MUL_B_EQ_C(fsm_new_num,  drv_float,    drv_new_num);
    ASSERT_A_MUL_B_EQ_C(fsm_new_num,  drv_double,   drv_new_num);
    ASSERT_A_MUL_B_EQ_C(fsm_new_num,  drv_new_num,  drv_new_num);

    //------
    //
    ASSERT_A_MUL_B_EQ_C(drm_float,  fsv_float,    drv_float);
    ASSERT_A_MUL_B_EQ_C(drm_float,  fsv_double,   drv_double);
    ASSERT_A_MUL_B_EQ_C(drm_float,  fsv_new_num,  drv_new_num);
    ASSERT_A_MUL_B_EQ_C(drm_float,  drv_float,    drv_float);
    ASSERT_A_MUL_B_EQ_C(drm_float,  drv_double,   drv_double);
    ASSERT_A_MUL_B_EQ_C(drm_float,  drv_new_num,  drv_new_num);

    ASSERT_A_MUL_B_EQ_C(drm_double,  fsv_float,    drv_double);
    ASSERT_A_MUL_B_EQ_C(drm_double,  fsv_double,   drv_double);
    ASSERT_A_MUL_B_EQ_C(drm_double,  fsv_new_num,  drv_new_num);
    ASSERT_A_MUL_B_EQ_C(drm_double,  drv_float,    drv_double);
    ASSERT_A_MUL_B_EQ_C(drm_double,  drv_double,   drv_double);
    ASSERT_A_MUL_B_EQ_C(drm_double,  drv_new_num,  drv_new_num);

    ASSERT_A_MUL_B_EQ_C(drm_new_num,  fsv_float,    drv_new_num);
    ASSERT_A_MUL_B_EQ_C(drm_new_num,  fsv_double,   drv_new_num);
    ASSERT_A_MUL_B_EQ_C(drm_new_num,  fsv_new_num,  drv_new_num);
    ASSERT_A_MUL_B_EQ_C(drm_new_num,  drv_float,    drv_new_num);
    ASSERT_A_MUL_B_EQ_C(drm_new_num,  drv_double,   drv_new_num);
    ASSERT_A_MUL_B_EQ_C(drm_new_num,  drv_new_num,  drv_new_num);
}

//--------------------------------------------------------------------------------------------------
//  This test verifies that VECTOR*MATRIX multiplications return the correct result type.
//--------------------------------------------------------------------------------------------------
//
void t407()
{
    PRINT_FNAME();

    using fsm_float   = STD_LA::fs_matrix<float, 4, 3>;
    using fsm_double  = STD_LA::fs_matrix<double, 4, 3>;
    using fsm_new_num = STD_LA::fs_matrix<new_num, 4, 3>;
    using drm_float   = STD_LA::dyn_matrix<float>;
    using drm_double  = STD_LA::dyn_matrix<double>;
    using drm_new_num = STD_LA::dyn_matrix<new_num>;

    using fsv_float   = STD_LA::fs_vector<float, 4>;
    using fsv_double  = STD_LA::fs_vector<double, 4>;
    using fsv_new_num = STD_LA::fs_vector<new_num, 4>;
    using drv_float   = STD_LA::dyn_vector<float>;
    using drv_double  = STD_LA::dyn_vector<double>;
    using drv_new_num = STD_LA::dyn_vector<new_num>;

    ASSERT_A_MUL_B_EQ_C(fsv_float,  fsm_float,    fsv_float);
    ASSERT_A_MUL_B_EQ_C(fsv_float,  fsm_double,   fsv_double);
    ASSERT_A_MUL_B_EQ_C(fsv_float,  fsm_new_num,  fsv_new_num);
    ASSERT_A_MUL_B_EQ_C(fsv_float,  drm_float,    drv_float);
    ASSERT_A_MUL_B_EQ_C(fsv_float,  drm_double,   drv_double);
    ASSERT_A_MUL_B_EQ_C(fsv_float,  drm_new_num,  drv_new_num);

    ASSERT_A_MUL_B_EQ_C(fsv_double,  fsm_float,    fsv_double);
    ASSERT_A_MUL_B_EQ_C(fsv_double,  fsm_double,   fsv_double);
    ASSERT_A_MUL_B_EQ_C(fsv_double,  fsm_new_num,  fsv_new_num);
    ASSERT_A_MUL_B_EQ_C(fsv_double,  drm_float,    drv_double);
    ASSERT_A_MUL_B_EQ_C(fsv_double,  drm_double,   drv_double);
    ASSERT_A_MUL_B_EQ_C(fsv_double,  drm_new_num,  drv_new_num);

    ASSERT_A_MUL_B_EQ_C(fsv_new_num,  fsm_float,    fsv_new_num);
    ASSERT_A_MUL_B_EQ_C(fsv_new_num,  fsm_double,   fsv_new_num);
    ASSERT_A_MUL_B_EQ_C(fsv_new_num,  fsm_new_num,  fsv_new_num);
    ASSERT_A_MUL_B_EQ_C(fsv_new_num,  drm_float,    drv_new_num);
    ASSERT_A_MUL_B_EQ_C(fsv_new_num,  drm_double,   drv_new_num);
    ASSERT_A_MUL_B_EQ_C(fsv_new_num,  drm_new_num,  drv_new_num);

    //------
    //
    ASSERT_A_MUL_B_EQ_C(drv_float,  fsm_float,    drv_float);
    ASSERT_A_MUL_B_EQ_C(drv_float,  fsm_double,   drv_double);
    ASSERT_A_MUL_B_EQ_C(drv_float,  fsm_new_num,  drv_new_num);
    ASSERT_A_MUL_B_EQ_C(drv_float,  drm_float,    drv_float);
    ASSERT_A_MUL_B_EQ_C(drv_float,  drm_double,   drv_double);
    ASSERT_A_MUL_B_EQ_C(drv_float,  drm_new_num,  drv_new_num);

    ASSERT_A_MUL_B_EQ_C(drv_double,  fsm_float,    drv_double);
    ASSERT_A_MUL_B_EQ_C(drv_double,  fsm_double,   drv_double);
    ASSERT_A_MUL_B_EQ_C(drv_double,  fsm_new_num,  drv_new_num);
    ASSERT_A_MUL_B_EQ_C(drv_double,  drm_float,    drv_double);
    ASSERT_A_MUL_B_EQ_C(drv_double,  drm_double,   drv_double);
    ASSERT_A_MUL_B_EQ_C(drv_double,  drm_new_num,  drv_new_num);

    ASSERT_A_MUL_B_EQ_C(drv_new_num,  fsm_float,    drv_new_num);
    ASSERT_A_MUL_B_EQ_C(drv_new_num,  fsm_double,   drv_new_num);
    ASSERT_A_MUL_B_EQ_C(drv_new_num,  fsm_new_num,  drv_new_num);
    ASSERT_A_MUL_B_EQ_C(drv_new_num,  drm_float,    drv_new_num);
    ASSERT_A_MUL_B_EQ_C(drv_new_num,  drm_double,   drv_new_num);
    ASSERT_A_MUL_B_EQ_C(drv_new_num,  drm_new_num,  drv_new_num);
}

//--------------------------------------------------------------------------------------------------
//  This test verifies that MATRIX*MATRIX multiplications return the correct result type.
//--------------------------------------------------------------------------------------------------
//
void t408()
{
    PRINT_FNAME();

    using fsm_float_34   = STD_LA::fs_matrix<float, 3, 4>;
    using fsm_float_45   = STD_LA::fs_matrix<float, 4, 5>;
    using fsm_float_35   = STD_LA::fs_matrix<float, 3, 5>;
    using fsm_double_34  = STD_LA::fs_matrix<double, 3, 4>;
    using fsm_double_45  = STD_LA::fs_matrix<double, 4, 5>;
    using fsm_double_35  = STD_LA::fs_matrix<double, 3, 5>;
    using fsm_new_num_34 = STD_LA::fs_matrix<new_num, 3, 4>;
    using fsm_new_num_45 = STD_LA::fs_matrix<new_num, 4, 5>;
    using fsm_new_num_35 = STD_LA::fs_matrix<new_num, 3, 5>;

    using fsm_float_tr_34   = decltype(std::declval<STD_LA::fs_matrix<float, 4, 3>>().t());
    using fsm_float_tr_45   = decltype(std::declval<STD_LA::fs_matrix<float, 5, 4>>().t());
    using fsm_double_tr_34  = decltype(std::declval<STD_LA::fs_matrix<double, 4, 3>>().t());
    using fsm_double_tr_45  = decltype(std::declval<STD_LA::fs_matrix<double, 5, 4>>().t());
    using fsm_new_num_tr_34 = decltype(std::declval<STD_LA::fs_matrix<new_num, 4, 3>>().t());
    using fsm_new_num_tr_45 = decltype(std::declval<STD_LA::fs_matrix<new_num, 5, 4>>().t());

    using drm_float      = STD_LA::dyn_matrix<float>;
    using drm_double     = STD_LA::dyn_matrix<double>;
    using drm_new_num    = STD_LA::dyn_matrix<new_num>;

    using drm_float_tr   = decltype(std::declval<drm_float>().t());
    using drm_double_tr  = decltype(std::declval<drm_double>().t());
    using drm_new_num_tr = decltype(std::declval<drm_new_num>().t());

    ASSERT_A_MUL_B_EQ_C(fsm_float_34,  fsm_float_45,       fsm_float_35);
    ASSERT_A_MUL_B_EQ_C(fsm_float_34,  fsm_double_45,      fsm_double_35);
    ASSERT_A_MUL_B_EQ_C(fsm_float_34,  fsm_new_num_45,     fsm_new_num_35);
    ASSERT_A_MUL_B_EQ_C(fsm_float_34,  fsm_float_tr_45,    fsm_float_35);
    ASSERT_A_MUL_B_EQ_C(fsm_float_34,  fsm_double_tr_45,   fsm_double_35);
    ASSERT_A_MUL_B_EQ_C(fsm_float_34,  fsm_new_num_tr_45,  fsm_new_num_35);
    ASSERT_A_MUL_B_EQ_C(fsm_float_34,  drm_float,          drm_float);
    ASSERT_A_MUL_B_EQ_C(fsm_float_34,  drm_double,         drm_double);
    ASSERT_A_MUL_B_EQ_C(fsm_float_34,  drm_new_num,        drm_new_num);
    ASSERT_A_MUL_B_EQ_C(fsm_float_34,  drm_float_tr,       drm_float);
    ASSERT_A_MUL_B_EQ_C(fsm_float_34,  drm_double_tr,      drm_double);
    ASSERT_A_MUL_B_EQ_C(fsm_float_34,  drm_new_num_tr,     drm_new_num);

    ASSERT_A_MUL_B_EQ_C(fsm_double_34,  fsm_float_45,       fsm_double_35);
    ASSERT_A_MUL_B_EQ_C(fsm_double_34,  fsm_double_45,      fsm_double_35);
    ASSERT_A_MUL_B_EQ_C(fsm_double_34,  fsm_new_num_45,     fsm_new_num_35);
    ASSERT_A_MUL_B_EQ_C(fsm_double_34,  fsm_float_tr_45,    fsm_double_35);
    ASSERT_A_MUL_B_EQ_C(fsm_double_34,  fsm_double_tr_45,   fsm_double_35);
    ASSERT_A_MUL_B_EQ_C(fsm_double_34,  fsm_new_num_tr_45,  fsm_new_num_35);
    ASSERT_A_MUL_B_EQ_C(fsm_double_34,  drm_float,          drm_double);
    ASSERT_A_MUL_B_EQ_C(fsm_double_34,  drm_double,         drm_double);
    ASSERT_A_MUL_B_EQ_C(fsm_double_34,  drm_new_num,        drm_new_num);
    ASSERT_A_MUL_B_EQ_C(fsm_double_34,  drm_float_tr,       drm_double);
    ASSERT_A_MUL_B_EQ_C(fsm_double_34,  drm_double_tr,      drm_double);
    ASSERT_A_MUL_B_EQ_C(fsm_double_34,  drm_new_num_tr,     drm_new_num);

    ASSERT_A_MUL_B_EQ_C(fsm_new_num_34,  fsm_float_45,       fsm_new_num_35);
    ASSERT_A_MUL_B_EQ_C(fsm_new_num_34,  fsm_double_45,      fsm_new_num_35);
    ASSERT_A_MUL_B_EQ_C(fsm_new_num_34,  fsm_new_num_45,     fsm_new_num_35);
    ASSERT_A_MUL_B_EQ_C(fsm_new_num_34,  fsm_float_tr_45,    fsm_new_num_35);
    ASSERT_A_MUL_B_EQ_C(fsm_new_num_34,  fsm_double_tr_45,   fsm_new_num_35);
    ASSERT_A_MUL_B_EQ_C(fsm_new_num_34,  fsm_new_num_tr_45,  fsm_new_num_35);
    ASSERT_A_MUL_B_EQ_C(fsm_new_num_34,  drm_float,          drm_new_num);
    ASSERT_A_MUL_B_EQ_C(fsm_new_num_34,  drm_double,         drm_new_num);
    ASSERT_A_MUL_B_EQ_C(fsm_new_num_34,  drm_new_num,        drm_new_num);
    ASSERT_A_MUL_B_EQ_C(fsm_new_num_34,  drm_float_tr,       drm_new_num);
    ASSERT_A_MUL_B_EQ_C(fsm_new_num_34,  drm_double_tr,      drm_new_num);
    ASSERT_A_MUL_B_EQ_C(fsm_new_num_34,  drm_new_num_tr,     drm_new_num);

    ASSERT_A_MUL_B_EQ_C(fsm_float_tr_34,  fsm_float_45,       fsm_float_35);
    ASSERT_A_MUL_B_EQ_C(fsm_float_tr_34,  fsm_double_45,      fsm_double_35);
    ASSERT_A_MUL_B_EQ_C(fsm_float_tr_34,  fsm_new_num_45,     fsm_new_num_35);
    ASSERT_A_MUL_B_EQ_C(fsm_float_tr_34,  fsm_float_tr_45,    fsm_float_35);
    ASSERT_A_MUL_B_EQ_C(fsm_float_tr_34,  fsm_double_tr_45,   fsm_double_35);
    ASSERT_A_MUL_B_EQ_C(fsm_float_tr_34,  fsm_new_num_tr_45,  fsm_new_num_35);
    ASSERT_A_MUL_B_EQ_C(fsm_float_tr_34,  drm_float,          drm_float);
    ASSERT_A_MUL_B_EQ_C(fsm_float_tr_34,  drm_double,         drm_double);
    ASSERT_A_MUL_B_EQ_C(fsm_float_tr_34,  drm_new_num,        drm_new_num);
    ASSERT_A_MUL_B_EQ_C(fsm_float_tr_34,  drm_float_tr,       drm_float);
    ASSERT_A_MUL_B_EQ_C(fsm_float_tr_34,  drm_double_tr,      drm_double);
    ASSERT_A_MUL_B_EQ_C(fsm_float_tr_34,  drm_new_num_tr,     drm_new_num);

    ASSERT_A_MUL_B_EQ_C(fsm_double_tr_34,  fsm_float_45,       fsm_double_35);
    ASSERT_A_MUL_B_EQ_C(fsm_double_tr_34,  fsm_double_45,      fsm_double_35);
    ASSERT_A_MUL_B_EQ_C(fsm_double_tr_34,  fsm_new_num_45,     fsm_new_num_35);
    ASSERT_A_MUL_B_EQ_C(fsm_double_tr_34,  fsm_float_tr_45,    fsm_double_35);
    ASSERT_A_MUL_B_EQ_C(fsm_double_tr_34,  fsm_double_tr_45,   fsm_double_35);
    ASSERT_A_MUL_B_EQ_C(fsm_double_tr_34,  fsm_new_num_tr_45,  fsm_new_num_35);
    ASSERT_A_MUL_B_EQ_C(fsm_double_tr_34,  drm_float,          drm_double);
    ASSERT_A_MUL_B_EQ_C(fsm_double_tr_34,  drm_double,         drm_double);
    ASSERT_A_MUL_B_EQ_C(fsm_double_tr_34,  drm_new_num,        drm_new_num);
    ASSERT_A_MUL_B_EQ_C(fsm_double_tr_34,  drm_float_tr,       drm_double);
    ASSERT_A_MUL_B_EQ_C(fsm_double_tr_34,  drm_double_tr,      drm_double);
    ASSERT_A_MUL_B_EQ_C(fsm_double_tr_34,  drm_new_num_tr,     drm_new_num);

    ASSERT_A_MUL_B_EQ_C(fsm_new_num_tr_34,  fsm_float_45,       fsm_new_num_35);
    ASSERT_A_MUL_B_EQ_C(fsm_new_num_tr_34,  fsm_double_45,      fsm_new_num_35);
    ASSERT_A_MUL_B_EQ_C(fsm_new_num_tr_34,  fsm_new_num_45,     fsm_new_num_35);
    ASSERT_A_MUL_B_EQ_C(fsm_new_num_tr_34,  fsm_float_tr_45,    fsm_new_num_35);
    ASSERT_A_MUL_B_EQ_C(fsm_new_num_tr_34,  fsm_double_tr_45,   fsm_new_num_35);
    ASSERT_A_MUL_B_EQ_C(fsm_new_num_tr_34,  fsm_new_num_tr_45,  fsm_new_num_35);
    ASSERT_A_MUL_B_EQ_C(fsm_new_num_tr_34,  drm_float,          drm_new_num);
    ASSERT_A_MUL_B_EQ_C(fsm_new_num_tr_34,  drm_double,         drm_new_num);
    ASSERT_A_MUL_B_EQ_C(fsm_new_num_tr_34,  drm_new_num,        drm_new_num);
    ASSERT_A_MUL_B_EQ_C(fsm_new_num_tr_34,  drm_float_tr,       drm_new_num);
    ASSERT_A_MUL_B_EQ_C(fsm_new_num_tr_34,  drm_double_tr,      drm_new_num);
    ASSERT_A_MUL_B_EQ_C(fsm_new_num_tr_34,  drm_new_num_tr,     drm_new_num);

    //------
    //
    ASSERT_A_MUL_B_EQ_C(drm_float,  fsm_float_45,       drm_float);
    ASSERT_A_MUL_B_EQ_C(drm_float,  fsm_double_45,      drm_double);
    ASSERT_A_MUL_B_EQ_C(drm_float,  fsm_new_num_45,     drm_new_num);
    ASSERT_A_MUL_B_EQ_C(drm_float,  fsm_float_tr_45,    drm_float);
    ASSERT_A_MUL_B_EQ_C(drm_float,  fsm_double_tr_45,   drm_double);
    ASSERT_A_MUL_B_EQ_C(drm_float,  fsm_new_num_tr_45,  drm_new_num);
    ASSERT_A_MUL_B_EQ_C(drm_float,  drm_float,          drm_float);
    ASSERT_A_MUL_B_EQ_C(drm_float,  drm_double,         drm_double);
    ASSERT_A_MUL_B_EQ_C(drm_float,  drm_new_num,        drm_new_num);
    ASSERT_A_MUL_B_EQ_C(drm_float,  drm_float_tr,       drm_float);
    ASSERT_A_MUL_B_EQ_C(drm_float,  drm_double_tr,      drm_double);
    ASSERT_A_MUL_B_EQ_C(drm_float,  drm_new_num_tr,     drm_new_num);

    ASSERT_A_MUL_B_EQ_C(drm_double,  fsm_float_45,       drm_double);
    ASSERT_A_MUL_B_EQ_C(drm_double,  fsm_double_45,      drm_double);
    ASSERT_A_MUL_B_EQ_C(drm_double,  fsm_new_num_45,     drm_new_num);
    ASSERT_A_MUL_B_EQ_C(drm_double,  fsm_float_tr_45,    drm_double);
    ASSERT_A_MUL_B_EQ_C(drm_double,  fsm_double_tr_45,   drm_double);
    ASSERT_A_MUL_B_EQ_C(drm_double,  fsm_new_num_tr_45,  drm_new_num);
    ASSERT_A_MUL_B_EQ_C(drm_double,  drm_float,          drm_double);
    ASSERT_A_MUL_B_EQ_C(drm_double,  drm_double,         drm_double);
    ASSERT_A_MUL_B_EQ_C(drm_double,  drm_new_num,        drm_new_num);
    ASSERT_A_MUL_B_EQ_C(drm_double,  drm_float_tr,       drm_double);
    ASSERT_A_MUL_B_EQ_C(drm_double,  drm_double_tr,      drm_double);
    ASSERT_A_MUL_B_EQ_C(drm_double,  drm_new_num_tr,     drm_new_num);

    ASSERT_A_MUL_B_EQ_C(drm_new_num,  fsm_float_45,       drm_new_num);
    ASSERT_A_MUL_B_EQ_C(drm_new_num,  fsm_double_45,      drm_new_num);
    ASSERT_A_MUL_B_EQ_C(drm_new_num,  fsm_new_num_45,     drm_new_num);
    ASSERT_A_MUL_B_EQ_C(drm_new_num,  fsm_float_tr_45,    drm_new_num);
    ASSERT_A_MUL_B_EQ_C(drm_new_num,  fsm_double_tr_45,   drm_new_num);
    ASSERT_A_MUL_B_EQ_C(drm_new_num,  fsm_new_num_tr_45,  drm_new_num);
    ASSERT_A_MUL_B_EQ_C(drm_new_num,  drm_float,          drm_new_num);
    ASSERT_A_MUL_B_EQ_C(drm_new_num,  drm_double,         drm_new_num);
    ASSERT_A_MUL_B_EQ_C(drm_new_num,  drm_new_num,        drm_new_num);
    ASSERT_A_MUL_B_EQ_C(drm_new_num,  drm_float_tr,       drm_new_num);
    ASSERT_A_MUL_B_EQ_C(drm_new_num,  drm_double_tr,      drm_new_num);
    ASSERT_A_MUL_B_EQ_C(drm_new_num,  drm_new_num_tr,     drm_new_num);

    ASSERT_A_MUL_B_EQ_C(drm_float_tr,  fsm_float_45,       drm_float);
    ASSERT_A_MUL_B_EQ_C(drm_float_tr,  fsm_double_45,      drm_double);
    ASSERT_A_MUL_B_EQ_C(drm_float_tr,  fsm_new_num_45,     drm_new_num);
    ASSERT_A_MUL_B_EQ_C(drm_float_tr,  fsm_float_tr_45,    drm_float);
    ASSERT_A_MUL_B_EQ_C(drm_float_tr,  fsm_double_tr_45,   drm_double);
    ASSERT_A_MUL_B_EQ_C(drm_float_tr,  fsm_new_num_tr_45,  drm_new_num);
    ASSERT_A_MUL_B_EQ_C(drm_float_tr,  drm_float,          drm_float);
    ASSERT_A_MUL_B_EQ_C(drm_float_tr,  drm_double,         drm_double);
    ASSERT_A_MUL_B_EQ_C(drm_float_tr,  drm_new_num,        drm_new_num);
    ASSERT_A_MUL_B_EQ_C(drm_float_tr,  drm_float_tr,       drm_float);
    ASSERT_A_MUL_B_EQ_C(drm_float_tr,  drm_double_tr,      drm_double);
    ASSERT_A_MUL_B_EQ_C(drm_float_tr,  drm_new_num_tr,     drm_new_num);

    ASSERT_A_MUL_B_EQ_C(drm_double_tr,  fsm_float_45,       drm_double);
    ASSERT_A_MUL_B_EQ_C(drm_double_tr,  fsm_double_45,      drm_double);
    ASSERT_A_MUL_B_EQ_C(drm_double_tr,  fsm_new_num_45,     drm_new_num);
    ASSERT_A_MUL_B_EQ_C(drm_double_tr,  fsm_float_tr_45,    drm_double);
    ASSERT_A_MUL_B_EQ_C(drm_double_tr,  fsm_double_tr_45,   drm_double);
    ASSERT_A_MUL_B_EQ_C(drm_double_tr,  fsm_new_num_tr_45,  drm_new_num);
    ASSERT_A_MUL_B_EQ_C(drm_double_tr,  drm_float,          drm_double);
    ASSERT_A_MUL_B_EQ_C(drm_double_tr,  drm_double,         drm_double);
    ASSERT_A_MUL_B_EQ_C(drm_double_tr,  drm_new_num,        drm_new_num);
    ASSERT_A_MUL_B_EQ_C(drm_double_tr,  drm_float_tr,       drm_double);
    ASSERT_A_MUL_B_EQ_C(drm_double_tr,  drm_double_tr,      drm_double);
    ASSERT_A_MUL_B_EQ_C(drm_double_tr,  drm_new_num_tr,     drm_new_num);

    ASSERT_A_MUL_B_EQ_C(drm_new_num_tr,  fsm_float_45,       drm_new_num);
    ASSERT_A_MUL_B_EQ_C(drm_new_num_tr,  fsm_double_45,      drm_new_num);
    ASSERT_A_MUL_B_EQ_C(drm_new_num_tr,  fsm_new_num_45,     drm_new_num);
    ASSERT_A_MUL_B_EQ_C(drm_new_num_tr,  fsm_float_tr_45,    drm_new_num);
    ASSERT_A_MUL_B_EQ_C(drm_new_num_tr,  fsm_double_tr_45,   drm_new_num);
    ASSERT_A_MUL_B_EQ_C(drm_new_num_tr,  fsm_new_num_tr_45,  drm_new_num);
    ASSERT_A_MUL_B_EQ_C(drm_new_num_tr,  drm_float,          drm_new_num);
    ASSERT_A_MUL_B_EQ_C(drm_new_num_tr,  drm_double,         drm_new_num);
    ASSERT_A_MUL_B_EQ_C(drm_new_num_tr,  drm_new_num,        drm_new_num);
    ASSERT_A_MUL_B_EQ_C(drm_new_num_tr,  drm_float_tr,       drm_new_num);
    ASSERT_A_MUL_B_EQ_C(drm_new_num_tr,  drm_double_tr,      drm_new_num);
    ASSERT_A_MUL_B_EQ_C(drm_new_num_tr,  drm_new_num_tr,     drm_new_num);
}

//--------------------------------------------------------------------------------------------------
//  Suffix "_tst" means "test"
//
template<class OT, class T1, class T2>
struct element_mul_traits_tst;

//- Promote any float/float multiplication to double.
//
template<class OT>
struct element_mul_traits_tst<OT, float, float>
{
    using element_type = double;
};

//------
//
template<class OT, class ET1, class ET2>
struct engine_mul_traits_tst;

template<class OT, class T1, size_t R1, size_t C1, class T2, size_t R2, size_t C2>
struct engine_mul_traits_tst<OT,
                             fs_matrix_engine_tst<T1, R1, C1>,
                             fs_matrix_engine_tst<T2, R2, C2>>
{
    static_assert(C1 == R2);
    using element_type = STD_LA::select_matrix_multiplication_element_t<OT, T1, T2>;
    using engine_type  = fs_matrix_engine_tst<element_type, R1, C2>;
};

template<class OT, class T1, size_t R1, size_t C1, class T2, size_t R2, size_t C2>
struct engine_mul_traits_tst<OT,
                             fs_matrix_engine_tst<T1, R1, C1>,
                             STD_LA::fs_matrix_engine<T2, R2, C2>>
{
    static_assert(C1 == R2);
    using element_type = STD_LA::select_matrix_multiplication_element_t<OT, T1, T2>;
    using engine_type  = fs_matrix_engine_tst<element_type, R1, C2>;
};

template<class OT, class T1, size_t R1, size_t C1, class T2, size_t R2, size_t C2>
struct engine_mul_traits_tst<OT,
                             STD_LA::fs_matrix_engine<T1, R1, C1>,
                             fs_matrix_engine_tst<T2, R2, C2>>
{
    static_assert(C1 == R2);
    using element_type = STD_LA::select_matrix_multiplication_element_t<OT, T1, T2>;
    using engine_type  = fs_matrix_engine_tst<element_type, R1, C2>;
};

template<class OT, class T1, size_t R1, size_t C1, class T2, size_t R2, size_t C2>
struct engine_mul_traits_tst<OT,
                             STD_LA::fs_matrix_engine<T1, R1, C1>,
                             STD_LA::fs_matrix_engine<T2, R2, C2>>
{
    static_assert(C1 == R2);
    using element_type = STD_LA::select_matrix_multiplication_element_t<OT, T1, T2>;
    using engine_type  = fs_matrix_engine_tst<element_type, R1, C2>;
};

template<class OT, class T1, size_t R1, size_t C1, class T2, size_t R2, size_t C2>
struct engine_mul_traits_tst<OT,
                             STD_LA::matrix_transpose_engine<fs_matrix_engine_tst<T1, R1, C1>>,
                             STD_LA::matrix_transpose_engine<fs_matrix_engine_tst<T2, R2, C2>>>
{
    static_assert(R1 == C2);
    using element_type = STD_LA::select_matrix_multiplication_element_t<OT, T1, T2>;
    using engine_type  = fs_matrix_engine_tst<element_type, C1, R2>;
};

//------
//
template<class OTR, class OP1, class OP2>
struct multiplication_traits_tst;

template<class OTR>
struct multiplication_traits_tst<OTR,
                                 STD_LA::matrix<fs_matrix_engine_tst<double, 5, 7>, OTR>,
                                 STD_LA::matrix<fs_matrix_engine_tst<double, 7, 9>, OTR>>
{
    using op_traits   = OTR;
    using engine_type = STD_LA::select_matrix_multiplication_engine_t<op_traits,
                                                         fs_matrix_engine_tst<double, 5, 7>,
                                                         fs_matrix_engine_tst<double, 7, 9>>;
    using result_type = STD_LA::matrix<engine_type, op_traits>;

    static result_type  multiply(STD_LA::matrix<fs_matrix_engine_tst<double, 5, 7>, OTR> const& m1,
                                 STD_LA::matrix<fs_matrix_engine_tst<double, 7, 9>, OTR> const& m2)
                        {
                            STD_LA::PrintOperandTypes<result_type>("multiplication_traits_TST (M*M)!!", m1, m2);
                            return result_type();
                        }
};

//------
//
struct test_mul_op_traits_tst
{
     template<class OT, class T1, class T2>
     using multiplication_element_traits = element_mul_traits_tst<OT, T1, T2>;

     template<class OT, class ET1, class ET2>
     using multiplication_engine_traits = engine_mul_traits_tst<OT, ET1, ET2>;

     template<class OT, class OP1, class OP2>
     using multiplication_arithmetic_traits = multiplication_traits_tst<OT, OP1, OP2>;
};


void t409()
{
    PRINT_FNAME();

    static_assert(STD_LA::detail::has_element_mul_traits_v<test_mul_op_traits_tst, float, float>);
    static_assert(!STD_LA::detail::has_element_mul_traits_v<test_mul_op_traits_tst, float, double>);
    static_assert(!STD_LA::detail::has_element_mul_traits_v<test_mul_op_traits_tst, double, float>);

    using t00 = STD_LA::detail::element_mul_result_t<test_mul_op_traits_tst, float, float>;
    PRINT_TYPE(t00);

    using t01 = STD_LA::detail::element_mul_result_t<test_mul_op_traits_tst, float, double>;
    PRINT_TYPE(t01);
}

void t410()
{
    PRINT_FNAME();

    using fsm_float_34       = STD_LA::fs_matrix<float, 3, 4>;
    using fsm_float_45       = STD_LA::fs_matrix<float, 4, 5>;
    using fsm_float_35       = STD_LA::fs_matrix<float, 3, 5>;
    using fsm_float_tst_34   = STD_LA::matrix<fs_matrix_engine_tst<float, 3, 4>, test_mul_op_traits_tst>;
    using fsm_float_tst_45   = STD_LA::matrix<fs_matrix_engine_tst<float, 4, 5>, test_mul_op_traits_tst>;
    using fsm_double_tst_34  = STD_LA::matrix<fs_matrix_engine_tst<double, 3, 4>, test_mul_op_traits_tst>;
    using fsm_double_tst_45  = STD_LA::matrix<fs_matrix_engine_tst<double, 4, 5>, test_mul_op_traits_tst>;
    using fsm_double_tst_35  = STD_LA::matrix<fs_matrix_engine_tst<double, 3, 5>, test_mul_op_traits_tst>;
    using fsm_new_num_tst_34 = STD_LA::matrix<fs_matrix_engine_tst<new_num, 3, 4>, test_mul_op_traits_tst>;
    using fsm_new_num_tst_45 = STD_LA::matrix<fs_matrix_engine_tst<new_num, 4, 5>, test_mul_op_traits_tst>;
    using fsm_new_num_tst_35 = STD_LA::matrix<fs_matrix_engine_tst<new_num, 3, 5>, test_mul_op_traits_tst>;

    using fsm_float_tst_tr_34 = decltype(std::declval<STD_LA::matrix<fs_matrix_engine_tst<float, 4, 3>, test_mul_op_traits_tst>>().t());
    using fsm_float_tst_tr_45 = decltype(std::declval<STD_LA::matrix<fs_matrix_engine_tst<float, 5, 4>, test_mul_op_traits_tst>>().t());
    using fsm_double_tst_tr_34 = decltype(std::declval<STD_LA::matrix<fs_matrix_engine_tst<double, 4, 3>, test_mul_op_traits_tst>>().t());
    using fsm_double_tst_tr_45 = decltype(std::declval<STD_LA::matrix<fs_matrix_engine_tst<double, 5, 4>, test_mul_op_traits_tst>>().t());
    using fsm_new_num_tst_tr_34 = decltype(std::declval<STD_LA::matrix<fs_matrix_engine_tst<new_num, 4, 3>, test_mul_op_traits_tst>>().t());
    using fsm_new_num_tst_tr_45 = decltype(std::declval<STD_LA::matrix<fs_matrix_engine_tst<new_num, 5, 4>, test_mul_op_traits_tst>>().t());

    using drm_double_tst  = STD_LA::matrix<STD_LA::dr_matrix_engine<double, std::allocator<double>>, test_mul_op_traits_tst>;
    using drm_new_num_tst = STD_LA::matrix<STD_LA::dr_matrix_engine<new_num, std::allocator<new_num>>, test_mul_op_traits_tst>;

    using t00 = STD_LA::detail::engine_mul_result_t<test_mul_op_traits_tst,
                                                    fs_matrix_engine_tst<float, 3, 4>,
                                                    fs_matrix_engine_tst<float, 4, 5>>;
    PRINT_TYPE(t00);

    using t01 = STD_LA::detail::engine_mul_result_t<test_mul_op_traits_tst,
                                                    STD_LA::fs_matrix_engine<float, 3, 4>,
                                                    fs_matrix_engine_tst<float, 4, 5>>;
    PRINT_TYPE(t01);

    ASSERT_A_MUL_B_EQ_C(fsm_float_34,      fsm_float_45,      fsm_float_35);
    ASSERT_A_MUL_B_EQ_C(fsm_float_34,      fsm_float_tst_45,  fsm_double_tst_35);
    ASSERT_A_MUL_B_EQ_C(fsm_float_tst_34,  fsm_float_45,      fsm_double_tst_35);
    ASSERT_A_MUL_B_EQ_C(fsm_float_tst_34,  fsm_float_tst_45,  fsm_double_tst_35);

    ASSERT_A_MUL_B_EQ_C(fsm_float_tst_tr_34,  fsm_float_tst_tr_45,  fsm_double_tst_35);
    ASSERT_A_MUL_B_EQ_C(fsm_float_tst_tr_34,  fsm_float_tst_45,     drm_double_tst);
    ASSERT_A_MUL_B_EQ_C(fsm_float_tst_34,     fsm_float_tst_tr_45,  drm_double_tst);

    ASSERT_A_MUL_B_EQ_C(fsm_double_tst_34,   fsm_double_tst_45,   fsm_double_tst_35);
    ASSERT_A_MUL_B_EQ_C(fsm_double_tst_34,   fsm_new_num_tst_45,  fsm_new_num_tst_35);
    ASSERT_A_MUL_B_EQ_C(fsm_new_num_tst_34,  fsm_double_tst_45,   fsm_new_num_tst_35);
    ASSERT_A_MUL_B_EQ_C(fsm_new_num_tst_34,  fsm_new_num_tst_45,  fsm_new_num_tst_35);

    ASSERT_A_MUL_B_EQ_C(fsm_double_tst_tr_34,   fsm_double_tst_tr_45,   fsm_double_tst_35);
    ASSERT_A_MUL_B_EQ_C(fsm_double_tst_tr_34,   fsm_new_num_tst_tr_45,  fsm_new_num_tst_35);
    ASSERT_A_MUL_B_EQ_C(fsm_new_num_tst_tr_34,  fsm_double_tst_tr_45,   fsm_new_num_tst_35);
    ASSERT_A_MUL_B_EQ_C(fsm_new_num_tst_tr_34,  fsm_new_num_tst_tr_45,  fsm_new_num_tst_35);

    ASSERT_A_MUL_B_EQ_C(fsm_double_tst_34,   drm_double_tst,      drm_double_tst);
    ASSERT_A_MUL_B_EQ_C(drm_double_tst,      fsm_new_num_tst_45,  drm_new_num_tst);
    ASSERT_A_MUL_B_EQ_C(fsm_new_num_tst_34,  drm_double_tst,      drm_new_num_tst);
    ASSERT_A_MUL_B_EQ_C(drm_new_num_tst,     fsm_new_num_tst_45,  drm_new_num_tst);

    using fsm_double_tst_57 = STD_LA::matrix<fs_matrix_engine_tst<double, 5, 7>, test_mul_op_traits_tst>;
    using fsm_double_tst_79 = STD_LA::matrix<fs_matrix_engine_tst<double, 7, 9>, test_mul_op_traits_tst>;
    using fsm_double_tst_59 = STD_LA::matrix<fs_matrix_engine_tst<double, 5, 9>, test_mul_op_traits_tst>;

    ASSERT_A_MUL_B_EQ_C(fsm_double_tst_57,  fsm_double_tst_79,  fsm_double_tst_59);
}

void
TestGroup40()
{
    PRINT_FNAME();

    t400();
    t401();
    t402();
    t403();
    t404();
    t405();
    t406();
    t407();
    t408();
    t409();
    t410();
}
#endif
