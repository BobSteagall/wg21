#ifndef LINEAR_ALGEBRA_FORWARD_DECLARATIONS_HPP_DEFINED
#define LINEAR_ALGEBRA_FORWARD_DECLARATIONS_HPP_DEFINED

namespace STD_LA {
USING_STD

using const_matrix_engine_tag     = integral_constant<int, 1>;
using mutable_matrix_engine_tag   = integral_constant<int, 2>;
using resizable_matrix_engine_tag = integral_constant<int, 3>;

//- Traits for verifying appropriate matrix element types.
//
template<class T>   struct number_traits;
template<class T>   struct is_complex;
template<class T>   struct is_field;
template<class T>   struct is_nc_ring;
template<class T>   struct is_ring;
template<class T>   struct is_semi_ring;

template<class T>   struct is_matrix_element;

struct default_matrix_operations {};

//- A traits type for element promotion.
//
template<class T1, class T2>    struct matrix_element_promotion;

//- Math object engine promotion, per arithmetical operation.
//
template<class T1>              struct matrix_element_negation_traits;
template<class T1, class T2>    struct matrix_addition_element_traits;
template<class T1, class T2>    struct matrix_subtraction_element_traits;
template<class T1, class T2>    struct matrix_element_multiplication_traits;

//- Math object engine promotion, per arithmetical operation.
//
template<class E1>              struct matrix_engine_negation_traits;
template<class OTR, class E1, class E2>     struct matrix_addition_engine_traits;
template<class OTR, class E1, class E2>     struct matrix_subtraction_engine_traits;
template<class E1, class E2>    struct matrix_engine_multiplication_traits;

//- Matrix arithmetic traits.
//
template<class OP1, class OTR>              struct matrix_negation_traits;
template<class OTR, class OP1, class OP2>   struct matrix_addition_traits;
template<class OP1, class OP2, class OTR>   struct matrix_subtraction_traits;
template<class OP1, class OP2, class OTR>   struct matrix_multiplication_traits;

//- Operator traits and traits promotion.
//
struct default_matrix_operation_traits;
template<class T1, class T2>    struct matrix_operation_traits_selector;

//- Basic linear algebra math object engine types.
//
template<class T, int32_t N>            class fs_vector_engine;
template<class T, int32_t R, int32_t C> class fs_matrix_engine;
template<class T, class AT>             class dr_vector_engine;
template<class T, class AT>             class dr_matrix_engine;
template<class ET>                      class tr_matrix_engine;

//- Primary mathematical object types.
//
template<class ET, class OT=default_matrix_operation_traits> class vector;
template<class ET, class OT=default_matrix_operation_traits> class matrix;

namespace detail {
//- Testing to see if an engine has mutable element indexing.
//
template<class ET>
constexpr bool  has_mutable_tag_v = ET::engine_category::value >= mutable_matrix_engine_tag::value;

template<class ET1, class ET2>
constexpr bool  is_mutable_engine_v = is_same_v<ET1, ET2> && has_mutable_tag_v<ET1>;


template<class ET1, class ET2>
using enable_if_mutable = enable_if_t<is_mutable_engine_v<ET1, ET2>, bool>;


//- Testing to see if an engine is resizable.
//
template<class ET>
constexpr bool  has_resizable_tag_v = ET::engine_category::value >= resizable_matrix_engine_tag::value;

template<class ET1, class ET2>
constexpr bool  is_resizable_engine_v = is_same_v<ET1, ET2> && has_resizable_tag_v<ET1>;


template<class ET1, class ET2>
using enable_if_resizable = enable_if_t<is_resizable_engine_v<ET1, ET2>, bool>;

template<class A1, class T1>
using rebind_alloc_t = typename allocator_traits<A1>::template rebind_alloc<T1>;

}       //- namespace detail

//------
//
template<class T1, class T2, class DEF>
struct non_void_traits_chooser;

template<class T1, class DEF>
struct non_void_traits_chooser<T1, void, DEF>
{
    using traits_type = T1;
};

template<class T2, class DEF>
struct non_void_traits_chooser<void, T2, DEF>
{
    using traits_type = T2;
};

template<class DEF>
struct non_void_traits_chooser<void, void, DEF>
{
    using traits_type = DEF;
};


}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_FORWARD_DECLARATIONS_HPP_DEFINED
