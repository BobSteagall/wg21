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

//--------------------------------------------------------------------------------------------------
//- The following are several traits types used to exercise the element, engine, and operation
//  type detection meta-functions.
//
//- This operation traits type is analogous to STD_LA::default_matrix_operations, but with a different name.
//
struct test_mul_op_traits_empty {};


//- This operation traits type has its element/engine/operation nested traits type as ordinary
//  type aliases.
//
//  Suffix "_ord" means "ordinary"
//
struct test_element_mul_traits_ord
{
    using element_type = dummy_type;
};

struct test_engine_mul_traits_ord
{
    using engine_type = dummy_type;
};

struct test_mul_traits_ord
{
    using result_type = dummy_type;
};

struct test_mul_op_traits_ord
{
    using element_multiplication_traits = test_element_mul_traits_ord;
    using engine_multiplication_traits  = test_engine_mul_traits_ord;
    using multiplication_traits         = test_mul_traits_ord;
};


//- This operation traits type is analogous to STD_LA::default_matrix_operation_traits, where its nested
//  traits types for element/engine/operation are template type aliases.
//
//  Suffix "_nta" means "nested type alias"
//
template<class T1, class T2>
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
    template<class T1, class T2>
    using element_multiplication_traits = test_element_mul_traits_nta<T1, T2>;

    template<class OT, class ET1, class ET2>
    using engine_multiplication_traits = test_engine_mul_traits_nta<OT, ET1, ET2>;

    template<class OT, class OP1, class OP2>
    using multiplication_traits = test_mul_traits_nta<OT, OP1, OP2>;
};


//- This operation traits type is has the element/engine/operation traits as nested class templates.
//
//  Suffix "_nct" means "nested class type"
//
struct test_mul_op_traits_nct
{
    template<class T1, class T2>
    struct element_multiplication_traits
    {
        using element_type = dummy_type;
    };

    template<class OT, class ET1, class ET2>
    struct engine_multiplication_traits
    {
        using engine_type = dummy_type;
    };

    template<class OT, class OP1, class OP2>
    struct multiplication_traits
    {
        using result_type = dummy_type;
    };
};


//- This operation traits type is has the element/engine/operation traits as nested class templates.
//- A couple of helper macros to assist in readability below
//
#define ASSERT_A_MULL_B_EQ_C(A, B, C) \
    static_assert(std::is_same_v<decltype(std::declval<A>() * std::declval<B>()), C>)

#define EXEC_A_MUL_B(A, B)  (void)(A() * B())


//--------------------------------------------------------------------------------------------------
//  This test ensures that the type detection meta-functions are working properly.  It exercises
//  only the detection meta-functions.
//--------------------------------------------------------------------------------------------------
//
void t100()
{
    PRINT_FNAME();

    //- Detect element traits.
    //
    using elem_t = double;

    static_assert(!STD_LA::detail::has_element_mul_traits_v<test_mul_op_traits_empty, elem_t, elem_t>);
    static_assert(!STD_LA::detail::has_element_mul_traits_v<STD_LA::default_matrix_operations, elem_t, elem_t>);
    static_assert(!STD_LA::detail::has_element_mul_traits_v<void, elem_t, elem_t>);

    static_assert(STD_LA::detail::has_element_mul_traits_v<STD_LA::default_matrix_operation_traits, elem_t, elem_t>);
    static_assert(STD_LA::detail::has_element_mul_traits_v<test_mul_op_traits_ord, elem_t, elem_t>);
    static_assert(STD_LA::detail::has_element_mul_traits_v<test_mul_op_traits_nta, elem_t, elem_t>);
    static_assert(STD_LA::detail::has_element_mul_traits_v<test_mul_op_traits_nct, elem_t, elem_t>);

    //- Detect engine traits.
    //
    using eng_t = STD_LA::dr_matrix_engine<elem_t, std::allocator<elem_t>>;

    static_assert(!STD_LA::detail::has_engine_mul_traits_v<test_mul_op_traits_empty, eng_t, eng_t>);
    static_assert(!STD_LA::detail::has_engine_mul_traits_v<STD_LA::default_matrix_operations, eng_t, eng_t>);
    static_assert(!STD_LA::detail::has_engine_mul_traits_v<void, eng_t, eng_t>);

    static_assert(STD_LA::detail::has_engine_mul_traits_v<STD_LA::default_matrix_operation_traits, eng_t, eng_t>);
    static_assert(STD_LA::detail::has_engine_mul_traits_v<test_mul_op_traits_ord, eng_t, eng_t>);
    static_assert(STD_LA::detail::has_engine_mul_traits_v<test_mul_op_traits_nta, eng_t, eng_t>);
    static_assert(STD_LA::detail::has_engine_mul_traits_v<test_mul_op_traits_nct, eng_t, eng_t>);

    //- Detect operation traits.
    //
    using opnd_t = STD_LA::dyn_matrix<elem_t>;

    static_assert(!STD_LA::detail::has_mul_traits_v<test_mul_op_traits_empty, opnd_t, opnd_t>);
    static_assert(!STD_LA::detail::has_mul_traits_v<STD_LA::default_matrix_operations, opnd_t, opnd_t>);
    static_assert(!STD_LA::detail::has_mul_traits_v<void, opnd_t, opnd_t>);

    static_assert(STD_LA::detail::has_mul_traits_v<STD_LA::default_matrix_operation_traits, opnd_t, opnd_t>);
    static_assert(STD_LA::detail::has_mul_traits_v<test_mul_op_traits_ord, opnd_t, opnd_t>);
    static_assert(STD_LA::detail::has_mul_traits_v<test_mul_op_traits_nta, opnd_t, opnd_t>);
    static_assert(STD_LA::detail::has_mul_traits_v<test_mul_op_traits_nct, opnd_t, opnd_t>);
}

