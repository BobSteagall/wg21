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
//=================================================================================================
//  Binary addition operators, which employ the addition arithmetic traits to do the work.
//=================================================================================================
//
template<class ET1, class OT1, class ET2, class OT2> inline constexpr
auto
operator +(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2)
{
    using op_traits  = select_matrix_operation_traits_t<OT1, OT2>;
    using op1_type   = vector<ET1, OT1>;
    using op2_type   = vector<ET2, OT2>;
    using add_traits = select_matrix_addition_arithmetic_t<op_traits, op1_type, op2_type>;

    return add_traits::add(v1, v2);
}

template<class ET1, class OT1, class ET2, class OT2> inline constexpr
auto
operator +(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2)
{
    using op_traits  = select_matrix_operation_traits_t<OT1, OT2>;
    using op1_type   = matrix<ET1, OT1>;
    using op2_type   = matrix<ET2, OT2>;
    using add_traits = select_matrix_addition_arithmetic_t<op_traits, op1_type, op2_type>;

    return add_traits::add(m1, m2);
}


//=================================================================================================
//  Binary subtraction operators, which employ the subtraction arithmetic traits to do the work.
//=================================================================================================
//
template<class ET1, class OT1, class ET2, class OT2> inline constexpr
auto
operator -(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2)
{
    using op_traits  = select_matrix_operation_traits_t<OT1, OT2>;
    using op1_type   = vector<ET1, OT1>;
    using op2_type   = vector<ET2, OT2>;
    using sub_traits = select_matrix_subtraction_arithmetic_t<op_traits, op1_type, op2_type>;

    return sub_traits::subtract(v1, v2);
}

template<class ET1, class OT1, class ET2, class OT2> inline constexpr
auto
operator -(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2)
{
    using op_traits  = select_matrix_operation_traits_t<OT1, OT2>;
    using op1_type   = matrix<ET1, OT1>;
    using op2_type   = matrix<ET2, OT2>;
    using sub_traits = select_matrix_subtraction_arithmetic_t<op_traits, op1_type, op2_type>;

    return sub_traits::subtract(m1, m2);
}


//=================================================================================================
//  Multiplication operators, which employ the multiplication arithmetic traits to do the work.
//=================================================================================================
//
template<class ET1, class OT1, class S2> inline constexpr
auto
operator *(vector<ET1, OT1> const& v1, S2 const& s2)
{
    using op_traits  = OT1;
    using op1_type   = vector<ET1, OT1>;
    using op2_type   = S2;
    using mul_traits = select_matrix_multiplication_arithmetic_t<op_traits, op1_type, op2_type>;

    return mul_traits::multiply(v1, s2);
}

template<class S1, class ET2, class OT2> inline constexpr
auto
operator *(S1 const& s1, vector<ET2, OT2> const& v2)
{
    using op_traits  = OT2;
    using op1_type   = S1;
    using op2_type   = vector<ET2, OT2>;
    using mul_traits = select_matrix_multiplication_arithmetic_t<op_traits, op1_type, op2_type>;

    return mul_traits::multiply(s1, v2);
}

//------
//
template<class ET1, class OT1, class S2> inline constexpr
auto
operator *(matrix<ET1, OT1> const& m1, S2 const& s2)
{
    using op_traits  = OT1;
    using op1_type   = matrix<ET1, OT1>;
    using op2_type   = S2;
    using mul_traits = select_matrix_multiplication_arithmetic_t<op_traits, op1_type, op2_type>;

    return mul_traits::multiply(m1, s2);
}

template<class S1, class ET2, class OT2> inline constexpr
auto
operator *(S1 const& s1, matrix<ET2, OT2> const& m2)
{
    using op_traits  = OT2;
    using op1_type   = S1;
    using op2_type   = matrix<ET2, OT2>;
    using mul_traits = select_matrix_multiplication_arithmetic_t<op_traits, op1_type, op2_type>;

    return mul_traits::multiply(s1, m2);
}

//------
//
template<class ET1, class OT1, class ET2, class OT2> inline constexpr
auto
operator *(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2)
{
    using op_traits  = select_matrix_operation_traits_t<OT1, OT2>;
    using op1_type   = vector<ET1, OT1>;
    using op2_type   = vector<ET2, OT2>;
    using mul_traits = select_matrix_multiplication_arithmetic_t<op_traits, op1_type, op2_type>;

    return mul_traits::multiply(v1, v2);
}

//------
//
template<class ET1, class OT1, class ET2, class OT2> inline constexpr
auto
operator *(matrix<ET1, OT1> const& m1, vector<ET2, OT2> const& v2)
{
    using op_traits  = select_matrix_operation_traits_t<OT1, OT2>;
    using op1_type   = matrix<ET1, OT1>;
    using op2_type   = vector<ET2, OT2>;
    using mul_traits = select_matrix_multiplication_arithmetic_t<op_traits, op1_type, op2_type>;

    return mul_traits::multiply(m1, v2);
}

//------
//
template<class ET1, class OT1, class ET2, class OT2> inline constexpr
auto
operator *(vector<ET1, OT1> const& v1, matrix<ET2, OT2> const& m2)
{
    using op_traits  = select_matrix_operation_traits_t<OT1, OT2>;
    using op1_type   = vector<ET1, OT1>;
    using op2_type   = matrix<ET2, OT2>;
    using mul_traits = select_matrix_multiplication_arithmetic_t<op_traits, op1_type, op2_type>;

    return mul_traits::multiply(v1, m2);
}

//------
//
template<class ET1, class OT1, class ET2, class OT2> inline constexpr
auto
operator *(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2)
{
    using op_traits  = select_matrix_operation_traits_t<OT1, OT2>;
    using op1_type   = matrix<ET1, OT1>;
    using op2_type   = matrix<ET2, OT2>;
    using mul_traits = select_matrix_multiplication_arithmetic_t<op_traits, op1_type, op2_type>;

    return mul_traits::multiply(m1, m2);
}


//=================================================================================================
//  Scalar division operators, which employ the division arithmetic traits to do the work.
//=================================================================================================
//
template<class ET1, class OT1, class S2> inline constexpr
auto
operator /(vector<ET1, OT1> const& v1, S2 const& s2)
{
    using op_traits  = OT1;
    using op1_type   = vector<ET1, OT1>;
    using op2_type   = S2;
    using div_traits = select_matrix_division_arithmetic_t<op_traits, op1_type, op2_type>;

    return div_traits::divide(v1, s2);
}

template<class ET1, class OT1, class S2> inline constexpr
auto
operator /(matrix<ET1, OT1> const& m1, S2 const& s2)
{
    using op_traits  = OT1;
    using op1_type   = matrix<ET1, OT1>;
    using op2_type   = S2;
    using div_traits = select_matrix_division_arithmetic_t<op_traits, op1_type, op2_type>;

    return div_traits::divide(m1, s2);
}


//------
//
template<class ET1, class OT1, class ET2, class OT2> inline constexpr
auto
inner_product(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2)
{
    using op_traits  = select_matrix_operation_traits_t<OT1, OT2>;
    using op1_type   = vector<ET1, OT1>;
    using op2_type   = vector<ET2, OT2>;
    using mul_traits = select_matrix_multiplication_arithmetic_t<op_traits, op1_type, op2_type>;

    return mul_traits::multiply(v1, v2);
}

template<class ET1, class OT1, class ET2, class OT2> inline constexpr
auto
outer_product(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2)
{
    //- TODO: This is going to require some thinking.  Engine type determination for
    //  this case must be worked out; current case only covers inner product.
}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_ARITHMETIC_OPERATORS_HPP_DEFINED
