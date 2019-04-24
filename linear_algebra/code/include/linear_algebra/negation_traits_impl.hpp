//==================================================================================================
//  File:       negation_traits_impl.hpp
//
//  Summary:    This header defines the static member functions of matrix_negation_traits that
//              perform the actual arithmetic.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_NEGATION_TRAITS_IMPL_HPP_DEFINED
#define LINEAR_ALGEBRA_NEGATION_TRAITS_IMPL_HPP_DEFINED

namespace STD_LA {
//==================================================================================================
//                         **** NEGATION TRAITS FUNCTION IMPLEMENTATION ****
//==================================================================================================
//
template<class OT, class ET1, class OT1>
inline auto
matrix_negation_traits<OT, vector<ET1, OT1>>::negate(vector<ET1, OT1> const& v1) -> result_type
{
    PrintOperandTypes<result_type>("negation_traits", v1);

	result_type     vr;

	if constexpr (result_requires_resize(vr))
	{
		vr.resize(v1.elements());
	}

	transform(v1.data(), v1.data() + v1.elements(), vr.data(),
		[](auto val) {return val * -1; });

	return vr;
}

//------
//
template<class OT, class ET1, class OT1>
inline auto
matrix_negation_traits<OT, matrix<ET1, OT1>>::negate(matrix<ET1, OT1> const& m1) -> result_type
{
    PrintOperandTypes<result_type>("negation_traits", m1);
    return result_type();
}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_NEGATION_TRAITS_IMPL_HPP_DEFINED
