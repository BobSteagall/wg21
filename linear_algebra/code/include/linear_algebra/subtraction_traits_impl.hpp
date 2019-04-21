#ifndef LINEAR_ALGEBRA_SUBTRACTION_TRAITS_IMPL_HPP_DEFINED
#define LINEAR_ALGEBRA_SUBTRACTION_TRAITS_IMPL_HPP_DEFINED

namespace STD_LA {
//==================================================================================================
//                       **** SUBTRACTION TRAITS FUNCTION IMPLEMENTATION ****
//==================================================================================================
//
template<class OT, class ET1, class OT1, class ET2, class OT2> inline auto
matrix_subtraction_traits<OT, vector<ET1, OT1>, vector<ET2, OT2>>::subtract
(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2) -> result_type
{
    PrintOperandTypes<result_type>("subtraction_traits", v1, v2);
    return result_type();
}

//------
//
template<class OT, class ET1, class OT1, class ET2, class OT2> inline auto
matrix_subtraction_traits<OT, matrix<ET1, OT1>, matrix<ET2, OT2>>::subtract
(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2) -> result_type
{
    PrintOperandTypes<result_type>("subtraction_traits", m1, m2);
    return result_type();
}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_SUBTRACTION_TRAITS_IMPL_HPP_DEFINED