//--------------------------------------------------------------------------------------------------
//  This test verifies that addition operations on matrices return the correct result type.
//--------------------------------------------------------------------------------------------------
//
void t101()
{
    PRINT_FNAME();

    using fsm_float      = STD_LA::fs_matrix<float, 2, 3>;
    using fsm_double     = STD_LA::fs_matrix<double, 2, 3>;
    using fsm_new_num    = STD_LA::fs_matrix<new_num, 2, 3>;
    using fsm_float_tr   = decltype(std::declval<STD_LA::fs_matrix<float, 3, 2>>().t());
    using fsm_double_tr  = decltype(std::declval<STD_LA::fs_matrix<double, 3, 2>>().t());
    using fsm_new_num_tr = decltype(std::declval<STD_LA::fs_matrix<new_num, 3, 2>>().t());

    using drm_float      = STD_LA::dyn_matrix<float>;
    using drm_double     = STD_LA::dyn_matrix<double>;
    using drm_new_num    = STD_LA::dyn_matrix<new_num>;
    using drm_float_tr   = decltype(std::declval<drm_float>().t());
    using drm_double_tr  = decltype(std::declval<drm_double>().t());
    using drm_new_num_tr = decltype(std::declval<drm_new_num>().t());

    ASSERT_A_MULL_B_EQ_C(fsm_float,  fsm_float,       fsm_float);
    ASSERT_A_MULL_B_EQ_C(fsm_float,  fsm_double,      fsm_double);
    ASSERT_A_MULL_B_EQ_C(fsm_float,  fsm_new_num,     fsm_new_num);
    ASSERT_A_MULL_B_EQ_C(fsm_float,  fsm_float_tr,    fsm_float);
    ASSERT_A_MULL_B_EQ_C(fsm_float,  fsm_double_tr,   fsm_double);
    ASSERT_A_MULL_B_EQ_C(fsm_float,  fsm_new_num_tr,  fsm_new_num);
    ASSERT_A_MULL_B_EQ_C(fsm_float,  drm_float,       drm_float);
    ASSERT_A_MULL_B_EQ_C(fsm_float,  drm_double,      drm_double);
    ASSERT_A_MULL_B_EQ_C(fsm_float,  drm_new_num,     drm_new_num);
    ASSERT_A_MULL_B_EQ_C(fsm_float,  drm_float_tr,    drm_float);
    ASSERT_A_MULL_B_EQ_C(fsm_float,  drm_double_tr,   drm_double);
    ASSERT_A_MULL_B_EQ_C(fsm_float,  drm_new_num_tr,  drm_new_num);

    ASSERT_A_MULL_B_EQ_C(fsm_double,  fsm_float,       fsm_double);
    ASSERT_A_MULL_B_EQ_C(fsm_double,  fsm_double,      fsm_double);
    ASSERT_A_MULL_B_EQ_C(fsm_double,  fsm_new_num,     fsm_new_num);
    ASSERT_A_MULL_B_EQ_C(fsm_double,  fsm_float_tr,    fsm_double);
    ASSERT_A_MULL_B_EQ_C(fsm_double,  fsm_double_tr,   fsm_double);
    ASSERT_A_MULL_B_EQ_C(fsm_double,  fsm_new_num_tr,  fsm_new_num);
    ASSERT_A_MULL_B_EQ_C(fsm_double,  drm_float,       drm_double);
    ASSERT_A_MULL_B_EQ_C(fsm_double,  drm_double,      drm_double);
    ASSERT_A_MULL_B_EQ_C(fsm_double,  drm_new_num,     drm_new_num);
    ASSERT_A_MULL_B_EQ_C(fsm_double,  drm_float_tr,    drm_double);
    ASSERT_A_MULL_B_EQ_C(fsm_double,  drm_double_tr,   drm_double);
    ASSERT_A_MULL_B_EQ_C(fsm_double,  drm_new_num_tr,  drm_new_num);

    ASSERT_A_MULL_B_EQ_C(fsm_new_num,  fsm_float,       fsm_new_num);
    ASSERT_A_MULL_B_EQ_C(fsm_new_num,  fsm_double,      fsm_new_num);
    ASSERT_A_MULL_B_EQ_C(fsm_new_num,  fsm_new_num,     fsm_new_num);
    ASSERT_A_MULL_B_EQ_C(fsm_new_num,  fsm_float_tr,    fsm_new_num);
    ASSERT_A_MULL_B_EQ_C(fsm_new_num,  fsm_double_tr,   fsm_new_num);
    ASSERT_A_MULL_B_EQ_C(fsm_new_num,  fsm_new_num_tr,  fsm_new_num);
    ASSERT_A_MULL_B_EQ_C(fsm_new_num,  drm_float,       drm_new_num);
    ASSERT_A_MULL_B_EQ_C(fsm_new_num,  drm_double,      drm_new_num);
    ASSERT_A_MULL_B_EQ_C(fsm_new_num,  drm_new_num,     drm_new_num);
    ASSERT_A_MULL_B_EQ_C(fsm_new_num,  drm_float_tr,    drm_new_num);
    ASSERT_A_MULL_B_EQ_C(fsm_new_num,  drm_double_tr,   drm_new_num);
    ASSERT_A_MULL_B_EQ_C(fsm_new_num,  drm_new_num_tr,  drm_new_num);

    ASSERT_A_MULL_B_EQ_C(fsm_float_tr,  fsm_float,       fsm_float);
    ASSERT_A_MULL_B_EQ_C(fsm_float_tr,  fsm_double,      fsm_double);
    ASSERT_A_MULL_B_EQ_C(fsm_float_tr,  fsm_new_num,     fsm_new_num);
    ASSERT_A_MULL_B_EQ_C(fsm_float_tr,  fsm_float_tr,    fsm_float);
    ASSERT_A_MULL_B_EQ_C(fsm_float_tr,  fsm_double_tr,   fsm_double);
    ASSERT_A_MULL_B_EQ_C(fsm_float_tr,  fsm_new_num_tr,  fsm_new_num);
    ASSERT_A_MULL_B_EQ_C(fsm_float_tr,  drm_float,       drm_float);
    ASSERT_A_MULL_B_EQ_C(fsm_float_tr,  drm_double,      drm_double);
    ASSERT_A_MULL_B_EQ_C(fsm_float_tr,  drm_new_num,     drm_new_num);
    ASSERT_A_MULL_B_EQ_C(fsm_float_tr,  drm_float_tr,    drm_float);
    ASSERT_A_MULL_B_EQ_C(fsm_float_tr,  drm_double_tr,   drm_double);
    ASSERT_A_MULL_B_EQ_C(fsm_float_tr,  drm_new_num_tr,  drm_new_num);

    ASSERT_A_MULL_B_EQ_C(fsm_double_tr,  fsm_float,       fsm_double);
    ASSERT_A_MULL_B_EQ_C(fsm_double_tr,  fsm_double,      fsm_double);
    ASSERT_A_MULL_B_EQ_C(fsm_double_tr,  fsm_new_num,     fsm_new_num);
    ASSERT_A_MULL_B_EQ_C(fsm_double_tr,  fsm_float_tr,    fsm_double);
    ASSERT_A_MULL_B_EQ_C(fsm_double_tr,  fsm_double_tr,   fsm_double);
    ASSERT_A_MULL_B_EQ_C(fsm_double_tr,  fsm_new_num_tr,  fsm_new_num);
    ASSERT_A_MULL_B_EQ_C(fsm_double_tr,  drm_float,       drm_double);
    ASSERT_A_MULL_B_EQ_C(fsm_double_tr,  drm_double,      drm_double);
    ASSERT_A_MULL_B_EQ_C(fsm_double_tr,  drm_new_num,     drm_new_num);
    ASSERT_A_MULL_B_EQ_C(fsm_double_tr,  drm_float_tr,    drm_double);
    ASSERT_A_MULL_B_EQ_C(fsm_double_tr,  drm_double_tr,   drm_double);
    ASSERT_A_MULL_B_EQ_C(fsm_double_tr,  drm_new_num_tr,  drm_new_num);

    ASSERT_A_MULL_B_EQ_C(fsm_new_num_tr,  fsm_float,       fsm_new_num);
    ASSERT_A_MULL_B_EQ_C(fsm_new_num_tr,  fsm_double,      fsm_new_num);
    ASSERT_A_MULL_B_EQ_C(fsm_new_num_tr,  fsm_new_num,     fsm_new_num);
    ASSERT_A_MULL_B_EQ_C(fsm_new_num_tr,  fsm_float_tr,    fsm_new_num);
    ASSERT_A_MULL_B_EQ_C(fsm_new_num_tr,  fsm_double_tr,   fsm_new_num);
    ASSERT_A_MULL_B_EQ_C(fsm_new_num_tr,  fsm_new_num_tr,  fsm_new_num);
    ASSERT_A_MULL_B_EQ_C(fsm_new_num_tr,  drm_float,       drm_new_num);
    ASSERT_A_MULL_B_EQ_C(fsm_new_num_tr,  drm_double,      drm_new_num);
    ASSERT_A_MULL_B_EQ_C(fsm_new_num_tr,  drm_new_num,     drm_new_num);
    ASSERT_A_MULL_B_EQ_C(fsm_new_num_tr,  drm_float_tr,    drm_new_num);
    ASSERT_A_MULL_B_EQ_C(fsm_new_num_tr,  drm_double_tr,   drm_new_num);
    ASSERT_A_MULL_B_EQ_C(fsm_new_num_tr,  drm_new_num_tr,  drm_new_num);

    //------
    //
    ASSERT_A_MULL_B_EQ_C(drm_float,  fsm_float,       drm_float);
    ASSERT_A_MULL_B_EQ_C(drm_float,  fsm_double,      drm_double);
    ASSERT_A_MULL_B_EQ_C(drm_float,  fsm_new_num,     drm_new_num);
    ASSERT_A_MULL_B_EQ_C(drm_float,  fsm_float_tr,    drm_float);
    ASSERT_A_MULL_B_EQ_C(drm_float,  fsm_double_tr,   drm_double);
    ASSERT_A_MULL_B_EQ_C(drm_float,  fsm_new_num_tr,  drm_new_num);
    ASSERT_A_MULL_B_EQ_C(drm_float,  drm_float,       drm_float);
    ASSERT_A_MULL_B_EQ_C(drm_float,  drm_double,      drm_double);
    ASSERT_A_MULL_B_EQ_C(drm_float,  drm_new_num,     drm_new_num);
    ASSERT_A_MULL_B_EQ_C(drm_float,  drm_float_tr,    drm_float);
    ASSERT_A_MULL_B_EQ_C(drm_float,  drm_double_tr,   drm_double);
    ASSERT_A_MULL_B_EQ_C(drm_float,  drm_new_num_tr,  drm_new_num);

    ASSERT_A_MULL_B_EQ_C(drm_double,  fsm_float,       drm_double);
    ASSERT_A_MULL_B_EQ_C(drm_double,  fsm_double,      drm_double);
    ASSERT_A_MULL_B_EQ_C(drm_double,  fsm_new_num,     drm_new_num);
    ASSERT_A_MULL_B_EQ_C(drm_double,  fsm_float_tr,    drm_double);
    ASSERT_A_MULL_B_EQ_C(drm_double,  fsm_double_tr,   drm_double);
    ASSERT_A_MULL_B_EQ_C(drm_double,  fsm_new_num_tr,  drm_new_num);
    ASSERT_A_MULL_B_EQ_C(drm_double,  drm_float,       drm_double);
    ASSERT_A_MULL_B_EQ_C(drm_double,  drm_double,      drm_double);
    ASSERT_A_MULL_B_EQ_C(drm_double,  drm_new_num,     drm_new_num);
    ASSERT_A_MULL_B_EQ_C(drm_double,  drm_float_tr,    drm_double);
    ASSERT_A_MULL_B_EQ_C(drm_double,  drm_double_tr,   drm_double);
    ASSERT_A_MULL_B_EQ_C(drm_double,  drm_new_num_tr,  drm_new_num);

    ASSERT_A_MULL_B_EQ_C(drm_new_num,  fsm_float,       drm_new_num);
    ASSERT_A_MULL_B_EQ_C(drm_new_num,  fsm_double,      drm_new_num);
    ASSERT_A_MULL_B_EQ_C(drm_new_num,  fsm_new_num,     drm_new_num);
    ASSERT_A_MULL_B_EQ_C(drm_new_num,  fsm_float_tr,    drm_new_num);
    ASSERT_A_MULL_B_EQ_C(drm_new_num,  fsm_double_tr,   drm_new_num);
    ASSERT_A_MULL_B_EQ_C(drm_new_num,  fsm_new_num_tr,  drm_new_num);
    ASSERT_A_MULL_B_EQ_C(drm_new_num,  drm_float,       drm_new_num);
    ASSERT_A_MULL_B_EQ_C(drm_new_num,  drm_double,      drm_new_num);
    ASSERT_A_MULL_B_EQ_C(drm_new_num,  drm_new_num,     drm_new_num);
    ASSERT_A_MULL_B_EQ_C(drm_new_num,  drm_float_tr,    drm_new_num);
    ASSERT_A_MULL_B_EQ_C(drm_new_num,  drm_double_tr,   drm_new_num);
    ASSERT_A_MULL_B_EQ_C(drm_new_num,  drm_new_num_tr,  drm_new_num);

    ASSERT_A_MULL_B_EQ_C(drm_float_tr,  fsm_float,       drm_float);
    ASSERT_A_MULL_B_EQ_C(drm_float_tr,  fsm_double,      drm_double);
    ASSERT_A_MULL_B_EQ_C(drm_float_tr,  fsm_new_num,     drm_new_num);
    ASSERT_A_MULL_B_EQ_C(drm_float_tr,  fsm_float_tr,    drm_float);
    ASSERT_A_MULL_B_EQ_C(drm_float_tr,  fsm_double_tr,   drm_double);
    ASSERT_A_MULL_B_EQ_C(drm_float_tr,  fsm_new_num_tr,  drm_new_num);
    ASSERT_A_MULL_B_EQ_C(drm_float_tr,  drm_float,       drm_float);
    ASSERT_A_MULL_B_EQ_C(drm_float_tr,  drm_double,      drm_double);
    ASSERT_A_MULL_B_EQ_C(drm_float_tr,  drm_new_num,     drm_new_num);
    ASSERT_A_MULL_B_EQ_C(drm_float_tr,  drm_float_tr,    drm_float);
    ASSERT_A_MULL_B_EQ_C(drm_float_tr,  drm_double_tr,   drm_double);
    ASSERT_A_MULL_B_EQ_C(drm_float_tr,  drm_new_num_tr,  drm_new_num);

    ASSERT_A_MULL_B_EQ_C(drm_double_tr,  fsm_float,       drm_double);
    ASSERT_A_MULL_B_EQ_C(drm_double_tr,  fsm_double,      drm_double);
    ASSERT_A_MULL_B_EQ_C(drm_double_tr,  fsm_new_num,     drm_new_num);
    ASSERT_A_MULL_B_EQ_C(drm_double_tr,  fsm_float_tr,    drm_double);
    ASSERT_A_MULL_B_EQ_C(drm_double_tr,  fsm_double_tr,   drm_double);
    ASSERT_A_MULL_B_EQ_C(drm_double_tr,  fsm_new_num_tr,  drm_new_num);
    ASSERT_A_MULL_B_EQ_C(drm_double_tr,  drm_float,       drm_double);
    ASSERT_A_MULL_B_EQ_C(drm_double_tr,  drm_double,      drm_double);
    ASSERT_A_MULL_B_EQ_C(drm_double_tr,  drm_new_num,     drm_new_num);
    ASSERT_A_MULL_B_EQ_C(drm_double_tr,  drm_float_tr,    drm_double);
    ASSERT_A_MULL_B_EQ_C(drm_double_tr,  drm_double_tr,   drm_double);
    ASSERT_A_MULL_B_EQ_C(drm_double_tr,  drm_new_num_tr,  drm_new_num);

    ASSERT_A_MULL_B_EQ_C(drm_new_num_tr,  fsm_float,       drm_new_num);
    ASSERT_A_MULL_B_EQ_C(drm_new_num_tr,  fsm_double,      drm_new_num);
    ASSERT_A_MULL_B_EQ_C(drm_new_num_tr,  fsm_new_num,     drm_new_num);
    ASSERT_A_MULL_B_EQ_C(drm_new_num_tr,  fsm_float_tr,    drm_new_num);
    ASSERT_A_MULL_B_EQ_C(drm_new_num_tr,  fsm_double_tr,   drm_new_num);
    ASSERT_A_MULL_B_EQ_C(drm_new_num_tr,  fsm_new_num_tr,  drm_new_num);
    ASSERT_A_MULL_B_EQ_C(drm_new_num_tr,  drm_float,       drm_new_num);
    ASSERT_A_MULL_B_EQ_C(drm_new_num_tr,  drm_double,      drm_new_num);
    ASSERT_A_MULL_B_EQ_C(drm_new_num_tr,  drm_new_num,     drm_new_num);
    ASSERT_A_MULL_B_EQ_C(drm_new_num_tr,  drm_float_tr,    drm_new_num);
    ASSERT_A_MULL_B_EQ_C(drm_new_num_tr,  drm_double_tr,   drm_new_num);
    ASSERT_A_MULL_B_EQ_C(drm_new_num_tr,  drm_new_num_tr,  drm_new_num);

    STD_LA::fs_matrix<double, 3, 3>     fm1;
    STD_LA::dyn_matrix<double>          dmd1(3, 3);

    cout << get_type_name<STD_LA::dyn_matrix<float>>() << endl;
    cout << get_type_name<STD_LA::dyn_matrix<double>>() << endl;
    cout << get_type_name<decltype(fm1 + dmd1)>() << endl;
    cout << get_type_name<decltype(fm1.t() + dmd1.t())>() << endl;
    fm1 * dmd1;

    PRINT_TYPE(fsm_float);
    PRINT_TYPE(fsm_double);
    PRINT_TYPE(fsm_new_num);
    cout << endl;

    PRINT_TYPE(fsm_float_tr);
    PRINT_TYPE(fsm_double_tr);
    PRINT_TYPE(fsm_new_num_tr);
    cout << endl;

    PRINT_TYPE(drm_float);
    PRINT_TYPE(drm_double);
    PRINT_TYPE(drm_new_num);
    cout << endl;

    PRINT_TYPE(drm_float_tr);
    PRINT_TYPE(drm_double_tr);
    PRINT_TYPE(drm_new_num_tr);
    cout << endl;
}

