#define ENABLE_TEST_PRINTING
#include "test_common.hpp"

using namespace STD_LA;
using namespace STD_LA::detail;
using namespace MDSPAN_NS;

//- A helper macro to assist in readability of test functions below.
//
#define ASSERT_A_DIV_B_EQ_C(A, B, C)    \
    static_assert(std::is_same_v<decltype(std::declval<A>() / std::declval<B>()), C>)


//--------------------------------------------------------------------------------------------------
//  The following are several traits types used to exercise the element, engine, and operation
//  type detection meta-functions in the private implementation.
//--------------------------------------------------------------------------------------------------
//- This traits type is used to verify that default operations are selected when they are not
//  declared in the operations traits type.
//
struct test_div_op_traits_empty {};


//- This operation traits type is analogous to STD_LA::matrix_operation_traits, where its nested
//  traits types for element/engine/arithmetic are alias templates.
//  Suffix "_nta" means "nested type alias"
//
template<class OT, class T1, class T2>
struct test_element_div_traits_nta
{
    using element_type = dummy_type;
};

template<class OT, class T1, class T2>
struct test_engine_div_traits_nta
{
    using engine_type = dummy_type;
};

template<class OT, class T1, class T2>
struct test_div_traits_nta
{
    using result_type = dummy_type;
};

struct test_div_op_traits_nta
{
    template<class OT, class T1, class T2>
    using division_element_traits = test_element_div_traits_nta<OT, T1, T2>;

    template<class OT, class ET1, class ET2>
    using division_engine_traits = test_engine_div_traits_nta<OT, ET1, ET2>;

    template<class OT, class OP1, class OP2>
    using division_arithmetic_traits = test_div_traits_nta<OT, OP1, OP2>;
};

//- This operation traits type has its element/engine/arithmetic traits as nested class templates.
//  Suffix "_nct" means "nested class type"
//
struct test_div_op_traits_nct
{
    template<class OT, class T1, class T2>
    struct division_element_traits
    {
        using element_type = dummy_type;
    };

    template<class OT, class ET1, class ET2>
    struct division_engine_traits
    {
        using engine_type = dummy_type;
    };

    template<class OT, class OP1, class OP2>
    struct division_arithmetic_traits
    {
        using result_type = dummy_type;
    };
};

//- The following are some invalid operation traits.
//
struct test_div_op_traits_bad_00
{
    using division_element_traits = int;
    using division_engine_traits = int;
    using division_arithmetic_traits = int;
};

struct test_div_op_traits_bad_01
{
    template<class OT>
    using division_element_traits = STD_LA::detail::division_element_traits<OT, int, int>;

    template<class OT>
    using division_engine_traits = STD_LA::detail::division_engine_traits<OT, int, int>;

    template<class OT>
    using division_arithmetic_traits = STD_LA::detail::division_arithmetic_traits<OT, int, int>;
};

struct test_div_op_traits_bad_02
{
    template<class OT, class T1>
    using division_element_traits = STD_LA::detail::division_element_traits<OT, T1, T1>;

    template<class OT, class T1>
    using division_engine_traits = STD_LA::detail::division_engine_traits<OT, T1, T1>;

    template<class OT, class T1>
    using division_arithmetic_traits = STD_LA::detail::division_arithmetic_traits<OT, T1, T1>;
};

