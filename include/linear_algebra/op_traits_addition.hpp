//==================================================================================================
//  File:       op_traits_addition.hpp
//
//  Summary:    This header defines several private traits types and alias templates that are used
//              to implement the binary addition operator.  This is a somewhat long file, divided
//              into five main sections:
//                  1. Extraction traits, that detect if a given operation traits type is
//                     part of a custom operation traits container type, along with alias
//                     templates to promote ease-of-use;
//                  2. Default operation traits definition for element promotion;
//                  3. Default operation traits definition for layout promotion;
//                  4. Default operation traits definition for engine promotion;
//                  5. Default operation traits definition for arithmetic.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_OP_TRAITS_ADDITION_HPP_DEFINED
#define LINEAR_ALGEBRA_OP_TRAITS_ADDITION_HPP_DEFINED

namespace STD_LA {
namespace detail {
template<class OT, class T1, class T2>      struct default_addition_element_traits;
template<class OT, class L1, class L2>      struct default_addition_layout_traits;
template<class OT, class ET1, class ET2>    struct default_addition_engine_traits;
template<class OT, class OP1, class OP2>    struct default_addition_arithmetic_traits;

//==================================================================================================
//                              **** ADDITION TRAITS EXTRACTORS ****
//==================================================================================================
//- These types employ the type detection idiom to detect a custom traits type related to addition;
//  if none is found, they "return" the default traits type, defined in sections below.
//
template<typename OT, typename U, typename V, typename = void>
struct addition_element_traits_extractor
{
    using type = default_addition_element_traits<OT,U,V>;
};

template<typename OT, typename U, typename V>
struct addition_element_traits_extractor<OT, U, V, void_t<typename OT::template addition_element_traits<OT,U,V>::element_type>>
{
    using type = typename OT::template addition_element_traits<OT, U, V>;
};

template<typename OT, typename U, typename V>
using addition_element_traits_t = typename addition_element_traits_extractor<OT, U, V>::type;


//------
//
template<typename OT, typename U, typename V, typename = void>
struct addition_layout_traits_extractor
{
    using type = default_addition_layout_traits<OT,U,V>;
};

template<typename OT, typename U, typename V>
struct addition_layout_traits_extractor<OT, U, V, void_t<typename OT::template addition_layout_traits<OT,U,V>::layout_type>>
{
    using type = typename OT::template addition_layout_traits<OT, U, V>;
};

template<typename OT, typename U, typename V>
using addition_layout_traits_t = typename addition_layout_traits_extractor<OT, U, V>::type;


//-----
//
template<typename OT, typename U, typename V, typename = void>
struct addition_engine_traits_extractor
{
    using type = default_addition_engine_traits<OT,U,V>;
};

template<typename OT, typename U, typename V>
struct addition_engine_traits_extractor<OT, U, V, void_t<typename OT::template addition_engine_traits<OT,U,V>::engine_type>>
{
    using type = typename OT::template addition_engine_traits<OT, U, V>;
};

template<typename OT, typename U, typename V>
using addition_engine_traits_t = typename addition_engine_traits_extractor<OT, U, V>::type;


//-----
//
template<typename OT, typename U, typename V, typename = void>
struct addition_arithmetic_traits_extractor
{
    using type = default_addition_arithmetic_traits<OT,U,V>;
};

template<typename OT, typename U, typename V>
struct addition_arithmetic_traits_extractor<OT, U, V, void_t<typename OT::template addition_arithmetic_traits<OT,U,V>::result_type>>
{
    using type = typename OT::template addition_arithmetic_traits<OT, U, V>;
};

template<typename OT, typename U, typename V>
using addition_arithmetic_traits_t = typename addition_arithmetic_traits_extractor<OT, U, V>::type;


//==================================================================================================
//                              **** ADDITION ELEMENT TRAITS ****
//==================================================================================================
//- The standard element addition traits type provides the default mechanism for determining the
//  result of adding two elements of (possibly) different types.
//
template<class COTR, class T1, class T2>
struct default_addition_element_traits
{
    using element_type = decltype(declval<T1>() + declval<T2>());
};


//==================================================================================================
//                              **** ADDITION LAYOUT TRAITS ****
//==================================================================================================
//- The standard layout addition traits type provides the default mechanism for determining the
//  data layout used by the matrix_storage_engine result when adding two matrices.
//
template<class COTR, class L1, class L2>
struct default_addition_layout_traits
{
    using layout_type = matrix_layout::row_major;
};

template<class COTR>
struct default_addition_layout_traits<COTR, matrix_layout::column_major, matrix_layout::column_major>
{
    using layout_type = matrix_layout::column_major;
};


//==================================================================================================
//                              **** ADDITION ENGINE TRAITS ****
//==================================================================================================
//- The standard engine addition traits type provides the default mechanism for determining the
//  correct engine type for a matrix/matrix or vector/vector addition.
//
template<class COTR, class ET1, class ET2>
struct default_addition_engine_traits
{
    //- Extract the element traits from the operation traits, and determine the resulting
    //  element type.
    //
    using element_type_1 = typename ET1::element_type;
    using element_type_2 = typename ET2::element_type;
    using element_traits = addition_element_traits_t<COTR, element_type_1, element_type_2>;
    using elem_type      = typename element_traits::element_type;

