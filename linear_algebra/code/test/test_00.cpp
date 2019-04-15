#include "linear_algebra.hpp"
#include "test_new_number.hpp"
#include "test_new_engine.hpp"
#include "test_new_arithmetic.hpp"

using cx_float  = std::complex<float>;
using cx_double = std::complex<double>;

using std::cout;
using std::endl;
using STD_LA::get_type_name;

//--------------------------------------------------------------------------------------------------
//- The following are several traits types used to exercise the element, engine, and operation
//  type detection meta-functions.
//
//- This operation traits type is analogous to STD_LA::default_matrix_operations, but with a different name.
//
struct test_add_op_traits_empty {};


//- This operation traits type has its element/engine/operation nested traits type as ordinary
//  type aliases.
//
//  Suffix "_ord" means "ordinary"
//
struct test_element_add_traits_ord {};
struct test_engine_add_traits_ord {};
struct test_add_traits_ord {};

struct test_add_op_traits_ord 
{
    using element_addition_traits = test_element_add_traits_ord;
    using engine_addition_traits  = test_engine_add_traits_ord;
    using addition_traits         = test_add_traits_ord;
};


//- This operation traits type is analogous to STD_LA::default_matrix_operation_traits, where its nested
//  traits types for element/engine/operation are template type aliases.
//
//  Suffix "_nta" means "nested type alias"
//
template<class T1, class T2>
struct test_element_add_traits_nta 
{
    using type = std::common_type_t<T1, T2>;
};

template<class OT, class T1, class T2>
struct test_engine_add_traits_nta {};

template<class OT, class T1, class T2>
struct test_add_traits_nta {};

struct test_add_op_traits_nta
{
    template<class T1, class T2>
    using element_addition_traits = test_element_add_traits_nta<T1, T2>;

    template<class OT, class T1, class T2>
    using engine_addition_traits = test_engine_add_traits_nta<OT, T1, T2>;

    template<class OT, class T1, class T2>
    using addition_traits = test_add_traits_nta<OT, T1, T2>;
};


//- This operation traits type is has the element/engine/operation traits as nested class templates.
//
//  Suffix "_nct" means "nested class type"
//
struct test_add_op_traits_nct
{
    template<class T1, class T2>
    struct element_addition_traits 
    {
        using type = std::common_type<T1, T2>;
    };

    template<class OT, class T1, class T2>
    struct engine_addition_traits {};

    template<class OT, class T1, class T2>
    struct addition_traits {};
};


//- This operation traits type is has the element/engine/operation traits as nested class templates.
//
//  Suffix "_tst" means "test"
//
template<class T1, class T2>
struct elem_prom_tst;

template<>
struct elem_prom_tst<float, float>
{
    using type = double;
};

//------
//
template<class OT, class E1, class E2>
struct engine_add_traits_tst;

template<class OT, class T1, int32_t R1, int32_t C1, class T2, int32_t R2, int32_t C2>
struct engine_add_traits_tst<OT, fs_matrix_engine_tst<T1, R1, C1>, fs_matrix_engine_tst<T2, R2, C2>>
{
    static_assert(R1 == R2);
    static_assert(C1 == C2);
    using traits_category = STD_LA::matrix_engine_addition_traits_tag;
    using element_type    = STD_LA::detail::element_add_type_t<OT, T1, T2>;
    using engine_type     = fs_matrix_engine_tst<element_type, R1, C1>;
};

//------
//
struct test_add_op_traits_tst
{
     template<class T1, class T2>
     using element_addition_traits = elem_prom_tst<T1, T2>;

     template<class OT, class E1, class E2>
     using engine_addition_traits = engine_add_traits_tst<OT, E1, E2>;
};


//- A couple of helper macros to assist in readability below
//
#define ASSERT_A_ADD_B_EQ_C(A, B, C) \
    static_assert(std::is_same_v<decltype(std::declval<A>() + std::declval<B>()), C>)

#define EXEC_A_ADD_B(A, B)  (void)(A() + B())