//--------------------------------------------------------------------------------------------------
//  This test ensures that the nested traits associated with division are properly validated.
//  It exercises only the validation meta-functions.
//
//  Note that the types for the second and third template parameters are unimportant for these
//  tests - it is the form of the nested template that is being validated (specifically, being
//  instantiable with three template arguments).
//--------------------------------------------------------------------------------------------------
//
TEST(DivTraits, Validation)
{
    static_assert(valid_division_element_traits<test_div_op_traits_empty, int, int>);
    static_assert(valid_division_element_traits<test_div_op_traits_nta, int, int>);
    static_assert(valid_division_element_traits<test_div_op_traits_nct, int, int>);

    static_assert(!valid_division_element_traits<test_div_op_traits_bad_00, int, int>);
    static_assert(!valid_division_element_traits<test_div_op_traits_bad_01, int, int>);
    static_assert(!valid_division_element_traits<test_div_op_traits_bad_02, int, int>);

    static_assert(valid_division_engine_traits<test_div_op_traits_empty, int, int>);
    static_assert(valid_division_engine_traits<test_div_op_traits_nta, int, int>);
    static_assert(valid_division_engine_traits<test_div_op_traits_nct, int, int>);

    static_assert(!valid_division_engine_traits<test_div_op_traits_bad_00, int, int>);
    static_assert(!valid_division_engine_traits<test_div_op_traits_bad_01, int, int>);
    static_assert(!valid_division_engine_traits<test_div_op_traits_bad_02, int, int>);

    static_assert(valid_division_arithmetic_traits<test_div_op_traits_empty, int, int>);
    static_assert(valid_division_arithmetic_traits<test_div_op_traits_nta, int, int>);
    static_assert(valid_division_arithmetic_traits<test_div_op_traits_nct, int, int>);

    static_assert(!valid_division_arithmetic_traits<test_div_op_traits_bad_00, int, int>);
    static_assert(!valid_division_arithmetic_traits<test_div_op_traits_bad_01, int, int>);
    static_assert(!valid_division_arithmetic_traits<test_div_op_traits_bad_02, int, int>);

    static_assert(valid_division_traits<test_div_op_traits_empty, int, int>);
    static_assert(valid_division_traits<test_div_op_traits_nta, int, int>);
    static_assert(valid_division_traits<test_div_op_traits_nct, int, int>);

    static_assert(!valid_division_traits<test_div_op_traits_bad_00, int, int>);
    static_assert(!valid_division_traits<test_div_op_traits_bad_01, int, int>);
    static_assert(!valid_division_traits<test_div_op_traits_bad_02, int, int>);
}


#if 0
//--------------------------------------------------------------------------------------------------
//  This test ensures that the element/engine/arithmetic nested traits contained by *validated*
//  operation traits types are properly extracted.  It exercises only the extraction meta-functions.
//--------------------------------------------------------------------------------------------------
//
TEST(DivTraits, Extraction)
{
    using mat_t = fixed_size_matrix<float, 2, 3>;
    using eng_t = typename mat_t::engine_type;

    //- Extracting from the library's default operation traits should yield library results.
    //
    static_assert(std::is_same_v<get_division_element_traits_t<matrix_operation_traits, int, int>,
                                 matrix_operation_traits::division_element_traits<matrix_operation_traits, int, int>>);

    static_assert(std::is_same_v<get_division_engine_traits_t<matrix_operation_traits, eng_t, eng_t>,
                                 matrix_operation_traits::division_engine_traits<matrix_operation_traits, eng_t, eng_t>>);

    static_assert(std::is_same_v<get_division_arithmetic_traits_t<matrix_operation_traits, mat_t, mat_t>,
                                 matrix_operation_traits::division_arithmetic_traits<matrix_operation_traits, mat_t, mat_t>>);

    //- Extracting from an empty operation traits type should yield library results.
    //
    static_assert(std::is_same_v<get_division_element_traits_t<test_div_op_traits_empty, int, int>,
                                 matrix_operation_traits::division_element_traits<test_div_op_traits_empty, int, int>>);

    static_assert(std::is_same_v<get_division_engine_traits_t<test_div_op_traits_empty, eng_t, eng_t>,
                                 matrix_operation_traits::division_engine_traits<test_div_op_traits_empty, eng_t, eng_t>>);

    static_assert(std::is_same_v<get_division_arithmetic_traits_t<test_div_op_traits_empty, int, int>,
                                 matrix_operation_traits::division_arithmetic_traits<test_div_op_traits_empty, int, int>>);

    //- Extracting a nested alias template specialization from a custom operation traits type should
    //  yield the specializations to which those aliases refer.
    //
    static_assert(std::is_same_v<get_division_element_traits_t<test_div_op_traits_nta, int, int>,
                                 test_element_div_traits_nta<test_div_op_traits_nta, int, int>>);

    static_assert(std::is_same_v<get_division_engine_traits_t<test_div_op_traits_nta, eng_t, eng_t>,
                                 test_engine_div_traits_nta<test_div_op_traits_nta, eng_t, eng_t>>);

    static_assert(std::is_same_v<get_division_arithmetic_traits_t<test_div_op_traits_nta, mat_t, mat_t>,
                                 test_arithmetic_div_traits_nta<test_div_op_traits_nta, mat_t, mat_t>>);

    //- Extracting a nested class template specialization from a custom operation traits type should
    //  yield those same nested specializations.
    //
    static_assert(std::is_same_v<get_division_element_traits_t<test_div_op_traits_nct, int, int>,
                                 test_div_op_traits_nct::division_element_traits<test_div_op_traits_nct, int, int>>);

    static_assert(std::is_same_v<get_division_engine_traits_t<test_div_op_traits_nct, eng_t, eng_t>,
                                 test_div_op_traits_nct::division_engine_traits<test_div_op_traits_nct, eng_t, eng_t>>);

    static_assert(std::is_same_v<get_division_arithmetic_traits_t<test_div_op_traits_nct, mat_t, mat_t>,
                                 test_div_op_traits_nct::division_arithmetic_traits<test_div_op_traits_nct, mat_t, mat_t>>);
}


