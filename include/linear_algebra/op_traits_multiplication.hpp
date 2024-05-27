//==================================================================================================
//  File:       op_traits_multiplication.hpp
//
//  Summary:    This header defines several private traits types and alias templates that are used
//              to implement the binary mutiplication operator.  This is a somewhat long file,
//              divided into five main sections:
//                  1. Extraction traits, that detect if a given operation traits type is
//                     part of a custom operation traits container type, along with alias
//                     templates to promote ease-of-use;
//                  2. Default operation traits definition for element promotion;
//                  3. Default operation traits definition for layout promotion;
//                  4. Default operation traits definition for engine promotion;
//                  5. Default operation traits definition for arithmetic.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_OP_TRAITS_MULTIPLICATION_HPP_DEFINED
#define LINEAR_ALGEBRA_OP_TRAITS_MULTIPLICATION_HPP_DEFINED

namespace STD_LA {
namespace detail {
template<class OT, class T1, class T2>      struct default_multiplication_element_traits;
template<class OT, class L1, class L2>      struct default_multiplication_layout_traits;
template<class OT, class ET1, class ET2>    struct default_multiplication_engine_traits;
template<class OT, class OP1, class OP2>    struct default_multiplication_arithmetic_traits;

//==================================================================================================
//                        **** MULTIPLICATION TRAITS EXTRACTORS ****
//==================================================================================================
//- These types employ the type detection idiom to detect a custom traits type related to
//  multiplication; if none is found, they "return" the default traits type,  defined in
//  sections below.
//
template<typename OT, typename U, typename V, typename = void>
struct multiplication_element_traits_extractor
{
    using type = default_multiplication_element_traits<OT,U,V>;
};

template<typename OT, typename U, typename V>
struct multiplication_element_traits_extractor<OT, U, V, void_t<typename OT::template multiplication_element_traits<OT,U,V>::element_type>>
{
    using type = typename OT::template multiplication_element_traits<OT, U, V>;
};

template<typename OT, typename U, typename V>
using multiplication_element_traits_t = typename multiplication_element_traits_extractor<OT, U, V>::type;


//------
//
template<typename OT, typename U, typename V, typename = void>
struct multiplication_layout_traits_extractor
{
    using type = default_multiplication_layout_traits<OT,U,V>;
};

template<typename OT, typename U, typename V>
struct multiplication_layout_traits_extractor<OT, U, V, void_t<typename OT::template multiplication_layout_traits<OT,U,V>::layout_type>>
{
    using type = typename OT::template multiplication_layout_traits<OT, U, V>;
};

template<typename OT, typename U, typename V>
using multiplication_layout_traits_t = typename multiplication_layout_traits_extractor<OT, U, V>::type;


//-----
//
template<typename OT, typename U, typename V, typename = void>
struct multiplication_engine_traits_extractor
{
    using type = default_multiplication_engine_traits<OT,U,V>;
};

template<typename OT, typename U, typename V>
struct multiplication_engine_traits_extractor<OT, U, V, void_t<typename OT::template multiplication_engine_traits<OT,U,V>::engine_type>>
{
    using type = typename OT::template multiplication_engine_traits<OT, U, V>;
};

template<typename OT, typename U, typename V>
using multiplication_engine_traits_t = typename multiplication_engine_traits_extractor<OT, U, V>::type;


//-----
//
template<typename OT, typename U, typename V, typename = void>
struct multiplication_arithmetic_traits_extractor
{
    using type = default_multiplication_arithmetic_traits<OT,U,V>;
};

template<typename OT, typename U, typename V>
struct multiplication_arithmetic_traits_extractor<OT, U, V, void_t<typename OT::template multiplication_arithmetic_traits<OT,U,V>::result_type>>
{
    using type = typename OT::template multiplication_arithmetic_traits<OT, U, V>;
};

template<typename OT, typename U, typename V>
using multiplication_arithmetic_traits_t = typename multiplication_arithmetic_traits_extractor<OT, U, V>::type;


//==================================================================================================
//                           **** MULTIPLICATION ELEMENT TRAITS ****
//==================================================================================================
//- The standard element division traits type provides the default mechanism for determining
//  the result of multiplying two elements of (possibly) different types.
//
template<class OT, class T1, class T2>
struct default_multiplication_element_traits
{
    using element_type = decltype(declval<T1>() * declval<T2>());
};


//==================================================================================================
//                           **** MULTIPLICATION LAYOUT TRAITS ****
//==================================================================================================
//- The standard element division traits type provides the default mechanism for determining
//  the result of multiplying two elements of (possibly) different types.
//
template<class COTR, class L1, class L2>
struct default_multiplication_layout_traits
{
    using layout_type = matrix_layout::row_major;
};

template<class COTR>
struct default_multiplication_layout_traits<COTR, matrix_layout::row_major, matrix_layout::column_major>
{
    using layout_type = matrix_layout::column_major;
};

template<class COTR>
struct default_multiplication_layout_traits<COTR, matrix_layout::column_major, matrix_layout::column_major>
{
    using layout_type = matrix_layout::column_major;
};


//==================================================================================================
//                            **** MULTIPLICATION ENGINE TRAITS ****
//==================================================================================================
//
//- The standard engine addition traits type provides the default mechanism for determining the
//  correct engine type for a matrix/matrix or vector/vector addition.
//
template<class COTR, class ET1, class ET2>
struct default_multiplication_engine_traits
{
  private:
    //- Extract the element traits from the operation traits, and determine the resulting
    //  element type.
    //
    using element_type_1 = typename ET1::element_type;
    using element_type_2 = typename ET2::element_type;
    using element_traits = multiplication_element_traits_t<COTR, element_type_1, element_type_2>;
    using elem_type      = typename element_traits::element_type;

