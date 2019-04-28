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

	transform(v1.begin(), v1.end(), vr.begin(), [](auto val){ return -val; });

	return vr;
}

//------
//
template<class OT, class ET1, class OT1>
inline auto
matrix_negation_traits<OT, matrix<ET1, OT1>>::negate(matrix<ET1, OT1> const& m1) -> result_type
{
    PrintOperandTypes<result_type>("negation_traits", m1);

	result_type		mr{};                           //- Braces here to avoid C4701 from MSVC
	auto const      rows = m1.rows();
	auto const      columns = m1.columns();

	if constexpr (result_requires_resize(mr))
	{
		mr.resize(rows, columns);
	}

	for (auto i = 0;  i < rows;  ++i)
    {
		for (auto j = 0;  j <  columns;  ++j)
        {
			mr(i, j) = -m1(i, j);
        }
    }
/*
	if constexpr (result_requires_resize(mr))
	{
		mr.resize(rows, columns);
		auto data = mr.data();
		for (auto i = 0; i < columns; ++i)
			for (auto j = 0; j < rows; ++j)
				*data++ = -m1(i, j);		// Safe because the resize means that mr capacity = size for rows and columns.
	}
	else
	{
		transform(m1.data(), m1.data() + (rows * columns), mr.data(),
			[](auto val) {return -val; });
	}
*/
	return mr;
}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_NEGATION_TRAITS_IMPL_HPP_DEFINED