//--------------------------------------------------------------------------------------------------
//  This test verifies that addition operations on matrices actually execute.  It prints the
//  operand and result types for manual review.
//--------------------------------------------------------------------------------------------------
//
void t102()
{
    PRINT_FNAME();

    using fsm_float      = STD_LA::fs_matrix<float, 2, 3>;
    using fsm_double     = STD_LA::fs_matrix<double, 2, 3>;
    using fsm_new_num    = STD_LA::fs_matrix<new_num, 2, 3>;
    using fsm_float_tr   = decltype(std::declval<STD_LA::fs_matrix<float, 3, 2>>().t());
    using fsm_double_tr  = decltype(std::declval<STD_LA::fs_matrix<double, 3, 2>>().t());
    using fsm_new_num_tr = decltype(std::declval<STD_LA::fs_matrix<new_num, 3, 2>>().t());

    using drm_float      = STD_LA::dyn_matrix<float>;
    using drm_double     = STD_LA::dyn_matrix<double>;
    using drm_new_num    = STD_LA::dyn_matrix<new_num>;
    using drm_float_tr   = decltype(std::declval<drm_float>().t());
    using drm_double_tr  = decltype(std::declval<drm_double>().t());
    using drm_new_num_tr = decltype(std::declval<drm_new_num>().t());

    EXEC_A_MUL_B(fsm_float,  fsm_float);
    EXEC_A_MUL_B(fsm_float,  fsm_double);
    EXEC_A_MUL_B(fsm_float,  fsm_new_num);
    EXEC_A_MUL_B(fsm_float,  fsm_float_tr);
    EXEC_A_MUL_B(fsm_float,  fsm_double_tr);
    EXEC_A_MUL_B(fsm_float,  fsm_new_num_tr);
    EXEC_A_MUL_B(fsm_float,  drm_float);
    EXEC_A_MUL_B(fsm_float,  drm_double);
    EXEC_A_MUL_B(fsm_float,  drm_new_num);
    EXEC_A_MUL_B(fsm_float,  drm_float_tr);
    EXEC_A_MUL_B(fsm_float,  drm_double_tr);
    EXEC_A_MUL_B(fsm_float,  drm_new_num_tr);

    EXEC_A_MUL_B(fsm_double,  fsm_float);
    EXEC_A_MUL_B(fsm_double,  fsm_double);
    EXEC_A_MUL_B(fsm_double,  fsm_new_num);
    EXEC_A_MUL_B(fsm_double,  fsm_float_tr);
    EXEC_A_MUL_B(fsm_double,  fsm_double_tr);
    EXEC_A_MUL_B(fsm_double,  fsm_new_num_tr);
    EXEC_A_MUL_B(fsm_double,  drm_float);
    EXEC_A_MUL_B(fsm_double,  drm_double);
    EXEC_A_MUL_B(fsm_double,  drm_new_num);
    EXEC_A_MUL_B(fsm_double,  drm_float_tr);
    EXEC_A_MUL_B(fsm_double,  drm_double_tr);
    EXEC_A_MUL_B(fsm_double,  drm_new_num_tr);

    EXEC_A_MUL_B(fsm_new_num,  fsm_float);
    EXEC_A_MUL_B(fsm_new_num,  fsm_double);
    EXEC_A_MUL_B(fsm_new_num,  fsm_new_num);
    EXEC_A_MUL_B(fsm_new_num,  fsm_float_tr);
    EXEC_A_MUL_B(fsm_new_num,  fsm_double_tr);
    EXEC_A_MUL_B(fsm_new_num,  fsm_new_num_tr);
    EXEC_A_MUL_B(fsm_new_num,  drm_float);
    EXEC_A_MUL_B(fsm_new_num,  drm_double);
    EXEC_A_MUL_B(fsm_new_num,  drm_new_num);
    EXEC_A_MUL_B(fsm_new_num,  drm_float_tr);
    EXEC_A_MUL_B(fsm_new_num,  drm_double_tr);
    EXEC_A_MUL_B(fsm_new_num,  drm_new_num_tr);

    EXEC_A_MUL_B(fsm_float_tr,  fsm_float);
    EXEC_A_MUL_B(fsm_float_tr,  fsm_double);
    EXEC_A_MUL_B(fsm_float_tr,  fsm_new_num);
    EXEC_A_MUL_B(fsm_float_tr,  fsm_float_tr);
    EXEC_A_MUL_B(fsm_float_tr,  fsm_double_tr);
    EXEC_A_MUL_B(fsm_float_tr,  fsm_new_num_tr);
    EXEC_A_MUL_B(fsm_float_tr,  drm_float);
    EXEC_A_MUL_B(fsm_float_tr,  drm_double);
    EXEC_A_MUL_B(fsm_float_tr,  drm_new_num);
    EXEC_A_MUL_B(fsm_float_tr,  drm_float_tr);
    EXEC_A_MUL_B(fsm_float_tr,  drm_double_tr);
    EXEC_A_MUL_B(fsm_float_tr,  drm_new_num_tr);

    EXEC_A_MUL_B(fsm_double_tr,  fsm_float);
    EXEC_A_MUL_B(fsm_double_tr,  fsm_double);
    EXEC_A_MUL_B(fsm_double_tr,  fsm_new_num);
    EXEC_A_MUL_B(fsm_double_tr,  fsm_float_tr);
    EXEC_A_MUL_B(fsm_double_tr,  fsm_double_tr);
    EXEC_A_MUL_B(fsm_double_tr,  fsm_new_num_tr);
    EXEC_A_MUL_B(fsm_double_tr,  drm_float);
    EXEC_A_MUL_B(fsm_double_tr,  drm_double);
    EXEC_A_MUL_B(fsm_double_tr,  drm_new_num);
    EXEC_A_MUL_B(fsm_double_tr,  drm_float_tr);
    EXEC_A_MUL_B(fsm_double_tr,  drm_double_tr);
    EXEC_A_MUL_B(fsm_double_tr,  drm_new_num_tr);

    EXEC_A_MUL_B(fsm_new_num_tr,  fsm_float);
    EXEC_A_MUL_B(fsm_new_num_tr,  fsm_double);
    EXEC_A_MUL_B(fsm_new_num_tr,  fsm_new_num);
    EXEC_A_MUL_B(fsm_new_num_tr,  fsm_float_tr);
    EXEC_A_MUL_B(fsm_new_num_tr,  fsm_double_tr);
    EXEC_A_MUL_B(fsm_new_num_tr,  fsm_new_num_tr);
    EXEC_A_MUL_B(fsm_new_num_tr,  drm_float);
    EXEC_A_MUL_B(fsm_new_num_tr,  drm_double);
    EXEC_A_MUL_B(fsm_new_num_tr,  drm_new_num);
    EXEC_A_MUL_B(fsm_new_num_tr,  drm_float_tr);
    EXEC_A_MUL_B(fsm_new_num_tr,  drm_double_tr);
    EXEC_A_MUL_B(fsm_new_num_tr,  drm_new_num_tr);

    //------
    //
    EXEC_A_MUL_B(drm_float,  fsm_float);
    EXEC_A_MUL_B(drm_float,  fsm_double);
    EXEC_A_MUL_B(drm_float,  fsm_new_num);
    EXEC_A_MUL_B(drm_float,  fsm_float_tr);
    EXEC_A_MUL_B(drm_float,  fsm_double_tr);
    EXEC_A_MUL_B(drm_float,  fsm_new_num_tr);
    EXEC_A_MUL_B(drm_float,  drm_float);
    EXEC_A_MUL_B(drm_float,  drm_double);
    EXEC_A_MUL_B(drm_float,  drm_new_num);
    EXEC_A_MUL_B(drm_float,  drm_float_tr);
    EXEC_A_MUL_B(drm_float,  drm_double_tr);
    EXEC_A_MUL_B(drm_float,  drm_new_num_tr);

    EXEC_A_MUL_B(drm_double,  fsm_float);
    EXEC_A_MUL_B(drm_double,  fsm_double);
    EXEC_A_MUL_B(drm_double,  fsm_new_num);
    EXEC_A_MUL_B(drm_double,  fsm_float_tr);
    EXEC_A_MUL_B(drm_double,  fsm_double_tr);
    EXEC_A_MUL_B(drm_double,  fsm_new_num_tr);
    EXEC_A_MUL_B(drm_double,  drm_float);
    EXEC_A_MUL_B(drm_double,  drm_double);
    EXEC_A_MUL_B(drm_double,  drm_new_num);
    EXEC_A_MUL_B(drm_double,  drm_float_tr);
    EXEC_A_MUL_B(drm_double,  drm_double_tr);
    EXEC_A_MUL_B(drm_double,  drm_new_num_tr);

    EXEC_A_MUL_B(drm_new_num,  fsm_float);
    EXEC_A_MUL_B(drm_new_num,  fsm_double);
    EXEC_A_MUL_B(drm_new_num,  fsm_new_num);
    EXEC_A_MUL_B(drm_new_num,  fsm_float_tr);
    EXEC_A_MUL_B(drm_new_num,  fsm_double_tr);
    EXEC_A_MUL_B(drm_new_num,  fsm_new_num_tr);
    EXEC_A_MUL_B(drm_new_num,  drm_float);
    EXEC_A_MUL_B(drm_new_num,  drm_double);
    EXEC_A_MUL_B(drm_new_num,  drm_new_num);
    EXEC_A_MUL_B(drm_new_num,  drm_float_tr);
    EXEC_A_MUL_B(drm_new_num,  drm_double_tr);
    EXEC_A_MUL_B(drm_new_num,  drm_new_num_tr);

    EXEC_A_MUL_B(drm_float_tr,  fsm_float);
    EXEC_A_MUL_B(drm_float_tr,  fsm_double);
    EXEC_A_MUL_B(drm_float_tr,  fsm_new_num);
    EXEC_A_MUL_B(drm_float_tr,  fsm_float_tr);
    EXEC_A_MUL_B(drm_float_tr,  fsm_double_tr);
    EXEC_A_MUL_B(drm_float_tr,  fsm_new_num_tr);
    EXEC_A_MUL_B(drm_float_tr,  drm_float);
    EXEC_A_MUL_B(drm_float_tr,  drm_double);
    EXEC_A_MUL_B(drm_float_tr,  drm_new_num);
    EXEC_A_MUL_B(drm_float_tr,  drm_float_tr);
    EXEC_A_MUL_B(drm_float_tr,  drm_double_tr);
    EXEC_A_MUL_B(drm_float_tr,  drm_new_num_tr);

    EXEC_A_MUL_B(drm_double_tr,  fsm_float);
    EXEC_A_MUL_B(drm_double_tr,  fsm_double);
    EXEC_A_MUL_B(drm_double_tr,  fsm_new_num);
    EXEC_A_MUL_B(drm_double_tr,  fsm_float_tr);
    EXEC_A_MUL_B(drm_double_tr,  fsm_double_tr);
    EXEC_A_MUL_B(drm_double_tr,  fsm_new_num_tr);
    EXEC_A_MUL_B(drm_double_tr,  drm_float);
    EXEC_A_MUL_B(drm_double_tr,  drm_double);
    EXEC_A_MUL_B(drm_double_tr,  drm_new_num);
    EXEC_A_MUL_B(drm_double_tr,  drm_float_tr);
    EXEC_A_MUL_B(drm_double_tr,  drm_double_tr);
    EXEC_A_MUL_B(drm_double_tr,  drm_new_num_tr);

    EXEC_A_MUL_B(drm_new_num_tr,  fsm_float);
    EXEC_A_MUL_B(drm_new_num_tr,  fsm_double);
    EXEC_A_MUL_B(drm_new_num_tr,  fsm_new_num);
    EXEC_A_MUL_B(drm_new_num_tr,  fsm_float_tr);
    EXEC_A_MUL_B(drm_new_num_tr,  fsm_double_tr);
    EXEC_A_MUL_B(drm_new_num_tr,  fsm_new_num_tr);
    EXEC_A_MUL_B(drm_new_num_tr,  drm_float);
    EXEC_A_MUL_B(drm_new_num_tr,  drm_double);
    EXEC_A_MUL_B(drm_new_num_tr,  drm_new_num);
    EXEC_A_MUL_B(drm_new_num_tr,  drm_float_tr);
    EXEC_A_MUL_B(drm_new_num_tr,  drm_double_tr);
    EXEC_A_MUL_B(drm_new_num_tr,  drm_new_num_tr);
}

