
//==================================================================================================
//  File:       arithmetic_operators.hpp
//
//  Summary:    This header defines the overloaded operators that implement basic arithmetic
//              operations on vectors and matrices.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_ARITHMETIC_OPERATORS_HPP_DEFINED
#define LINEAR_ALGEBRA_ARITHMETIC_OPERATORS_HPP_DEFINED

namespace STD_LA {
//==================================================================================================
//  Addition operator, which employs the addition arithmetic traits to do the work.
//==================================================================================================
//
template<class ET1, class COT1, class ET2, class COT2> inline constexpr
auto
operator +(basic_matrix<ET1, COT1> const& m1, basic_matrix<ET2, COT2> const& m2)
{
    using op_traits  = select_matrix_operation_traits_t<COT1, COT2>;
    using op1_type   = basic_matrix<ET1, COT1>;
    using op2_type   = basic_matrix<ET2, COT2>;
    using add_traits = get_addition_arithmetic_traits_t<op_traits, op1_type, op2_type>;

    return add_traits::add(m1, m2);
}
#if 0
//==================================================================================================
//  Subtraction operator, which employs the subtraction arithmetic traits to do the work.
//==================================================================================================
//
template<class ET1, class COT1, class ET2, class COT2> inline constexpr
auto
operator -(matrix<ET1, COT1> const& m1, matrix<ET2, COT2> const& m2)
{
    using op_traits  = select_matrix_operation_traits_t<COT1, COT2>;
    using op1_type   = matrix<ET1, COT1>;
    using op2_type   = matrix<ET2, COT2>;
    using sub_traits = select_matrix_subtraction_arithmetic_t<op_traits, op1_type, op2_type>;

    return sub_traits::subtract(m1, m2);
}

//==================================================================================================
//  Multiplication operators, which employ the multiplication arithmetic traits to do the work.
//==================================================================================================
//
template<class ET1, class COT1, class S2> inline constexpr
auto
operator *(matrix<ET1, COT1> const& m1, S2 const& s2)
{
    using op_traits  = COT1;
    using op1_type   = matrix<ET1, COT1>;
    using op2_type   = S2;
    using mul_traits = select_matrix_multiplication_arithmetic_t<op_traits, op1_type, op2_type>;

    return mul_traits::multiply(m1, s2);
}

template<class S1, class ET2, class COT2> inline constexpr
auto
operator *(S1 const& s1, matrix<ET2, COT2> const& m2)
{
    using op_traits  = COT2;
    using op1_type   = S1;
    using op2_type   = matrix<ET2, COT2>;
    using mul_traits = select_matrix_multiplication_arithmetic_t<op_traits, op1_type, op2_type>;

    return mul_traits::multiply(s1, m2);
}

template<class ET1, class COT1, class ET2, class COT2> inline constexpr
auto
operator *(matrix<ET1, COT1> const& m1, matrix<ET2, COT2> const& m2)
{
    using op_traits  = select_matrix_operation_traits_t<COT1, COT2>;
    using op1_type   = matrix<ET1, COT1>;
    using op2_type   = matrix<ET2, COT2>;
    using mul_traits = select_matrix_multiplication_arithmetic_t<op_traits, op1_type, op2_type>;

    return mul_traits::multiply(m1, m2);
}

//==================================================================================================
//  Scalar division operator, which employs the division arithmetic traits to do the work.
//==================================================================================================
//
template<class ET1, class COT1, class S2> inline constexpr
auto
operator /(matrix<ET1, COT1> const& m1, S2 const& s2)
{
    using op_traits  = COT1;
    using op1_type   = matrix<ET1, COT1>;
    using op2_type   = S2;
    using div_traits = select_matrix_division_arithmetic_t<op_traits, op1_type, op2_type>;

    return div_traits::divide(m1, s2);
}

#endif

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_ARITHMETIC_OPERATORS_HPP_DEFINED
