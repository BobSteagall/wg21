#ifndef LINEAR_ALGEBRA_FORWARD_DECLARATIONS_HPP_DEFINED
#define LINEAR_ALGEBRA_FORWARD_DECLARATIONS_HPP_DEFINED

namespace STD_LA {
USING_STD

//- Some tags for specifying how engines should behave.
//
using const_vector_engine_tag     = integral_constant<int, 1>;
using mutable_vector_engine_tag   = integral_constant<int, 2>;
using resizable_vector_engine_tag = integral_constant<int, 3>;

using const_matrix_engine_tag     = integral_constant<int, 4>;
using mutable_matrix_engine_tag   = integral_constant<int, 5>;
using resizable_matrix_engine_tag = integral_constant<int, 6>;

//- A user-specializable traits type for verifying appropriate matrix element types.
//
template<class T>   struct number_traits;

//- Traits for verifying appropriate matrix element types.
//
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
template<class T1>              struct matrix_negation_element_traits;
template<class T1, class T2>    struct matrix_addition_element_traits;
template<class T1, class T2>    struct matrix_subtraction_element_traits;
template<class T1, class T2>    struct matrix_multiplication_element_traits;

//- Math object engine promotion, per arithmetical operation.
//
template<class OT, class ET1>               struct matrix_negation_engine_traits;
template<class OT, class EY1, class ET2>    struct matrix_addition_engine_traits;
template<class OT, class ET1, class ET2>    struct matrix_subtraction_engine_traits;
template<class OT, class ET1, class ET2>    struct matrix_multiplication_engine_traits;

//- Matrix arithmetic traits.
//
template<class OT, class OP1>               struct matrix_negation_traits;
template<class OT, class OP1, class OP2>    struct matrix_addition_traits;
template<class OT, class OP1, class OP2>    struct matrix_subtraction_traits;
template<class OT, class OP1, class OP2>    struct matrix_multiplication_traits;

//- The default element promotion, engine promotion, and arithmetic operation traits for
//  the four defined arithmetic operators
//
struct default_matrix_operation_traits;

//- A user-specializable traits type for choosing between two operator traits types
//  in the binary arithmetic operators.
//
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

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_FORWARD_DECLARATIONS_HPP_DEFINED
