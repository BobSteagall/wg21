//==================================================================================================
//  File:       mdpsan_support.hpp
//
//  Summary:    This header defines a number of private facilities to support the use of mdspan
//              throughout the library.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_MDSPAN_SUPPORT_HPP_DEFINED
#define LINEAR_ALGEBRA_MDSPAN_SUPPORT_HPP_DEFINED

namespace STD_LA {
namespace detail {

using dyn_mdspan_extents = MDSPAN_NS::extents<size_t, std::dynamic_extent, std::dynamic_extent>;
using dyn_mdspan_strides = array<size_t, 2>;
using dyn_mdspan_layout  = MDSPAN_NS::layout_stride;
using dyn_mdspan_mapping = typename dyn_mdspan_layout::mapping<dyn_mdspan_extents>;

//--------------------------------------------------------------------------------------------------
//  Trait:      is_2d_mdspan<T>
//  Variable:   is_2d_mdspan_v<T>
//
//  This private traits type determines whether its template parameter is a specialization of
//  mdspan<T, X, ML, MA> with a two-dimensional extents template parameter.
//--------------------------------------------------------------------------------------------------
//
template<class T>
struct is_2d_mdspan : public false_type
{};

template<class T, class IT, size_t X0, size_t X1, class SL, class SA>
struct is_2d_mdspan<mdspan<T, extents<IT, X0, X1>, SL, SA>> : public true_type
{};

//------
//
template<class T> inline constexpr
bool    is_2d_mdspan_v = is_2d_mdspan<T>::value;


//--------------------------------------------------------------------------------------------------
//  Trait:      mdspan_layout_mapper<T>
//  Alias:      get_mdspan_layout_t<T>
//
//  This private traits type maps a linear algebra element layout tag (row_major, etc.) into a
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
//  Trait:      detect_nested_mdspan_types<T>
//  Aliases:    get_mdspan_type_t<T> and get_const_mdspan_type_t<T>
//
//  This private traits type and the associated alias templates determine whether the template
//  parameter type has nested type aliases "mdspan_type" and "const_mdspan_type".  If both
//  aliases are present, then the associated aliases templates refer to the corresponding
//  mdspan types.  Otherwise, the alias templates refer to the void type.
//--------------------------------------------------------------------------------------------------
//
template<class ET, class = void>
struct detect_nested_mdspan_types
:   public false_type
{
    using mdspan_type       = void;
    using const_mdspan_type = void;
};

template<class ET>
struct detect_nested_mdspan_types<ET, void_t<typename ET::mdspan_type, typename ET::const_mdspan_type>>
:   public true_type
{
    using mdspan_type       = typename ET::mdspan_type;
    using const_mdspan_type = typename ET::const_mdspan_type;
};

//------
//
template<class ET>
using get_mdspan_type_t = typename detect_nested_mdspan_types<ET>::mdspan_type;

template<class ET>
using get_const_mdspan_type_t = typename detect_nested_mdspan_types<ET>::const_mdspan_type;


//--------------------------------------------------------------------------------------------------
//  Types:  negation_accessor<T, WA>
//          conjugate_accessor<T, WW>
//
//  These are specialized mdspan accessor policy types used for negation, transpose, and hermitian
//  views.  They wrap another accessor type WA for an element type T.  They differ mainly in how
//  they provide access to the underlying element.
//--------------------------------------------------------------------------------------------------
//
template<class T, class WA = MDSPAN_NS::default_accessor<T>>
struct negation_accessor
{
    using offset_policy    = negation_accessor;
    using element_type     = T;
    using reference        = T;
    using pointer          = typename WA::data_handle_type;
    using data_handle_type = typename WA::data_handle_type;

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
    using offset_policy    = conjugate_accessor;
    using element_type     = T;
    using reference        = T;
    using pointer          = typename WA::data_handle_type;
    using data_handle_type = typename WA::data_handle_type;

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


//--------------------------------------------------------------------------------------------------
//  Class Template:     mdspan_view_traits<T>
//
//  This traits class template provides services for determining the types of, and constructing,
//  mdspan objects that can index the set of elements represented by a matrix view engine.
//--------------------------------------------------------------------------------------------------
//
template<class T>
struct mdspan_view_traits;

//- This partial specialization is used when a view engine has a void mdspan interface.
//
template<>
struct mdspan_view_traits<void>
{
    static constexpr bool   has_mdspan = false;

