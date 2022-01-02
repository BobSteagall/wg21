#define ENABLE_TEST_PRINTING
#include "test_common.hpp"

using namespace STD_LA;
using namespace STD_LA::detail;
using namespace MDSPAN_NS;

//- A helper macro to assist in readability of test functions below.
//
#define ASSERT_A_ADD_B_EQ_C(A, B, C)    \
    static_assert(std::is_same_v<decltype(std::declval<A>() + std::declval<B>()), C>)


//--------------------------------------------------------------------------------------------------
//  The following are several traits types used to exercise the element, engine, and operation
//  type detection meta-functions in the private implementation.
//--------------------------------------------------------------------------------------------------
//- This traits type is used to verify that default operations are selected when they are not
//  declared in the operations traits type.
//
struct test_add_op_traits_empty {};


//- This operation traits type is analogous to STD_LA::matrix_operation_traits, where its nested
//  traits types for element/engine/arithmetic are alias templates.
//  Suffix "_nta" means "nested type alias"
//
template<class OT, class T1, class T2>
struct test_element_add_traits_nta
{
    using element_type = dummy_type;
};

template<class OT, class T1, class T2>
struct test_engine_add_traits_nta
{
    using engine_type = dummy_type;
};

template<class OT, class T1, class T2>
struct test_arithmetic_add_traits_nta
{
    using result_type = dummy_type;
};

struct test_add_op_traits_nta
{
    template<class OT, class T1, class T2>
    using addition_element_traits = test_element_add_traits_nta<OT, T1, T2>;

    template<class OT, class ET1, class ET2>
    using addition_engine_traits = test_engine_add_traits_nta<OT, ET1, ET2>;

    template<class OT, class OP1, class OP2>
    using addition_arithmetic_traits = test_arithmetic_add_traits_nta<OT, OP1, OP2>;
};

//- This operation traits type has its element/engine/arithmetic traits as nested class templates.
//  Suffix "_nct" means "nested class type"
//
struct test_add_op_traits_nct
{
    template<class OT, class T1, class T2>
    struct addition_element_traits
    {
        using element_type = dummy_type;
    };

    template<class OT, class ET1, class ET2>
    struct addition_engine_traits
    {
        using engine_type = dummy_type;
    };

    template<class OT, class OP1, class OP2>
    struct addition_arithmetic_traits
    {
        using result_type = dummy_type;
    };
};

//- The following are some invalid operation traits.
//
struct test_add_op_traits_bad_00
{
    using addition_element_traits = int;
    using addition_engine_traits = int;
    using addition_arithmetic_traits = int;
};

struct test_add_op_traits_bad_01
{
    template<class OT>
    using addition_element_traits = STD_LA::detail::addition_element_traits<OT, int, int>;

    template<class OT>
    using addition_engine_traits = STD_LA::detail::addition_engine_traits<OT, int, int>;

    template<class OT>
    using addition_arithmetic_traits = STD_LA::detail::addition_arithmetic_traits<OT, int, int>;
};

struct test_add_op_traits_bad_02
{
    template<class OT, class T1>
    using addition_element_traits = STD_LA::detail::addition_element_traits<OT, T1, T1>;

    template<class OT, class T1>
    using addition_engine_traits = STD_LA::detail::addition_engine_traits<OT, T1, T1>;

    template<class OT, class T1>
    using addition_arithmetic_traits = STD_LA::detail::addition_arithmetic_traits<OT, T1, T1>;
};