//--------------------------------------------------------------------------------------------------
//  This test verifies that addition operations on vectors return the correct result type.
//--------------------------------------------------------------------------------------------------
//
void t103()
{
    PRINT_FNAME();

    using fsv_float      = STD_LA::fs_vector<float, 3>;
    using fsv_double     = STD_LA::fs_vector<double, 3>;
    using fsv_new_num    = STD_LA::fs_vector<new_num, 3>;

    using drv_float      = STD_LA::dyn_vector<float>;
    using drv_double     = STD_LA::dyn_vector<double>;
    using drv_new_num    = STD_LA::dyn_vector<new_num>;

    ASSERT_A_MULL_B_EQ_C(fsv_float,  fsv_float,       float);
    ASSERT_A_MULL_B_EQ_C(fsv_float,  fsv_double,      double);
    ASSERT_A_MULL_B_EQ_C(fsv_float,  fsv_new_num,     new_num);
    ASSERT_A_MULL_B_EQ_C(fsv_float,  drv_float,       float);
    ASSERT_A_MULL_B_EQ_C(fsv_float,  drv_double,      double);
    ASSERT_A_MULL_B_EQ_C(fsv_float,  drv_new_num,     new_num);

    ASSERT_A_MULL_B_EQ_C(fsv_double,  fsv_float,       double);
    ASSERT_A_MULL_B_EQ_C(fsv_double,  fsv_double,      double);
    ASSERT_A_MULL_B_EQ_C(fsv_double,  fsv_new_num,     new_num);
    ASSERT_A_MULL_B_EQ_C(fsv_double,  drv_float,       double);
    ASSERT_A_MULL_B_EQ_C(fsv_double,  drv_double,      double);
    ASSERT_A_MULL_B_EQ_C(fsv_double,  drv_new_num,     new_num);

    ASSERT_A_MULL_B_EQ_C(fsv_new_num,  fsv_float,       new_num);
    ASSERT_A_MULL_B_EQ_C(fsv_new_num,  fsv_double,      new_num);
    ASSERT_A_MULL_B_EQ_C(fsv_new_num,  fsv_new_num,     new_num);
    ASSERT_A_MULL_B_EQ_C(fsv_new_num,  drv_float,       new_num);
    ASSERT_A_MULL_B_EQ_C(fsv_new_num,  drv_double,      new_num);
    ASSERT_A_MULL_B_EQ_C(fsv_new_num,  drv_new_num,     new_num);

    //------
    //
    ASSERT_A_MULL_B_EQ_C(drv_float,  fsv_float,       float);
    ASSERT_A_MULL_B_EQ_C(drv_float,  fsv_double,      double);
    ASSERT_A_MULL_B_EQ_C(drv_float,  fsv_new_num,     new_num);
    ASSERT_A_MULL_B_EQ_C(drv_float,  drv_float,       float);
    ASSERT_A_MULL_B_EQ_C(drv_float,  drv_double,      double);
    ASSERT_A_MULL_B_EQ_C(drv_float,  drv_new_num,     new_num);

    ASSERT_A_MULL_B_EQ_C(drv_double,  fsv_float,       double);
    ASSERT_A_MULL_B_EQ_C(drv_double,  fsv_double,      double);
    ASSERT_A_MULL_B_EQ_C(drv_double,  fsv_new_num,     new_num);
    ASSERT_A_MULL_B_EQ_C(drv_double,  drv_float,       double);
    ASSERT_A_MULL_B_EQ_C(drv_double,  drv_double,      double);
    ASSERT_A_MULL_B_EQ_C(drv_double,  drv_new_num,     new_num);

    ASSERT_A_MULL_B_EQ_C(drv_new_num,  fsv_float,       new_num);
    ASSERT_A_MULL_B_EQ_C(drv_new_num,  fsv_double,      new_num);
    ASSERT_A_MULL_B_EQ_C(drv_new_num,  fsv_new_num,     new_num);
    ASSERT_A_MULL_B_EQ_C(drv_new_num,  drv_float,       new_num);
    ASSERT_A_MULL_B_EQ_C(drv_new_num,  drv_double,      new_num);
    ASSERT_A_MULL_B_EQ_C(drv_new_num,  drv_new_num,     new_num);

    PRINT_TYPE(fsv_float);
    PRINT_TYPE(fsv_double);
    PRINT_TYPE(fsv_new_num);
    cout << endl;

    PRINT_TYPE(drv_float);
    PRINT_TYPE(drv_double);
    PRINT_TYPE(drv_new_num);
    cout << endl;
}

