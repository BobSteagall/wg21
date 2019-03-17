#ifndef LINEAR_ALGEBRA_OPERATOR_PROMOTION_TRAITS_HPP_DEFINED
#define LINEAR_ALGEBRA_OPERATOR_PROMOTION_TRAITS_HPP_DEFINED

#include "operator_traits.hpp"

namespace STD_LA {
//=================================================================================================
//                             **** OPERATOR TRAITS PROMOTION ****
//=================================================================================================
//  Traits type that determines which sets of operator traits to use in an expression.
//=================================================================================================
//
template<class T1, class T2>
struct matrix_operator_traits_promotion;

template<class T1>
struct matrix_operator_traits_promotion<T1, T1>
{
    using traits_type = T1;
};

template<class T1>
struct matrix_operator_traits_promotion<T1, default_matrix_operator_traits>
{
    using traits_type = T1;
};

template<class T1>
struct matrix_operator_traits_promotion<default_matrix_operator_traits, T1>
{
    using traits_type = T1;
};

template<>
struct matrix_operator_traits_promotion<default_matrix_operator_traits, default_matrix_operator_traits>
{
    using traits_type = default_matrix_operator_traits;
};

//- Alias interface to trait.
//
template<class T1, class T2>
using matrix_operator_traits_promotion_t = typename matrix_operator_traits_promotion<T1,T2>::traits_type;

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_OPERATOR_PROMOTION_TRAITS_HPP_DEFINED
