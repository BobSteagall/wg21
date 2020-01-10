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
template<class ET>
class matrix_column_view<ET, const_vector_engine_tag>
{
    static_assert(detail::is_matrix_engine_v<ET>);
    using engine_type = ET;

  public:
    using engine_category = const_vector_engine_tag;
    using element_type    = typename ET::element_type;
    using value_type      = typename ET::value_type;
    using difference_type = typename ET::difference_type;
    using size_type       = typename ET::size_type;
    using reference       = typename ET::const_reference;
    using const_reference = typename ET::const_reference;
    using pointer         = typename ET::const_pointer;
    using const_pointer   = typename ET::const_pointer;
    using iterator        = detail::vector_const_iterator<matrix_column_view const>;
    using const_iterator  = detail::vector_const_iterator<matrix_column_view const>;

  public:
    constexpr matrix_column_view();
    constexpr matrix_column_view(ET const& eng, size_type col);
    constexpr matrix_column_view(matrix_column_view&&) noexcept = default;
    constexpr matrix_column_view(matrix_column_view const&) = default;

    constexpr matrix_column_view&   operator =(matrix_column_view&&) noexcept = default;
    constexpr matrix_column_view&   operator =(matrix_column_view const&) = default;

    constexpr size_type     capacity() const noexcept;
    constexpr size_type     elements() const noexcept;

    constexpr reference     operator ()(size_type i) const;
    constexpr iterator      begin() const noexcept;
    constexpr iterator      end() const noexcept;

    constexpr const_iterator    cbegin() const noexcept;
    constexpr const_iterator    cend() const noexcept;

    constexpr void  assign(matrix_column_view const& rhs);
    constexpr void  swap(matrix_column_view& rhs);

  private:
    engine_type const*  mp_other;
    size_type           m_column;
};

template<class ET> constexpr 
matrix_column_view<ET,const_vector_engine_tag>::matrix_column_view()
:   mp_other(nullptr)
,   m_column(0)
{}

template<class ET> constexpr 
matrix_column_view<ET,const_vector_engine_tag>::matrix_column_view(ET const& eng, size_type col)
:   mp_other(&eng)
,   m_column(col)
{}

template<class ET> constexpr 
typename matrix_column_view<ET,const_vector_engine_tag>::reference
matrix_column_view<ET,const_vector_engine_tag>::operator ()(size_type i) const
{
    return (*mp_other)(i, m_column);
}

template<class ET> constexpr 
typename matrix_column_view<ET,const_vector_engine_tag>::size_type
matrix_column_view<ET,const_vector_engine_tag>::capacity() const noexcept
{
    return mp_other->rows();
}

template<class ET> constexpr 
typename matrix_column_view<ET,const_vector_engine_tag>::size_type
matrix_column_view<ET,const_vector_engine_tag>::elements() const noexcept
{
    return mp_other->rows();
}

template<class ET> constexpr 
typename matrix_column_view<ET,const_vector_engine_tag>::iterator
matrix_column_view<ET,const_vector_engine_tag>::begin() const noexcept
{
    return iterator(this, 0, mp_other->rows());
}

template<class ET> constexpr 
typename matrix_column_view<ET,const_vector_engine_tag>::iterator
matrix_column_view<ET,const_vector_engine_tag>::end() const noexcept
{
    return iterator(this, mp_other->rows(), mp_other->rows());
}

template<class ET> constexpr 
typename matrix_column_view<ET,const_vector_engine_tag>::const_iterator
matrix_column_view<ET,const_vector_engine_tag>::cbegin() const noexcept
{
    return const_iterator(this, 0, mp_other->rows());
}

template<class ET> constexpr 
typename matrix_column_view<ET,const_vector_engine_tag>::const_iterator
matrix_column_view<ET,const_vector_engine_tag>::cend() const noexcept
{
    return const_iterator(this, mp_other->rows(), mp_other->rows());
}

template<class ET> constexpr 
void
matrix_column_view<ET,const_vector_engine_tag>::assign(matrix_column_view const& rhs)
{
    mp_other = rhs.mp_other;
    m_column = rhs.m_column;
}

template<class ET> constexpr 
void
matrix_column_view<ET,const_vector_engine_tag>::swap(matrix_column_view& rhs)
{
    std::swap(mp_other, rhs.mp_other);
    std::swap(m_column, rhs.m_column);
}


