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
class matrix_column_view
{
    static_assert(detail::is_matrix_engine_v<ET>);

  public:
    using engine_type     = ET;
    using engine_category = const_vector_engine_tag;
    using element_type    = typename engine_type::element_type;
    using value_type      = typename engine_type::value_type;
    using difference_type = typename engine_type::difference_type;
    using index_type      = typename engine_type::index_type;
    using const_reference = typename engine_type::const_reference;
    using reference       = typename engine_type::const_reference;
    using const_pointer   = typename engine_type::const_pointer;
    using pointer         = typename engine_type::const_pointer;
    using const_iterator  = detail::vector_const_iterator<matrix_column_view>;
    using iterator        = const_iterator;

    static constexpr bool   is_fixed_size   = engine_type::is_fixed_size;
    static constexpr bool   is_resizable    = false;

    static constexpr bool   is_column_major = engine_type::is_column_major;
    static constexpr bool   is_dense        = engine_type::is_dense;
    static constexpr bool   is_rectangular  = engine_type::is_rectangular;
    static constexpr bool   is_row_major    = engine_type::is_row_major;

  public:
    constexpr matrix_column_view();
    constexpr matrix_column_view(engine_type const& eng, index_type col);
    constexpr matrix_column_view(matrix_column_view&&) noexcept = default;
    constexpr matrix_column_view(matrix_column_view const&) = default;

    constexpr matrix_column_view&   operator =(matrix_column_view&&) noexcept = default;
    constexpr matrix_column_view&   operator =(matrix_column_view const&) = default;

    constexpr const_reference   operator ()(index_type i) const;
    constexpr const_iterator    begin() const noexcept;
    constexpr const_iterator    end() const noexcept;
    constexpr index_type        capacity() const noexcept;
    constexpr index_type        elements() const noexcept;

    constexpr void      assign(matrix_column_view const& rhs);
    constexpr void      swap(matrix_column_view& rhs);

  private:
    engine_type const*  mp_other;
    index_type          m_column;
};

template<class ET> constexpr 
matrix_column_view<ET>::matrix_column_view()
:   mp_other(nullptr)
,   m_column(0)
{}

template<class ET> constexpr 
matrix_column_view<ET>::matrix_column_view(engine_type const& eng, index_type col)
:   mp_other(&eng)
,   m_column(col)
{}

template<class ET> constexpr 
typename matrix_column_view<ET>::const_reference
matrix_column_view<ET>::operator ()(index_type i) const
{
    return (*mp_other)(i, m_column);
}

template<class ET> constexpr 
typename matrix_column_view<ET>::index_type
matrix_column_view<ET>::capacity() const noexcept
{
    return mp_other->rows();
}

template<class ET> constexpr 
typename matrix_column_view<ET>::index_type
matrix_column_view<ET>::elements() const noexcept
{
    return mp_other->rows();
}

template<class ET> constexpr 
typename matrix_column_view<ET>::const_iterator
matrix_column_view<ET>::begin() const noexcept
{
    return const_iterator(this, 0, mp_other->rows());
}

template<class ET> constexpr 
typename matrix_column_view<ET>::const_iterator
matrix_column_view<ET>::end() const noexcept
{
    return const_iterator(this, mp_other->rows(), mp_other->rows());
}

template<class ET> constexpr 
void
matrix_column_view<ET>::assign(matrix_column_view const& rhs)
{
    mp_other = rhs.mp_other;
    m_column = rhs.m_column;
}

template<class ET> constexpr 
void
matrix_column_view<ET>::swap(matrix_column_view& rhs)
{
    std::swap(mp_other, rhs.mp_other);
    std::swap(m_column, rhs.m_column);
}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_COLUMN_VIEWS_HPP_DEFINED