    //- Get the extents for each engine
    //
    static constexpr size_t     R1 = engine_extents_helper<ET1>::rows();
    static constexpr size_t     C1 = engine_extents_helper<ET1>::columns();
    static constexpr size_t     R2 = engine_extents_helper<ET2>::rows();
    static constexpr size_t     C2 = engine_extents_helper<ET2>::columns();

    //- Determine if there are dynamic row or column extents.
    //
    static constexpr bool   dyn_rows = (R1 == std::dynamic_extent);
    static constexpr bool   dyn_cols = (C2 == std::dynamic_extent);
    static constexpr bool   dyn_size = (dyn_rows || dyn_cols);

    //- Validate the extents.
    //
    static_assert((C1 == R2  ||  C1 == std::dynamic_extent  ||  R2 == std::dynamic_extent),
                  "mis-matched/invalid number of rows and columns for multiplication");

    //- Decide on the new extents.
    //
    static constexpr size_t     RR = (dyn_rows) ? std::dynamic_extent : R1;
    static constexpr size_t     CR = (dyn_cols) ? std::dynamic_extent : C2;

    //- Determine the resulting allocator type.
    //
    using owning_type_1     = get_owning_engine_type_t<ET1>;
    using owning_type_2     = get_owning_engine_type_t<ET2>;
    using allocation_traits = engine_allocation_traits<owning_type_1, owning_type_2, dyn_size, RR, CR, elem_type>;
    using allocator_type    = typename allocation_traits::allocator_type;

    //- Determine required engine template parameters from the traits types.
    //
    using layout_type_1 = get_layout_t<ET1>;
    using layout_type_2 = get_layout_t<ET2>;
    using layout_traits = multiplication_layout_traits_t<COTR, layout_type_1, layout_type_2>;
    using layout_type   = typename layout_traits::layout_type;
    using extents_type  = extents<size_t, RR, CR>;

  public:
    using element_type = elem_type;
    using engine_type  = matrix_storage_engine<element_type, extents_type, allocator_type, layout_type>;
};


template<class COTR, class S1, class ET2>
struct default_multiplication_engine_traits<COTR, matrix_scalar_engine<S1>, ET2>
{
  private:
    //- Extract the element traits from the operation traits, and determine the resulting
    //  element type.
    //
    using element_type_1 = S1;
    using element_type_2 = typename ET2::element_type;
    using element_traits = multiplication_element_traits_t<COTR, element_type_1, element_type_2>;
    using elem_type      = typename element_traits::element_type;

    //- Get the extents for the engine
    //
    static constexpr size_t     R2 = engine_extents_helper<ET2>::rows();
    static constexpr size_t     C2 = engine_extents_helper<ET2>::columns();

    //- Determine if there are dynamic row or column extents.
    //
    static constexpr bool   dyn_rows = (R2 == std::dynamic_extent);
    static constexpr bool   dyn_cols = (C2 == std::dynamic_extent);
    static constexpr bool   dyn_size = (dyn_rows || dyn_cols);

    //- Decide on the new extents.
    //
    static constexpr size_t     RR = R2;
    static constexpr size_t     CR = C2;

    //- Determine the resulting allocator type.
    //
    using owning_type_2     = get_owning_engine_type_t<ET2>;
    using allocation_traits = engine_allocation_traits<owning_type_2, owning_type_2, dyn_size, RR, CR, elem_type>;
    using allocator_type    = typename allocation_traits::allocator_type;

    //- Determine the resulting layout type.
    //
    using layout_type_2 = get_layout_t<ET2>;
    using layout_traits = multiplication_layout_traits_t<COTR, layout_type_2, layout_type_2>;
    using layout_type   = typename layout_traits::layout_type;
    using extents_type  = extents<size_t, RR, CR>;

  public:
    using element_type = elem_type;
    using engine_type  = matrix_storage_engine<element_type, extents_type, allocator_type, layout_type>;
};


template<class COTR, class ET1, class S2>
struct default_multiplication_engine_traits<COTR, ET1, matrix_scalar_engine<S2>>
{
  private:
    //- Extract the element traits from the operation traits, and determine the resulting
    //  element type.
    //
    using element_type_1 = typename ET1::element_type;
    using element_type_2 = S2;
    using element_traits = multiplication_element_traits_t<COTR, element_type_1, element_type_2>;
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

