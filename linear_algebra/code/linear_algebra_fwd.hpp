#ifndef LINEAR_ALGEBRA_FWD_HPP_DEFINED
#define LINEAR_ALGEBRA_FWD_HPP_DEFINED

#include <cstdint>
#include <complex>
#include <memory>
#include <tuple>
#include <type_traits>

namespace std::la {

//- Verifying appropriate matrix element types.
//
template<class T>   struct is_complex;
template<class T>   struct is_matrix_element;

//- Element promotion.
//
template<class T1, class T2>    struct matrix_element_promotion;

//- Basic math object engine types.
//
template<class T, size_t R, size_t C>           class fs_matrix_engine;
template<class T, class AT = std::allocator<T>> class dr_matrix_engine;
template<class ET>                              class matrix_transpose_engine;

//- Math object engine promotion, per arithmetical operation.
//
template<class E1>              struct matrix_negation_engine_promotion;
template<class E1, class E2>    struct matrix_addition_engine_promotion;
template<class E1, class E2>    struct matrix_subtraction_engine_promotion;
template<class E1, class E2>    struct matrix_multiplication_engine_promotion;

//- Primary mathematical object types.
//
struct matrix_operator_traits;

template<class ET, class OT=matrix_operator_traits> class column_vector;
template<class ET, class OT=matrix_operator_traits> class row_vector;
template<class ET, class OT=matrix_operator_traits> class matrix;

//- Matrix arithmetic traits.
//
template<class OP1>             struct matrix_negation_traits;
template<class OP1, class OP2>  struct matrix_addition_traits;
template<class OP1, class OP2>  struct matrix_subtraction_traits;
template<class OP1, class OP2>  struct matrix_multiplication_traits;

//- Operator traits and traits promotion.
//
struct matrix_operator_traits;
template<class T1, class T2>    struct matrix_operator_traits_promotion;

}       //- std::la namespace
#endif  //- LINEAR_ALGEBRA_FWD_HPP_DEFINED