//--------------------------------------------------------------------------------------------------
//  This test verifies that VECTOR/SCALAR divisions return the correct result type.
//--------------------------------------------------------------------------------------------------
//
void t501()
{
    PRINT_FNAME();

    using fsv_float   = STD_LA::fs_vector<float, 3>;
    using fsv_double  = STD_LA::fs_vector<double, 3>;
    using fsv_new_num = STD_LA::fs_vector<new_num, 3>;

    using drv_float   = STD_LA::dyn_vector<float>;
    using drv_double  = STD_LA::dyn_vector<double>;
    using drv_new_num = STD_LA::dyn_vector<new_num>;

    ASSERT_A_DIV_B_EQ_C(fsv_float,  float,      fsv_float);
    ASSERT_A_DIV_B_EQ_C(fsv_float,  double,     fsv_double);
    ASSERT_A_DIV_B_EQ_C(fsv_float,  new_num,    fsv_new_num);

    ASSERT_A_DIV_B_EQ_C(fsv_double,  float,     fsv_double);
    ASSERT_A_DIV_B_EQ_C(fsv_double,  double,    fsv_double);
    ASSERT_A_DIV_B_EQ_C(fsv_double,  new_num,   fsv_new_num);

    ASSERT_A_DIV_B_EQ_C(fsv_new_num,  float,    fsv_new_num);
    ASSERT_A_DIV_B_EQ_C(fsv_new_num,  double,   fsv_new_num);
    ASSERT_A_DIV_B_EQ_C(fsv_new_num,  new_num,  fsv_new_num);

    //------
    //
    ASSERT_A_DIV_B_EQ_C(drv_float,  float,      drv_float);
    ASSERT_A_DIV_B_EQ_C(drv_float,  double,     drv_double);
    ASSERT_A_DIV_B_EQ_C(drv_float,  new_num,    drv_new_num);

    ASSERT_A_DIV_B_EQ_C(drv_double,  float,     drv_double);
    ASSERT_A_DIV_B_EQ_C(drv_double,  double,    drv_double);
    ASSERT_A_DIV_B_EQ_C(drv_double,  new_num,   drv_new_num);

    ASSERT_A_DIV_B_EQ_C(drv_new_num,  float,    drv_new_num);
    ASSERT_A_DIV_B_EQ_C(drv_new_num,  double,   drv_new_num);
    ASSERT_A_DIV_B_EQ_C(drv_new_num,  new_num,  drv_new_num);
}

