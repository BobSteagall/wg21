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

	index_type_r const  elems = static_cast<index_type_r>(v1.elements());

	result_type     vr;
	index_type_r    ir;
	index_type_1    i1;

	if constexpr (result_requires_resize(vr))
	{
		vr.resize(elems);
	}

	for (ir = 0, i1 = 0;  ir < elems;  ++ir, ++i1)
	{
		vr(ir) = v1(i1) * s2;
	}

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

	index_type_r const  elems = static_cast<index_type_r>(v2.elements());

	result_type     vr;
	index_type_r    ir;
	index_type_2    i2;

	if constexpr (result_requires_resize(vr))
	{
		vr.resize(elems);
	}

	for (ir = 0, i2 = 0;  ir < elems;  ++ir, ++i2)
	{
		vr(ir) = s1 * v2(i2);
	}

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

	index_type_r const  rows = static_cast<index_type_r>(m1.rows());
	index_type_r const  cols = static_cast<index_type_r>(m1.columns());

	result_type		mr;
	index_type_r    ir, jr;
	index_type_1    i1, j1;

	if constexpr (result_requires_resize(mr))
	{
		mr.resize(rows, cols);
	}

	for (ir = 0, i1 = 0;  ir < rows;  ++ir, ++i1)
	{
		for (jr = 0, j1 = 0;  jr < cols;  ++jr, ++j1)
		{
			mr(ir, jr) = m1(i1, j1) * s2;
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

	index_type_r const  rows = static_cast<index_type_r>(m2.rows());
	index_type_r const  cols = static_cast<index_type_r>(m2.columns());

	result_type		mr;
	index_type_r    ir, jr;
	index_type_2    i2, j2;

	if constexpr (result_requires_resize(mr))
	{
		mr.resize(rows, cols);
	}

	for (ir = 0, i2 = 0;  ir < rows;  ++ir, ++i2)
	{
		for (jr = 0, j2 = 0;  jr < cols;  ++jr, ++j2)
		{
			mr(ir, jr) = s1 * m2(i2, j2);
		}
	}

	return mr;
}

//---------------
//- vector*vector
//
template<class OTR, class ET1, class OT1, class ET2, class OT2>
inline auto
matrix_multiplication_traits<OTR, vector<ET1, OT1>, vector<ET2, OT2>>::multiply
(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2) -> result_type
{
    PrintOperandTypes<result_type>("multiplication_traits (v*v)", v1, v2);

	index_type_1 const  elems = static_cast<index_type_1>(v1.elements());

	result_type     er{};
	index_type_1    i1;
	index_type_2    i2;

	for (i1 = 0, i2 = 0;  i1 < elems;  ++i1, ++i2)
	{
		er += v1(i1) * v2(i2);
	}

	return er;
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

	index_type_r const  elems = static_cast<index_type_r>(m1.rows());
	index_type_1 const  inner = static_cast<index_type_1>(m1.columns());

	result_type		vr;
	index_type_r    ir;
	index_type_1    i1, k1;
	index_type_2    k2;

	if constexpr (result_requires_resize(vr))
	{
		vr.resize(elems);
	}

	for (ir = 0, i1 = 0;  ir < elems;  ++ir, ++i1)
	{
		typename result_type::element_type	er{};

		for (k1 = 0, k2 = 0;  k1 < inner;  ++k1, ++k2)
		{
			er += m1(i1, k1) * v2(k2);
		}

		vr(ir) = er;
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

	index_type_r const  elems = static_cast<index_type_r>(m2.columns());
	index_type_2 const  inner = static_cast<index_type_2>(m2.rows());

	result_type		vr;
	index_type_r    jr;
	index_type_1    k1;
	index_type_2    k2, j2;

	if constexpr (result_requires_resize(vr))
	{
		vr.resize(elems);
	}

	for (jr = 0, j2 = 0;  jr < elems;  ++jr, ++j2)
	{
		typename result_type::element_type	er{};

		for (k1 = 0, k2 = 0;  k2 < inner;  ++k1, ++k2)
		{
			er += v1(k1) * m2(k2, j2);
		}

		vr(jr) = er;
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

	index_type_r const  rows  = static_cast<index_type_r>(m1.rows());
	index_type_r const  cols  = static_cast<index_type_r>(m2.columns());
	index_type_1 const  inner = m1.columns();

	result_type		mr;
	index_type_r    ir, jr;
	index_type_1    i1, k1;
	index_type_2    j2, k2;

	if constexpr (result_requires_resize(mr))
	{
		mr.resize(rows, cols);
	}

	for (ir = 0, i1 = 0;  ir < rows;  ++ir, ++i1)
	{
		for (jr = 0, j2 = 0;  jr < cols;  ++jr, ++j2)
		{
			typename result_type::element_type  er{};

			for (k1 = 0, k2 = 0;  k1 < inner;  ++k1, ++k2)
			{
				er += m1(i1, k1) * m2(k2, j2);
			}
			mr(ir, jr) = er;
		}
	}

	return mr;
}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_MULTIPLICATION_TRAITS_IMPL_HPP_DEFINED
