//==================================================================================================
//  File:       addition_traits_impl.hpp
//
//  Summary:    This header defines the static member functions of matrix_addition_traits that
//              perform the actual arithmetic.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_ADDITION_TRAITS_IMPL_HPP_DEFINED
#define LINEAR_ALGEBRA_ADDITION_TRAITS_IMPL_HPP_DEFINED

namespace STD_LA {
//=================================================================================================
//                        **** ADDITION TRAITS FUNCTION IMPLEMENTATION ****
//==================================================================================================
//
template<class OT, class ET1, class OT1, class ET2, class OT2>
inline auto
matrix_addition_traits<OT, vector<ET1, OT1>, vector<ET2, OT2>>::add
(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2) -> result_type
{
    PrintOperandTypes<result_type>("addition_traits", v1, v2);

    result_type     vr;

    if constexpr (result_requires_resize(vr))
    {
        vr.resize(v1.elements());
    }

    transform(v1.data(), v1.data() + v1.elements(), v2.data(), vr.data(),
              [](auto lhs, auto rhs) {return lhs + rhs; });

    return vr;
}

//------
//
template<class OT, class ET1, class OT1, class ET2, class OT2>
inline auto
matrix_addition_traits<OT, matrix<ET1, OT1>, matrix<ET2, OT2>>::add
(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2) -> result_type
{
    PrintOperandTypes<result_type>("addition_traits", m1, m2);
    return result_type();
}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_ADDITION_TRAITS_IMPL_HPP_DEFINED
