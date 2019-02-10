#ifndef MATRIX_OPERATOR_TRAITS_HPP_DEFINED
#define MATRIX_OPERATOR_TRAITS_HPP_DEFINED

#include "matrix_arithmetic_traits.hpp"

namespace std::la {
//=================================================================================================
//                                  **** OPERATOR TRAITS ****
//=================================================================================================
//  Traits type that refers to the four basic arithmetic traits types.
//=================================================================================================
//
struct matrix_operator_traits
{
    template<class OP1>
    using negation_traits = matrix_negation_traits<OP1>;

    template<class OP1, class OP2>
    using addition_traits = matrix_addition_traits<OP1, OP2>;

    template<class OP1, class OP2>
    using subtraction_traits = matrix_subtraction_traits<OP1, OP2>;

    template<class OP1, class OP2>
    using multiplication_traits = matrix_multiplication_traits<OP1, OP2>;
};


//=================================================================================================
//                             **** OPERATOR TRAITS PROMOTION ****
//=================================================================================================
//  Traits type that determines which operator traits to use in an expression.
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
struct matrix_operator_traits_promotion<T1, matrix_operator_traits>
{
    using traits_type = T1;
};

template<class T1>
struct matrix_operator_traits_promotion<matrix_operator_traits, T1>
{
    using traits_type = T1;
};

template<>
struct matrix_operator_traits_promotion<matrix_operator_traits, matrix_operator_traits>
{
    using traits_type = matrix_operator_traits;
};

template<class T1, class T2>
using matrix_operator_traits_promotion_t = typename matrix_operator_traits_promotion<T1,T2>::traits_type;


}       //- std::la namespace
#endif  //- MATRIX_OPERATOR_TRAITS_HPP_DEFINED
