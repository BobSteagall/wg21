//==================================================================================================
//  File:       subtraction_traits.hpp
//
//  Summary:    This header defines several private and public traits types that are used to
//              implement the binary subtraction operator.  This is a somewhat long file, divided
//              into two main sections, each with three sub-sections:
//                  1. Private traits definitions that:
//                      A. determine the element promotion traits to be used for subtraction
//                      B. determine the engine promotion traits to be used for subtraction
//                      C. determine the arithmetic traits to be used to perform subtraction
//                  2. Public traits definitions that:
//                      A. perform element promotion for subtraction
//                      B. perform engine promotion for subtraction
//                      C. perform the actual act of subtraction
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_SUBTRACTION_TRAITS_HPP_DEFINED
#define LINEAR_ALGEBRA_SUBTRACTION_TRAITS_HPP_DEFINED

namespace STD_LA {
namespace detail {
//==================================================================================================
//                              **** SUBTRACTION ELEMENT TRAITS ****
//==================================================================================================
//- The standard element subtraction traits type provides the default mechanism for determining the
//  result of subtracting two elements of (possibly) different types.
//
template<class OTR, class T1, class T2>
struct subtraction_element_traits
{
    using element_type = decltype(declval<T1>() - declval<T2>());
};


//==================================================================================================
//                              **** SUBTRACTION ENGINE TRAITS ****
//==================================================================================================
//- The standard engine subtraction traits type provides the default mechanism for determining the
//  correct engine type for a matrix/matrix or vector/vector subtraction.
//
template<class OTR, class ET1, class ET2>
struct subtraction_engine_traits
{
    //- Get the extents for each engine
    //
    static constexpr size_t     R1 = engine_extents_helper<ET1>::rows();
    static constexpr size_t     C1 = engine_extents_helper<ET1>::columns();
    static constexpr size_t     R2 = engine_extents_helper<ET2>::rows();
    static constexpr size_t     C2 = engine_extents_helper<ET2>::columns();

    //- Determine if there are dynamic row or column extents.
    //
    static constexpr bool   dyn_rows = ((R1 == dynamic_extent) || (R2 == dynamic_extent));
    static constexpr bool   dyn_cols = ((C1 == dynamic_extent) || (C2 == dynamic_extent));
    static constexpr bool   dyn_size = (dyn_rows || dyn_cols);

    //- Validate the extents.
    //
    static_assert((dyn_rows || R1 == R2), "mis-matched/invalid number of rows for subtraction");
    static_assert((dyn_cols || C1 == C2), "mis-matched/invalid number of columns for subtraction");

    //- Decide on the new extents.
    //
    static constexpr size_t     RR = (dyn_rows) ? dynamic_extent : R1;
    static constexpr size_t     CR = (dyn_cols) ? dynamic_extent : C1;

    //- Extract the element traits from the operation traits, and determine the resulting
    //  element type.
    //
    using element_type_1 = typename ET1::element_type;
    using element_type_2 = typename ET2::element_type;
    using element_traits = get_subtraction_element_traits_t<OTR, element_type_1, element_type_2>;
    using elem_type      = typename element_traits::element_type;

    //- Determine the appropriate allocation and layout traits for the resulting engine type.
    //
    using owning_type_1     = get_owning_engine_type_t<ET1>;
    using owning_type_2     = get_owning_engine_type_t<ET2>;
    using allocation_traits = engine_allocation_traits<owning_type_1, owning_type_2, dyn_size, RR, CR, elem_type>;
    using layout_traits     = engine_layout_traits<ET1, ET2, false>;

    //- Determine required engine template parameters from the traits types.
    //
    using extents_type   = extents<RR, CR>;
    using allocator_type = typename allocation_traits::allocator_type;
    using layout_type    = typename layout_traits::layout_type;

  public:
    using element_type = typename element_traits::element_type;
    using engine_type  = matrix_storage_engine<element_type, extents_type, allocator_type, layout_type>;
};


//==================================================================================================
//                              **** SUBTRACTION ARITHMETIC TRAITS ****
//==================================================================================================
//- The standard subtraction arithmetic traits type provides the default mechanism for computing the
//  result of a matrix/matrix subtraction.
//
template<class OTR, class ET1, class COT1, class ET2, class COT2>
struct subtraction_arithmetic_traits<OTR, matrix<ET1, COT1>, matrix<ET2, COT2>>
{
  private:
    using element_type_1 = typename ET1::element_type;
    using element_type_2 = typename ET2::element_type;
    using element_traits = get_subtraction_element_traits_t<OTR, element_type_1, element_type_2>;

    using engine_type_1 = typename matrix<ET1, COT1>::engine_type;
    using engine_type_2 = typename matrix<ET2, COT2>::engine_type;
    using engine_traits = get_subtraction_engine_traits_t<OTR, engine_type_1, engine_type_2>;

    static_assert(std::is_same_v<typename element_traits::element_type,
                                 typename engine_traits::engine_type::element_type>);

  public:
    using element_type = typename element_traits::element_type;
    using engine_type  = typename engine_traits::engine_type;
    using result_type  = matrix<engine_type, OTR>;

    static constexpr result_type
    subtract(matrix<ET1, COT1> const& m1, matrix<ET2, COT2> const& m2)
    {
        using size_type_1 = typename matrix<ET1, COT1>::size_type;
        using size_type_2 = typename matrix<ET2, COT2>::size_type;
        using size_type_r = typename result_type::size_type;

        size_type_r     rows = static_cast<size_type_r>(m1.rows());
        size_type_r     cols = static_cast<size_type_r>(m1.columns());
        result_type		mr;

        if constexpr (detail::reshapable_matrix_engine<engine_type>)
        {
            mr.resize(rows, cols);
        }

        size_type_r    ir = 0;
        size_type_1    i1 = 0;
        size_type_2    i2 = 0;

        for (;  ir < rows;  ++ir, ++i1, ++i2)
        {
            size_type_r    jr = 0;
            size_type_1    j1 = 0;
            size_type_2    j2 = 0;

            for (;  jr < cols;  ++jr, ++j1, ++j2)
            {
                mr(ir, jr) = m1(i1, j1) - m2(i2, j2);
            }
        }

        return mr;
    }
};

}       //- namespace detail
}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_SUBTRACTION_TRAITS_HPP_DEFINED
