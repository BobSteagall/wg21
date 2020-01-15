//==================================================================================================
//  File:       transpose_engine.hpp
//
//  Summary:    This header defines an engine that acts as a "view" of matrix transpose.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_TRANSPOSE_ENGINE_HPP_DEFINED
#define LINEAR_ALGEBRA_TRANSPOSE_ENGINE_HPP_DEFINED

namespace STD_LA {
//==================================================================================================
//  Matrix transpose engine, meant to act as a "view" of a matrix transpose in expressions, in
//  order to help avoid unnecessary allocation and element copying.
//==================================================================================================
//
template<class ET, class MCT>
class transpose_engine
{
    static_assert(is_matrix_engine_v<ET>);
    static_assert(is_matrix_engine_tag<MCT>);

#ifdef LA_USE_MDSPAN
    using src_spn_type = detail::noe_mdspan_t<ET, MCT>;
    using src_extents  = typename src_spn_type::extents_type;
    using extents_type = extents<src_extents::static_extent(1), src_extents::static_extent(0)>;
    using strides_type = array<typename extents_type::index_type, 2>;
    using layout_type  = layout_stride<src_extents::static_extent(1), src_extents::static_extent(0)>;
    using mapping_type = typename layout_type::template mapping<extents_type>;
    using idx_type     = typename extents_type::index_type;
#endif

  public:
    //- Types
    //
    using engine_category = MCT;
    using element_type    = typename ET::element_type;
    using value_type      = typename ET::value_type;
    using pointer         = detail::noe_pointer_t<ET, MCT>;
    using const_pointer   = typename ET::const_pointer;
    using reference       = detail::noe_reference_t<ET, MCT>;
    using const_reference = typename ET::const_reference;
    using difference_type = typename ET::difference_type;
    using size_type       = typename ET::size_type;
    using size_tuple      = typename ET::size_tuple;

#ifdef LA_USE_MDSPAN
    using span_type       = basic_mdspan<element_type, extents_type, layout_type>;
    using const_span_type = basic_mdspan<element_type const, extents_type, layout_type>;
#endif

    //- Construct/copy/destroy
    //
    ~transpose_engine() noexcept = default;

    constexpr transpose_engine();
    constexpr transpose_engine(transpose_engine&&) noexcept = default;
    constexpr transpose_engine(transpose_engine const&) = default;

    constexpr transpose_engine&     operator =(transpose_engine&&) noexcept = default;
    constexpr transpose_engine&     operator =(transpose_engine const&) = default;

    //- Capacity
    //
    constexpr size_type     columns() const noexcept;
    constexpr size_type     rows() const noexcept;
    constexpr size_tuple    size() const noexcept;

    constexpr size_type     column_capacity() const noexcept;
    constexpr size_type     row_capacity() const noexcept;
    constexpr size_tuple    capacity() const noexcept;

    //- Element access
    //
    constexpr reference     operator ()(size_type i, size_type j) const;

#ifdef LA_USE_MDSPAN
    //- Data access
    //
    constexpr span_type     span() const noexcept;
#endif

    //- Modifiers
    //
    constexpr void      swap(transpose_engine& rhs);

  private:
    template<class ET2, class OT2>  friend class matrix;
    using referent_type = detail::noe_referent_t<ET, MCT>;

    referent_type*      mp_other;

    constexpr transpose_engine(referent_type& eng);
};

//------------------------
//- Construct/copy/destroy
//
template<class ET, class MCT> constexpr
transpose_engine<ET, MCT>::transpose_engine()
:   mp_other(nullptr)
{}

//----------
//- Capacity
//
template<class ET, class MCT> constexpr
typename transpose_engine<ET, MCT>::size_type
transpose_engine<ET, MCT>::columns() const noexcept
{
    return mp_other->rows();
}

template<class ET, class MCT> constexpr
typename transpose_engine<ET, MCT>::size_type
transpose_engine<ET, MCT>::rows() const noexcept
{
    return mp_other->columns();
}

template<class ET, class MCT> constexpr
typename transpose_engine<ET, MCT>::size_tuple
transpose_engine<ET, MCT>::size() const noexcept
{
    return size_tuple(mp_other->columns(), mp_other->rows());
}

template<class ET, class MCT> constexpr
typename transpose_engine<ET, MCT>::size_type
transpose_engine<ET, MCT>::column_capacity() const noexcept
{
    return mp_other->row_capacity();
}

template<class ET, class MCT> constexpr
typename transpose_engine<ET, MCT>::size_type
transpose_engine<ET, MCT>::row_capacity() const noexcept
{
    return mp_other->column_capacity();
}

template<class ET, class MCT> constexpr
typename transpose_engine<ET, MCT>::size_tuple
transpose_engine<ET, MCT>::capacity() const noexcept
{
    return size_tuple(mp_other->column_capacity(), mp_other->row_capacity());
}

//----------------
//- Element access
//
template<class ET, class MCT> constexpr
typename transpose_engine<ET, MCT>::reference
transpose_engine<ET, MCT>::operator ()(size_type i, size_type j) const
{
    return (*mp_other)(j, i);
}

#ifdef LA_USE_MDSPAN
//-------------
//- Data access
//
template<class ET, class MCT> constexpr
typename transpose_engine<ET, MCT>::span_type
transpose_engine<ET, MCT>::span() const noexcept
{
    src_spn_type    src_span = mp_other->span();

    if constexpr (src_span.is_strided())
    {
        extents_type    dst_extents(src_span.extent(1), src_span.extent(0));
        strides_type    dst_strides{src_span.stride(1), src_span.stride(0)};
        mapping_type    mapping(dst_extents, dst_strides);

        return span_type(src_span.data(), mapping);
    }
    else
    {
    }
}

#endif
//-----------
//- Modifiers
//
template<class ET, class MCT> constexpr
void
transpose_engine<ET, MCT>::swap(transpose_engine& rhs)
{
    std::swap(mp_other, rhs.mp_other);
}

template<class ET, class MCT> constexpr
transpose_engine<ET, MCT>::transpose_engine(referent_type& eng)
:   mp_other(&eng)
{}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_TRANSPOSE_ENGINE_HPP_DEFINED
