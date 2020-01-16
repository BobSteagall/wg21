//==================================================================================================
//  File:       column_engine.hpp
//
//  Summary:    This header defines engines that act as views of columns.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_COLUMN_ENGINE_HPP_DEFINED
#define LINEAR_ALGEBRA_COLUMN_ENGINE_HPP_DEFINED

namespace STD_LA {
//=================================================================================================
//  Matrix column engine, meant to act as an rvalue-ish "view" of a matrix column in expressions,
//  in order to help avoid unnecessary allocation and element copying.
//==================================================================================================
//
template<class ET, class VCT>
class column_engine
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
    using iterator        = detail::noe_iterator_t<ET, VCT, column_engine>;
    using const_iterator  = detail::vector_const_iterator<column_engine>;
#endif

#ifdef LA_USE_MDSPAN
    using span_type       = detail::noe_mdspan_column_t<detail::noe_mdspan_t<ET, VCT>>;
    using const_span_type = detail::noe_mdspan_column_t<typename ET::const_span_type>;
#endif

    //- Construct/copy/destroy
    //
    ~column_engine() noexcept = default;

    constexpr column_engine() noexcept;
    constexpr column_engine(column_engine&&) noexcept = default;
    constexpr column_engine(column_engine const&) noexcept = default;

    constexpr column_engine&    operator =(column_engine&&) noexcept = default;
    constexpr column_engine&    operator =(column_engine const&) noexcept = default;

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
    constexpr void      swap(column_engine& rhs);

  private:
    template<class ET2, class OT2>  friend class vector;
    using referent_type = detail::noe_referent_t<ET, VCT>;

    referent_type*  mp_other;
    size_type       m_column;

    constexpr column_engine(referent_type& eng, size_type col);
};

//------------------------
//- Construct/copy/destroy
//
template<class ET, class VCT> constexpr
column_engine<ET,VCT>::column_engine() noexcept
:   mp_other(nullptr)
,   m_column(0)
{}

#ifdef LA_USE_VECTOR_ENGINE_ITERATORS
//-----------
//- Iterators
//
template<class ET, class VCT> constexpr
typename column_engine<ET,VCT>::iterator
column_engine<ET,VCT>::begin() const noexcept
{
    return iterator(this, 0, mp_other->rows());
}

template<class ET, class VCT> constexpr
typename column_engine<ET,VCT>::iterator
column_engine<ET,VCT>::end() const noexcept
{
    return iterator(this, mp_other->rows(), mp_other->rows());
}

template<class ET, class VCT> constexpr
typename column_engine<ET,VCT>::const_iterator
column_engine<ET,VCT>::cbegin() const noexcept
{
    return const_iterator(this, 0, mp_other->rows());
}

template<class ET, class VCT> constexpr
typename column_engine<ET,VCT>::const_iterator
column_engine<ET,VCT>::cend() const noexcept
{
    return const_iterator(this, mp_other->rows(), mp_other->rows());
}

#endif
//----------
//- Capacity
//
template<class ET, class VCT> constexpr
typename column_engine<ET,VCT>::size_type
column_engine<ET,VCT>::capacity() const noexcept
{
    return mp_other->rows();
}

template<class ET, class VCT> constexpr
typename column_engine<ET,VCT>::size_type
column_engine<ET,VCT>::elements() const noexcept
{
    return mp_other->rows();
}

//----------------
//- Element access
//
template<class ET, class VCT> constexpr
typename column_engine<ET,VCT>::reference
column_engine<ET,VCT>::operator ()(size_type i) const
{
    return (*mp_other)(i, m_column);
}

#ifdef LA_USE_MDSPAN

template<class ET, class VCT> constexpr
typename column_engine<ET,VCT>::span_type
column_engine<ET,VCT>::span() const noexcept
{
    return detail::noe_mdspan_column(mp_other->span(), m_column);
}

#endif
//-----------
//- Modifiers
//
template<class ET, class VCT> constexpr
void
column_engine<ET,VCT>::swap(column_engine& rhs)
{
    std::swap(mp_other, rhs.mp_other);
    std::swap(m_column, rhs.m_column);
}

//------------------------
//- Private implementation
//
template<class ET, class VCT> constexpr
column_engine<ET,VCT>::column_engine(referent_type& eng, size_type col)
:   mp_other(&eng)
,   m_column(col)
{}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_COLUMN_ENGINE_HPP_DEFINED