//--------------------------------------------------------------------------------------------------
//  This test ensures that the nested traits associated with addition are properly validated.
//  It exercises only the validation meta-functions.
//
//  Note that the types for the second and third template parameters are unimportant for these
//  tests - it is the form of the nested template that is being validated (specifically, being
//  instantiable with three template arguments).
//--------------------------------------------------------------------------------------------------
//
TEST(AddTraits, Validation)
{
    static_assert(valid_addition_element_traits<test_add_op_traits_empty, int, int>);
    static_assert(valid_addition_element_traits<test_add_op_traits_nta, int, int>);
    static_assert(valid_addition_element_traits<test_add_op_traits_nct, int, int>);

    static_assert(!valid_addition_element_traits<test_add_op_traits_bad_00, int, int>);
    static_assert(!valid_addition_element_traits<test_add_op_traits_bad_01, int, int>);
    static_assert(!valid_addition_element_traits<test_add_op_traits_bad_02, int, int>);

    static_assert(valid_addition_engine_traits<test_add_op_traits_empty, int, int>);
    static_assert(valid_addition_engine_traits<test_add_op_traits_nta, int, int>);
    static_assert(valid_addition_engine_traits<test_add_op_traits_nct, int, int>);

    static_assert(!valid_addition_engine_traits<test_add_op_traits_bad_00, int, int>);
    static_assert(!valid_addition_engine_traits<test_add_op_traits_bad_01, int, int>);
    static_assert(!valid_addition_engine_traits<test_add_op_traits_bad_02, int, int>);

    static_assert(valid_addition_arithmetic_traits<test_add_op_traits_empty, int, int>);
    static_assert(valid_addition_arithmetic_traits<test_add_op_traits_nta, int, int>);
    static_assert(valid_addition_arithmetic_traits<test_add_op_traits_nct, int, int>);

    static_assert(!valid_addition_arithmetic_traits<test_add_op_traits_bad_00, int, int>);
    static_assert(!valid_addition_arithmetic_traits<test_add_op_traits_bad_01, int, int>);
    static_assert(!valid_addition_arithmetic_traits<test_add_op_traits_bad_02, int, int>);

    static_assert(valid_addition_traits<test_add_op_traits_empty, int, int>);
    static_assert(valid_addition_traits<test_add_op_traits_nta, int, int>);
    static_assert(valid_addition_traits<test_add_op_traits_nct, int, int>);

    static_assert(!valid_addition_traits<test_add_op_traits_bad_00, int, int>);
    static_assert(!valid_addition_traits<test_add_op_traits_bad_01, int, int>);
    static_assert(!valid_addition_traits<test_add_op_traits_bad_02, int, int>);
}


//--------------------------------------------------------------------------------------------------
//  This test ensures that the element/engine/arithmetic nested traits contained by *validated*
//  operation traits types are properly extracted.  It exercises only the extraction meta-functions.
//--------------------------------------------------------------------------------------------------
//
TEST(AddTraits, Extraction)
{
    using mat_t = fixed_size_matrix<float, 2, 3>;
    using eng_t = typename mat_t::engine_type;

    //- Extracting from the library's default operation traits should yield library results.
    //
    static_assert(std::is_same_v<extract_addition_element_traits_t<matrix_operation_traits, int, int>,
                                 matrix_operation_traits::addition_element_traits<matrix_operation_traits, int, int>>);

    static_assert(std::is_same_v<extract_addition_engine_traits_t<matrix_operation_traits, eng_t, eng_t>,
                                 matrix_operation_traits::addition_engine_traits<matrix_operation_traits, eng_t, eng_t>>);

    static_assert(std::is_same_v<extract_addition_arithmetic_traits_t<matrix_operation_traits, mat_t, mat_t>,
                                 matrix_operation_traits::addition_arithmetic_traits<matrix_operation_traits, mat_t, mat_t>>);

    //- Extracting from an empty operation traits type should yield library results.
    //
    static_assert(std::is_same_v<extract_addition_element_traits_t<test_add_op_traits_empty, int, int>,
                                 matrix_operation_traits::addition_element_traits<test_add_op_traits_empty, int, int>>);

    static_assert(std::is_same_v<extract_addition_engine_traits_t<test_add_op_traits_empty, eng_t, eng_t>,
                                 matrix_operation_traits::addition_engine_traits<test_add_op_traits_empty, eng_t, eng_t>>);

    static_assert(std::is_same_v<extract_addition_arithmetic_traits_t<test_add_op_traits_empty, int, int>,
                                 matrix_operation_traits::addition_arithmetic_traits<test_add_op_traits_empty, int, int>>);

    //- Extracting a nested alias template specialization from a custom operation traits type should
    //  yield the specializations to which those aliases refer.
    //
    static_assert(std::is_same_v<extract_addition_element_traits_t<test_add_op_traits_nta, int, int>,
                                 test_element_add_traits_nta<test_add_op_traits_nta, int, int>>);

    static_assert(std::is_same_v<extract_addition_engine_traits_t<test_add_op_traits_nta, eng_t, eng_t>,
                                 test_engine_add_traits_nta<test_add_op_traits_nta, eng_t, eng_t>>);

    static_assert(std::is_same_v<extract_addition_arithmetic_traits_t<test_add_op_traits_nta, mat_t, mat_t>,
                                 test_arithmetic_add_traits_nta<test_add_op_traits_nta, mat_t, mat_t>>);

    //- Extracting a nested class template specialization from a custom operation traits type should
    //  yield those same nested specializations.
    //
    static_assert(std::is_same_v<extract_addition_element_traits_t<test_add_op_traits_nct, int, int>,
                                 test_add_op_traits_nct::addition_element_traits<test_add_op_traits_nct, int, int>>);

    static_assert(std::is_same_v<extract_addition_engine_traits_t<test_add_op_traits_nct, eng_t, eng_t>,
                                 test_add_op_traits_nct::addition_engine_traits<test_add_op_traits_nct, eng_t, eng_t>>);

    static_assert(std::is_same_v<extract_addition_arithmetic_traits_t<test_add_op_traits_nct, mat_t, mat_t>,
                                 test_add_op_traits_nct::addition_arithmetic_traits<test_add_op_traits_nct, mat_t, mat_t>>);
}


