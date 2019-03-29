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
    template<class T1, class T2>
    using element_promotion_traits = matrix_element_promotion<T1, T2>;

    template<class OP1, class OTR>
    using negation_traits = matrix_negation_traits<OP1, OTR>;

    template<class OP1, class OP2, class OTR>
    using addition_traits = matrix_addition_traits<OP1, OP2, OTR>;

    template<class OP1, class OP2, class OTR>
    using subtraction_traits = matrix_subtraction_traits<OP1, OP2, OTR>;

    template<class OP1, class OP2, class OTR>
    using multiplication_traits = matrix_multiplication_traits<OP1, OP2, OTR>;
};

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_OPERATOR_TRAITS_HPP_DEFINED
