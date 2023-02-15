//==================================================================================================
//  File:       op_traits_division.hpp
//
//  Summary:    This header defines several private and public traits types that are used to
//              implement the binary scalar division operator.  This is a somewhat long file,
//              divided into two main sections, each with three subsections:
//                  1. Private traits definitions that:
//                      A. determine the element promotion traits to be used for scalar division
//                      B. determine the engine promotion traits to be used for scalar division
//                      C. determine the arithmetic traits to be used to perform scalar division
//                  2. Public traits definitions that:
//                      A. perform element promotion for scalar division
//                      B. perform engine promotion for scalar division
//                      C. perform the actual act of scalar division
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_OP_TRAITS_DIVISION_HPP_DEFINED
#define LINEAR_ALGEBRA_OP_TRAITS_DIVISION_HPP_DEFINED

namespace STD_LA {
namespace detail {
template<class OT, class T1, class T2>      struct default_division_element_traits;
template<class OT, class L1, class L2>      struct default_division_layout_traits;
template<class OT, class ET1, class ET2>    struct default_division_engine_traits;
template<class OT, class OP1, class OP2>    struct default_division_arithmetic_traits;

//==================================================================================================
//                              **** DIVISION TRAITS EXTRACTORS ****
//==================================================================================================
//- These types employ the type detection idiom to detect a custom traits type related to
//  scalar division; if none is found, they "return" the default traits type, defined below.
//
template<typename OT, typename U, typename V, typename = void>
struct division_element_traits_extractor
{
    using type = default_division_element_traits<OT,U,V>;
};

template<typename OT, typename U, typename V>
struct division_element_traits_extractor<OT, U, V, void_t<typename OT::template division_element_traits<OT,U,V>::element_type>>
{
    using type = typename OT::template division_element_traits<OT, U, V>;
};

template<typename OT, typename U, typename V>
using division_element_traits_t = typename division_element_traits_extractor<OT, U, V>::type;


//------
//
template<typename OT, typename U, typename V, typename = void>
struct division_layout_traits_extractor
{
    using type = default_division_layout_traits<OT,U,V>;
};

template<typename OT, typename U, typename V>
struct division_layout_traits_extractor<OT, U, V, void_t<typename OT::template division_layout_traits<OT,U,V>::layout_type>>
{
    using type = typename OT::template division_layout_traits<OT, U, V>;
};

template<typename OT, typename U, typename V>
using division_layout_traits_t = typename division_layout_traits_extractor<OT, U, V>::type;


//-----
//
template<typename OT, typename U, typename V, typename = void>
struct division_engine_traits_extractor
{
    using type = default_division_engine_traits<OT,U,V>;
};

template<typename OT, typename U, typename V>
struct division_engine_traits_extractor<OT, U, V, void_t<typename OT::template division_engine_traits<OT,U,V>::engine_type>>
{
    using type = typename OT::template division_engine_traits<OT, U, V>;
};

template<typename OT, typename U, typename V>
using division_engine_traits_t = typename division_engine_traits_extractor<OT, U, V>::type;


//-----
//
template<typename OT, typename U, typename V, typename = void>
struct division_arithmetic_traits_extractor
{
    using type = default_division_arithmetic_traits<OT,U,V>;
};

template<typename OT, typename U, typename V>
struct division_arithmetic_traits_extractor<OT, U, V, void_t<typename OT::template division_arithmetic_traits<OT,U,V>::result_type>>
{
    using type = typename OT::template division_arithmetic_traits<OT, U, V>;
};

template<typename OT, typename U, typename V>
using division_arithmetic_traits_t = typename division_arithmetic_traits_extractor<OT, U, V>::type;


//==================================================================================================
//                              **** DIVISION ELEMENT TRAITS ****
//==================================================================================================
//- The standard element division traits type provides the default mechanism for determining
//  the result of multiplying two elements of (possibly) different types.
//
template<class OT, class T1, class T2>
struct default_division_element_traits
{
    using element_type = decltype(declval<T1>() / declval<T2>());
};


//==================================================================================================
//                               **** DIVISION LAYOUT TRAITS ****
//==================================================================================================
//- The standard layout addition traits type provides the default mechanism for determining the
//  resulting data layout when adding two matrices having different layouts..
//
template<class COTR, class L1, class L2>
struct default_division_layout_traits
{
    using layout_type = matrix_layout::row_major;
};

template<class COTR>
struct default_division_layout_traits<COTR, matrix_layout::column_major, matrix_layout::column_major>
{
    using layout_type = matrix_layout::column_major;
};


//==================================================================================================
//                              **** DIVISION ENGINE TRAITS ****
//==================================================================================================
//
//- The standard engine addition traits type provides the default mechanism for determining the
//  correct engine type for a matrix/matrix or vector/vector addition.
//
template<class COTR, class ET1, class S2>
struct default_division_engine_traits
{
    //- Extract the element traits from the operation traits, and determine the resulting
    //  element type.
    //
    using element_type_1 = typename ET1::element_type;
    using element_type_2 = S2;
    using element_traits = division_element_traits_t<COTR, element_type_1, element_type_2>;
    using elem_type      = typename element_traits::element_type;

