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

    size_type_r const   elems = static_cast<size_type_r>(v1.elements());

    result_type     vr;
    size_type_r     ir;
    size_type_1     i1;

    if constexpr (vr.is_resizable())
    {
        vr.resize(elems);
    }

    for (ir = 0, i1 = 0;  ir < elems;  ++ir, ++i1)
    {
        vr(ir) = -v1(i1);
    }

    return vr;
}

//------
//
template<class OT, class ET1, class OT1>
inline auto
matrix_negation_traits<OT, matrix<ET1, OT1>>::negate(matrix<ET1, OT1> const& m1) -> result_type
{
    size_type_r const  rows = static_cast<size_type_r>(m1.rows());
    size_type_r const  cols = static_cast<size_type_r>(m1.columns());

    result_type		mr;
    size_type_r     ir, jr;
    size_type_1     i1, j1;

    if constexpr (mr.is_resizable())
    {
        mr.resize(rows, cols);
    }

    for (ir = 0, i1 = 0;  ir < rows;  ++ir, ++i1)
    {
        for (jr = 0, j1 = 0;  jr < cols;  ++jr, ++j1)
        {
            mr(ir, jr) = -m1(i1, j1);
        }
    }

    return mr;
}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_NEGATION_TRAITS_IMPL_HPP_DEFINED
