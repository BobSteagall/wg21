//==================================================================================================
//  File:       column_views.hpp
//
//  Summary:    This header defines engines that act as views of columns.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_COLUMN_VIEWS_HPP_DEFINED
#define LINEAR_ALGEBRA_COLUMN_VIEWS_HPP_DEFINED

namespace STD_LA {
//==================================================================================================
//  Matrix column view engine, meant to act as an rvalue-ish "view" of a matrix column in 
//  expressions, in order to prevent unnecessary allocation and element copying.
//==================================================================================================
//
template<class ET, class VCT>
class matrix_column_engine
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
    using iterator        = detail::view_iterator_t<ET, VCT, matrix_column_engine>;
    using const_iterator  = detail::vector_const_iterator<matrix_column_engine>;

    //- Construct/copy/destroy
    //
    ~matrix_column_engine() noexcept = default;

    constexpr matrix_column_engine() noexcept;
    constexpr matrix_column_engine(matrix_column_engine&&) noexcept = default;
    constexpr matrix_column_engine(matrix_column_engine const&) noexcept = default;

    constexpr matrix_column_engine&     operator =(matrix_column_engine&&) noexcept = default;
    constexpr matrix_column_engine&     operator =(matrix_column_engine const&) noexcept = default;

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
    constexpr void      swap(matrix_column_engine& rhs);

  private:
    template<class ET2, class OT2>  friend class vector;
    using referent_type = detail::view_referent_t<ET, VCT>;

    referent_type*  mp_other;
    size_type       m_column;

    constexpr matrix_column_engine(referent_type& eng, size_type col);
};

//------------------------
//- Construct/copy/destroy
//
template<class ET, class VCT> constexpr 
matrix_column_engine<ET,VCT>::matrix_column_engine() noexcept
:   mp_other(nullptr)
,   m_column(0)
{}

//-----------
//- Iterators
//
template<class ET, class VCT> constexpr 
typename matrix_column_engine<ET,VCT>::iterator
matrix_column_engine<ET,VCT>::begin() const noexcept
{
    return iterator(this, 0, mp_other->rows());
}

template<class ET, class VCT> constexpr 
typename matrix_column_engine<ET,VCT>::iterator
matrix_column_engine<ET,VCT>::end() const noexcept
{
    return iterator(this, mp_other->rows(), mp_other->rows());
}

template<class ET, class VCT> constexpr 
typename matrix_column_engine<ET,VCT>::const_iterator
matrix_column_engine<ET,VCT>::cbegin() const noexcept
{
    return const_iterator(this, 0, mp_other->rows());
}

template<class ET, class VCT> constexpr 
typename matrix_column_engine<ET,VCT>::const_iterator
matrix_column_engine<ET,VCT>::cend() const noexcept
{
    return const_iterator(this, mp_other->rows(), mp_other->rows());
}

//----------
//- Capacity
//
template<class ET, class VCT> constexpr 
typename matrix_column_engine<ET,VCT>::size_type
matrix_column_engine<ET,VCT>::capacity() const noexcept
{
    return mp_other->rows();
}

template<class ET, class VCT> constexpr 
typename matrix_column_engine<ET,VCT>::size_type
matrix_column_engine<ET,VCT>::elements() const noexcept
{
    return mp_other->rows();
}

//----------------
//- Element access
//
template<class ET, class VCT> constexpr 
typename matrix_column_engine<ET,VCT>::reference
matrix_column_engine<ET,VCT>::operator ()(size_type i) const
{
    return (*mp_other)(i, m_column);
}

//-----------
//- Modifiers
//
template<class ET, class VCT> constexpr 
void
matrix_column_engine<ET,VCT>::swap(matrix_column_engine& rhs)
{
    std::swap(mp_other, rhs.mp_other);
    std::swap(m_column, rhs.m_column);
}

//------------------------
//- Private implementation
//
template<class ET, class VCT> constexpr 
matrix_column_engine<ET,VCT>::matrix_column_engine(referent_type& eng, size_type col)
:   mp_other(&eng)
,   m_column(col)
{}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_COLUMN_VIEWS_HPP_DEFINED
