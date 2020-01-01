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
template<class ET>
class matrix_row_view
{
    static_assert(detail::is_matrix_engine_v<ET>);

  public:
    using engine_type     = ET;
    using engine_category = const_vector_engine_tag;
    using element_type    = typename engine_type::element_type;
    using value_type      = typename engine_type::value_type;
    using reference       = typename engine_type::const_reference;
    using pointer         = typename engine_type::const_pointer;
    using const_reference = typename engine_type::const_reference;
    using const_pointer   = typename engine_type::const_pointer;
    using const_iterator  = detail::vector_const_iterator<matrix_row_view>;
    using iterator        = const_iterator;
    using difference_type = typename engine_type::difference_type;
    using size_type       = typename engine_type::size_type;

    static constexpr bool   is_fixed_size   = engine_type::is_fixed_size;
    static constexpr bool   is_resizable    = false;

    static constexpr bool   is_column_major = engine_type::is_column_major;
    static constexpr bool   is_dense        = engine_type::is_dense;
    static constexpr bool   is_rectangular  = engine_type::is_rectangular;
    static constexpr bool   is_row_major    = engine_type::is_row_major;

  public:
    constexpr matrix_row_view();
    constexpr matrix_row_view(engine_type const& eng, size_type row);
    constexpr matrix_row_view(matrix_row_view&&) noexcept = default;
    constexpr matrix_row_view(matrix_row_view const&) = default;

    constexpr matrix_row_view&  operator =(matrix_row_view&&) noexcept = default;
    constexpr matrix_row_view&  operator =(matrix_row_view const&) = default;

    constexpr const_reference   operator ()(size_type j) const;
    constexpr const_iterator    begin() const noexcept;
    constexpr const_iterator    end() const noexcept;
    constexpr size_type         capacity() const noexcept;
    constexpr size_type         elements() const noexcept;

    constexpr void      assign(matrix_row_view const& rhs);
    constexpr void      swap(matrix_row_view& rhs);

  private:
    engine_type const*  mp_other;
    size_type           m_row;
};

template<class ET> constexpr 
matrix_row_view<ET>::matrix_row_view()
:   mp_other(nullptr)
,   m_row(0)
{}

template<class ET> constexpr 
matrix_row_view<ET>::matrix_row_view(engine_type const& eng, size_type row)
:   mp_other(&eng)
,   m_row(row)
{}

template<class ET> constexpr 
typename matrix_row_view<ET>::const_reference
matrix_row_view<ET>::operator ()(size_type j) const
{
    return (*mp_other)(m_row, j);
}

template<class ET> constexpr 
typename matrix_row_view<ET>::const_iterator
matrix_row_view<ET>::begin() const noexcept
{
    return const_iterator(this, 0, mp_other->columns());
}

template<class ET> constexpr 
typename matrix_row_view<ET>::const_iterator
matrix_row_view<ET>::end() const noexcept
{
    return const_iterator(this, mp_other->columns(), mp_other->columns());
}

template<class ET> constexpr 
typename matrix_row_view<ET>::size_type
matrix_row_view<ET>::capacity() const noexcept
{
    return mp_other->columns();
}

template<class ET> constexpr 
typename matrix_row_view<ET>::size_type
matrix_row_view<ET>::elements() const noexcept
{
    return mp_other->columns();
}

template<class ET> constexpr 
void
matrix_row_view<ET>::assign(matrix_row_view const& rhs)
{
    mp_other = rhs.mp_other;
    m_row    = rhs.m_row;
}

template<class ET> constexpr 
void
matrix_row_view<ET>::swap(matrix_row_view& rhs)
{
    std::swap(mp_other, rhs.mp_other);
    std::swap(m_row, rhs.m_row);
}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_ROW_VIEWS_HPP_DEFINED
