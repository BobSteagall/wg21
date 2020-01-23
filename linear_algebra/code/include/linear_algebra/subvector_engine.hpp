//==================================================================================================
//  File:       subvector_engine.hpp
//
//  Summary:    This header defines engines that act as views of rows and columns.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_SUBVECTOR_ENGINE_HPP_DEFINED
#define LINEAR_ALGEBRA_SUBVECTOR_ENGINE_HPP_DEFINED

namespace STD_LA {
//=================================================================================================
//  Matrix column engine, meant to act as an rvalue-ish "view" of a matrix row or column,
//  in order to help avoid unnecessary allocation and element copying.
//==================================================================================================
//
template<class ET, class VCT, class RCT>
class subvector_engine
{
    static_assert(is_matrix_engine_v<ET>);
    static_assert(is_vector_engine_tag<VCT>);
    static_assert(is_same_v<RCT, column_tag> || is_same_v<RCT, row_tag>);

    static constexpr bool   this_represents_a_column = is_same_v<RCT, column_tag>;

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
    using span_type       = detail::noe_mdspan_column_t<detail::noe_mdspan_t<ET, VCT>>;
    using const_span_type = detail::noe_mdspan_column_t<detail::noe_const_mdspan_t<ET, VCT>>;
#endif

    //- Construct/copy/destroy
    //
    ~subvector_engine() noexcept = default;

    constexpr subvector_engine() noexcept;
    constexpr subvector_engine(subvector_engine&&) noexcept = default;
    constexpr subvector_engine(subvector_engine const&) noexcept = default;

    constexpr subvector_engine&    operator =(subvector_engine&&) noexcept = default;
    constexpr subvector_engine&    operator =(subvector_engine const&) noexcept = default;

    template<class ET2, detail::enable_if_convertible_engine<ET2, ET> = true>
    constexpr subvector_engine&    operator =(ET2 const& rhs);
    template<class U, detail::enable_if_writable<ET, ET> = true>
    constexpr subvector_engine&    operator =(initializer_list<U> list);

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
    constexpr void      swap(subvector_engine& rhs);

  private:
    template<class ET2, class OT2>  friend class vector;
    using referent_type = detail::noe_referent_t<ET, VCT>;

    referent_type*  mp_other;
    size_type       m_fixed;

    constexpr subvector_engine(referent_type& eng, size_type col);
};

//------------------------
//- Construct/copy/destroy
//
template<class ET, class VCT, class RCT> constexpr
subvector_engine<ET,VCT,RCT>::subvector_engine() noexcept
:   mp_other(nullptr)
,   m_fixed(0)
{}

template<class ET, class VCT, class RCT>
template<class ET2, detail::enable_if_convertible_engine<ET2, ET>> constexpr
subvector_engine<ET,VCT,RCT>&
subvector_engine<ET,VCT,RCT>::operator =(ET2 const& rhs)
{
    detail::check_source_engine_size(rhs, elements());
    detail::assign_from_vector_engine(*this, rhs);
    return *this;
}

template<class ET, class VCT, class RCT>
template<class U, detail::enable_if_writable<ET, ET>> constexpr
subvector_engine<ET,VCT,RCT>&
subvector_engine<ET,VCT,RCT>::operator =(initializer_list<U> rhs)
{
    detail::check_source_init_list(rhs, elements());
    detail::assign_from_vector_list(*this, rhs);
    return *this;
}

//----------
//- Capacity
//
template<class ET, class VCT, class RCT> constexpr
typename subvector_engine<ET,VCT,RCT>::size_type
subvector_engine<ET,VCT,RCT>::capacity() const noexcept
{
    return elements();
}

template<class ET, class VCT, class RCT> constexpr
typename subvector_engine<ET,VCT,RCT>::size_type
subvector_engine<ET,VCT,RCT>::elements() const noexcept
{
    if constexpr (this_represents_a_column)
    {
        return mp_other->rows();
    }
    else
    {
        return mp_other->columns();
    }
}

//----------------
//- Element access
//
template<class ET, class VCT, class RCT> constexpr
typename subvector_engine<ET,VCT,RCT>::reference
subvector_engine<ET,VCT,RCT>::operator ()(size_type i) const
{
    if constexpr (this_represents_a_column)
    {
        return (*mp_other)(i, m_fixed);
    }
    else
    {
        return (*mp_other)(m_fixed, i);
    }
}

#ifdef LA_USE_MDSPAN

template<class ET, class VCT, class RCT> constexpr
typename subvector_engine<ET,VCT,RCT>::span_type
subvector_engine<ET,VCT,RCT>::span() const noexcept
{
    if constexpr (this_represents_a_column)
    {
        return detail::noe_mdspan_column(mp_other->span(), m_fixed);
    }
    else
    {
        return detail::noe_mdspan_row(mp_other->span(), m_fixed);
    }
}

#endif
//-----------
//- Modifiers
//
template<class ET, class VCT, class RCT> constexpr
void
subvector_engine<ET,VCT,RCT>::swap(subvector_engine& rhs)
{
    std::swap(mp_other, rhs.mp_other);
    std::swap(m_fixed, rhs.m_fixed);
}

//------------------------
//- Private implementation
//
template<class ET, class VCT, class RCT> constexpr
subvector_engine<ET,VCT,RCT>::subvector_engine(referent_type& eng, size_type fix)
:   mp_other(&eng)
,   m_fixed(fix)
{}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_SUBVECTOR_ENGINE_HPP_DEFINED