TEST(AddTraits, EnginePromotion)
{
    PRINT_FNAME();

    using fxd_float   = fixed_size_matrix<float, 2, 3>;
    using fxd_double  = fixed_size_matrix<double, 2, 3>;
    using fxd_new_num = fixed_size_matrix<new_num, 2, 3>;

    using gen_float   = general_matrix<float, 2, 3>;
    using gen_double  = general_matrix<double, 2, 3>;
    using gen_new_num = general_matrix<new_num, 2, 3>;

    using dyn_float   = dynamic_matrix<float>;
    using dyn_double  = dynamic_matrix<double>;
    using dyn_new_num = dynamic_matrix<new_num>;

    PRINT_TYPE(fxd_float);
    PRINT_TYPE(gen_float);
    PRINT_TYPE(dyn_float);

    ASSERT_A_ADD_B_EQ_C(fxd_float,  fxd_float,    fxd_float);
    ASSERT_A_ADD_B_EQ_C(fxd_float,  fxd_double,   fxd_double);
    ASSERT_A_ADD_B_EQ_C(fxd_float,  fxd_new_num,  fxd_new_num);
    ASSERT_A_ADD_B_EQ_C(fxd_float,  gen_float,    gen_float);
    ASSERT_A_ADD_B_EQ_C(fxd_float,  gen_double,   gen_double);
    ASSERT_A_ADD_B_EQ_C(fxd_float,  gen_new_num,  gen_new_num);
    ASSERT_A_ADD_B_EQ_C(fxd_float,  dyn_float,    dyn_float);
    ASSERT_A_ADD_B_EQ_C(fxd_float,  dyn_double,   dyn_double);
    ASSERT_A_ADD_B_EQ_C(fxd_float,  dyn_new_num,  dyn_new_num);

    ASSERT_A_ADD_B_EQ_C(fxd_double,  fxd_float,    fxd_double);
    ASSERT_A_ADD_B_EQ_C(fxd_double,  fxd_double,   fxd_double);
    ASSERT_A_ADD_B_EQ_C(fxd_double,  fxd_new_num,  fxd_new_num);
    ASSERT_A_ADD_B_EQ_C(fxd_double,  gen_float,    gen_double);
    ASSERT_A_ADD_B_EQ_C(fxd_double,  gen_double,   gen_double);
    ASSERT_A_ADD_B_EQ_C(fxd_double,  gen_new_num,  gen_new_num);
    ASSERT_A_ADD_B_EQ_C(fxd_double,  dyn_float,    dyn_double);
    ASSERT_A_ADD_B_EQ_C(fxd_double,  dyn_double,   dyn_double);
    ASSERT_A_ADD_B_EQ_C(fxd_double,  dyn_new_num,  dyn_new_num);

    ASSERT_A_ADD_B_EQ_C(fxd_new_num,  fxd_float,    fxd_new_num);
    ASSERT_A_ADD_B_EQ_C(fxd_new_num,  fxd_double,   fxd_new_num);
    ASSERT_A_ADD_B_EQ_C(fxd_new_num,  fxd_new_num,  fxd_new_num);
    ASSERT_A_ADD_B_EQ_C(fxd_new_num,  gen_float,    gen_new_num);
    ASSERT_A_ADD_B_EQ_C(fxd_new_num,  gen_double,   gen_new_num);
    ASSERT_A_ADD_B_EQ_C(fxd_new_num,  gen_new_num,  gen_new_num);
    ASSERT_A_ADD_B_EQ_C(fxd_new_num,  dyn_float,    dyn_new_num);
    ASSERT_A_ADD_B_EQ_C(fxd_new_num,  dyn_double,   dyn_new_num);
    ASSERT_A_ADD_B_EQ_C(fxd_new_num,  dyn_new_num,  dyn_new_num);

    ASSERT_A_ADD_B_EQ_C(gen_float,  fxd_float,    gen_float);
    ASSERT_A_ADD_B_EQ_C(gen_float,  fxd_double,   gen_double);
    ASSERT_A_ADD_B_EQ_C(gen_float,  fxd_new_num,  gen_new_num);
    ASSERT_A_ADD_B_EQ_C(gen_float,  gen_float,    gen_float);
    ASSERT_A_ADD_B_EQ_C(gen_float,  gen_double,   gen_double);
    ASSERT_A_ADD_B_EQ_C(gen_float,  gen_new_num,  gen_new_num);
    ASSERT_A_ADD_B_EQ_C(gen_float,  dyn_float,    dyn_float);
    ASSERT_A_ADD_B_EQ_C(gen_float,  dyn_double,   dyn_double);
    ASSERT_A_ADD_B_EQ_C(gen_float,  dyn_new_num,  dyn_new_num);

    ASSERT_A_ADD_B_EQ_C(gen_double,  fxd_float,    gen_double);
    ASSERT_A_ADD_B_EQ_C(gen_double,  fxd_double,   gen_double);
    ASSERT_A_ADD_B_EQ_C(gen_double,  fxd_new_num,  gen_new_num);
    ASSERT_A_ADD_B_EQ_C(gen_double,  gen_float,    gen_double);
    ASSERT_A_ADD_B_EQ_C(gen_double,  gen_double,   gen_double);
    ASSERT_A_ADD_B_EQ_C(gen_double,  gen_new_num,  gen_new_num);
    ASSERT_A_ADD_B_EQ_C(gen_double,  dyn_float,    dyn_double);
    ASSERT_A_ADD_B_EQ_C(gen_double,  dyn_double,   dyn_double);
    ASSERT_A_ADD_B_EQ_C(gen_double,  dyn_new_num,  dyn_new_num);

    ASSERT_A_ADD_B_EQ_C(gen_new_num,  fxd_float,    gen_new_num);
    ASSERT_A_ADD_B_EQ_C(gen_new_num,  fxd_double,   gen_new_num);
    ASSERT_A_ADD_B_EQ_C(gen_new_num,  fxd_new_num,  gen_new_num);
    ASSERT_A_ADD_B_EQ_C(gen_new_num,  gen_float,    gen_new_num);
    ASSERT_A_ADD_B_EQ_C(gen_new_num,  gen_double,   gen_new_num);
    ASSERT_A_ADD_B_EQ_C(gen_new_num,  gen_new_num,  gen_new_num);
    ASSERT_A_ADD_B_EQ_C(gen_new_num,  dyn_float,    dyn_new_num);
    ASSERT_A_ADD_B_EQ_C(gen_new_num,  dyn_double,   dyn_new_num);
    ASSERT_A_ADD_B_EQ_C(gen_new_num,  dyn_new_num,  dyn_new_num);

    ASSERT_A_ADD_B_EQ_C(dyn_float,  fxd_float,    dyn_float);
    ASSERT_A_ADD_B_EQ_C(dyn_float,  fxd_double,   dyn_double);
    ASSERT_A_ADD_B_EQ_C(dyn_float,  fxd_new_num,  dyn_new_num);
    ASSERT_A_ADD_B_EQ_C(dyn_float,  gen_float,    dyn_float);
    ASSERT_A_ADD_B_EQ_C(dyn_float,  gen_double,   dyn_double);
    ASSERT_A_ADD_B_EQ_C(dyn_float,  gen_new_num,  dyn_new_num);
    ASSERT_A_ADD_B_EQ_C(dyn_float,  dyn_float,    dyn_float);
    ASSERT_A_ADD_B_EQ_C(dyn_float,  dyn_double,   dyn_double);
    ASSERT_A_ADD_B_EQ_C(dyn_float,  dyn_new_num,  dyn_new_num);

    ASSERT_A_ADD_B_EQ_C(dyn_double,  fxd_float,    dyn_double);
    ASSERT_A_ADD_B_EQ_C(dyn_double,  fxd_double,   dyn_double);
    ASSERT_A_ADD_B_EQ_C(dyn_double,  fxd_new_num,  dyn_new_num);
    ASSERT_A_ADD_B_EQ_C(dyn_double,  gen_float,    dyn_double);
    ASSERT_A_ADD_B_EQ_C(dyn_double,  gen_double,   dyn_double);
    ASSERT_A_ADD_B_EQ_C(dyn_double,  gen_new_num,  dyn_new_num);
    ASSERT_A_ADD_B_EQ_C(dyn_double,  dyn_float,    dyn_double);
    ASSERT_A_ADD_B_EQ_C(dyn_double,  dyn_double,   dyn_double);
    ASSERT_A_ADD_B_EQ_C(dyn_double,  dyn_new_num,  dyn_new_num);

    ASSERT_A_ADD_B_EQ_C(dyn_new_num,  fxd_float,    dyn_new_num);
    ASSERT_A_ADD_B_EQ_C(dyn_new_num,  fxd_double,   dyn_new_num);
    ASSERT_A_ADD_B_EQ_C(dyn_new_num,  fxd_new_num,  dyn_new_num);
    ASSERT_A_ADD_B_EQ_C(dyn_new_num,  gen_float,    dyn_new_num);
    ASSERT_A_ADD_B_EQ_C(dyn_new_num,  gen_double,   dyn_new_num);
    ASSERT_A_ADD_B_EQ_C(dyn_new_num,  gen_new_num,  dyn_new_num);
    ASSERT_A_ADD_B_EQ_C(dyn_new_num,  dyn_float,    dyn_new_num);
    ASSERT_A_ADD_B_EQ_C(dyn_new_num,  dyn_double,   dyn_new_num);
    ASSERT_A_ADD_B_EQ_C(dyn_new_num,  dyn_new_num,  dyn_new_num);
}