    using negation_mdspan_type  = void;
    using conjugate_mdspan_type = void;
    using hermitian_mdspan_type = void;
    using submatrix_mdspan_type = void;
    using transpose_mdspan_type = void;
};

//- This partial specialization is used when the view engine has a non-void mdspan interface.
//  Note that member template parameter type EST is shorthand for "engine span type".
//
template<class T, class IT, size_t X0, size_t X1, class ML, class MA>
struct mdspan_view_traits<mdspan<T, extents<IT, X0, X1>, ML, MA>>
{
    static constexpr bool   has_mdspan = true;

    using negation_mdspan_type  = mdspan<T, dyn_mdspan_extents, dyn_mdspan_layout, negation_accessor<T, MA>>;
    using conjugate_mdspan_type = mdspan<T, dyn_mdspan_extents, dyn_mdspan_layout, conjugate_accessor<T, MA>>;
    using hermitian_mdspan_type = mdspan<T, dyn_mdspan_extents, dyn_mdspan_layout, conjugate_accessor<T, MA>>;
    using transpose_mdspan_type = mdspan<T, dyn_mdspan_extents, dyn_mdspan_layout, MA>;
    using submatrix_mdspan_type = mdspan<T, dyn_mdspan_extents, dyn_mdspan_layout, MA>;

    template<class EST>
    static constexpr negation_mdspan_type
    make_negation(EST const& s)
    {
        dyn_mdspan_extents  ext(s.extent(0), s.extent(1));
        dyn_mdspan_strides  str{s.stride(0), s.stride(1)};
        dyn_mdspan_mapping  map(ext, str);

        return negation_mdspan_type(s.data_handle(), map, negation_accessor<T, MA>());
    }

    template<class EST>
    static constexpr conjugate_mdspan_type
    make_conjugate(EST const& s)
    {
        dyn_mdspan_extents  ext(s.extent(0), s.extent(1));
        dyn_mdspan_strides  str{s.stride(0), s.stride(1)};
        dyn_mdspan_mapping  map(ext, str);

        return conjugate_mdspan_type(s.data_handle(), map, conjugate_accessor<T, MA>());
    }

    template<class EST>
    static constexpr hermitian_mdspan_type
    make_hermitian(EST const& s)
    {
        dyn_mdspan_extents  ext(s.extent(1), s.extent(0));
        dyn_mdspan_strides  str{s.stride(1), s.stride(0)};
        dyn_mdspan_mapping  map(ext, str);

        return hermitian_mdspan_type(s.data_handle(), map, conjugate_accessor<T, MA>());
    }

    template<class EST>
    static constexpr transpose_mdspan_type
    make_transpose(EST const& s)
    {
        dyn_mdspan_extents  ext(s.extent(1), s.extent(0));
        dyn_mdspan_strides  str{s.stride(1), s.stride(0)};
        dyn_mdspan_mapping  map(ext, str);

        return transpose_mdspan_type(s.data_handle(), map, MA());
    }

    template<class EST, class S1, class S2, class S3, class S4>
    static constexpr submatrix_mdspan_type
    make_submatrix(EST const& s, S1 row_index, S2 row_count, S3 col_index, S4 col_count)
    {
        using idx_t  = decltype(std::dynamic_extent);
        using pair_t = std::pair<idx_t, idx_t>;

        pair_t  row_set(static_cast<idx_t>(row_index), static_cast<idx_t>(row_index + row_count));
        pair_t  col_set(static_cast<idx_t>(col_index), static_cast<idx_t>(col_index + col_count));

        if constexpr (std::is_same_v<EST, submatrix_mdspan_type>)
        {
            return submdspan(s, row_set, col_set);
        }
        else
        {
            dyn_mdspan_extents  ext(s.extent(0), s.extent(1));
            dyn_mdspan_strides  str{s.stride(0), s.stride(1)};
            dyn_mdspan_mapping  map(ext, str);

            return submdspan(submatrix_mdspan_type(s.data_handle(), map), row_set, col_set);
        }
    }
};

}       //- detail namespace
}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_MDSPAN_SUPPORT_HPP_DEFINED
