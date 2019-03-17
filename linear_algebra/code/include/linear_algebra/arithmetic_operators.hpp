#ifndef LINEAR_ALGEBRA_ARITHMETIC_OPERATORS_HPP_DEFINED
#define LINEAR_ALGEBRA_ARITHMETIC_OPERATORS_HPP_DEFINED

#include "operator_promotion_traits.hpp"

namespace STD_LA {
//=================================================================================================
//  Unary negation operators, which forward to the negation traits do the work.
//=================================================================================================
//
template<class ET1, class OT1>
inline auto
operator -(column_vector<ET1, OT1> const& cv1)
{
    using op1_type   = column_vector<ET1, OT1>;
    using op_traits  = OT1;
    using neg_traits = typename OT1::template negation_traits<op1_type, op_traits>;

    return neg_traits::negate(cv1);
}

template<class ET1, class OT1>
inline auto
operator -(row_vector<ET1, OT1> const& rv1)
{
    using op1_type   = row_vector<ET1, OT1>;
    using op_traits  = OT1;
    using neg_traits = typename OT1::template negation_traits<op1_type, op_traits>;

    return neg_traits::negate(rv1);
}

template<class ET1, class OT1>
inline auto
operator -(matrix<ET1, OT1> const& m1)
{
    using op1_type   = matrix<ET1, OT1>;
    using op_traits  = OT1;
    using neg_traits = typename OT1::template negation_traits<op1_type, op_traits>;

    return neg_traits::negate(m1);
}


//=================================================================================================
//  Binary addition operators, which forward to the addition traits do the work.
//=================================================================================================
//
template<class ET1, class OT1, class ET2, class OT2>
inline auto
operator +(column_vector<ET1, OT1> const& cv1, column_vector<ET2, OT2> const& cv2)
{
    using op_traits  = matrix_operator_traits_promotion_t<OT1, OT2>;
    using op1_type   = column_vector<ET1, OT1>;
    using op2_type   = column_vector<ET2, OT2>;
    using add_traits = typename op_traits::template addition_traits<op1_type, op2_type, op_traits>;

    return add_traits::add(cv1, cv2);
}

template<class ET1, class OT1, class ET2, class OT2>
inline auto
operator +(row_vector<ET1, OT1> const& rv1, row_vector<ET2, OT2> const& rv2)
{
    using op_traits  = matrix_operator_traits_promotion_t<OT1, OT2>;
    using op1_type   = row_vector<ET1, OT1>;
    using op2_type   = row_vector<ET2, OT2>;
    using add_traits = typename op_traits::template addition_traits<op1_type, op2_type, op_traits>;

    return add_traits::add(rv1, rv2);
}

template<class ET1, class OT1, class ET2, class OT2>
inline auto
operator +(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2)
{
    using op_traits  = matrix_operator_traits_promotion_t<OT1, OT2>;
    using op1_type   = matrix<ET1, OT1>;
    using op2_type   = matrix<ET2, OT2>;
    using add_traits = typename op_traits::template addition_traits<op1_type, op2_type, op_traits>;

    return add_traits::add(m1, m2);
}


//=================================================================================================
//  Binary subtraction operators, which forward to the subraction traits do the work.
//=================================================================================================
//
template<class ET1, class OT1, class ET2, class OT2>
inline auto
operator -(column_vector<ET1, OT1> const& cv1, column_vector<ET2, OT2> const& cv2)
{
    using op_traits  = matrix_operator_traits_promotion_t<OT1, OT2>;
    using op1_type   = column_vector<ET1, OT1>;
    using op2_type   = column_vector<ET2, OT2>;
    using sub_traits = typename op_traits::template subtraction_traits<op1_type, op2_type, op_traits>;

    return sub_traits::subtract(cv1, cv2);
}

template<class ET1, class OT1, class ET2, class OT2>
inline auto
operator -(row_vector<ET1, OT1> const& rv1, row_vector<ET2, OT2> const& rv2)
{
    using op_traits  = matrix_operator_traits_promotion_t<OT1, OT2>;
    using op1_type   = row_vector<ET1, OT1>;
    using op2_type   = row_vector<ET2, OT2>;
    using sub_traits = typename op_traits::template subtraction_traits<op1_type, op2_type, op_traits>;

    return sub_traits::subtract(rv1, rv2);
}

template<class ET1, class OT1, class ET2, class OT2>
inline auto
operator -(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2)
{
    using op_traits  = matrix_operator_traits_promotion_t<OT1, OT2>;
    using op1_type   = matrix<ET1, OT1>;
    using op2_type   = matrix<ET2, OT2>;
    using sub_traits = typename op_traits::template subtraction_traits<op1_type, op2_type, op_traits>;

    return sub_traits::subtract(m1, m2);
}

//=================================================================================================
//  Multiplication operators, which forward to the traits types that perform the multiplications.
//=================================================================================================
//- column_vector*scalar
//
template<class ET1, class OT1, class S2>
inline auto
operator *(column_vector<ET1, OT1> const& cv1, S2 s2)
{
    using op_traits  = OT1;
    using op1_type   = column_vector<ET1, OT1>;
    using op2_type   = S2;
    using mul_traits = typename op_traits::template multiplication_traits<op1_type, op2_type, op_traits>;

    return mul_traits::multiply(cv1, s2);
}

template<class S1, class ET2, class OT2>
inline auto
operator *(S1 s1, column_vector<ET2, OT2> const& cv2)
{
    using op_traits  = OT2;
    using op1_type   = S1;
    using op2_type   = column_vector<ET2, OT2>;
    using mul_traits = typename op_traits::template multiplication_traits<op1_type, op2_type, op_traits>;

    return mul_traits::multiply(s1, cv2);
}

//-------------------
//- row_vector*scalar
//
template<class ET1, class OT1, class S2>
inline auto
operator *(row_vector<ET1, OT1> const& rv1, S2 s2)
{
    using op_traits  = OT1;
    using op1_type   = row_vector<ET1, OT1>;
    using op2_type   = S2;
    using mul_traits = typename op_traits::template multiplication_traits<op1_type, op2_type, op_traits>;

    return mul_traits::multiply(rv1, s2);
}

template<class S1, class ET2, class OT2>
inline auto
operator *(S1 s1, row_vector<ET2, OT2> const& rv2)
{
    using op_traits  = OT2;
    using op1_type   = S1;
    using op2_type   = row_vector<ET2, OT2>;
    using mul_traits = typename op_traits::template multiplication_traits<op1_type, op2_type, op_traits>;

    return mul_traits::multiply(s1, rv2);
}

//---------------
//- matrix*scalar
//
template<class ET1, class OT1, class S2>
inline auto
operator *(matrix<ET1, OT1> const& m1, S2 s2)
{
    using op_traits  = OT1;
    using op1_type   = matrix<ET1, OT1>;
    using op2_type   = S2;
    using mul_traits = typename op_traits::template multiplication_traits<op1_type, op2_type, op_traits>;

    return mul_traits::multiply(m1, s2);
}

template<class S1, class ET2, class OT2>
inline auto
operator *(S1 s1, matrix<ET2, OT2> const& m2)
{
    using op_traits  = OT2;
    using op1_type   = S1;
    using op2_type   = matrix<ET2, OT2>;
    using mul_traits = typename op_traits::template multiplication_traits<op2_type, op1_type, op_traits>;

    return mul_traits::multiply(m2, s1);
}

//---------------
//- vector*vector
//
template<class ET1, class OT1, class ET2, class OT2>
inline auto
operator *(row_vector<ET1, OT1> const& rv1, column_vector<ET2, OT2> const& cv2)
{
    using op_traits  = matrix_operator_traits_promotion_t<OT1, OT2>;
    using op1_type   = row_vector<ET1, OT1>;
    using op2_type   = column_vector<ET2, OT2>;
    using mul_traits = typename op_traits::template multiplication_traits<op1_type, op2_type, op_traits>;

    return mul_traits::multiply(rv1, cv2);
}

template<class ET1, class OT1, class ET2, class OT2>
inline auto
operator *(column_vector<ET1, OT1> const& cv1, row_vector<ET2, OT2> const& rv2)
{
    using op_traits  = matrix_operator_traits_promotion_t<OT1, OT2>;
    using op1_type   = column_vector<ET1, OT1>;
    using op2_type   = row_vector<ET2, OT2>;
    using mul_traits = typename op_traits::template multiplication_traits<op1_type, op2_type, op_traits>;

    return mul_traits::multiply(cv1, rv2);
}

//---------------
//- matrix*vector
//
template<class ET1, class OT1, class ET2, class OT2>
inline auto
operator *(matrix<ET1, OT1> const& m1, column_vector<ET2, OT2> const& cv2)
{
    using op_traits  = matrix_operator_traits_promotion_t<OT1, OT2>;
    using op1_type   = matrix<ET1, OT1>;
    using op2_type   = column_vector<ET2, OT2>;
    using mul_traits = typename op_traits::template multiplication_traits<op1_type, op2_type, op_traits>;

    return mul_traits::multiply(m1, cv2);
}

template<class ET1, class OT1, class ET2, class OT2>
inline auto
operator *(matrix<ET1, OT1> const& m1, row_vector<ET2, OT2> const& rv2)
{
    using op_traits  = matrix_operator_traits_promotion_t<OT1, OT2>;
    using op1_type   = matrix<ET1, OT1>;
    using op2_type   = row_vector<ET2, OT2>;
    using mul_traits = typename op_traits::template multiplication_traits<op1_type, op2_type, op_traits>;

    return mul_traits::multiply(m1, rv2);
}

//---------------
//- vector*matrix
//
template<class ET1, class OT1, class ET2, class OT2>
inline auto
operator *(column_vector<ET1, OT1> const& cv1, matrix<ET2, OT2> const& m2)
{
    using op_traits  = matrix_operator_traits_promotion_t<OT1, OT2>;
    using op1_type   = column_vector<ET1, OT1>;
    using op2_type   = matrix<ET2, OT2>;
    using mul_traits = typename op_traits::template multiplication_traits<op1_type, op2_type, op_traits>;

    return mul_traits::multiply(cv1, m2);
}

template<class ET1, class OT1, class ET2, class OT2>
inline auto
operator *(row_vector<ET1, OT1> const& rv1, matrix<ET2, OT2> const& m2)
{
    using op_traits  = matrix_operator_traits_promotion_t<OT1, OT2>;
    using op1_type   = row_vector<ET1, OT1>;
    using op2_type   = matrix<ET2, OT2>;
    using mul_traits = typename op_traits::template multiplication_traits<op1_type, op2_type, op_traits>;

    return mul_traits::multiply(rv1, m2);
}

//---------------
//- matrix*matrix
//
template<class ET1, class OT1, class ET2, class OT2>
inline auto
operator *(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2)
{
    using op_traits  = matrix_operator_traits_promotion_t<OT1, OT2>;
    using op1_type   = matrix<ET1, OT1>;
    using op2_type   = matrix<ET2, OT2>;
    using mul_traits = typename op_traits::template multiplication_traits<op1_type, op2_type, op_traits>;

    return mul_traits::multiply(m1, m2);
}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_ARITHMETIC_OPERATORS_HPP_DEFINED
