//==================================================================================================
//  File:       row_views.hpp
//
//  Summary:    This header defines engines that act as views of rows and columns.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_ROW_VIEWS_HPP_DEFINED
#define LINEAR_ALGEBRA_ROW_VIEWS_HPP_DEFINED

namespace STD_LA {
//==================================================================================================
//  Matrix row view engine, meant to act as an rvalue-ish "view" of a matrix row in expressions, 
//  in order to prevent unnecessary allocation and element copying.
//==================================================================================================
//
template<class ET, class VCT>
class matrix_row_view
{
    static_assert(detail::is_matrix_engine_v<ET>);
    static_assert(detail::is_vector_engine_tag<VCT>);

  public:
    //- Types
    //
    using engine_category = VCT;
    using element_type    = typename ET::element_type;
    using value_type      = typename ET::value_type;
    using pointer         = detail::view_pointer_t<ET, VCT>;
    using const_pointer   = typename ET::const_pointer;
    using reference       = detail::view_reference_t<ET, VCT>;
    using const_reference = typename ET::const_reference;
    using difference_type = typename ET::difference_type;
    using size_type       = typename ET::size_type;
    using iterator        = detail::view_iterator_t<ET, VCT, matrix_row_view>;
    using const_iterator  = detail::vector_const_iterator<matrix_row_view>;

    //- Construct/copy/destroy
    //
    ~matrix_row_view() noexcept = default;

    constexpr matrix_row_view() noexcept;
    constexpr matrix_row_view(matrix_row_view&&) noexcept = default;
    constexpr matrix_row_view(matrix_row_view const&) = default;

    constexpr matrix_row_view&  operator =(matrix_row_view&&) noexcept = default;
    constexpr matrix_row_view&  operator =(matrix_row_view const&) noexcept = default;

    //- Iterators
    //
    constexpr iterator          begin() const noexcept;
    constexpr iterator          end() const noexcept;
    constexpr const_iterator    cbegin() const noexcept;
    constexpr const_iterator    cend() const noexcept;

    //- Capacity
    //
    constexpr size_type     capacity() const noexcept;
    constexpr size_type     elements() const noexcept;

    //- Element access
    //
    constexpr reference     operator ()(size_type i) const;

    //- Modifiers
    //
    constexpr void      swap(matrix_row_view& rhs);

  private:
    template<class ET2, class OT2>  friend class vector;
    using referent_type = detail::view_referent_t<ET, VCT>;

    referent_type*  mp_other;
    size_type       m_row;

    constexpr matrix_row_view(referent_type& eng, size_type row);
};

//------------------------
//- Construct/copy/destroy
//
template<class ET, class VCT> constexpr 
matrix_row_view<ET, VCT>::matrix_row_view() noexcept
:   mp_other(nullptr)
,   m_row(0)
{}

//-----------
//- Iterators
//
template<class ET, class VCT> constexpr 
typename matrix_row_view<ET, VCT>::iterator
matrix_row_view<ET, VCT>::begin() const noexcept
{
    return iterator(this, 0, mp_other->columns());
}

template<class ET, class VCT> constexpr 
typename matrix_row_view<ET, VCT>::iterator
matrix_row_view<ET, VCT>::end() const noexcept
{
    return iterator(this, mp_other->columns(), mp_other->columns());
}

template<class ET, class VCT> constexpr 
typename matrix_row_view<ET, VCT>::const_iterator
matrix_row_view<ET, VCT>::cbegin() const noexcept
{
    return const_iterator(this, 0, mp_other->columns());
}

template<class ET, class VCT> constexpr 
typename matrix_row_view<ET, VCT>::const_iterator
matrix_row_view<ET, VCT>::cend() const noexcept
{
    return const_iterator(this, mp_other->columns(), mp_other->columns());
}

//----------
//- Capacity
//
template<class ET, class VCT> constexpr 
typename matrix_row_view<ET, VCT>::size_type
matrix_row_view<ET, VCT>::capacity() const noexcept
{
    return mp_other->columns();
}

template<class ET, class VCT> constexpr 
typename matrix_row_view<ET, VCT>::size_type
matrix_row_view<ET, VCT>::elements() const noexcept
{
    return mp_other->columns();
}

//----------------
//- Element access
//
template<class ET, class VCT> constexpr 
typename matrix_row_view<ET, VCT>::reference
matrix_row_view<ET, VCT>::operator ()(size_type j) const
{
    return (*mp_other)(m_row, j);
}

//-----------
//- Modifiers
//
template<class ET, class VCT> constexpr 
void
matrix_row_view<ET, VCT>::swap(matrix_row_view& rhs)
{
    std::swap(mp_other, rhs.mp_other);
    std::swap(m_row, rhs.m_row);
}

//------------------------
//- Private implementation
//
template<class ET, class VCT> constexpr 
matrix_row_view<ET, VCT>::matrix_row_view(referent_type& eng, size_type row)
:   mp_other(&eng)
,   m_row(row)
{}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_ROW_VIEWS_HPP_DEFINED