//--------------------------------------------------------------------------------------------------
//  This test verifies that addition operations on vectors actually execute.  It prints the
//  operand and result types for manual review.
//--------------------------------------------------------------------------------------------------
//
void t104()
{
    PRINT_FNAME();

    using fsv_float      = STD_LA::fs_vector<float, 3>;
    using fsv_double     = STD_LA::fs_vector<double, 3>;
    using fsv_new_num    = STD_LA::fs_vector<new_num, 3>;

    using drv_float      = STD_LA::dyn_vector<float>;
    using drv_double     = STD_LA::dyn_vector<double>;
    using drv_new_num    = STD_LA::dyn_vector<new_num>;

    EXEC_A_MUL_B(fsv_float,  fsv_float);
    EXEC_A_MUL_B(fsv_float,  fsv_double);
    EXEC_A_MUL_B(fsv_float,  fsv_new_num);
    EXEC_A_MUL_B(fsv_float,  drv_float);
    EXEC_A_MUL_B(fsv_float,  drv_double);
    EXEC_A_MUL_B(fsv_float,  drv_new_num);

    EXEC_A_MUL_B(fsv_double,  fsv_float);
    EXEC_A_MUL_B(fsv_double,  fsv_double);
    EXEC_A_MUL_B(fsv_double,  fsv_new_num);
    EXEC_A_MUL_B(fsv_double,  drv_float);
    EXEC_A_MUL_B(fsv_double,  drv_double);
    EXEC_A_MUL_B(fsv_double,  drv_new_num);

    EXEC_A_MUL_B(fsv_new_num,  fsv_float);
    EXEC_A_MUL_B(fsv_new_num,  fsv_double);
    EXEC_A_MUL_B(fsv_new_num,  fsv_new_num);
    EXEC_A_MUL_B(fsv_new_num,  drv_float);
    EXEC_A_MUL_B(fsv_new_num,  drv_double);
    EXEC_A_MUL_B(fsv_new_num,  drv_new_num);

    //------
    //
    EXEC_A_MUL_B(drv_float,  fsv_float);
    EXEC_A_MUL_B(drv_float,  fsv_double);
    EXEC_A_MUL_B(drv_float,  fsv_new_num);
    EXEC_A_MUL_B(drv_float,  drv_float);
    EXEC_A_MUL_B(drv_float,  drv_double);
    EXEC_A_MUL_B(drv_float,  drv_new_num);

    EXEC_A_MUL_B(drv_double,  fsv_float);
    EXEC_A_MUL_B(drv_double,  fsv_double);
    EXEC_A_MUL_B(drv_double,  fsv_new_num);
    EXEC_A_MUL_B(drv_double,  drv_float);
    EXEC_A_MUL_B(drv_double,  drv_double);
    EXEC_A_MUL_B(drv_double,  drv_new_num);

    EXEC_A_MUL_B(drv_new_num,  fsv_float);
    EXEC_A_MUL_B(drv_new_num,  fsv_double);
    EXEC_A_MUL_B(drv_new_num,  fsv_new_num);
    EXEC_A_MUL_B(drv_new_num,  drv_float);
    EXEC_A_MUL_B(drv_new_num,  drv_double);
    EXEC_A_MUL_B(drv_new_num,  drv_new_num);
}