    //- Get the extents for each engine
    //
    static constexpr size_t     R1 = engine_extents_helper<ET1>::rows();
    static constexpr size_t     C1 = engine_extents_helper<ET1>::columns();
    static constexpr size_t     R2 = engine_extents_helper<ET2>::rows();
    static constexpr size_t     C2 = engine_extents_helper<ET2>::columns();

    //- Determine if there are dynamic row or column extents.
    //
    static constexpr bool   dyn_rows = ((R1 == std::dynamic_extent) || (R2 == std::dynamic_extent));
    static constexpr bool   dyn_cols = ((C1 == std::dynamic_extent) || (C2 == std::dynamic_extent));
    static constexpr bool   dyn_size = (dyn_rows || dyn_cols);

    //- Validate the extents.
    //
    static_assert((dyn_rows || R1 == R2), "mis-matched/invalid number of rows for addition");
    static_assert((dyn_cols || C1 == C2), "mis-matched/invalid number of columns for addition");

    //- Determine the new extents.
    //
    static constexpr size_t     RR = (dyn_rows) ? std::dynamic_extent : R1;
    static constexpr size_t     CR = (dyn_cols) ? std::dynamic_extent : C1;

    //- Determine the resulting allocator type.
    //
    using owning_type_1     = get_owning_engine_type_t<ET1>;
    using owning_type_2     = get_owning_engine_type_t<ET2>;
    using allocation_traits = engine_allocation_traits<owning_type_1, owning_type_2, dyn_size, RR, CR, elem_type>;
    using allocator_type    = typename allocation_traits::allocator_type;

    //- Determine the resulting layout type.
    //
    using layout_type_1 = get_layout_t<ET1>;
    using layout_type_2 = get_layout_t<ET2>;
    using layout_traits = addition_layout_traits_t<COTR, layout_type_1, layout_type_2>;
    using layout_type   = typename layout_traits::layout_type;
    using extents_type  = extents<size_t, RR, CR>;

  public:
    using element_type = elem_type;
    using engine_type  = matrix_storage_engine<element_type, extents_type, allocator_type, layout_type>;
};


//==================================================================================================
//                              **** ADDITION ARITHMETIC TRAITS ****
//==================================================================================================
//- The standard addition arithmetic traits type provides the default mechanism for computing the
//  result of a matrix/matrix addition.
//
template<class COTR, class ET1, class COT1, class ET2, class COT2>
struct default_addition_arithmetic_traits<COTR, matrix<ET1, COT1>, matrix<ET2, COT2>>
{
  private:
    using engine_type_1 = typename matrix<ET1, COT1>::engine_type;
    using engine_type_2 = typename matrix<ET2, COT2>::engine_type;
    using engine_traits = addition_engine_traits_t<COTR, engine_type_1, engine_type_2>;

  public:
    using element_type = typename engine_traits::element_type;
    using engine_type  = typename engine_traits::engine_type;
    using result_type  = matrix<engine_type, COTR>;

    static constexpr result_type
    add(matrix<ET1, COT1> const& m1, matrix<ET2, COT2> const& m2)
    {
        using size_type_1 = typename matrix<ET1, COT1>::size_type;
        using size_type_2 = typename matrix<ET2, COT2>::size_type;
        using size_type_r = typename result_type::size_type;

        size_type_r     rows = static_cast<size_type_r>(m1.rows());
        size_type_r     cols = static_cast<size_type_r>(m1.columns());
        result_type     mr;

        if constexpr (detail::reshapable_matrix_engine<engine_type>)
        {
            mr.resize(rows, cols);
        }

        size_type_r     ir = 0;
        size_type_1     i1 = 0;
        size_type_2     i2 = 0;

        for (;  ir < rows;  ++ir, ++i1, ++i2)
        {
            size_type_r     jr = 0;
            size_type_1     j1 = 0;
            size_type_2     j2 = 0;

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
#endif  //- LINEAR_ALGEBRA_OP_TRAITS_ADDITION_HPP_DEFINED
