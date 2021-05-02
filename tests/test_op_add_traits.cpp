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
struct test_add_op_traits_empty
{};


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
    static_assert(std::is_same_v<get_addition_element_traits_t<matrix_operation_traits, int, int>,
                                 matrix_operation_traits::addition_element_traits<matrix_operation_traits, int, int>>);

    static_assert(std::is_same_v<get_addition_engine_traits_t<matrix_operation_traits, eng_t, eng_t>,
                                 matrix_operation_traits::addition_engine_traits<matrix_operation_traits, eng_t, eng_t>>);

    static_assert(std::is_same_v<get_addition_arithmetic_traits_t<matrix_operation_traits, mat_t, mat_t>,
                                 matrix_operation_traits::addition_arithmetic_traits<matrix_operation_traits, mat_t, mat_t>>);

    //- Extracting from an empty operation traits type should yield library results.
    //
    static_assert(std::is_same_v<get_addition_element_traits_t<test_add_op_traits_empty, int, int>,
                                 matrix_operation_traits::addition_element_traits<test_add_op_traits_empty, int, int>>);

    static_assert(std::is_same_v<get_addition_engine_traits_t<test_add_op_traits_empty, eng_t, eng_t>,
                                 matrix_operation_traits::addition_engine_traits<test_add_op_traits_empty, eng_t, eng_t>>);

    static_assert(std::is_same_v<get_addition_arithmetic_traits_t<test_add_op_traits_empty, int, int>,
                                 matrix_operation_traits::addition_arithmetic_traits<test_add_op_traits_empty, int, int>>);

    //- Extracting a nested alias template specialization from a custom operation traits type should
    //  yield the specializations to which those aliases refer.
    //
    static_assert(std::is_same_v<get_addition_element_traits_t<test_add_op_traits_nta, int, int>,
                                 test_element_add_traits_nta<test_add_op_traits_nta, int, int>>);

    static_assert(std::is_same_v<get_addition_engine_traits_t<test_add_op_traits_nta, eng_t, eng_t>,
                                 test_engine_add_traits_nta<test_add_op_traits_nta, eng_t, eng_t>>);

    static_assert(std::is_same_v<get_addition_arithmetic_traits_t<test_add_op_traits_nta, mat_t, mat_t>,
                                 test_arithmetic_add_traits_nta<test_add_op_traits_nta, mat_t, mat_t>>);

    //- Extracting a nested class template specialization from a custom operation traits type should
    //  yield those same nested specializations.
    //
    static_assert(std::is_same_v<get_addition_element_traits_t<test_add_op_traits_nct, int, int>,
                                 test_add_op_traits_nct::addition_element_traits<test_add_op_traits_nct, int, int>>);

    static_assert(std::is_same_v<get_addition_engine_traits_t<test_add_op_traits_nct, eng_t, eng_t>,
                                 test_add_op_traits_nct::addition_engine_traits<test_add_op_traits_nct, eng_t, eng_t>>);

    static_assert(std::is_same_v<get_addition_arithmetic_traits_t<test_add_op_traits_nct, mat_t, mat_t>,
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