//--------------------------------------------------------------------------------------------------
//  Suffix "_tst" means "test"
//
template<class T1, class T2>
struct element_mul_traits_tst;

//- Promote any float/float addition to double.
//
template<>
struct element_mul_traits_tst<float, float>
{
    using element_type = double;
};

//------
//
template<class OT, class ET1, class ET2>
struct engine_mul_traits_tst;

template<class OT, class T1, int32_t R1, int32_t C1, class T2, int32_t R2, int32_t C2>
struct engine_mul_traits_tst<OT,
                             fs_matrix_engine_tst<T1, R1, C1>,
                             fs_matrix_engine_tst<T2, R2, C2>>
{
    static_assert(R1 == R2);
    static_assert(C1 == C2);
    using element_type = STD_LA::matrix_multiplication_element_t<OT, T1, T2>;
    using engine_type  = fs_matrix_engine_tst<element_type, R1, C1>;
};

template<class OT, class T1, int32_t R1, int32_t C1, class T2, int32_t R2, int32_t C2>
struct engine_mul_traits_tst<OT,
                             STD_LA::fs_matrix_engine<T1, R1, C1>,
                             STD_LA::fs_matrix_engine<T2, R2, C2>>
{
    static_assert(R1 == R2);
    static_assert(C1 == C2);
    using element_type = STD_LA::matrix_multiplication_element_t<OT, T1, T2>;
    using engine_type  = fs_matrix_engine_tst<element_type, R1, C1>;
};