//--------------------------------------------------------------------------------------------------
//  This test ensures that the type detection meta-functions are working properly.  It exercises
//  only the detection meta-functions.
//--------------------------------------------------------------------------------------------------
//
void t200()
{
    PRINT_FNAME();

    //- Detect element traits.
    //
    using elem_t = double;

    static_assert(!STD_LA::detail::has_element_add_traits_v<test_add_op_traits_empty, elem_t, elem_t>);
    static_assert(!STD_LA::detail::has_element_add_traits_v<STD_LA::default_matrix_operations, elem_t, elem_t>);
    static_assert(!STD_LA::detail::has_element_add_traits_v<void, elem_t, elem_t>);

    static_assert(STD_LA::detail::has_element_add_traits_v<STD_LA::default_matrix_operation_traits, elem_t, elem_t>);
    static_assert(STD_LA::detail::has_element_add_traits_v<test_add_op_traits_ord, elem_t, elem_t>);
    static_assert(STD_LA::detail::has_element_add_traits_v<test_add_op_traits_nta, elem_t, elem_t>);
    static_assert(STD_LA::detail::has_element_add_traits_v<test_add_op_traits_nct, elem_t, elem_t>);

    //- Detect engine traits.
    //
    using eng_t = STD_LA::dr_matrix_engine<elem_t, std::allocator<elem_t>>;

    static_assert(!STD_LA::detail::has_engine_add_traits_v<test_add_op_traits_empty, eng_t, eng_t>);
    static_assert(!STD_LA::detail::has_engine_add_traits_v<STD_LA::default_matrix_operations, eng_t, eng_t>);
    static_assert(!STD_LA::detail::has_engine_add_traits_v<void, eng_t, eng_t>);

    static_assert(STD_LA::detail::has_engine_add_traits_v<STD_LA::default_matrix_operation_traits, eng_t, eng_t>);
    static_assert(STD_LA::detail::has_engine_add_traits_v<test_add_op_traits_ord, eng_t, eng_t>);
    static_assert(STD_LA::detail::has_engine_add_traits_v<test_add_op_traits_nta, eng_t, eng_t>);
    static_assert(STD_LA::detail::has_engine_add_traits_v<test_add_op_traits_nct, eng_t, eng_t>);

    //- Detect operation traits.
    //
    using opnd_t = STD_LA::dyn_matrix<elem_t>;

    static_assert(!STD_LA::detail::has_add_traits_v<test_add_op_traits_empty, opnd_t, opnd_t>);
    static_assert(!STD_LA::detail::has_add_traits_v<STD_LA::default_matrix_operations, opnd_t, opnd_t>);
    static_assert(!STD_LA::detail::has_add_traits_v<void, opnd_t, opnd_t>);

    static_assert(STD_LA::detail::has_add_traits_v<STD_LA::default_matrix_operation_traits, opnd_t, opnd_t>);
    static_assert(STD_LA::detail::has_add_traits_v<test_add_op_traits_ord, opnd_t, opnd_t>);
    static_assert(STD_LA::detail::has_add_traits_v<test_add_op_traits_nta, opnd_t, opnd_t>);
    static_assert(STD_LA::detail::has_add_traits_v<test_add_op_traits_nct, opnd_t, opnd_t>);
}

