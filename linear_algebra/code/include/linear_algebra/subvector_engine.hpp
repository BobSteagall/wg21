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
template<class ET, class VCT>
class subvector_engine
{
    static_assert(is_vector_engine_v<ET>);
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
    using span_type       = detail::noe_mdspan_subvector_t<detail::noe_mdspan_t<ET, VCT>>;
    using const_span_type = detail::noe_mdspan_subvector_t<detail::noe_const_mdspan_t<ET, VCT>>;
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
    template<class U, class ET2 = ET, detail::enable_if_writable<ET2, ET> = true>
    constexpr subvector_engine&    operator =(initializer_list<U> list);

    //- Capacity
    //
    constexpr size_type     capacity() const noexcept;
    constexpr size_type     elements() const noexcept;

    //- Element access
    //
    constexpr reference     operator ()(size_type i) const;

    //- Data access
    //
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
    size_type       m_start;
    size_type       m_count;

    constexpr subvector_engine(referent_type& eng, size_type start, size_type count);
};

//------------------------
//- Construct/copy/destroy
//
template<class ET, class VCT> constexpr
subvector_engine<ET,VCT>::subvector_engine() noexcept
:   mp_other(nullptr)
,   m_start(0)
,   m_count(0)
{}

template<class ET, class VCT>
template<class ET2, detail::enable_if_convertible_engine<ET2, ET>> constexpr
subvector_engine<ET,VCT>&
subvector_engine<ET,VCT>::operator =(ET2 const& rhs)
{
    detail::check_source_engine_size(rhs, elements());
    detail::assign_from_vector_engine(*this, rhs);
    return *this;
}

template<class ET, class VCT>
template<class U, class ET2, detail::enable_if_writable<ET2, ET>> constexpr
subvector_engine<ET,VCT>&
subvector_engine<ET,VCT>::operator =(initializer_list<U> rhs)
{
    detail::check_source_init_list(rhs, elements());
    detail::assign_from_vector_list(*this, rhs);
    return *this;
}

//----------
//- Capacity
//
template<class ET, class VCT> constexpr
typename subvector_engine<ET,VCT>::size_type
subvector_engine<ET,VCT>::capacity() const noexcept
{
    return m_count;
}

template<class ET, class VCT> constexpr
typename subvector_engine<ET,VCT>::size_type
subvector_engine<ET,VCT>::elements() const noexcept
{
    return m_count;
}

//----------------
//- Element access
//
template<class ET, class VCT> constexpr
typename subvector_engine<ET,VCT>::reference
subvector_engine<ET,VCT>::operator ()(size_type i) const
{
    return (*mp_other)(i + m_start);
}

//-------------
//- Data access
//
#ifdef LA_USE_MDSPAN

template<class ET, class VCT> constexpr
typename subvector_engine<ET,VCT>::span_type
subvector_engine<ET,VCT>::span() const noexcept
{
    return detail::noe_mdspan_subvector(mp_other->span(), m_start, m_count);
}

#endif
//-----------
//- Modifiers
//
template<class ET, class VCT> constexpr
void
subvector_engine<ET,VCT>::swap(subvector_engine& rhs)
{
    std::swap(mp_other, rhs.mp_other);
    std::swap(m_start, rhs.m_start);
    std::swap(m_count, rhs.m_count);
}

//------------------------
//- Private implementation
//
template<class ET, class VCT> constexpr
subvector_engine<ET,VCT>::subvector_engine(referent_type& eng, size_type start, size_type count)
:   mp_other(&eng)
,   m_start(start)
,   m_count(count)
{}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_SUBVECTOR_ENGINE_HPP_DEFINED
