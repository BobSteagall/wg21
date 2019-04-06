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

//- A traits type for element promotion.
//
template<class T1, class T2>    struct matrix_element_promotion;

//- Math object engine promotion, per arithmetical operation.
//
template<class T1>              struct matrix_element_negation_traits;
template<class T1, class T2>    struct matrix_element_addition_traits;
template<class T1, class T2>    struct matrix_element_subtraction_traits;
template<class T1, class T2>    struct matrix_element_multiplication_traits;

//- Math object engine promotion, per arithmetical operation.
//
template<class E1>              struct matrix_engine_negation_traits;
template<class E1, class E2>    struct matrix_engine_addition_traits;
template<class E1, class E2>    struct matrix_engine_subtraction_traits;
template<class E1, class E2>    struct matrix_engine_multiplication_traits;

//- Matrix arithmetic traits.
//
template<class OP1, class OTR>              struct matrix_negation_traits;
template<class OP1, class OP2, class OTR>   struct matrix_addition_traits;
template<class OP1, class OP2, class OTR>   struct matrix_subtraction_traits;
template<class OP1, class OP2, class OTR>   struct matrix_multiplication_traits;

//- Operator traits and traits promotion.
//
struct default_matrix_operation_traits;
template<class T1, class T2>    struct matrix_operation_traits_selector;

//- Basic linear algebra math object engine types.
//
template<class T, int32_t N>                class fs_vector_engine;
template<class T, int32_t R, int32_t C>     class fs_matrix_engine;
template<class T, class AT=allocator<T>>    class dr_vector_engine;
template<class T, class AT=allocator<T>>    class dr_matrix_engine;
template<class ET>                          class matrix_transpose_engine;

//- Primary mathematical object types.
//
template<class ET, class OT=default_matrix_operation_traits> class vector;
template<class ET, class OT=default_matrix_operation_traits> class matrix;


}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_FORWARD_DECLARATIONS_HPP_DEFINED