//--------------------------------------------------------------------------------------------------
//  This test verifies that addition operations on matrices return the correct result type.
//--------------------------------------------------------------------------------------------------
//
void t201()
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

    ASSERT_A_ADD_B_EQ_C(fsm_float,  fsm_float,       fsm_float);
    ASSERT_A_ADD_B_EQ_C(fsm_float,  fsm_double,      fsm_double);
    ASSERT_A_ADD_B_EQ_C(fsm_float,  fsm_new_num,     fsm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_float,  fsm_float_tr,    fsm_float);
    ASSERT_A_ADD_B_EQ_C(fsm_float,  fsm_double_tr,   fsm_double);
    ASSERT_A_ADD_B_EQ_C(fsm_float,  fsm_new_num_tr,  fsm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_float,  drm_float,       drm_float);
    ASSERT_A_ADD_B_EQ_C(fsm_float,  drm_double,      drm_double);
    ASSERT_A_ADD_B_EQ_C(fsm_float,  drm_new_num,     drm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_float,  drm_float_tr,    drm_float);
    ASSERT_A_ADD_B_EQ_C(fsm_float,  drm_double_tr,   drm_double);
    ASSERT_A_ADD_B_EQ_C(fsm_float,  drm_new_num_tr,  drm_new_num);

    ASSERT_A_ADD_B_EQ_C(fsm_double,  fsm_float,       fsm_double);
    ASSERT_A_ADD_B_EQ_C(fsm_double,  fsm_double,      fsm_double);
    ASSERT_A_ADD_B_EQ_C(fsm_double,  fsm_new_num,     fsm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_double,  fsm_float_tr,    fsm_double);
    ASSERT_A_ADD_B_EQ_C(fsm_double,  fsm_double_tr,   fsm_double);
    ASSERT_A_ADD_B_EQ_C(fsm_double,  fsm_new_num_tr,  fsm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_double,  drm_float,       drm_double);
    ASSERT_A_ADD_B_EQ_C(fsm_double,  drm_double,      drm_double);
    ASSERT_A_ADD_B_EQ_C(fsm_double,  drm_new_num,     drm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_double,  drm_float_tr,    drm_double);
    ASSERT_A_ADD_B_EQ_C(fsm_double,  drm_double_tr,   drm_double);
    ASSERT_A_ADD_B_EQ_C(fsm_double,  drm_new_num_tr,  drm_new_num);

    ASSERT_A_ADD_B_EQ_C(fsm_new_num,  fsm_float,       fsm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num,  fsm_double,      fsm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num,  fsm_new_num,     fsm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num,  fsm_float_tr,    fsm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num,  fsm_double_tr,   fsm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num,  fsm_new_num_tr,  fsm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num,  drm_float,       drm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num,  drm_double,      drm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num,  drm_new_num,     drm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num,  drm_float_tr,    drm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num,  drm_double_tr,   drm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num,  drm_new_num_tr,  drm_new_num);

    ASSERT_A_ADD_B_EQ_C(fsm_float_tr,  fsm_float,       fsm_float);
    ASSERT_A_ADD_B_EQ_C(fsm_float_tr,  fsm_double,      fsm_double);
    ASSERT_A_ADD_B_EQ_C(fsm_float_tr,  fsm_new_num,     fsm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_float_tr,  fsm_float_tr,    fsm_float);
    ASSERT_A_ADD_B_EQ_C(fsm_float_tr,  fsm_double_tr,   fsm_double);
    ASSERT_A_ADD_B_EQ_C(fsm_float_tr,  fsm_new_num_tr,  fsm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_float_tr,  drm_float,       drm_float);
    ASSERT_A_ADD_B_EQ_C(fsm_float_tr,  drm_double,      drm_double);
    ASSERT_A_ADD_B_EQ_C(fsm_float_tr,  drm_new_num,     drm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_float_tr,  drm_float_tr,    drm_float);
    ASSERT_A_ADD_B_EQ_C(fsm_float_tr,  drm_double_tr,   drm_double);
    ASSERT_A_ADD_B_EQ_C(fsm_float_tr,  drm_new_num_tr,  drm_new_num);

    ASSERT_A_ADD_B_EQ_C(fsm_double_tr,  fsm_float,       fsm_double);
    ASSERT_A_ADD_B_EQ_C(fsm_double_tr,  fsm_double,      fsm_double);
    ASSERT_A_ADD_B_EQ_C(fsm_double_tr,  fsm_new_num,     fsm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_double_tr,  fsm_float_tr,    fsm_double);
    ASSERT_A_ADD_B_EQ_C(fsm_double_tr,  fsm_double_tr,   fsm_double);
    ASSERT_A_ADD_B_EQ_C(fsm_double_tr,  fsm_new_num_tr,  fsm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_double_tr,  drm_float,       drm_double);
    ASSERT_A_ADD_B_EQ_C(fsm_double_tr,  drm_double,      drm_double);
    ASSERT_A_ADD_B_EQ_C(fsm_double_tr,  drm_new_num,     drm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_double_tr,  drm_float_tr,    drm_double);
    ASSERT_A_ADD_B_EQ_C(fsm_double_tr,  drm_double_tr,   drm_double);
    ASSERT_A_ADD_B_EQ_C(fsm_double_tr,  drm_new_num_tr,  drm_new_num);

    ASSERT_A_ADD_B_EQ_C(fsm_new_num_tr,  fsm_float,       fsm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num_tr,  fsm_double,      fsm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num_tr,  fsm_new_num,     fsm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num_tr,  fsm_float_tr,    fsm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num_tr,  fsm_double_tr,   fsm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num_tr,  fsm_new_num_tr,  fsm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num_tr,  drm_float,       drm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num_tr,  drm_double,      drm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num_tr,  drm_new_num,     drm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num_tr,  drm_float_tr,    drm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num_tr,  drm_double_tr,   drm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num_tr,  drm_new_num_tr,  drm_new_num);

    //------
    //
    ASSERT_A_ADD_B_EQ_C(drm_float,  fsm_float,       drm_float);
    ASSERT_A_ADD_B_EQ_C(drm_float,  fsm_double,      drm_double);
    ASSERT_A_ADD_B_EQ_C(drm_float,  fsm_new_num,     drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_float,  fsm_float_tr,    drm_float);
    ASSERT_A_ADD_B_EQ_C(drm_float,  fsm_double_tr,   drm_double);
    ASSERT_A_ADD_B_EQ_C(drm_float,  fsm_new_num_tr,  drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_float,  drm_float,       drm_float);
    ASSERT_A_ADD_B_EQ_C(drm_float,  drm_double,      drm_double);
    ASSERT_A_ADD_B_EQ_C(drm_float,  drm_new_num,     drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_float,  drm_float_tr,    drm_float);
    ASSERT_A_ADD_B_EQ_C(drm_float,  drm_double_tr,   drm_double);
    ASSERT_A_ADD_B_EQ_C(drm_float,  drm_new_num_tr,  drm_new_num);

    ASSERT_A_ADD_B_EQ_C(drm_double,  fsm_float,       drm_double);
    ASSERT_A_ADD_B_EQ_C(drm_double,  fsm_double,      drm_double);
    ASSERT_A_ADD_B_EQ_C(drm_double,  fsm_new_num,     drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_double,  fsm_float_tr,    drm_double);
    ASSERT_A_ADD_B_EQ_C(drm_double,  fsm_double_tr,   drm_double);
    ASSERT_A_ADD_B_EQ_C(drm_double,  fsm_new_num_tr,  drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_double,  drm_float,       drm_double);
    ASSERT_A_ADD_B_EQ_C(drm_double,  drm_double,      drm_double);
    ASSERT_A_ADD_B_EQ_C(drm_double,  drm_new_num,     drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_double,  drm_float_tr,    drm_double);
    ASSERT_A_ADD_B_EQ_C(drm_double,  drm_double_tr,   drm_double);
    ASSERT_A_ADD_B_EQ_C(drm_double,  drm_new_num_tr,  drm_new_num);

    ASSERT_A_ADD_B_EQ_C(drm_new_num,  fsm_float,       drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_new_num,  fsm_double,      drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_new_num,  fsm_new_num,     drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_new_num,  fsm_float_tr,    drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_new_num,  fsm_double_tr,   drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_new_num,  fsm_new_num_tr,  drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_new_num,  drm_float,       drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_new_num,  drm_double,      drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_new_num,  drm_new_num,     drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_new_num,  drm_float_tr,    drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_new_num,  drm_double_tr,   drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_new_num,  drm_new_num_tr,  drm_new_num);

    ASSERT_A_ADD_B_EQ_C(drm_float_tr,  fsm_float,       drm_float);
    ASSERT_A_ADD_B_EQ_C(drm_float_tr,  fsm_double,      drm_double);
    ASSERT_A_ADD_B_EQ_C(drm_float_tr,  fsm_new_num,     drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_float_tr,  fsm_float_tr,    drm_float);
    ASSERT_A_ADD_B_EQ_C(drm_float_tr,  fsm_double_tr,   drm_double);
    ASSERT_A_ADD_B_EQ_C(drm_float_tr,  fsm_new_num_tr,  drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_float_tr,  drm_float,       drm_float);
    ASSERT_A_ADD_B_EQ_C(drm_float_tr,  drm_double,      drm_double);
    ASSERT_A_ADD_B_EQ_C(drm_float_tr,  drm_new_num,     drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_float_tr,  drm_float_tr,    drm_float);
    ASSERT_A_ADD_B_EQ_C(drm_float_tr,  drm_double_tr,   drm_double);
    ASSERT_A_ADD_B_EQ_C(drm_float_tr,  drm_new_num_tr,  drm_new_num);

    ASSERT_A_ADD_B_EQ_C(drm_double_tr,  fsm_float,       drm_double);
    ASSERT_A_ADD_B_EQ_C(drm_double_tr,  fsm_double,      drm_double);
    ASSERT_A_ADD_B_EQ_C(drm_double_tr,  fsm_new_num,     drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_double_tr,  fsm_float_tr,    drm_double);
    ASSERT_A_ADD_B_EQ_C(drm_double_tr,  fsm_double_tr,   drm_double);
    ASSERT_A_ADD_B_EQ_C(drm_double_tr,  fsm_new_num_tr,  drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_double_tr,  drm_float,       drm_double);
    ASSERT_A_ADD_B_EQ_C(drm_double_tr,  drm_double,      drm_double);
    ASSERT_A_ADD_B_EQ_C(drm_double_tr,  drm_new_num,     drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_double_tr,  drm_float_tr,    drm_double);
    ASSERT_A_ADD_B_EQ_C(drm_double_tr,  drm_double_tr,   drm_double);
    ASSERT_A_ADD_B_EQ_C(drm_double_tr,  drm_new_num_tr,  drm_new_num);

    ASSERT_A_ADD_B_EQ_C(drm_new_num_tr,  fsm_float,       drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_new_num_tr,  fsm_double,      drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_new_num_tr,  fsm_new_num,     drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_new_num_tr,  fsm_float_tr,    drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_new_num_tr,  fsm_double_tr,   drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_new_num_tr,  fsm_new_num_tr,  drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_new_num_tr,  drm_float,       drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_new_num_tr,  drm_double,      drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_new_num_tr,  drm_new_num,     drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_new_num_tr,  drm_float_tr,    drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_new_num_tr,  drm_double_tr,   drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_new_num_tr,  drm_new_num_tr,  drm_new_num);

    STD_LA::fs_matrix<double, 3, 3>     fm1;
    STD_LA::dyn_matrix<double>          dmd1(3, 3);

    cout << get_type_name<STD_LA::dyn_matrix<float>>() << endl;
    cout << get_type_name<STD_LA::dyn_matrix<double>>() << endl;
    cout << get_type_name<decltype(fm1 + dmd1)>() << endl;
    cout << get_type_name<decltype(fm1.t() + dmd1.t())>() << endl;
    fm1 + dmd1;

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
void t202()
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

    EXEC_A_ADD_B(fsm_float,  fsm_float);
    EXEC_A_ADD_B(fsm_float,  fsm_double);
    EXEC_A_ADD_B(fsm_float,  fsm_new_num);
    EXEC_A_ADD_B(fsm_float,  fsm_float_tr);
    EXEC_A_ADD_B(fsm_float,  fsm_double_tr);
    EXEC_A_ADD_B(fsm_float,  fsm_new_num_tr);
    EXEC_A_ADD_B(fsm_float,  drm_float);
    EXEC_A_ADD_B(fsm_float,  drm_double);
    EXEC_A_ADD_B(fsm_float,  drm_new_num);
    EXEC_A_ADD_B(fsm_float,  drm_float_tr);
    EXEC_A_ADD_B(fsm_float,  drm_double_tr);
    EXEC_A_ADD_B(fsm_float,  drm_new_num_tr);

    EXEC_A_ADD_B(fsm_double,  fsm_float);
    EXEC_A_ADD_B(fsm_double,  fsm_double);
    EXEC_A_ADD_B(fsm_double,  fsm_new_num);
    EXEC_A_ADD_B(fsm_double,  fsm_float_tr);
    EXEC_A_ADD_B(fsm_double,  fsm_double_tr);
    EXEC_A_ADD_B(fsm_double,  fsm_new_num_tr);
    EXEC_A_ADD_B(fsm_double,  drm_float);
    EXEC_A_ADD_B(fsm_double,  drm_double);
    EXEC_A_ADD_B(fsm_double,  drm_new_num);
    EXEC_A_ADD_B(fsm_double,  drm_float_tr);
    EXEC_A_ADD_B(fsm_double,  drm_double_tr);
    EXEC_A_ADD_B(fsm_double,  drm_new_num_tr);

    EXEC_A_ADD_B(fsm_new_num,  fsm_float);
    EXEC_A_ADD_B(fsm_new_num,  fsm_double);
    EXEC_A_ADD_B(fsm_new_num,  fsm_new_num);
    EXEC_A_ADD_B(fsm_new_num,  fsm_float_tr);
    EXEC_A_ADD_B(fsm_new_num,  fsm_double_tr);
    EXEC_A_ADD_B(fsm_new_num,  fsm_new_num_tr);
    EXEC_A_ADD_B(fsm_new_num,  drm_float);
    EXEC_A_ADD_B(fsm_new_num,  drm_double);
    EXEC_A_ADD_B(fsm_new_num,  drm_new_num);
    EXEC_A_ADD_B(fsm_new_num,  drm_float_tr);
    EXEC_A_ADD_B(fsm_new_num,  drm_double_tr);
    EXEC_A_ADD_B(fsm_new_num,  drm_new_num_tr);

    EXEC_A_ADD_B(fsm_float_tr,  fsm_float);
    EXEC_A_ADD_B(fsm_float_tr,  fsm_double);
    EXEC_A_ADD_B(fsm_float_tr,  fsm_new_num);
    EXEC_A_ADD_B(fsm_float_tr,  fsm_float_tr);
    EXEC_A_ADD_B(fsm_float_tr,  fsm_double_tr);
    EXEC_A_ADD_B(fsm_float_tr,  fsm_new_num_tr);
    EXEC_A_ADD_B(fsm_float_tr,  drm_float);
    EXEC_A_ADD_B(fsm_float_tr,  drm_double);
    EXEC_A_ADD_B(fsm_float_tr,  drm_new_num);
    EXEC_A_ADD_B(fsm_float_tr,  drm_float_tr);
    EXEC_A_ADD_B(fsm_float_tr,  drm_double_tr);
    EXEC_A_ADD_B(fsm_float_tr,  drm_new_num_tr);

    EXEC_A_ADD_B(fsm_double_tr,  fsm_float);
    EXEC_A_ADD_B(fsm_double_tr,  fsm_double);
    EXEC_A_ADD_B(fsm_double_tr,  fsm_new_num);
    EXEC_A_ADD_B(fsm_double_tr,  fsm_float_tr);
    EXEC_A_ADD_B(fsm_double_tr,  fsm_double_tr);
    EXEC_A_ADD_B(fsm_double_tr,  fsm_new_num_tr);
    EXEC_A_ADD_B(fsm_double_tr,  drm_float);
    EXEC_A_ADD_B(fsm_double_tr,  drm_double);
    EXEC_A_ADD_B(fsm_double_tr,  drm_new_num);
    EXEC_A_ADD_B(fsm_double_tr,  drm_float_tr);
    EXEC_A_ADD_B(fsm_double_tr,  drm_double_tr);
    EXEC_A_ADD_B(fsm_double_tr,  drm_new_num_tr);

    EXEC_A_ADD_B(fsm_new_num_tr,  fsm_float);
    EXEC_A_ADD_B(fsm_new_num_tr,  fsm_double);
    EXEC_A_ADD_B(fsm_new_num_tr,  fsm_new_num);
    EXEC_A_ADD_B(fsm_new_num_tr,  fsm_float_tr);
    EXEC_A_ADD_B(fsm_new_num_tr,  fsm_double_tr);
    EXEC_A_ADD_B(fsm_new_num_tr,  fsm_new_num_tr);
    EXEC_A_ADD_B(fsm_new_num_tr,  drm_float);
    EXEC_A_ADD_B(fsm_new_num_tr,  drm_double);
    EXEC_A_ADD_B(fsm_new_num_tr,  drm_new_num);
    EXEC_A_ADD_B(fsm_new_num_tr,  drm_float_tr);
    EXEC_A_ADD_B(fsm_new_num_tr,  drm_double_tr);
    EXEC_A_ADD_B(fsm_new_num_tr,  drm_new_num_tr);

    //------
    //
    EXEC_A_ADD_B(drm_float,  fsm_float);
    EXEC_A_ADD_B(drm_float,  fsm_double);
    EXEC_A_ADD_B(drm_float,  fsm_new_num);
    EXEC_A_ADD_B(drm_float,  fsm_float_tr);
    EXEC_A_ADD_B(drm_float,  fsm_double_tr);
    EXEC_A_ADD_B(drm_float,  fsm_new_num_tr);
    EXEC_A_ADD_B(drm_float,  drm_float);
    EXEC_A_ADD_B(drm_float,  drm_double);
    EXEC_A_ADD_B(drm_float,  drm_new_num);
    EXEC_A_ADD_B(drm_float,  drm_float_tr);
    EXEC_A_ADD_B(drm_float,  drm_double_tr);
    EXEC_A_ADD_B(drm_float,  drm_new_num_tr);

    EXEC_A_ADD_B(drm_double,  fsm_float);
    EXEC_A_ADD_B(drm_double,  fsm_double);
    EXEC_A_ADD_B(drm_double,  fsm_new_num);
    EXEC_A_ADD_B(drm_double,  fsm_float_tr);
    EXEC_A_ADD_B(drm_double,  fsm_double_tr);
    EXEC_A_ADD_B(drm_double,  fsm_new_num_tr);
    EXEC_A_ADD_B(drm_double,  drm_float);
    EXEC_A_ADD_B(drm_double,  drm_double);
    EXEC_A_ADD_B(drm_double,  drm_new_num);
    EXEC_A_ADD_B(drm_double,  drm_float_tr);
    EXEC_A_ADD_B(drm_double,  drm_double_tr);
    EXEC_A_ADD_B(drm_double,  drm_new_num_tr);

    EXEC_A_ADD_B(drm_new_num,  fsm_float);
    EXEC_A_ADD_B(drm_new_num,  fsm_double);
    EXEC_A_ADD_B(drm_new_num,  fsm_new_num);
    EXEC_A_ADD_B(drm_new_num,  fsm_float_tr);
    EXEC_A_ADD_B(drm_new_num,  fsm_double_tr);
    EXEC_A_ADD_B(drm_new_num,  fsm_new_num_tr);
    EXEC_A_ADD_B(drm_new_num,  drm_float);
    EXEC_A_ADD_B(drm_new_num,  drm_double);
    EXEC_A_ADD_B(drm_new_num,  drm_new_num);
    EXEC_A_ADD_B(drm_new_num,  drm_float_tr);
    EXEC_A_ADD_B(drm_new_num,  drm_double_tr);
    EXEC_A_ADD_B(drm_new_num,  drm_new_num_tr);

    EXEC_A_ADD_B(drm_float_tr,  fsm_float);
    EXEC_A_ADD_B(drm_float_tr,  fsm_double);
    EXEC_A_ADD_B(drm_float_tr,  fsm_new_num);
    EXEC_A_ADD_B(drm_float_tr,  fsm_float_tr);
    EXEC_A_ADD_B(drm_float_tr,  fsm_double_tr);
    EXEC_A_ADD_B(drm_float_tr,  fsm_new_num_tr);
    EXEC_A_ADD_B(drm_float_tr,  drm_float);
    EXEC_A_ADD_B(drm_float_tr,  drm_double);
    EXEC_A_ADD_B(drm_float_tr,  drm_new_num);
    EXEC_A_ADD_B(drm_float_tr,  drm_float_tr);
    EXEC_A_ADD_B(drm_float_tr,  drm_double_tr);
    EXEC_A_ADD_B(drm_float_tr,  drm_new_num_tr);

    EXEC_A_ADD_B(drm_double_tr,  fsm_float);
    EXEC_A_ADD_B(drm_double_tr,  fsm_double);
    EXEC_A_ADD_B(drm_double_tr,  fsm_new_num);
    EXEC_A_ADD_B(drm_double_tr,  fsm_float_tr);
    EXEC_A_ADD_B(drm_double_tr,  fsm_double_tr);
    EXEC_A_ADD_B(drm_double_tr,  fsm_new_num_tr);
    EXEC_A_ADD_B(drm_double_tr,  drm_float);
    EXEC_A_ADD_B(drm_double_tr,  drm_double);
    EXEC_A_ADD_B(drm_double_tr,  drm_new_num);
    EXEC_A_ADD_B(drm_double_tr,  drm_float_tr);
    EXEC_A_ADD_B(drm_double_tr,  drm_double_tr);
    EXEC_A_ADD_B(drm_double_tr,  drm_new_num_tr);

    EXEC_A_ADD_B(drm_new_num_tr,  fsm_float);
    EXEC_A_ADD_B(drm_new_num_tr,  fsm_double);
    EXEC_A_ADD_B(drm_new_num_tr,  fsm_new_num);
    EXEC_A_ADD_B(drm_new_num_tr,  fsm_float_tr);
    EXEC_A_ADD_B(drm_new_num_tr,  fsm_double_tr);
    EXEC_A_ADD_B(drm_new_num_tr,  fsm_new_num_tr);
    EXEC_A_ADD_B(drm_new_num_tr,  drm_float);
    EXEC_A_ADD_B(drm_new_num_tr,  drm_double);
    EXEC_A_ADD_B(drm_new_num_tr,  drm_new_num);
    EXEC_A_ADD_B(drm_new_num_tr,  drm_float_tr);
    EXEC_A_ADD_B(drm_new_num_tr,  drm_double_tr);
    EXEC_A_ADD_B(drm_new_num_tr,  drm_new_num_tr);
}