template<class OT, class T1, int32_t R1, int32_t C1, class T2, int32_t R2, int32_t C2>
struct engine_mul_traits_tst<OT,
                             STD_LA::tr_matrix_engine<fs_matrix_engine_tst<T1, R1, C1>>,
                             STD_LA::tr_matrix_engine<fs_matrix_engine_tst<T2, R2, C2>>>
{
    static_assert(R1 == R2);
    static_assert(C1 == C2);
    using element_type = STD_LA::matrix_multiplication_element_t<OT, T1, T2>;
    using engine_type  = fs_matrix_engine_tst<element_type, C1, R1>;
};

//------
//
template<class OTR, class OP1, class OP2>
struct multiplication_traits_tst;

template<class OTR>
struct multiplication_traits_tst<OTR,
                           STD_LA::matrix<fs_matrix_engine_tst<double, 3, 4>, OTR>,
                           STD_LA::matrix<fs_matrix_engine_tst<double, 3, 4>, OTR>>
{
    using op_traits   = OTR;
    using engine_type = STD_LA::matrix_multiplication_engine_t<op_traits,
                                                         fs_matrix_engine_tst<double, 3, 4>,
                                                         fs_matrix_engine_tst<double, 3, 4>>;
    using result_type = STD_LA::matrix<engine_type, op_traits>;

    static result_type  multiply(STD_LA::matrix<fs_matrix_engine_tst<double, 3, 4>, OTR> const& m1,
                                 STD_LA::matrix<fs_matrix_engine_tst<double, 3, 4>, OTR> const& m2)
                        {
                            STD_LA::PrintOperandTypes<result_type>("multiplication_traits_TST", m1, m2);
                            return result_type();
                        }
};

//------
//
struct test_mul_op_traits_tst
{
     template<class T1, class T2>
     using element_multiplication_traits = element_mul_traits_tst<T1, T2>;