//--------------------------------------------------------------------------------------------------
//  Suffix "_tst" means "test"
//
template<class OT, class T1, class T2>
struct element_add_traits_tst;

//- Promote any float/float addition to double.
//
template<class OT>
struct element_add_traits_tst<OT, float, float>
{
    using element_type = double;
};

//------
//
template<class OTR, class ET1, class ET2>
struct engine_add_traits_tst;

template<class OTR, class T1, size_t R1, size_t C1, class T2, size_t R2, size_t C2>
struct engine_add_traits_tst<OTR,
                             fs_matrix_engine_tst<T1, R1, C1>,
                             fs_matrix_engine_tst<T2, R2, C2>>
{
    static_assert(R1 == R2  &&  R1 > 0);
    static_assert(C1 == C2  &&  C1 > 0);
    using element_traits = extract_addition_element_traits_t<OTR, T1, T2>;

    using element_type = typename element_traits::element_type;
    using engine_type  = fs_matrix_engine_tst<element_type, R1, C1>;
};

template<class OTR,
         class T1, size_t R1, size_t C1,
         class T2, size_t R2, size_t C2, class AT, class LT>
requires
    is_valid_fixed_engine_size_v<R2, C2>
struct engine_add_traits_tst<OTR,
                             fs_matrix_engine_tst<T1, R1, C1>,
                             matrix_storage_engine<T2, R2, C2, AT, LT>>
{
    static_assert(R1 == R2);
    static_assert(C1 == C2);
    using element_traits = extract_addition_element_traits_t<OTR, T1, T2>;

    using element_type = typename element_traits::element_type;
    using engine_type  = fs_matrix_engine_tst<element_type, R1, C1>;
};