    //- Decide on the new extents.
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
    using layout_traits = multiplication_layout_traits_t<COTR, layout_type_1, layout_type_1>;
    using layout_type   = typename layout_traits::layout_type;
    using extents_type  = extents<size_t, RR, CR>;

  public:
    using element_type = elem_type;
    using engine_type  = matrix_storage_engine<element_type, extents_type, allocator_type, layout_type>;
};


//==================================================================================================
//                          **** MULTIPLICATION ARITHMETIC TRAITS ****
//==================================================================================================
//
//- The standard addition arithmetic traits type provides the default mechanism for computing the
//  result of a matrix/matrix addition.
//
template<class COTR, class ET1, class COT1, class ET2, class COT2>
struct default_multiplication_arithmetic_traits<COTR, matrix<ET1, COT1>, matrix<ET2, COT2>>
{
    using engine_type_1 = typename matrix<ET1, COT1>::engine_type;
    using engine_type_2 = typename matrix<ET2, COT2>::engine_type;
    using engine_traits = multiplication_engine_traits_t<COTR, engine_type_1, engine_type_2>;

  public:
    using element_type = typename engine_traits::element_type;
    using engine_type  = typename engine_traits::engine_type;
    using result_type  = matrix<engine_type, COTR>;

    static constexpr result_type
    multiply(matrix<ET1, COT1> const& m1, matrix<ET2, COT2> const& m2)
    {
        using size_type_1 = typename matrix<ET1, COT1>::size_type;
        using size_type_2 = typename matrix<ET2, COT2>::size_type;
        using size_type_r = typename result_type::size_type;

        size_type_r     rows  = static_cast<size_type_r>(m1.rows());
        size_type_r     cols  = static_cast<size_type_r>(m2.columns());
        size_type_1     inner = m1.columns();
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
            size_type_2     j2 = 0;

            for (;  jr < cols;  ++jr, ++j2)
            {
                element_type    er{};
                size_type_1     k1 = 0;
                size_type_2     k2 = 0;

                for (k1 = 0, k2 = 0;  k1 < inner;  ++k1, ++k2)
                {
                    er = er + (m1(i1, k1) * m2(k2, j2));
                }

                mr(ir, jr) = er;
            }
        }

        return mr;
    }
};


template<class COTR, class S1, class ET2, class COT2>
struct default_multiplication_arithmetic_traits<COTR, S1, matrix<ET2, COT2>>
{
    using engine_type_1 = matrix_scalar_engine<S1>;
    using engine_type_2 = typename matrix<ET2, COT2>::engine_type;
    using engine_traits = multiplication_engine_traits_t<COTR, engine_type_1, engine_type_2>;

  public:
    using element_type = typename engine_traits::element_type;
    using engine_type  = typename engine_traits::engine_type;
    using result_type  = matrix<engine_type, COTR>;

    static constexpr result_type
    multiply(S1 const& s1, matrix<ET2, COT2> const& m2)
    {
        using size_type_2 = typename matrix<ET2, COT2>::size_type;
        using size_type_r = typename result_type::size_type;

        size_type_r     rows = static_cast<size_type_r>(m2.rows());
        size_type_r     cols = static_cast<size_type_r>(m2.columns());
        result_type		mr;

        if constexpr (detail::reshapable_matrix_engine<engine_type>)
        {
            mr.resize(rows, cols);
        }

        size_type_r     ir = 0;
        size_type_2     i2 = 0;

        for (;  ir < rows;  ++ir, ++i2)
        {
            size_type_r     jr = 0;
            size_type_2     j2 = 0;

            for (;  jr < cols;  ++jr, ++j2)
            {
                mr(ir, jr) = s1 * m2(i2, j2);
            }
        }

        return mr;
    }
};


template<class COTR, class ET1, class COT1, class S2>
struct default_multiplication_arithmetic_traits<COTR, matrix<ET1, COT1>, S2>
{
    using engine_type_1 = typename matrix<ET1, COT1>::engine_type;
    using engine_type_2 = matrix_scalar_engine<S2>;
    using engine_traits = multiplication_engine_traits_t<COTR, engine_type_1, engine_type_2>;

  public:
    using element_type = typename engine_traits::element_type;
    using engine_type  = typename engine_traits::engine_type;
    using result_type  = matrix<engine_type, COTR>;

    static constexpr result_type
    multiply(matrix<ET1, COT1> const& m1, S2 const& s2)
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
                mr(ir, jr) = m1(i1, j1) * s2;
            }
        }

        return mr;
    }
};

}       //- namespace detail
}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_OP_TRAITS_MULTIPLICATION_HPP_DEFINED