//--------------------------------------------------------------------------------------------------
//  This test verifies that addition operations on vectors return the correct result type.
//--------------------------------------------------------------------------------------------------
//
void t203()
{
    PRINT_FNAME();

    using fsv_float      = STD_LA::fs_vector<float, 3>;
    using fsv_double     = STD_LA::fs_vector<double, 3>;
    using fsv_new_num    = STD_LA::fs_vector<new_num, 3>;

    using drv_float      = STD_LA::dyn_vector<float>;
    using drv_double     = STD_LA::dyn_vector<double>;
    using drv_new_num    = STD_LA::dyn_vector<new_num>;

    ASSERT_A_ADD_B_EQ_C(fsv_float,  fsv_float,       fsv_float);
    ASSERT_A_ADD_B_EQ_C(fsv_float,  fsv_double,      fsv_double);
    ASSERT_A_ADD_B_EQ_C(fsv_float,  fsv_new_num,     fsv_new_num);
    ASSERT_A_ADD_B_EQ_C(fsv_float,  drv_float,       drv_float);
    ASSERT_A_ADD_B_EQ_C(fsv_float,  drv_double,      drv_double);
    ASSERT_A_ADD_B_EQ_C(fsv_float,  drv_new_num,     drv_new_num);

    ASSERT_A_ADD_B_EQ_C(fsv_double,  fsv_float,       fsv_double);
    ASSERT_A_ADD_B_EQ_C(fsv_double,  fsv_double,      fsv_double);
    ASSERT_A_ADD_B_EQ_C(fsv_double,  fsv_new_num,     fsv_new_num);
    ASSERT_A_ADD_B_EQ_C(fsv_double,  drv_float,       drv_double);
    ASSERT_A_ADD_B_EQ_C(fsv_double,  drv_double,      drv_double);
    ASSERT_A_ADD_B_EQ_C(fsv_double,  drv_new_num,     drv_new_num);

    ASSERT_A_ADD_B_EQ_C(fsv_new_num,  fsv_float,       fsv_new_num);
    ASSERT_A_ADD_B_EQ_C(fsv_new_num,  fsv_double,      fsv_new_num);
    ASSERT_A_ADD_B_EQ_C(fsv_new_num,  fsv_new_num,     fsv_new_num);
    ASSERT_A_ADD_B_EQ_C(fsv_new_num,  drv_float,       drv_new_num);
    ASSERT_A_ADD_B_EQ_C(fsv_new_num,  drv_double,      drv_new_num);
    ASSERT_A_ADD_B_EQ_C(fsv_new_num,  drv_new_num,     drv_new_num);

    //------
    //
    ASSERT_A_ADD_B_EQ_C(drv_float,  fsv_float,       drv_float);
    ASSERT_A_ADD_B_EQ_C(drv_float,  fsv_double,      drv_double);
    ASSERT_A_ADD_B_EQ_C(drv_float,  fsv_new_num,     drv_new_num);
    ASSERT_A_ADD_B_EQ_C(drv_float,  drv_float,       drv_float);
    ASSERT_A_ADD_B_EQ_C(drv_float,  drv_double,      drv_double);
    ASSERT_A_ADD_B_EQ_C(drv_float,  drv_new_num,     drv_new_num);

    ASSERT_A_ADD_B_EQ_C(drv_double,  fsv_float,       drv_double);
    ASSERT_A_ADD_B_EQ_C(drv_double,  fsv_double,      drv_double);
    ASSERT_A_ADD_B_EQ_C(drv_double,  fsv_new_num,     drv_new_num);
    ASSERT_A_ADD_B_EQ_C(drv_double,  drv_float,       drv_double);
    ASSERT_A_ADD_B_EQ_C(drv_double,  drv_double,      drv_double);
    ASSERT_A_ADD_B_EQ_C(drv_double,  drv_new_num,     drv_new_num);

    ASSERT_A_ADD_B_EQ_C(drv_new_num,  fsv_float,       drv_new_num);
    ASSERT_A_ADD_B_EQ_C(drv_new_num,  fsv_double,      drv_new_num);
    ASSERT_A_ADD_B_EQ_C(drv_new_num,  fsv_new_num,     drv_new_num);
    ASSERT_A_ADD_B_EQ_C(drv_new_num,  drv_float,       drv_new_num);
    ASSERT_A_ADD_B_EQ_C(drv_new_num,  drv_double,      drv_new_num);
    ASSERT_A_ADD_B_EQ_C(drv_new_num,  drv_new_num,     drv_new_num);

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
void t204()
{
    PRINT_FNAME();

    using fsv_float      = STD_LA::fs_vector<float, 3>;
    using fsv_double     = STD_LA::fs_vector<double, 3>;
    using fsv_new_num    = STD_LA::fs_vector<new_num, 3>;

    using drv_float      = STD_LA::dyn_vector<float>;
    using drv_double     = STD_LA::dyn_vector<double>;
    using drv_new_num    = STD_LA::dyn_vector<new_num>;

    EXEC_A_ADD_B(fsv_float,  fsv_float);
    EXEC_A_ADD_B(fsv_float,  fsv_double);
    EXEC_A_ADD_B(fsv_float,  fsv_new_num);
    EXEC_A_ADD_B(fsv_float,  drv_float);
    EXEC_A_ADD_B(fsv_float,  drv_double);
    EXEC_A_ADD_B(fsv_float,  drv_new_num);

    EXEC_A_ADD_B(fsv_double,  fsv_float);
    EXEC_A_ADD_B(fsv_double,  fsv_double);
    EXEC_A_ADD_B(fsv_double,  fsv_new_num);
    EXEC_A_ADD_B(fsv_double,  drv_float);
    EXEC_A_ADD_B(fsv_double,  drv_double);
    EXEC_A_ADD_B(fsv_double,  drv_new_num);

    EXEC_A_ADD_B(fsv_new_num,  fsv_float);
    EXEC_A_ADD_B(fsv_new_num,  fsv_double);
    EXEC_A_ADD_B(fsv_new_num,  fsv_new_num);
    EXEC_A_ADD_B(fsv_new_num,  drv_float);
    EXEC_A_ADD_B(fsv_new_num,  drv_double);
    EXEC_A_ADD_B(fsv_new_num,  drv_new_num);

    //------
    //
    EXEC_A_ADD_B(drv_float,  fsv_float);
    EXEC_A_ADD_B(drv_float,  fsv_double);
    EXEC_A_ADD_B(drv_float,  fsv_new_num);
    EXEC_A_ADD_B(drv_float,  drv_float);
    EXEC_A_ADD_B(drv_float,  drv_double);
    EXEC_A_ADD_B(drv_float,  drv_new_num);

    EXEC_A_ADD_B(drv_double,  fsv_float);
    EXEC_A_ADD_B(drv_double,  fsv_double);
    EXEC_A_ADD_B(drv_double,  fsv_new_num);
    EXEC_A_ADD_B(drv_double,  drv_float);
    EXEC_A_ADD_B(drv_double,  drv_double);
    EXEC_A_ADD_B(drv_double,  drv_new_num);

    EXEC_A_ADD_B(drv_new_num,  fsv_float);
    EXEC_A_ADD_B(drv_new_num,  fsv_double);
    EXEC_A_ADD_B(drv_new_num,  fsv_new_num);
    EXEC_A_ADD_B(drv_new_num,  drv_float);
    EXEC_A_ADD_B(drv_new_num,  drv_double);
    EXEC_A_ADD_B(drv_new_num,  drv_new_num);
}

void t2000()
{

    static_assert(STD_LA::detail::has_element_add_traits_v<test_add_op_traits_tst, float, float>);
    static_assert(!STD_LA::detail::has_element_add_traits_v<test_add_op_traits_tst, float, double>);
    static_assert(!STD_LA::detail::has_element_add_traits_v<test_add_op_traits_tst, double, float>);

    using t00 = STD_LA::detail::element_add_traits_t<test_add_op_traits_tst, float, float>;
    PRINT_TYPE(t00);

    using t01 = STD_LA::detail::element_add_traits_t<test_add_op_traits_tst, float, double>;
    PRINT_TYPE(t01);

    using t02 = STD_LA::detail::engine_add_traits_t<test_add_op_traits_tst, 
                                                    fs_matrix_engine_tst<float, 3, 4>,
                                                    fs_matrix_engine_tst<double, 3, 4>>;
    PRINT_TYPE(t02);

    using t03 = typename t02::engine_type;
    PRINT_TYPE(t03);
}

void
TestGroup00()
{
    PRINT_FNAME();
    t2000();
    t200();
    t201();
    t202();
    t203();
    t204();
}
