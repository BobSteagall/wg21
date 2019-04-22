#ifndef LINEAR_ALGEBRA_MULTIPLICATION_TRAITS_IMPL_HPP_DEFINED
#define LINEAR_ALGEBRA_MULTIPLICATION_TRAITS_HPP_DEFINED

namespace STD_LA {
//==================================================================================================
//                   **** MULTIPLICATION TRAITS FUNCTION IMPLEMENTATION ****
//==================================================================================================
//
//- vector*scalar
//
template<class OTR, class ET1, class OT1, class T2> 
inline auto
matrix_multiplication_traits<OTR, vector<ET1, OT1>, T2>::multiply
(vector<ET1, OT1> const& v1, T2 const& s2) -> result_type
{
    PrintOperandTypes<result_type>("multiplication_traits (v*s)", v1, s2);
    return result_type();
}

//---------------
//- scalar*vector
//
template<class OTR, class T1, class ET2, class OT2> 
inline auto
matrix_multiplication_traits<OTR, T1, vector<ET2, OT2>>::multiply
(T1 const& s1, vector<ET2, OT2> const& v2) -> result_type
{
    PrintOperandTypes<result_type>("multiplication_traits (s*v)", s1, v2);
    return result_type();
}

//---------------
//- matrix*scalar
//
template<class OTR, class ET1, class OT1, class T2> 
inline auto
matrix_multiplication_traits<OTR, matrix<ET1, OT1>, T2>::multiply
(matrix<ET1, OT1> const& m1, T2 const& s2) -> result_type
{
    PrintOperandTypes<result_type>("multiplication_traits (m*s)", m1, s2);
    return result_type();
}

//---------------
//- scalar*matrix
//
template<class OTR, class T1, class ET2, class OT2> 
inline auto
matrix_multiplication_traits<OTR, T1, matrix<ET2, OT2>>::multiply
(T1 const& s1, matrix<ET2, OT2> const& m2) -> result_type
{
    PrintOperandTypes<result_type>("multiplication_traits (s*m)", s1, m2);
    return result_type();
}

//- vector*vector
//
template<class OTR, class ET1, class OT1, class ET2, class OT2> 
inline auto
matrix_multiplication_traits<OTR, vector<ET1, OT1>, vector<ET2, OT2>>::multiply
(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2) -> result_type
{
    PrintOperandTypes<result_type>("multiplication_traits (v*v)", v1, v2);
    return result_type();
}

//---------------
//- matrix*vector
//
template<class OTR, class ET1, class OT1, class ET2, class OT2> 
inline auto
matrix_multiplication_traits<OTR, matrix<ET1, OT1>, vector<ET2, OT2>>::multiply
(matrix<ET1, OT1> const& m1, vector<ET2, OT2> const& m2) -> result_type
{
    PrintOperandTypes<result_type>("multiplication_traits (m*v) ", m1, m2);
    return result_type();
}

//---------------
//- vector*matrix
//
template<class OTR, class ET1, class OT1, class ET2, class OT2> 
inline auto
matrix_multiplication_traits<OTR, vector<ET1, OT1>, matrix<ET2, OT2>>::multiply
(vector<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2) -> result_type
{
    PrintOperandTypes<result_type>("multiplication_traits (v*m)", m1, m2);
    return result_type();
}

//---------------
//- matrix*matrix
//
template<class OTR, class ET1, class OT1, class ET2, class OT2> 
inline auto
matrix_multiplication_traits<OTR, matrix<ET1, OT1>, matrix<ET2, OT2>>::multiply
(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2) -> result_type
{
    PrintOperandTypes<result_type>("multiplication_traits (m*m)", m1, m2);
    return result_type();
}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_MULTIPLICATION_TRAITS_HPP_DEFINED