//--------------------------------------------------------------------------------------------------
//  This test verifies that MATRIX/SCALAR divisions return the correct result type.
//--------------------------------------------------------------------------------------------------
//
void t502()
{
    PRINT_FNAME();

    using fsm_float   = STD_LA::fs_matrix<float, 3, 4>;
    using fsm_double  = STD_LA::fs_matrix<double, 3, 4>;
    using fsm_new_num = STD_LA::fs_matrix<new_num, 3, 4>;

    using drm_float   = STD_LA::dyn_matrix<float>;
    using drm_double  = STD_LA::dyn_matrix<double>;
    using drm_new_num = STD_LA::dyn_matrix<new_num>;

    ASSERT_A_DIV_B_EQ_C(fsm_float,  float,      fsm_float);
    ASSERT_A_DIV_B_EQ_C(fsm_float,  double,     fsm_double);
    ASSERT_A_DIV_B_EQ_C(fsm_float,  new_num,    fsm_new_num);

    ASSERT_A_DIV_B_EQ_C(fsm_double,  float,     fsm_double);
    ASSERT_A_DIV_B_EQ_C(fsm_double,  double,    fsm_double);
    ASSERT_A_DIV_B_EQ_C(fsm_double,  new_num,   fsm_new_num);

    ASSERT_A_DIV_B_EQ_C(fsm_new_num,  float,    fsm_new_num);
    ASSERT_A_DIV_B_EQ_C(fsm_new_num,  double,   fsm_new_num);
    ASSERT_A_DIV_B_EQ_C(fsm_new_num,  new_num,  fsm_new_num);

    //------
    //
    ASSERT_A_DIV_B_EQ_C(drm_float,  float,      drm_float);
    ASSERT_A_DIV_B_EQ_C(drm_float,  double,     drm_double);
    ASSERT_A_DIV_B_EQ_C(drm_float,  new_num,    drm_new_num);

    ASSERT_A_DIV_B_EQ_C(drm_double,  float,     drm_double);
    ASSERT_A_DIV_B_EQ_C(drm_double,  double,    drm_double);
    ASSERT_A_DIV_B_EQ_C(drm_double,  new_num,   drm_new_num);

    ASSERT_A_DIV_B_EQ_C(drm_new_num,  float,    drm_new_num);
    ASSERT_A_DIV_B_EQ_C(drm_new_num,  double,   drm_new_num);
    ASSERT_A_DIV_B_EQ_C(drm_new_num,  new_num,  drm_new_num);
}

//--------------------------------------------------------------------------------------------------
//  Suffix "_tst" means "test"
//
template<class OT, class T1, class T2>
struct element_div_traits_tst;

//- Promote any float/float division to double.
//
template<class OT>
struct element_div_traits_tst<OT, float, float>
{
    using element_type = double;
};

//------
//
template<class OT, class ET1, class ET2>
struct engine_div_traits_tst;

template<class OT, class T1, size_t R1, size_t C1, class T2>
struct engine_div_traits_tst<OT,
                             fs_matrix_engine_tst<T1, R1, C1>,
                             STD_LA::scalar_engine<T2>>
{
    using element_type = STD_LA::select_matrix_division_element_t<OT, T1, T2>;
    using engine_type  = fs_matrix_engine_tst<element_type, R1, C1>;
};

template<class OT, class T1, size_t R1, size_t C1, class T2>
struct engine_div_traits_tst<OT,
                             STD_LA::fs_matrix_engine<T1, R1, C1>,
                             STD_LA::scalar_engine<T2>>
{
    using element_type = STD_LA::select_matrix_division_element_t<OT, T1, T2>;
    using engine_type  = fs_matrix_engine_tst<element_type, R1, C1>;
};

template<class OT, class T1, size_t R1, size_t C1, class T2>
struct engine_div_traits_tst<OT,
                             STD_LA::matrix_transpose_engine<fs_matrix_engine_tst<T1, R1, C1>>,
                             STD_LA::scalar_engine<T2>>
{
    using element_type = STD_LA::select_matrix_division_element_t<OT, T1, T2>;
    using engine_type  = fs_matrix_engine_tst<element_type, C1, R1>;
};

//------
//
template<class OTR, class OP1, class OP2>
struct division_traits_tst;

template<class OTR>
struct division_traits_tst<OTR,
                           STD_LA::matrix<fs_matrix_engine_tst<double, 5, 7>, OTR>,
                           double>
{
    using op_traits   = OTR;
    using engine_type = STD_LA::select_matrix_division_engine_t<op_traits,
                                                         fs_matrix_engine_tst<double, 5, 7>,
                                                         STD_LA::scalar_engine<double>>;
    using result_type = STD_LA::matrix<engine_type, op_traits>;

    static result_type  divide(STD_LA::matrix<fs_matrix_engine_tst<double, 5, 7>, OTR> const& m1,
                               double s2)
                        {
                            STD_LA::PrintOperandTypes<result_type>("division_traits_TST (M/S)!!", m1, s2);
                            return result_type();
                        }
};

//------
//
struct test_div_op_traits_tst
{
     template<class OT, class T1, class T2>
     using division_element_traits = element_div_traits_tst<OT, T1, T2>;

     template<class OT, class ET1, class ET2>
     using division_engine_traits = engine_div_traits_tst<OT, ET1, ET2>;

