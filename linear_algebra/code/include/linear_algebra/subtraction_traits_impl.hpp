//==================================================================================================
//  File:       subtraction_traits_impl.hpp
//
//  Summary:    This header defines the static member functions of matrix_subtraction_traits that
//              perform the actual arithmetic.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_SUBTRACTION_TRAITS_IMPL_HPP_DEFINED
#define LINEAR_ALGEBRA_SUBTRACTION_TRAITS_IMPL_HPP_DEFINED

namespace STD_LA {
//==================================================================================================
//                       **** SUBTRACTION TRAITS FUNCTION IMPLEMENTATION ****
//==================================================================================================
//
template<class OT, class ET1, class OT1, class ET2, class OT2>
inline auto
matrix_subtraction_traits<OT, vector<ET1, OT1>, vector<ET2, OT2>>::subtract
(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2) -> result_type
{
    PrintOperandTypes<result_type>("subtraction_traits", v1, v2);

    index_type_r const  elems = static_cast<index_type_r>(v1.elements());

    result_type     vr;
    index_type_r    ir;
    index_type_1    i1;
    index_type_2    i2;

    if constexpr (result_requires_resize(vr))
    {
        vr.resize(elems);
    }

    for (ir = 0, i1 = 0, i2 = 0;  ir < elems;  ++ir, ++i1, ++i2)
    {
        vr(ir) = v1(i1) - v2(i2);
    }

    return vr;
}

//------
//
template<class OT, class ET1, class OT1, class ET2, class OT2>
inline auto
matrix_subtraction_traits<OT, matrix<ET1, OT1>, matrix<ET2, OT2>>::subtract
(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2) -> result_type
{
    PrintOperandTypes<result_type>("subtraction_traits", m1, m2);

    index_type_r const  rows = static_cast<index_type_r>(m1.rows());
    index_type_r const  cols = static_cast<index_type_r>(m1.columns());

    result_type		mr;
    index_type_r    ir, jr;
    index_type_1    i1, j1;
    index_type_2    i2, j2;

    if constexpr (result_requires_resize(mr))
    {
        mr.resize(rows, cols);
    }

    for (ir = 0, i1 = 0, i2 = 0;  ir < rows;  ++ir, ++i1, ++i2)
    {
        for (jr = 0, j1 = 0, j2 = 0;  jr < cols;  ++jr, ++j1, ++j2)
        {
            mr(ir, jr) = m1(i1, j1) - m2(i2, j2);
        }
    }

    return mr;
}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_SUBTRACTION_TRAITS_IMPL_HPP_DEFINED