    //- Get the extents for the engine
    //
    static constexpr size_t     R1 = engine_extents_helper<ET1>::rows();
    static constexpr size_t     C1 = engine_extents_helper<ET1>::columns();

    //- Determine if there are dynamic row or column extents.
    //
    static constexpr bool   dyn_rows = (R1 == std::dynamic_extent);
    static constexpr bool   dyn_cols = (C1 == std::dynamic_extent);
    static constexpr bool   dyn_size = (dyn_rows || dyn_cols);

    //- Determine the new extents.
    //
    static constexpr size_t     RR = R1;
    static constexpr size_t     CR = C1;

    //- Determine the resulting allocator type.
    //
    using owning_type_1     = get_owning_engine_type_t<ET1>;
    using allocation_traits = engine_allocation_traits<owning_type_1, owning_type_1, dyn_size, RR, CR, elem_type>;
    using allocator_type    = typename allocation_traits::allocator_type;

    //- Determine the resulting layout type.
    //
    using layout_type_1 = get_layout_t<ET1>;
    using layout_traits = division_layout_traits_t<COTR, layout_type_1, layout_type_1>;
    using layout_type   = typename layout_traits::layout_type;

  public:
    using element_type = elem_type;
    using engine_type  = matrix_storage_engine<element_type, RR, CR, allocator_type, layout_type>;
};


//==================================================================================================
//                            **** DIVISION ARITHMETIC TRAITS ****
//==================================================================================================
//
//- The standard addition arithmetic traits type provides the default mechanism for computing the
//  result of a matrix/matrix addition.
//
template<class COTR, class ET1, class COT1, class S2>
struct default_division_arithmetic_traits<COTR, matrix<ET1, COT1>, S2>
{
  private:
    using engine_type_1 = typename matrix<ET1, COT1>::engine_type;
    using engine_type_2 = S2;
    using engine_traits = division_engine_traits_t<COTR, engine_type_1, engine_type_2>;

  public:
    using element_type = typename engine_traits::element_type;
    using engine_type  = typename engine_traits::engine_type;
    using result_type  = matrix<engine_type, COTR>;

    static constexpr result_type
    divide(matrix<ET1, COT1> const& m1, S2 const& s2)
    {
        using size_type_1 = typename matrix<ET1, COT1>::size_type;
        using size_type_r = typename result_type::size_type;

        size_type_r     rows = static_cast<size_type_r>(m1.rows());
        size_type_r     cols = static_cast<size_type_r>(m1.columns());
        result_type		mr;

        if constexpr (detail::reshapable_matrix_engine<engine_type>)
        {
            mr.resize(rows, cols);
        }

        size_type_r     ir = 0;
        size_type_1     i1 = 0;

        for (;  ir < rows;  ++ir, ++i1)
        {
            size_type_r     jr = 0;
            size_type_1     j1 = 0;

            for (;  jr < cols;  ++jr, ++j1)
            {
                mr(ir, jr) = m1(i1, j1) / s2;
            }
        }

        return mr;
    }
};

}       //- namespace detail
}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_OP_TRAITS_DIVISION_HPP_DEFINED