     template<class OT, class OP1, class OP2>
     using division_arithmetic_traits = division_traits_tst<OT, OP1, OP2>;
};


void t503()
{
    PRINT_FNAME();

    static_assert(STD_LA::detail::has_element_div_traits_v<test_div_op_traits_tst, float, float>);
    static_assert(!STD_LA::detail::has_element_div_traits_v<test_div_op_traits_tst, float, double>);
    static_assert(!STD_LA::detail::has_element_div_traits_v<test_div_op_traits_tst, double, float>);

    using t00 = STD_LA::detail::element_div_result_t<test_div_op_traits_tst, float, float>;
    PRINT_TYPE(t00);

    using t01 = STD_LA::detail::element_div_result_t<test_div_op_traits_tst, float, double>;
    PRINT_TYPE(t01);
}

void t504()
{
    PRINT_FNAME();

    using fsm_float       = STD_LA::fs_matrix<float, 2, 3>;
    using fsm_double      = STD_LA::fs_matrix<double, 2, 3>;
    using fsm_new_num     = STD_LA::fs_matrix<new_num, 2, 3>;
    using fsm_float_tst   = STD_LA::matrix<fs_matrix_engine_tst<float, 2, 3>, test_div_op_traits_tst>;
    using fsm_double_tst  = STD_LA::matrix<fs_matrix_engine_tst<double, 2, 3>, test_div_op_traits_tst>;
    using fsm_new_num_tst = STD_LA::matrix<fs_matrix_engine_tst<new_num, 2, 3>, test_div_op_traits_tst>;

    using fsm_float_tr       = decltype(std::declval<STD_LA::fs_matrix<float, 3, 2>>().t());
    using fsm_double_tr      = decltype(std::declval<STD_LA::fs_matrix<double, 3, 2>>().t());
    using fsm_new_num_tr     = decltype(std::declval<STD_LA::fs_matrix<new_num, 3, 2>>().t());
    using fsm_float_tst_tr   = decltype(std::declval<STD_LA::matrix<fs_matrix_engine_tst<float, 3, 2>, test_div_op_traits_tst>>().t());
    using fsm_double_tst_tr  = decltype(std::declval<STD_LA::matrix<fs_matrix_engine_tst<double, 3, 2>, test_div_op_traits_tst>>().t());
    using fsm_new_num_tst_tr = decltype(std::declval<STD_LA::matrix<fs_matrix_engine_tst<new_num, 3, 2>, test_div_op_traits_tst>>().t());

    using drm_float_tst   = STD_LA::matrix<STD_LA::dr_matrix_engine<double>, test_div_op_traits_tst>;
    using drm_double_tst  = STD_LA::matrix<STD_LA::dr_matrix_engine<double>, test_div_op_traits_tst>;
    using drm_new_num_tst = STD_LA::matrix<STD_LA::dr_matrix_engine<new_num>, test_div_op_traits_tst>;

    using t00 = STD_LA::detail::engine_div_result_t<test_div_op_traits_tst,
                                                    fs_matrix_engine_tst<float, 3, 4>,
                                                    STD_LA::scalar_engine<float>>;
    PRINT_TYPE(t00);

    using t01 = STD_LA::detail::engine_div_result_t<test_div_op_traits_tst,
                                                    STD_LA::fs_matrix_engine<float, 3, 4>,
                                                    STD_LA::scalar_engine<float>>;
    PRINT_TYPE(t01);

    ASSERT_A_DIV_B_EQ_C(fsm_float,  float,    fsm_float);
    ASSERT_A_DIV_B_EQ_C(fsm_float,  double,   fsm_double);
    ASSERT_A_DIV_B_EQ_C(fsm_float,  new_num,  fsm_new_num);
    ASSERT_A_DIV_B_EQ_C(fsm_double,  float,    fsm_double);
    ASSERT_A_DIV_B_EQ_C(fsm_double,  double,   fsm_double);
    ASSERT_A_DIV_B_EQ_C(fsm_double,  new_num,  fsm_new_num);
    ASSERT_A_DIV_B_EQ_C(fsm_new_num,  float,    fsm_new_num);
    ASSERT_A_DIV_B_EQ_C(fsm_new_num,  double,   fsm_new_num);
    ASSERT_A_DIV_B_EQ_C(fsm_new_num,  new_num,  fsm_new_num);

    ASSERT_A_DIV_B_EQ_C(fsm_float_tst,  float,    fsm_double_tst);
    ASSERT_A_DIV_B_EQ_C(fsm_float_tst,  double,   fsm_double_tst);
    ASSERT_A_DIV_B_EQ_C(fsm_float_tst,  new_num,  fsm_new_num_tst);
    ASSERT_A_DIV_B_EQ_C(fsm_double_tst,  float,    fsm_double_tst);
    ASSERT_A_DIV_B_EQ_C(fsm_double_tst,  double,   fsm_double_tst);
    ASSERT_A_DIV_B_EQ_C(fsm_double_tst,  new_num,  fsm_new_num_tst);
    ASSERT_A_DIV_B_EQ_C(fsm_new_num_tst,  float,    fsm_new_num_tst);
    ASSERT_A_DIV_B_EQ_C(fsm_new_num_tst,  double,   fsm_new_num_tst);
    ASSERT_A_DIV_B_EQ_C(fsm_new_num_tst,  new_num,  fsm_new_num_tst);

    ASSERT_A_DIV_B_EQ_C(fsm_float_tr,  float,    fsm_float);
    ASSERT_A_DIV_B_EQ_C(fsm_float_tr,  double,   fsm_double);
    ASSERT_A_DIV_B_EQ_C(fsm_float_tr,  new_num,  fsm_new_num);
    ASSERT_A_DIV_B_EQ_C(fsm_double_tr,  float,    fsm_double);
    ASSERT_A_DIV_B_EQ_C(fsm_double_tr,  double,   fsm_double);
    ASSERT_A_DIV_B_EQ_C(fsm_double_tr,  new_num,  fsm_new_num);
    ASSERT_A_DIV_B_EQ_C(fsm_new_num_tr,  float,    fsm_new_num);
    ASSERT_A_DIV_B_EQ_C(fsm_new_num_tr,  double,   fsm_new_num);
    ASSERT_A_DIV_B_EQ_C(fsm_new_num_tr,  new_num,  fsm_new_num);

    ASSERT_A_DIV_B_EQ_C(fsm_float_tst_tr,  float,    fsm_double_tst);
    ASSERT_A_DIV_B_EQ_C(fsm_float_tst_tr,  double,   fsm_double_tst);
    ASSERT_A_DIV_B_EQ_C(fsm_float_tst_tr,  new_num,  fsm_new_num_tst);
    ASSERT_A_DIV_B_EQ_C(fsm_double_tst_tr,  float,    fsm_double_tst);
    ASSERT_A_DIV_B_EQ_C(fsm_double_tst_tr,  double,   fsm_double_tst);
    ASSERT_A_DIV_B_EQ_C(fsm_double_tst_tr,  new_num,  fsm_new_num_tst);
    ASSERT_A_DIV_B_EQ_C(fsm_new_num_tst_tr,  float,    fsm_new_num_tst);
    ASSERT_A_DIV_B_EQ_C(fsm_new_num_tst_tr,  double,   fsm_new_num_tst);
    ASSERT_A_DIV_B_EQ_C(fsm_new_num_tst_tr,  new_num,  fsm_new_num_tst);

    ASSERT_A_DIV_B_EQ_C(drm_float_tst,  float,    drm_double_tst);
    ASSERT_A_DIV_B_EQ_C(drm_float_tst,  double,   drm_double_tst);
    ASSERT_A_DIV_B_EQ_C(drm_float_tst,  new_num,  drm_new_num_tst);
    ASSERT_A_DIV_B_EQ_C(drm_double_tst,  float,    drm_double_tst);
    ASSERT_A_DIV_B_EQ_C(drm_double_tst,  double,   drm_double_tst);
    ASSERT_A_DIV_B_EQ_C(drm_double_tst,  new_num,  drm_new_num_tst);
    ASSERT_A_DIV_B_EQ_C(drm_new_num_tst,  float,    drm_new_num_tst);
    ASSERT_A_DIV_B_EQ_C(drm_new_num_tst,  double,   drm_new_num_tst);
    ASSERT_A_DIV_B_EQ_C(drm_new_num_tst,  new_num,  drm_new_num_tst);
}

void
TestGroup50()
{
    PRINT_FNAME();

    t500();
    t501();
    t502();
    t503();
    t504();
}

#endif
