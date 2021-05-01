//==================================================================================================
//  File:       addition_traits.hpp
//
//  Summary:    This header defines several private and public traits types that are used to
//              implement the binary addition operator.  This is a somewhat long file, divided
//              into two main sections, each with three sub-sections:
//                  1. Private traits definitions that:
//                      A. determine the element promotion traits to be used for addition
//                      B. determine the engine promotion traits to be used for addition
//                      C. determine the arithmetic traits to be used to perform addition
//                  2. Public traits definitions that:
//                      A. perform element promotion for addition
//                      B. perform engine promotion for addition
//                      C. perform the actual act of addition
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_ADDITION_TRAITS_HPP_DEFINED
#define LINEAR_ALGEBRA_ADDITION_TRAITS_HPP_DEFINED

namespace STD_LA {
namespace detail {

//==================================================================================================
//                              **** LAYOUT ADDITION TRAITS ****
//==================================================================================================
//- The standard layout addition traits type provides the default mechanism for determining the
//  resulting data layout when adding two matrices having different layouts..
//
template<class OTR>
struct addition_layout_traits<OTR, matrix_layout::row_major, matrix_layout::row_major>
{
    using layout_type = matrix_layout::row_major;
};

template<class OTR>
struct addition_layout_traits<OTR, matrix_layout::row_major, matrix_layout::column_major>
{
    using layout_type = matrix_layout::row_major;
};

template<class OTR>
struct addition_layout_traits<OTR, matrix_layout::column_major, matrix_layout::row_major>
{
    using layout_type = matrix_layout::row_major;
};

template<class OTR>
struct addition_layout_traits<OTR, matrix_layout::column_major, matrix_layout::column_major>
{
    using layout_type = matrix_layout::column_major;
};


//==================================================================================================
//                              **** ELEMENT ADDITION TRAITS ****
//==================================================================================================
//- The standard element subtraction traits type provides the default mechanism for determining the
//  result of subtracting two elements of (possibly) different types.
//
template<class OTR, class T1, class T2>
struct addition_element_traits
{
    using element_type = decltype(declval<T1>() + declval<T2>());
};


//==================================================================================================
//                              **** ENGINE ADDITION TRAITS ****
//==================================================================================================
//- The standard engine addition traits type provides the default mechanism for determining the
//  correct engine type for a matrix/matrix or vector/vector addition.
//
template<class OTR, class ET1, class ET2>
struct addition_engine_traits
{
  private:
    using element_type_1 = typename ET1::element_type;
    using element_type_2 = typename ET2::element_type;
    using element_traits = get_addition_element_traits_t<OTR, element_type_1, element_type_2>;

  public:
    using element_type = typename element_traits::element_type;
    using engine_type  = dynamic_matrix_engine<element_type>;
};


template<class OTR,
         class T1, ptrdiff_t R1, ptrdiff_t C1, class AT1, class LT1,
         class T2, ptrdiff_t R2, ptrdiff_t C2, class AT2, class LT2>
struct addition_engine_traits<OTR,
                              matrix_storage_engine<T1, extents<R1, C1>, AT1, LT1>,
                              matrix_storage_engine<T2, extents<R2, C2>, AT2, LT2>>
{
  private:
    static constexpr bool   dyn_rows = ((R1 == dynamic_extent) || (R2 == dynamic_extent));
    static constexpr bool   dyn_cols = ((C1 == dynamic_extent) || (C2 == dynamic_extent));

    //- Validate the size template parameters.
    //
    static_assert((dyn_rows || R1 == R2), "mis-matched/invalid number of rows for addition");
    static_assert((dyn_cols || C1 == C2), "mis-matched/invalid number of columns for addition");

    //- Compute the new extents.
    //
    static constexpr ptrdiff_t  RR = (dyn_rows) ? dynamic_extent : R1;
    static constexpr ptrdiff_t  CR = (dyn_cols) ? dynamic_extent : C1;

    //- Extract element traits from the operation traits, and determine allocation and layout traits.
    //
    using element_traits    = get_addition_element_traits_t<OTR, T1, T2>;
    using allocation_traits = detail::allocation_traits<OTR, AT1, AT2, typename element_traits::element_type>;
    using layout_traits     = detail::addition_layout_traits<OTR, LT1, LT2>;

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
//                              **** ENGINE ADDITION TRAITS ****
//==================================================================================================
//- The standard addition arithmetic traits type provides the default mechanism for computing the
//  result of a matrix/matrix addition.
//
template<class OTR, class ET1, class COT1, class ET2, class COT2>
struct addition_arithmetic_traits<OTR, basic_matrix<ET1, COT1>, basic_matrix<ET2, COT2>>
{
  private:
    using element_type_1  = typename ET1::element_type;
    using element_type_2  = typename ET2::element_type;
    using element_traits  = get_addition_element_traits_t<OTR, element_type_1, element_type_2>;

    using owning_engine_1 = typename basic_matrix<ET1, COT1>::owning_engine_type;
    using owning_engine_2 = typename basic_matrix<ET2, COT2>::owning_engine_type;
    using engine_traits   = get_addition_engine_traits_t<OTR, owning_engine_1, owning_engine_2>;

  public:
    using element_type = typename element_traits::element_type;
    using engine_type  = typename engine_traits::engine_type;
    using result_type  = basic_matrix<engine_type, OTR>;

    static_assert(std::is_same_v<element_type, typename engine_type::element_type>);

    static constexpr result_type
    add(basic_matrix<ET1, COT1> const& m1, basic_matrix<ET2, COT2> const& m2)
    {
        using index_type_1 = typename basic_matrix<ET1, COT1>::index_type;
        using index_type_2 = typename basic_matrix<ET2, COT2>::index_type;
        using index_type_r = typename result_type::index_type;

        index_type_r    rows = static_cast<index_type_r>(m1.rows());
        index_type_r    cols = static_cast<index_type_r>(m1.columns());
        result_type		mr;

        if constexpr (detail::reshapable_matrix_engine<engine_type>)
        {
            mr.resize(rows, cols);
        }

        index_type_r    ir = 0;
        index_type_1    i1 = 0;
        index_type_2    i2 = 0;

        for (;  ir < rows;  ++ir, ++i1, ++i2)
        {
            index_type_r    jr = 0;
            index_type_1    j1 = 0;
            index_type_2    j2 = 0;

            for (;  jr < cols;  ++jr, ++j1, ++j2)
            {
                mr(ir, jr) = m1(i1, j1) + m2(i2, j2);
            }
        }

        return mr;
    }
};

}       //- namespace detail
}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_ADDITION_TRAITS_HPP_DEFINED
