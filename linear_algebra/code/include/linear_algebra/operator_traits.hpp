#ifndef LINEAR_ALGEBRA_OPERATOR_TRAITS_HPP_DEFINED
#define LINEAR_ALGEBRA_OPERATOR_TRAITS_HPP_DEFINED

#include "arithmetic_traits.hpp"

namespace STD_LA {
//=================================================================================================
//                                  **** OPERATOR TRAITS ****
//=================================================================================================
//  Traits type that refers to the four basic arithmetic traits types.
//=================================================================================================
//
struct default_matrix_operator_traits
{
    //- Default element promotion traits.
    //
    template<class T1, class T2>
    using element_promotion_traits = matrix_element_promotion<T1, T2>;

    //- Default engine promotion traits.
    //
    template<class ET1>
    using negation_engine_promotion_traits = matrix_negation_engine_promotion<ET1>;

    template<class ET1, class ET2>
    using addition_engine_promotion_traits = matrix_addition_engine_promotion<ET1, ET2>;

    template<class ET1, class ET2>
    using subtraction_engine_promotion_traits = matrix_subtraction_engine_promotion<ET1, ET2>;

    template<class ET1, class ET2>
    using multiplication_engine_promotion_traits = matrix_multiplication_engine_promotion<ET1, ET2>;

    //- Default arithmetic operation traits.
    //
    template<class OP1, class OTR>
    using negation_traits = matrix_negation_traits<OP1, OTR>;

    template<class OP1, class OP2, class OTR>
    using addition_traits = matrix_addition_traits<OP1, OP2, OTR>;

    template<class OP1, class OP2, class OTR>
    using subtraction_traits = matrix_subtraction_traits<OP1, OP2, OTR>;

    template<class OP1, class OP2, class OTR>
    using multiplication_traits = matrix_multiplication_traits<OP1, OP2, OTR>;
};

//=================================================================================================
//                             **** OPERATOR TRAITS SELECTION ****
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


#if 0

given operator traits types OT1 and OT2:

if OT1 and OT2 are both default
    result = default
else if OT1 == OT2
    result = OT2
else if exactly OT1 == default or OT2 == default
    result = default | OT
else
    result = user-supplied operator traits selector chooses

#endif

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_OPERATOR_TRAITS_HPP_DEFINED
