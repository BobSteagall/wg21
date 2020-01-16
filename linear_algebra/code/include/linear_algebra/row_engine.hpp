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

#ifdef LA_USE_VECTOR_ENGINE_ITERATORS
    using iterator        = detail::noe_iterator_t<ET, VCT, row_engine>;
    using const_iterator  = detail::vector_const_iterator<row_engine>;
#endif

#ifdef LA_USE_MDSPAN
    using span_type       = detail::noe_mdspan_row_t<detail::noe_mdspan_t<ET, VCT>>;
    using const_span_type = detail::noe_mdspan_row_t<typename ET::const_span_type>;
#endif

    //- Construct/copy/destroy
    //
    ~row_engine() noexcept = default;

    constexpr row_engine() noexcept;
    constexpr row_engine(row_engine&&) noexcept = default;
    constexpr row_engine(row_engine const&) noexcept = default;

    constexpr row_engine&   operator =(row_engine&&) noexcept = default;
    constexpr row_engine&   operator =(row_engine const&) noexcept = default;

#ifdef LA_USE_VECTOR_ENGINE_ITERATORS
    //- Iterators
    //
    constexpr iterator          begin() const noexcept;
    constexpr iterator          end() const noexcept;
    constexpr const_iterator    cbegin() const noexcept;
    constexpr const_iterator    cend() const noexcept;
#endif

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

#ifdef LA_USE_VECTOR_ENGINE_ITERATORS
//-----------
//- Iterators
//
template<class ET, class VCT> constexpr
typename row_engine<ET, VCT>::iterator
row_engine<ET, VCT>::begin() const noexcept
{
    return iterator(this, 0, mp_other->columns());
}

template<class ET, class VCT> constexpr
typename row_engine<ET, VCT>::iterator
row_engine<ET, VCT>::end() const noexcept
{
    return iterator(this, mp_other->columns(), mp_other->columns());
}

template<class ET, class VCT> constexpr
typename row_engine<ET, VCT>::const_iterator
row_engine<ET, VCT>::cbegin() const noexcept
{
    return const_iterator(this, 0, mp_other->columns());
}

template<class ET, class VCT> constexpr
typename row_engine<ET, VCT>::const_iterator
row_engine<ET, VCT>::cend() const noexcept
{
    return const_iterator(this, mp_other->columns(), mp_other->columns());
}

#endif
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