template<class OTR,
         class T1, size_t R1, size_t C1, class AT, class LT,
         class T2, size_t R2, size_t C2>
requires
    is_valid_fixed_engine_size_v<R1, C1>
struct engine_add_traits_tst<OTR,
                             matrix_storage_engine<T1, R1, C1, AT, LT>,
                             fs_matrix_engine_tst<T2, R2, C2>>
{
    static_assert(R1 == R2);
    static_assert(C1 == C2);
    using element_traits = extract_addition_element_traits_t<OTR, T1, T2>;

    using element_type = typename element_traits::element_type;
    using engine_type  = fs_matrix_engine_tst<element_type, R1, C1>;
};

//------
//
template<class OTR, class OP1, class OP2>
struct addition_traits_tst;

template<class OTR, class COT1, class COT2>
struct addition_traits_tst<OTR,
                              matrix<fs_matrix_engine_tst<double, 3, 4>, COT1>,
                              matrix<fs_matrix_engine_tst<double, 3, 4>, COT2>>
{
    using op_type_1     = matrix<fs_matrix_engine_tst<double, 3, 4>, COT1>;
    using op_type_2     = matrix<fs_matrix_engine_tst<double, 3, 4>, COT2>;
    using op_traits     = OTR;
    using engine_type_1 = typename op_type_1::engine_type;
    using engine_type_2 = typename op_type_2::engine_type;

    using engine_traits = extract_addition_engine_traits_t<op_traits, engine_type_1, engine_type_2>;
    using engine_type   = typename engine_traits::engine_type;
    using result_type   = matrix<engine_type, op_traits>;

    static result_type  add(op_type_1 const& m1, op_type_2 const& m2)
                        {
                            STD_LA::PrintOperandTypes<result_type>("addition_traits_TST", m1, m2);
                            return result_type();
                        }
};