     template<class OT, class ET1, class ET2>
     using engine_multiplication_traits = engine_mul_traits_tst<OT, ET1, ET2>;

     template<class OT, class OP1, class OP2>
     using multiplication_traits = multiplication_traits_tst<OT, OP1, OP2>;
};


void t105()
{
    PRINT_FNAME();

    static_assert(STD_LA::detail::has_element_mul_traits_v<test_mul_op_traits_tst, float, float>);
    static_assert(!STD_LA::detail::has_element_mul_traits_v<test_mul_op_traits_tst, float, double>);
    static_assert(!STD_LA::detail::has_element_mul_traits_v<test_mul_op_traits_tst, double, float>);

    using t00 = STD_LA::detail::element_mul_traits_t<test_mul_op_traits_tst, float, float>;
    PRINT_TYPE(t00);

    using t01 = STD_LA::detail::element_mul_traits_t<test_mul_op_traits_tst, float, double>;
    PRINT_TYPE(t01);
}

void t106()
{
    PRINT_FNAME();

    using fsm_float       = STD_LA::fs_matrix<float, 2, 3>;
    using fsm_float_tst   = STD_LA::matrix<fs_matrix_engine_tst<float, 2, 3>, test_mul_op_traits_tst>;
    using fsm_double_tst  = STD_LA::matrix<fs_matrix_engine_tst<double, 2, 3>, test_mul_op_traits_tst>;
    using fsm_new_num_tst = STD_LA::matrix<fs_matrix_engine_tst<new_num, 2, 3>, test_mul_op_traits_tst>;

    using fsm_float_tst_tr   = decltype(std::declval<STD_LA::matrix<fs_matrix_engine_tst<float, 3, 2>, test_mul_op_traits_tst>>().t());
    using fsm_double_tst_tr  = decltype(std::declval<STD_LA::matrix<fs_matrix_engine_tst<double, 3, 2>, test_mul_op_traits_tst>>().t());
    using fsm_new_num_tst_tr = decltype(std::declval<STD_LA::matrix<fs_matrix_engine_tst<new_num, 3, 2>, test_mul_op_traits_tst>>().t());

    using drm_double_tst  = STD_LA::matrix<STD_LA::dr_matrix_engine<double, std::allocator<double>>, test_mul_op_traits_tst>;
    using drm_new_num_tst = STD_LA::matrix<STD_LA::dr_matrix_engine<new_num, std::allocator<new_num>>, test_mul_op_traits_tst>;

    using t00 = STD_LA::detail::engine_mul_traits_t<test_mul_op_traits_tst,
                                                    fs_matrix_engine_tst<float, 3, 4>,
                                                    fs_matrix_engine_tst<float, 3, 4>>;
    PRINT_TYPE(t00);

    using t01 = typename t00::engine_type;
    PRINT_TYPE(t01);

    using t02 = STD_LA::detail::engine_mul_traits_t<test_mul_op_traits_tst,
                                                    STD_LA::fs_matrix_engine<new_num, 3, 4>,
                                                    fs_matrix_engine_tst<float, 3, 4>>;
    using t03 = typename t02::engine_type;
    PRINT_TYPE(t03);

    ASSERT_A_MULL_B_EQ_C(fsm_float_tst,  fsm_float_tst,  fsm_double_tst);
    ASSERT_A_MULL_B_EQ_C(fsm_float,      fsm_float_tst,  drm_double_tst);
    ASSERT_A_MULL_B_EQ_C(fsm_float_tst,  fsm_float,      drm_double_tst);

    EXEC_A_MUL_B(fsm_float_tst, fsm_float_tst);
    EXEC_A_MUL_B(fsm_float,     fsm_float_tst);
    EXEC_A_MUL_B(fsm_float_tst, fsm_float);
    EXEC_A_MUL_B(fsm_float,     fsm_float);

    ASSERT_A_MULL_B_EQ_C(fsm_float_tst_tr,  fsm_float_tst_tr,  fsm_double_tst);
    ASSERT_A_MULL_B_EQ_C(fsm_float_tst_tr,  fsm_float_tst,     drm_double_tst);
    ASSERT_A_MULL_B_EQ_C(fsm_float_tst,     fsm_float_tst_tr,  drm_double_tst);

    EXEC_A_MUL_B(fsm_float_tst_tr, fsm_float_tst_tr);
    EXEC_A_MUL_B(fsm_float_tst_tr, fsm_float_tst);
    EXEC_A_MUL_B(fsm_float_tst,    fsm_float_tst_tr);

    ASSERT_A_MULL_B_EQ_C(fsm_double_tst,   fsm_double_tst,   fsm_double_tst);
    ASSERT_A_MULL_B_EQ_C(fsm_double_tst,   fsm_new_num_tst,  fsm_new_num_tst);
    ASSERT_A_MULL_B_EQ_C(fsm_new_num_tst,  fsm_double_tst,   fsm_new_num_tst);
    ASSERT_A_MULL_B_EQ_C(fsm_new_num_tst,  fsm_new_num_tst,  fsm_new_num_tst);

    EXEC_A_MUL_B(fsm_double_tst,   fsm_double_tst);
    EXEC_A_MUL_B(fsm_double_tst,   fsm_new_num_tst);
    EXEC_A_MUL_B(fsm_new_num_tst,  fsm_double_tst);
    EXEC_A_MUL_B(fsm_new_num_tst,  fsm_new_num_tst);

    ASSERT_A_MULL_B_EQ_C(fsm_double_tst_tr,   fsm_double_tst_tr,   fsm_double_tst);
    ASSERT_A_MULL_B_EQ_C(fsm_double_tst_tr,   fsm_new_num_tst_tr,  fsm_new_num_tst);
    ASSERT_A_MULL_B_EQ_C(fsm_new_num_tst_tr,  fsm_double_tst_tr,   fsm_new_num_tst);
    ASSERT_A_MULL_B_EQ_C(fsm_new_num_tst_tr,  fsm_new_num_tst_tr,  fsm_new_num_tst);

    EXEC_A_MUL_B(fsm_double_tst_tr,   fsm_double_tst_tr);
    EXEC_A_MUL_B(fsm_double_tst_tr,   fsm_new_num_tst_tr);
    EXEC_A_MUL_B(fsm_new_num_tst_tr,  fsm_double_tst_tr);
    EXEC_A_MUL_B(fsm_new_num_tst_tr,  fsm_new_num_tst_tr);

    using fsm_double_tst_34  = STD_LA::matrix<fs_matrix_engine_tst<double, 3, 4>, test_mul_op_traits_tst>;
    EXEC_A_MUL_B(fsm_double_tst,      fsm_double_tst);
    EXEC_A_MUL_B(fsm_double_tst_34,   fsm_double_tst_34);
}

void
TestGroup10()
{
    PRINT_FNAME();

    t100();
    t101();
    t102();
    t103();
    t104();

    t105();
    t106();
}
