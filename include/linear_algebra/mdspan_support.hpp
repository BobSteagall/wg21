//==================================================================================================
//  File:       mdpsan_support.hpp
//
//  Summary:    This header defines a number of private facilities to support the use of
//              mdspan across the entire library.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_MDSPAN_SUPPORT_HPP_DEFINED
#define LINEAR_ALGEBRA_MDSPAN_SUPPORT_HPP_DEFINED

namespace STD_LA {

using MDSPAN_NS::extents;
using MDSPAN_NS::dynamic_extent;
using MDSPAN_NS::layout_left;
using MDSPAN_NS::layout_left;
using MDSPAN_NS::layout_stride;

namespace detail {

using dyn_mdspan_extents = extents<dynamic_extent, dynamic_extent>;
using dyn_mdspan_strides = array<size_t, 2>;
using dyn_mdspan_layout  = layout_stride;
using dyn_mdspan_mapping = typename dyn_mdspan_layout::mapping<dyn_mdspan_extents>;

//--------------------------------------------------------------------------------------------------
//  Trait:      is_1d_mdspan<T>
//  Variable:   is_1d_mdspan_v<T>
//
//  This private traits type determines whether its template parameter is a specialization of
//  mdspan<T, X, L, A> with a one-dimensional extents template parameter.
//--------------------------------------------------------------------------------------------------
//
template<class T>
struct is_1d_mdspan : public false_type
{};

template<class T, size_t X0, class SL, class SA>
struct is_1d_mdspan<mdspan<T, extents<X0>, SL, SA>> : public true_type
{};

//------
//
template<class T> inline constexpr
bool    is_1d_mdspan_v = is_1d_mdspan<T>::value;


//--------------------------------------------------------------------------------------------------
//  Trait:      is_2d_mdspan<T>
//  Variable:   is_2d_mdspan_v<T>
//
//  This private traits type determines whether its template parameter is a specialization of
//  mdspan<T, X, L, A> with a two-dimensional extents template parameter.
//--------------------------------------------------------------------------------------------------
//
template<class T>
struct is_2d_mdspan : public false_type
{};

template<class T, size_t X0, size_t X1, class SL, class SA>
struct is_2d_mdspan<mdspan<T, extents<X0, X1>, SL, SA>> : public true_type
{};

//------
//
template<class T> inline constexpr
bool    is_2d_mdspan_v = is_2d_mdspan<T>::value;


//--------------------------------------------------------------------------------------------------
//  Trait:      mdspan_layout_mapper<T>
//  Alias:      get_mdspan_layout_t<T>
//
//  This private traits type maps a linear algebra element layout (row_major, etc.) into a
//  corresponding mdspan layout policy (layout_right, etc.).
//--------------------------------------------------------------------------------------------------
//
template<class L>   struct mdspan_layout_mapper;

template<>
struct mdspan_layout_mapper<matrix_layout::row_major>
{
    using layout_type = MDSPAN_NS::layout_right;
};

template<>
struct mdspan_layout_mapper<matrix_layout::column_major>
{
    using layout_type = MDSPAN_NS::layout_left;
};

template<>
struct mdspan_layout_mapper<void>
{
    using layout_type = MDSPAN_NS::layout_right;
};

//------
//
template<class L>
using get_mdspan_layout_t = typename mdspan_layout_mapper<L>::layout_type;


//--------------------------------------------------------------------------------------------------
//  Types:  passthru_accessor<T, WA>
//          negation_accessor<T, WA>
//          conjugate_accessor<T, WW>
//
//  These are specialized mdspan accessor policy types used for negation, transpose, and hermitian
//  views.  They wrap another accessor type WA for an element type T.  They differ mainly in how
//  they provide access to the underlying element.
//--------------------------------------------------------------------------------------------------
//
template<class T, class WA = MDSPAN_NS::default_accessor<T>>
struct passthru_accessor
{
    using offset_policy = passthru_accessor;
    using element_type  = T;
    using reference     = typename WA::reference;
    using pointer       = typename WA::pointer;

    constexpr pointer
    offset(pointer p, size_t i) const noexcept
    {
        return WA().offset(p, i);
    }

    constexpr reference
    access(pointer p, size_t i) const noexcept
    {
        return WA().access(p, i);
    }

    constexpr pointer
    decay(pointer p) const noexcept
    {
        return WA().decay(p);
    }
};

template<class T, class WA = MDSPAN_NS::default_accessor<T>>
struct negation_accessor
{
    using offset_policy = negation_accessor;
    using element_type  = T;
    using reference     = T;
    using pointer       = typename WA::pointer;

    constexpr pointer
    offset(pointer p, size_t i) const noexcept
    {
        return WA().offset(p, i);
    }

    constexpr reference
    access(pointer p, size_t i) const noexcept
    {
        return -(WA().access(p, i));
    }

    constexpr pointer
    decay(pointer p) const noexcept
    {
        return WA().decay(p);
    }
};

template<class T, class WA = MDSPAN_NS::default_accessor<T>>
struct conjugate_accessor
{
    using offset_policy = conjugate_accessor;
    using element_type  = T;
    using reference     = T;
    using pointer       = typename WA::pointer;

    constexpr pointer
    offset(pointer p, size_t i) const noexcept
    {
        return WA().offset(p, i);
    }

    constexpr reference
    access(pointer p, size_t i) const noexcept
    {
        return std::conj(WA().access(p, i));
    }

    constexpr pointer
    decay(pointer p) const noexcept
    {
        return WA().decay(p);
    }
};

}       //- detail namespace
}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_MDSPAN_SUPPORT_HPP_DEFINED
