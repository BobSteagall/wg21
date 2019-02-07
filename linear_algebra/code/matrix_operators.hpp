#ifndef MATRIX_OPERATORS_HPP_DEFINED
#define MATRIX_OPERATORS_HPP_DEFINED

#include "matrix_operator_traits.hpp"

namespace std::la {
//=================================================================================================
//  Unary negation operators, which forward to the negation traits do the work.
//=================================================================================================
//
template<class E1>
inline auto
operator -(column_vector<E1> const& cv)
{
    return matrix_negation_traits<column_vector<E1>>::negate(cv);
}

template<class E1>
inline auto
operator -(row_vector<E1> const& cv)
{
    return matrix_negation_traits<row_vector<E1>>::negate(cv);
}

template<class E1>
inline auto
operator -(matrix<E1> const& cv)
{
    return matrix_negation_traits<matrix<E1>>::negate(cv);
}


//=================================================================================================
//  Binary addition operators, which forward to the addition traits do the work.
//=================================================================================================
//
template<class ET1, class AT1, class ET2, class AT2>
inline auto
operator +(column_vector<ET1,AT1> const& cv1, column_vector<ET2,AT2> const& cv2)
{
    using operator_traits = matrix_operator_traits_promotion_t<AT1, AT2>;
    using op1_type        = column_vector<ET1, AT1>;
    using op2_type        = column_vector<ET2, AT2>;
    using addition_traits = typename operator_traits::template addition_traits<op1_type, op2_type>;

    return addition_traits::add(cv1, cv2);
}

template<class E1, class E2>
inline auto
operator +(row_vector<E1> const& rv1, row_vector<E2> const& rv2)
{
    return matrix_addition_traits<row_vector<E1>, row_vector<E2>>::add(rv1, rv2);
}

template<class E1, class E2>
inline auto
operator +(matrix<E1> const& m1, matrix<E2> const& m2)
{
    return matrix_addition_traits<matrix<E1>, matrix<E2>>::add(m1, m2);
}


//=================================================================================================
//  Binary subtraction operators, which forward to the subraction traits do the work.
//=================================================================================================
//
template<class E1, class E2>
inline auto
operator -(column_vector<E1> const& cv1, column_vector<E2> const& cv2)
{
    return matrix_subtraction_traits<column_vector<E1>, column_vector<E2>>::subtract(cv1, cv2);
}

template<class E1, class E2>
inline auto
operator -(row_vector<E1> const& rv1, row_vector<E2> const& rv2)
{
    return matrix_subtraction_traits<row_vector<E1>, row_vector<E2>>::subtract(rv1, rv2);
}

template<class E1, class E2>
inline auto
operator -(matrix<E1> const& m1, matrix<E2> const& m2)
{
    return matrix_subtraction_traits<matrix<E1>, matrix<E2>>::subtract(m1, m2);
}

//=================================================================================================
//  Multiplication operators, which forward to the traits types that perform the multiplications.
//=================================================================================================
//- column_vector*scalar
//
template<class E1, class E2>
inline auto
operator *(column_vector<E1> const& cv, E2 s)
{
    return matrix_multiplication_traits<column_vector<E1>, E2>::multiply(cv, s);
}

template<class E1, class E2>
inline auto
operator *(E1 s, column_vector<E2> const& cv)
{
    return matrix_multiplication_traits<column_vector<E2>, E1>::multiply(cv, s);
}

//-------------------
//- row_vector*scalar
//
template<class E1, class E2>
inline auto
operator *(row_vector<E1> const& rv, E2 s)
{
    return matrix_multiplication_traits<row_vector<E1>, E2>::multiply(rv, s);
}

template<class E1, class E2>
inline auto
operator *(E1 s, row_vector<E2> const& rv)
{
    return matrix_multiplication_traits<row_vector<E2>, E1>::multiply(rv, s);
}

//---------------
//- matrix*scalar
//
template<class E1, class E2>
inline auto
operator *(matrix<E1> const& m, E2 s)
{
    return matrix_multiplication_traits<matrix<E1>, E2>::multiply(m, s);
}

template<class E1, class E2>
inline auto
operator *(E1 s, matrix<E2> const& m)
{
    return matrix_multiplication_traits<matrix<E2>, E1>::multiply(m, s);
}

//---------------
//- vector*vector
//
template<class E1, class E2>
inline auto
operator *(row_vector<E1> const& rv, column_vector<E2> const& cv)
{
    return matrix_multiplication_traits<row_vector<E1>, column_vector<E2>>::multiply(rv, cv);
}

template<class E1, class E2>
inline auto
operator *(column_vector<E1> const& cv, row_vector<E2> const& rv)
{
    return matrix_multiplication_traits<column_vector<E1>, row_vector<E2>>::multiply(cv, rv);
}

//---------------
//- matrix*vector
//
template<class E1, class E2>
inline auto
operator *(matrix<E1> const& m, column_vector<E2> const& cv)
{
    return matrix_multiplication_traits<matrix<E1>, column_vector<E2>>::multiply(m, cv);
}

template<class E1, class E2>
inline auto
operator *(matrix<E1> const& m, row_vector<E2> const& rv)
{
    return matrix_multiplication_traits<matrix<E1>, row_vector<E2>>::multiply(m, rv);
}

//---------------
//- vector*matrix
//
template<class E1, class E2>
inline auto
operator *(column_vector<E1> const& cv, matrix<E2> const& m)
{
    return matrix_multiplication_traits<column_vector<E1>, matrix<E2>>::multiply(cv, m);
}

template<class E1, class E2>
inline auto
operator *(row_vector<E1> const& rv, matrix<E2> const& m)
{
    return matrix_multiplication_traits<row_vector<E1>, matrix<E2>>::multiply(rv, m);
}

//---------------
//- matrix*matrix
//
template<class E1, class E2>
inline auto
operator *(matrix<E1> const& m1, matrix<E2> const& m2)
{
    return matrix_multiplication_traits<matrix<E1>, matrix<E2>>::multiply(m1, m2);
}

}       //- std::la namespace
#endif  //- MATRIX_OPERATORS_HPP_DEFINED