//------
//
struct add_op_traits_tst
{
     template<class OT, class T1, class T2>
     using addition_element_traits = element_add_traits_tst<OT, T1, T2>;

     template<class OT, class ET1, class ET2>
     using addition_engine_traits = engine_add_traits_tst<OT, ET1, ET2>;

     template<class OT, class OP1, class OP2>
     using addition_arithmetic_traits = addition_traits_tst<OT, OP1, OP2>;
};


TEST(AddTraits, CustomTraits)
{
    PRINT_FNAME();

    using fsm_float       = fixed_size_matrix<float, 2, 3>;
    using fsm_float_tst   = matrix<fs_matrix_engine_tst<float, 2, 3>, add_op_traits_tst>;
    using fsm_double_tst  = matrix<fs_matrix_engine_tst<double, 2, 3>, add_op_traits_tst>;
    using fsm_new_num_tst = matrix<fs_matrix_engine_tst<new_num, 2, 3>, add_op_traits_tst>;

    using fsm_float_tst_tr   = decltype(std::declval<matrix<fs_matrix_engine_tst<float, 3, 2>, add_op_traits_tst>>().t());
    using fsm_double_tst_tr  = decltype(std::declval<matrix<fs_matrix_engine_tst<double, 3, 2>, add_op_traits_tst>>().t());
    using fsm_new_num_tst_tr = decltype(std::declval<matrix<fs_matrix_engine_tst<new_num, 3, 2>, add_op_traits_tst>>().t());

    using fsm_double_tst2  = fixed_size_matrix<double, 2, 3, add_op_traits_tst>;
    using fsm_new_num_tst2 = fixed_size_matrix<new_num, 2, 3, add_op_traits_tst>;

    using drm_double_tst  = dynamic_matrix<double, add_op_traits_tst>;
    using drm_new_num_tst = dynamic_matrix<new_num, add_op_traits_tst>;

    ASSERT_A_ADD_B_EQ_C(fsm_float,      fsm_float,      fsm_float);
    ASSERT_A_ADD_B_EQ_C(fsm_float_tst,  fsm_float_tst,  fsm_double_tst);
    ASSERT_A_ADD_B_EQ_C(fsm_float_tst,  fsm_float,      fsm_double_tst);
    ASSERT_A_ADD_B_EQ_C(fsm_float_tst,  fsm_float_tst,  fsm_double_tst);

    ASSERT_A_ADD_B_EQ_C(fsm_float_tst_tr,  fsm_float_tst_tr,  fsm_double_tst2);
    ASSERT_A_ADD_B_EQ_C(fsm_float_tst_tr,  fsm_float_tst,     fsm_double_tst2);
    ASSERT_A_ADD_B_EQ_C(fsm_float_tst,     fsm_float_tst_tr,  fsm_double_tst2);

    ASSERT_A_ADD_B_EQ_C(fsm_double_tst,   fsm_double_tst,   fsm_double_tst);
    ASSERT_A_ADD_B_EQ_C(fsm_double_tst,   fsm_new_num_tst,  fsm_new_num_tst);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num_tst,  fsm_double_tst,   fsm_new_num_tst);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num_tst,  fsm_new_num_tst,  fsm_new_num_tst);

    ASSERT_A_ADD_B_EQ_C(fsm_double_tst_tr,   fsm_double_tst_tr,   fsm_double_tst2);

    PRINT_TYPE(decltype(std::declval<fsm_double_tst_tr>()));
    PRINT_TYPE(decltype(std::declval<fsm_new_num_tst_tr>()));
    PRINT_TYPE(decltype(std::declval<fsm_double_tst_tr>() + std::declval<fsm_double_tst_tr>()));
    PRINT_TYPE(decltype(std::declval<fsm_double_tst_tr>() + std::declval<fsm_new_num_tst_tr>()));
    PRINT_TYPE(decltype(std::declval<fsm_double_tst_tr>() + std::declval<fsm_new_num_tst_tr>()));

    ASSERT_A_ADD_B_EQ_C(fsm_double_tst_tr,   fsm_new_num_tst_tr,  fsm_new_num_tst2);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num_tst_tr,  fsm_double_tst_tr,   fsm_new_num_tst2);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num_tst_tr,  fsm_new_num_tst_tr,  fsm_new_num_tst2);

    ASSERT_A_ADD_B_EQ_C(fsm_double_tst,   drm_double_tst,   drm_double_tst);
    ASSERT_A_ADD_B_EQ_C(drm_double_tst,   fsm_new_num_tst,  drm_new_num_tst);
    ASSERT_A_ADD_B_EQ_C(drm_new_num_tst,  fsm_double_tst,   drm_new_num_tst);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num_tst,  drm_new_num_tst,  drm_new_num_tst);

    using fsm_double_tst_34 = matrix<fs_matrix_engine_tst<double, 3, 4>, add_op_traits_tst>;

    ASSERT_A_ADD_B_EQ_C(fsm_double_tst,     fsm_double_tst,     fsm_double_tst);
    ASSERT_A_ADD_B_EQ_C(fsm_double_tst_34,  fsm_double_tst_34,  fsm_double_tst_34);
}
