//#define ENABLE_TEST_PRINTING
#include "test_common.hpp"

//- A helper macro to assist in readability of test functions below.
//
#define ASSERT_A_DIV_B_EQ_C(A, B, C)    \
    static_assert(std::is_same_v<decltype(std::declval<A>() / std::declval<B>()), C>)


//--------------------------------------------------------------------------------------------------
//  The following are several traits types used to exercise the element, engine, and operation
//  type detection meta-functions in the private implementation.  See test function t500() below.
//--------------------------------------------------------------------------------------------------
//
//- This traits type is used to verify that default operations are selected when they are not
//  declared in the operations traits type.
//
struct test_div_op_traits_empty {};


//- This operation traits type has its element/engine/arithmetic nested traits type as ordinary
//  (i.e., non-template) type aliases.
//
//  Suffix "_ord" means "ordinary"
//
struct test_element_div_traits_ord
{
    using element_type = dummy_type;
};

struct test_engine_div_traits_ord
{
    using engine_type = dummy_type;
};

struct test_div_traits_ord
{
    using result_type = dummy_type;
};

struct test_div_op_traits_ord
{
    using division_element_traits = test_element_div_traits_ord;
    using division_engine_traits  = test_engine_div_traits_ord;
    using division_arithmetic_traits         = test_div_traits_ord;
};


//- This operation traits type is analogous to STD_LA::matrix_operation_traits, where its nested
//  traits types for element/engine/arithmetic are alias templates.
//
//  Suffix "_nta" means "nested type alias"
//
template<class T1, class T2>
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
    template<class T1, class T2>
    using division_element_traits = test_element_div_traits_nta<T1, T2>;

    template<class OT, class ET1, class ET2>
    using division_engine_traits = test_engine_div_traits_nta<OT, ET1, ET2>;

    template<class OT, class OP1, class OP2>
    using division_arithmetic_traits = test_div_traits_nta<OT, OP1, OP2>;
};


//- This operation traits type has its element/engine/arithmetic traits as nested class templates.
//
//  Suffix "_nct" means "nested class type"
//
struct test_div_op_traits_nct
{
    template<class T1, class T2>
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


//--------------------------------------------------------------------------------------------------
//  This test ensures that the type detection meta-functions are working properly.  It exercises
//  only the detection meta-functions.
//--------------------------------------------------------------------------------------------------
//
void t500()
{
    PRINT_FNAME();

    //- Detect element traits.
    //
    using elem_t = double;

    static_assert(!STD_LA::detail::has_element_div_traits_v<test_div_op_traits_empty, elem_t, elem_t>);
    static_assert(!STD_LA::detail::has_element_div_traits_v<void, elem_t, elem_t>);

    static_assert(STD_LA::detail::has_element_div_traits_v<STD_LA::matrix_operation_traits, elem_t, elem_t>);
    static_assert(STD_LA::detail::has_element_div_traits_v<test_div_op_traits_ord, elem_t, elem_t>);
    static_assert(STD_LA::detail::has_element_div_traits_v<test_div_op_traits_nta, elem_t, elem_t>);
    static_assert(STD_LA::detail::has_element_div_traits_v<test_div_op_traits_nct, elem_t, elem_t>);

    //- Detect engine traits.
    //
    using m_eng_t = STD_LA::dr_matrix_engine<elem_t, std::allocator<elem_t>>;
    using s_eng_t = STD_LA::scalar_engine<elem_t>;

    static_assert(!STD_LA::detail::has_engine_div_traits_v<test_div_op_traits_empty, m_eng_t, s_eng_t>);
    static_assert(!STD_LA::detail::has_engine_div_traits_v<void, m_eng_t, s_eng_t>);

    static_assert(STD_LA::detail::has_engine_div_traits_v<STD_LA::matrix_operation_traits, m_eng_t, s_eng_t>);
    static_assert(STD_LA::detail::has_engine_div_traits_v<test_div_op_traits_ord, m_eng_t, s_eng_t>);
    static_assert(STD_LA::detail::has_engine_div_traits_v<test_div_op_traits_nta, m_eng_t, s_eng_t>);
    static_assert(STD_LA::detail::has_engine_div_traits_v<test_div_op_traits_nct, m_eng_t, s_eng_t>);

    //- Detect operation traits.
    //
    using opnd_t = STD_LA::dyn_matrix<elem_t>;

    static_assert(!STD_LA::detail::has_div_traits_v<test_div_op_traits_empty, opnd_t, elem_t>);
    static_assert(!STD_LA::detail::has_div_traits_v<void, opnd_t, elem_t>);

    static_assert(STD_LA::detail::has_div_traits_v<STD_LA::matrix_operation_traits, opnd_t, elem_t>);
    static_assert(STD_LA::detail::has_div_traits_v<test_div_op_traits_ord, opnd_t, elem_t>);
    static_assert(STD_LA::detail::has_div_traits_v<test_div_op_traits_nta, opnd_t, elem_t>);
    static_assert(STD_LA::detail::has_div_traits_v<test_div_op_traits_nct, opnd_t, elem_t>);
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
template<class T1, class T2>
struct element_div_traits_tst;

//- Promote any float/float division to double.
//
template<>
struct element_div_traits_tst<float, float>
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

template<class OT, class T1, ptrdiff_t R1, ptrdiff_t C1, class T2>
struct engine_div_traits_tst<OT,
                             STD_LA::fs_matrix_engine<T1, R1, C1>,
                             STD_LA::scalar_engine<T2>>
{
    using element_type = STD_LA::select_matrix_division_element_t<OT, T1, T2>;
    using engine_type  = fs_matrix_engine_tst<element_type, R1, C1>;
};

template<class OT, class T1, size_t R1, size_t C1, class MCT1, class T2>
struct engine_div_traits_tst<OT,
                             STD_LA::matrix_transpose_engine<fs_matrix_engine_tst<T1, R1, C1>, MCT1>,
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
     template<class T1, class T2>
     using division_element_traits = element_div_traits_tst<T1, T2>;

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
