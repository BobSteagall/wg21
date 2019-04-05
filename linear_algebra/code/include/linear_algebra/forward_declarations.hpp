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
template<class T>   struct is_field;
template<class T>   struct is_ring;
template<class T>   struct is_semi_ring;
template<class T>   struct is_complex;
template<class T>   struct is_matrix_element;

//- A traits type for element promotion.
//
template<class T1, class T2>    struct matrix_element_promotion;

//- Basic linear algebra math object engine types.
//
template<class T, int32_t N>                class fs_vector_engine;
template<class T, int32_t R, int32_t C>     class fs_matrix_engine;
template<class T, class AT=allocator<T>>    class dr_vector_engine;
template<class T, class AT=allocator<T>>    class dr_matrix_engine;
template<class ET>                          class matrix_transpose_engine;

//- Math object engine promotion, per arithmetical operation.
//
template<class E1>              struct matrix_negation_engine_promotion;
template<class E1, class E2>    struct matrix_addition_engine_promotion;
template<class E1, class E2>    struct matrix_subtraction_engine_promotion;
template<class E1, class E2>    struct matrix_multiplication_engine_promotion;

//- Primary mathematical object types.
//
struct default_matrix_operator_traits;

template<class ET, class OT=default_matrix_operator_traits> class vector;
template<class ET, class OT=default_matrix_operator_traits> class matrix;

//- Matrix arithmetic traits.
//
template<class OP1, class OTR>              struct matrix_negation_traits;
template<class OP1, class OP2, class OTR>   struct matrix_addition_traits;
template<class OP1, class OP2, class OTR>   struct matrix_subtraction_traits;
template<class OP1, class OP2, class OTR>   struct matrix_multiplication_traits;

//- Operator traits and traits promotion.
//
struct default_matrix_operator_traits;
template<class T1, class T2>    struct matrix_operator_traits_selector;

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_FORWARD_DECLARATIONS_HPP_DEFINED
