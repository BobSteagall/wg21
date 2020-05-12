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
    PRINT_OP_TYPES(result_type, "negation_traits", v1);

    index_type_r const   elems = static_cast<index_type_r>(v1.size());

    result_type     vr;
    index_type_r     ir;
    index_type_1     i1;

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
    index_type_r const  rows = static_cast<index_type_r>(m1.rows());
    index_type_r const  cols = static_cast<index_type_r>(m1.columns());

    result_type		mr;
    index_type_r     ir, jr;
    index_type_1     i1, j1;

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
