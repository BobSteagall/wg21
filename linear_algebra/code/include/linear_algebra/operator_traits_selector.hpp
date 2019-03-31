#ifndef LINEAR_ALGEBRA_OPERATOR_TRAITS_SELECTOR_HPP_DEFINED
#define LINEAR_ALGEBRA_OPERATOR_TRAITS_SELECTOR_HPP_DEFINED

#include "operator_traits.hpp"

namespace STD_LA {
//=================================================================================================
//                             **** OPERATOR TRAITS PROMOTION ****
//=================================================================================================
//  Traits type that selects the set of operator traits to use in an expression.
//=================================================================================================
//
template<class T1, class T2>
struct matrix_operator_traits_selector;

template<class T1>
struct matrix_operator_traits_selector<T1, T1>
{
    using traits_type = T1;
};

template<class T1>
struct matrix_operator_traits_selector<T1, default_matrix_operator_traits>
{
    using traits_type = T1;
};

template<class T1>
struct matrix_operator_traits_selector<default_matrix_operator_traits, T1>
{
    using traits_type = T1;
};

template<>
struct matrix_operator_traits_selector<default_matrix_operator_traits, default_matrix_operator_traits>
{
    using traits_type = default_matrix_operator_traits;
};

//- Alias interface to trait.
//
template<class T1, class T2>
using matrix_operator_traits_selector_t = typename matrix_operator_traits_selector<T1,T2>::traits_type;

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_OPERATOR_TRAITS_SELECTOR_HPP_DEFINED
