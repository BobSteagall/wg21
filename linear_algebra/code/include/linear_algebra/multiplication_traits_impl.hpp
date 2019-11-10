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

	result_type     vr{};

	if constexpr (result_requires_resize(vr))
	{
		vr.resize(v1.elements());
	}

	transform(v1.begin(), v1.end(), vr.begin(), [&](auto val) { return val * s2; });

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

	return v2 * s1; //Exploit commutativity...
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

	result_type		mr{};
	auto const      rows = m1.rows();
	auto const      columns = m1.columns();

	if constexpr (result_requires_resize(mr))
	{
		mr.resize(rows, columns);
    }

	for (auto i = 0;  i < rows;  ++i)
    {
		for (auto j = 0;  j < columns;  ++j)
        {
			mr(i, j) = m1(i, j) * s2;
        }
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

	return m2 * s1; //Exploit commutativity...
}

//- vector*vector
//
template<class OTR, class ET1, class OT1, class ET2, class OT2>
inline auto
matrix_multiplication_traits<OTR, vector<ET1, OT1>, vector<ET2, OT2>>::multiply
(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2) -> result_type
{
    PrintOperandTypes<result_type>("multiplication_traits (v*v)", v1, v2);

	return std::inner_product(v1.begin(), v1.end(), v2.begin(), result_type(0));
}

//---------------
//- matrix*vector
//
template<class OTR, class ET1, class OT1, class ET2, class OT2>
inline auto
matrix_multiplication_traits<OTR, matrix<ET1, OT1>, vector<ET2, OT2>>::multiply
(matrix<ET1, OT1> const& m1, vector<ET2, OT2> const& v2) -> result_type
{
    PrintOperandTypes<result_type>("multiplication_traits (m*v) ", m1, v2);

	result_type     vr{};
	auto const      rows = m1.rows();
	auto const      columns = m1.columns();

	if constexpr (result_requires_resize(vr))
	{
		vr.resize(rows);
	}

	for (auto i = 0; i < rows; ++i)
	{
		typename result_type::element_type e(0);
		for (auto j = 0; j < columns; ++j)
		{
			e += m1(i, j) * v2(j);
		}
		vr(i) = e;
	}

	return vr;
}

//---------------
//- vector*matrix
//
template<class OTR, class ET1, class OT1, class ET2, class OT2>
inline auto
matrix_multiplication_traits<OTR, vector<ET1, OT1>, matrix<ET2, OT2>>::multiply
(vector<ET1, OT1> const& v1, matrix<ET2, OT2> const& m2) -> result_type
{
    PrintOperandTypes<result_type>("multiplication_traits (v*m)", v1, m2);

	// In fact, v*m = m(trans) * v, so we could reuse the m*v function

	result_type     vr{};
	auto const      rows = m2.rows();
	auto const      columns = m2.columns();

	if constexpr (result_requires_resize(vr))
	{
		vr.resize(columns);
	}

	for (auto i = 0; i < columns; ++i)
	{
		typename result_type::element_type e(0);
		for (auto j = 0; j < rows; ++j)
		{
			e += m2(i, j) * v1(j);
		}
		vr(i) = e;
	}

	return vr;
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

	result_type		mr{};
	auto const      result_rows = m1.rows();
	auto const      result_columns = m2.columns();

	if constexpr (result_requires_resize(mr))
	{
		mr.resize(result_rows, result_columns);
	}

	for (auto i = 0; i < result_rows; ++i)
	{
		for (auto j = 0; j < result_columns; ++j)
		{
			typename result_type::element_type e(0);
			for (auto k = 0; k < m2.rows(); ++k)
			{
				auto x = m1(i, k);
				auto y = m2(k, j);
				e += x * y;
			}
			mr(i, j) = e;
		}
	}

	return mr;
}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_MULTIPLICATION_TRAITS_IMPL_HPP_DEFINED