//==================================================================================================
//  File:       row_engine.hpp
//
//  Summary:    This header defines an engine that acts as a "view" of a matrix row.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_ROW_ENGINE_HPP_DEFINED
#define LINEAR_ALGEBRA_ROW_ENGINE_HPP_DEFINED

namespace STD_LA {
//==================================================================================================
//  Matrix row engine, meant to act as a "view" of a matrix row in expressions, in order to help
//  avoid unnecessary allocation and element copying.
//==================================================================================================
//
template<class ET, class VCT>
class row_engine
{
    static_assert(is_matrix_engine_v<ET>);
    static_assert(is_vector_engine_tag<VCT>);

  public:
    //- Types
    //
    using engine_category = VCT;
    using element_type    = typename ET::element_type;
    using value_type      = typename ET::value_type;
    using pointer         = detail::noe_pointer_t<ET, VCT>;
    using const_pointer   = typename ET::const_pointer;
    using reference       = detail::noe_reference_t<ET, VCT>;
    using const_reference = typename ET::const_reference;
    using difference_type = typename ET::difference_type;
    using size_type       = typename ET::size_type;

#ifdef LA_USE_MDSPAN
    using span_type       = detail::noe_mdspan_row_t<detail::noe_mdspan_t<ET, VCT>>;
    using const_span_type = detail::noe_mdspan_row_t<detail::noe_const_mdspan_t<ET, VCT>>;
#endif

    //- Construct/copy/destroy
    //
    ~row_engine() noexcept = default;

    constexpr row_engine() noexcept;
    constexpr row_engine(row_engine&&) noexcept = default;
    constexpr row_engine(row_engine const&) noexcept = default;

    constexpr row_engine&   operator =(row_engine&&) noexcept = default;
    constexpr row_engine&   operator =(row_engine const&) noexcept = default;

    template<class ET2, detail::enable_if_convertible_engine<ET2, ET> = true>
    constexpr row_engine&   operator =(ET2 const& rhs);
    template<class U, detail::enable_if_writable<ET, ET> = true>
    constexpr row_engine&   operator =(initializer_list<U> list);

    //- Capacity
    //
    constexpr size_type     capacity() const noexcept;
    constexpr size_type     elements() const noexcept;

    //- Element access
    //
    constexpr reference     operator ()(size_type i) const;

#ifdef LA_USE_MDSPAN
    constexpr span_type     span() const noexcept;
#endif

    //- Modifiers
    //
    constexpr void      swap(row_engine& rhs);

  private:
    template<class ET2, class OT2>  friend class vector;
    using referent_type = detail::noe_referent_t<ET, VCT>;

    referent_type*  mp_other;
    size_type       m_row;

    constexpr row_engine(referent_type& eng, size_type row);
};

//------------------------
//- Construct/copy/destroy
//
template<class ET, class VCT> constexpr
row_engine<ET, VCT>::row_engine() noexcept
:   mp_other(nullptr)
,   m_row(0)
{}

template<class ET, class VCT>
template<class ET2, detail::enable_if_convertible_engine<ET2, ET>> constexpr
row_engine<ET,VCT>&
row_engine<ET,VCT>::operator =(ET2 const& rhs)
{
    detail::check_source_engine_size(rhs, elements());
    detail::assign_from_vector_engine(*this, rhs);
    return *this;
}

template<class ET, class VCT>
template<class U, detail::enable_if_writable<ET, ET>> constexpr
row_engine<ET,VCT>&
row_engine<ET,VCT>::operator =(initializer_list<U> rhs)
{
    detail::check_source_init_list(rhs, elements());
    detail::assign_from_vector_list(*this, rhs);
    return *this;
}

//----------
//- Capacity
//
template<class ET, class VCT> constexpr
typename row_engine<ET, VCT>::size_type
row_engine<ET, VCT>::capacity() const noexcept
{
    return mp_other->columns();
}

template<class ET, class VCT> constexpr
typename row_engine<ET, VCT>::size_type
row_engine<ET, VCT>::elements() const noexcept
{
    return mp_other->columns();
}

//----------------
//- Element access
//
template<class ET, class VCT> constexpr
typename row_engine<ET, VCT>::reference
row_engine<ET, VCT>::operator ()(size_type j) const
{
    return (*mp_other)(m_row, j);
}

#ifdef LA_USE_MDSPAN

template<class ET, class VCT> constexpr
typename row_engine<ET,VCT>::span_type
row_engine<ET,VCT>::span() const noexcept
{
    return detail::noe_mdspan_row(mp_other->span(), m_row);
}

#endif

//-----------
//- Modifiers
//
template<class ET, class VCT> constexpr
void
row_engine<ET, VCT>::swap(row_engine& rhs)
{
    std::swap(mp_other, rhs.mp_other);
    std::swap(m_row, rhs.m_row);
}

//------------------------
//- Private implementation
//
template<class ET, class VCT> constexpr
row_engine<ET, VCT>::row_engine(referent_type& eng, size_type row)
:   mp_other(&eng)
,   m_row(row)
{}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_ROW_ENGINE_HPP_DEFINED
