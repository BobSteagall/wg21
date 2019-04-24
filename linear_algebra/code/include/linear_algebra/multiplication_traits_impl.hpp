//==================================================================================================
//  File:       multiplication_traits_impl.hpp
//
//  Summary:    This header defines the static member functions of matrix_multiplication_traits
//              that perform the actual arithmetic.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_MULTIPLICATION_TRAITS_IMPL_HPP_DEFINED
#define LINEAR_ALGEBRA_MULTIPLICATION_TRAITS_IMPL_HPP_DEFINED

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

	result_type     vr;

	if constexpr (result_requires_resize(vr))
	{
		vr.resize(v1.elements());
	}

	transform(v1.data(), v1.data() + v1.elements(), vr.data(),
		[&](auto val) {return val * s2; });

	return vr;
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

	result_type     vr;

	if constexpr (result_requires_resize(vr))
	{
		vr.resize(v2.elements());
	}

	transform(v2.data(), v2.data() + v2.elements(), vr.data(),
		[&](auto val) {return val * s1; });

	return vr;
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

	result_type		mr;
	const auto		rows = m1.rows();
	const auto		columns = m1.columns();

	if constexpr (result_requires_resize(mr))
	{
		mr.resize(rows, columns);
		auto data = mr.data();
		for (auto i = 0; i < columns; ++i)
			for (auto j = 0; j < rows; ++j)
				* data++ = m1(i, j) * s2;		// Safe because the resize means that mr capacity = size for rows and columns.
	}
	else
	{
		transform(m1.data(), m1.data() + (rows * columns), mr.data(),
			[&](auto val) {return val * s2; });
	}

	return mr;
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

	result_type		mr;
	const auto		rows = m2.rows();
	const auto		columns = m2.columns();

	if constexpr (result_requires_resize(mr))
	{
		mr.resize(rows, columns);
		auto data = mr.data();
		for (auto i = 0; i < columns; ++i)
			for (auto j = 0; j < rows; ++j)
				* data++ = m2(i, j) * s1;		// Safe because the resize means that mr capacity = size for rows and columns.
	}
	else
	{
		transform(m2.data(), m2.data() + (rows * columns), mr.data(),
			[&](auto val) {return val * s1; });
	}

	return mr;
}

//- vector*vector
//
template<class OTR, class ET1, class OT1, class ET2, class OT2>
inline auto
matrix_multiplication_traits<OTR, vector<ET1, OT1>, vector<ET2, OT2>>::multiply
(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2) -> result_type
{
    PrintOperandTypes<result_type>("multiplication_traits (v*v)", v1, v2);
    return std::inner_product(v1.data(), v1.data() + v1.elements(), v2.data(), result_type(0));
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
#endif  //- LINEAR_ALGEBRA_MULTIPLICATION_TRAITS_IMPL_HPP_DEFINED