template<class ET>
class matrix_column_view<ET, mutable_vector_engine_tag>
{
    static_assert(detail::is_matrix_engine_v<ET>);
    using engine_type = ET;

  public:
    using engine_category = mutable_vector_engine_tag;
    using element_type    = typename ET::element_type;
    using value_type      = typename ET::value_type;
    using difference_type = typename ET::difference_type;
    using size_type       = typename ET::size_type;
    using reference       = typename ET::reference;
    using const_reference = typename ET::const_reference;
    using pointer         = typename ET::pointer;
    using const_pointer   = typename ET::const_pointer;
    using iterator        = detail::vector_iterator<matrix_column_view>;
    using const_iterator  = detail::vector_const_iterator<matrix_column_view>;

  public:
    constexpr matrix_column_view();
    constexpr matrix_column_view(ET& eng, size_type col);
    constexpr matrix_column_view(matrix_column_view&&) noexcept = default;
    constexpr matrix_column_view(matrix_column_view const&) = default;

    constexpr matrix_column_view&   operator =(matrix_column_view&&) noexcept = default;
    constexpr matrix_column_view&   operator =(matrix_column_view const&) = default;

    constexpr size_type     capacity() const noexcept;
    constexpr size_type     elements() const noexcept;

    constexpr reference     operator ()(size_type i) const;
    constexpr iterator      begin() const noexcept;
    constexpr iterator      end() const noexcept;

    constexpr const_iterator    cbegin() const noexcept;
    constexpr const_iterator    cend() const noexcept;

    constexpr void  assign(matrix_column_view const& rhs);
    constexpr void  swap(matrix_column_view& rhs);

  private:
    engine_type*    mp_other;
    size_type       m_column;
};

template<class ET> constexpr 
matrix_column_view<ET,mutable_vector_engine_tag>::matrix_column_view()
:   mp_other(nullptr)
,   m_column(0)
{}

template<class ET> constexpr 
matrix_column_view<ET,mutable_vector_engine_tag>::matrix_column_view(ET& eng, size_type col)
:   mp_other(&eng)
,   m_column(col)
{}

template<class ET> constexpr 
typename matrix_column_view<ET,mutable_vector_engine_tag>::reference
matrix_column_view<ET,mutable_vector_engine_tag>::operator ()(size_type i) const
{
    return (*mp_other)(i, m_column);
}

template<class ET> constexpr 
typename matrix_column_view<ET,mutable_vector_engine_tag>::size_type
matrix_column_view<ET,mutable_vector_engine_tag>::capacity() const noexcept
{
    return mp_other->rows();
}

template<class ET> constexpr 
typename matrix_column_view<ET,mutable_vector_engine_tag>::size_type
matrix_column_view<ET,mutable_vector_engine_tag>::elements() const noexcept
{
    return mp_other->rows();
}

template<class ET> constexpr 
typename matrix_column_view<ET,mutable_vector_engine_tag>::iterator
matrix_column_view<ET,mutable_vector_engine_tag>::begin() const noexcept
{
    return iterator(this, 0, mp_other->rows());
}

template<class ET> constexpr 
typename matrix_column_view<ET,mutable_vector_engine_tag>::iterator
matrix_column_view<ET,mutable_vector_engine_tag>::end() const noexcept
{
    return iterator(this, mp_other->rows(), mp_other->rows());
}

template<class ET> constexpr 
typename matrix_column_view<ET,mutable_vector_engine_tag>::const_iterator
matrix_column_view<ET,mutable_vector_engine_tag>::cbegin() const noexcept
{
    return const_iterator(this, 0, mp_other->rows());
}

template<class ET> constexpr 
typename matrix_column_view<ET,mutable_vector_engine_tag>::const_iterator
matrix_column_view<ET,mutable_vector_engine_tag>::cend() const noexcept
{
    return const_iterator(this, mp_other->rows(), mp_other->rows());
}

template<class ET> constexpr 
void
matrix_column_view<ET,mutable_vector_engine_tag>::assign(matrix_column_view const& rhs)
{
    mp_other = rhs.mp_other;
    m_column = rhs.m_column;
}

template<class ET> constexpr 
void
matrix_column_view<ET,mutable_vector_engine_tag>::swap(matrix_column_view& rhs)
{
    std::swap(mp_other, rhs.mp_other);
    std::swap(m_column, rhs.m_column);
}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_COLUMN_VIEWS_HPP_DEFINED
