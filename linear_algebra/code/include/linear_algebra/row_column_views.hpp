//==================================================================================================
//  File:       row_column_views.hpp
//
//  Summary:    This header defines engines that act as views of rows and columns.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_ROW_COLUMN_VIEWS_HPP_DEFINED
#define LINEAR_ALGEBRA_ROW_COLUMN_VIEWS_HPP_DEFINED

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
    using reference       = typename engine_type::const_reference;
    using pointer         = typename engine_type::const_pointer;
    using const_reference = typename engine_type::const_reference;
    using const_pointer   = typename engine_type::const_pointer;
    using iterator        = detail::vector_const_iterator<matrix_column_view>;
    using const_iterator  = detail::vector_const_iterator<matrix_column_view>;
    using difference_type = typename engine_type::difference_type;
    using index_type      = typename engine_type::index_type;
    using size_type       = typename engine_type::size_type;

    using is_fixed_size   = typename engine_type::is_fixed_size;
    using is_resizable    = false_type;

    using is_column_major = typename engine_type::is_row_major;
    using is_dense        = typename engine_type::is_dense;
    using is_rectangular  = typename engine_type::is_rectangular;
    using is_row_major    = typename engine_type::is_column_major;

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
    constexpr size_type         capacity() const noexcept;
    constexpr size_type         elements() const noexcept;

    constexpr void      assign(matrix_column_view const& rhs);

  private:
    engine_type const*  mp_other;
    index_type          m_column;
};

template<class ET> inline 
constexpr
matrix_column_view<ET>::matrix_column_view()
:   mp_other(nullptr)
,   m_column(0)
{}

template<class ET> inline 
constexpr
matrix_column_view<ET>::matrix_column_view(engine_type const& eng, index_type col)
:   mp_other(&eng)
,   m_column(col)
{}

template<class ET> inline 
constexpr typename matrix_column_view<ET>::const_reference
matrix_column_view<ET>::operator ()(index_type i) const
{
    return (*mp_other)(i, m_column);
}

template<class ET> inline 
constexpr typename matrix_column_view<ET>::size_type
matrix_column_view<ET>::capacity() const noexcept
{
    return mp_other->rows();
}

template<class ET> inline 
constexpr typename matrix_column_view<ET>::size_type
matrix_column_view<ET>::elements() const noexcept
{
    return mp_other->rows();
}

template<class ET> inline 
constexpr typename matrix_column_view<ET>::const_iterator
matrix_column_view<ET>::begin() const noexcept
{
    return const_iterator(this, 0, mp_other->rows());
}

template<class ET> inline 
constexpr typename matrix_column_view<ET>::const_iterator
matrix_column_view<ET>::end() const noexcept
{
    return const_iterator(this, mp_other->rows(), mp_other->rows());
}

template<class ET> inline 
constexpr void
matrix_column_view<ET>::assign(matrix_column_view const& rhs)
{
    mp_other = rhs.mp_other;
    m_column = rhs.m_column;
}


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
    using iterator        = detail::vector_const_iterator<matrix_row_view>;
    using const_iterator  = detail::vector_const_iterator<matrix_row_view>;
    using difference_type = typename engine_type::difference_type;
    using index_type      = typename engine_type::index_type;
    using size_type       = typename engine_type::size_type;

    using is_fixed_size   = typename engine_type::is_fixed_size;
    using is_resizable    = false_type;

    using is_column_major = typename engine_type::is_row_major;
    using is_dense        = typename engine_type::is_dense;
    using is_rectangular  = typename engine_type::is_rectangular;
    using is_row_major    = typename engine_type::is_column_major;

  public:
    constexpr matrix_row_view();
    constexpr matrix_row_view(engine_type const& eng, index_type row);
    constexpr matrix_row_view(matrix_row_view&&) noexcept = default;
    constexpr matrix_row_view(matrix_row_view const&) = default;

    constexpr matrix_row_view&  operator =(matrix_row_view&&) noexcept = default;
    constexpr matrix_row_view&  operator =(matrix_row_view const&) = default;

    constexpr const_reference   operator ()(index_type j) const;
    constexpr const_iterator    begin() const noexcept;
    constexpr const_iterator    end() const noexcept;
    constexpr size_type         capacity() const noexcept;
    constexpr size_type         elements() const noexcept;

    constexpr void      assign(matrix_row_view const& rhs);

  private:
    engine_type const*  mp_other;
    index_type          m_row;
};

template<class ET> inline 
constexpr
matrix_row_view<ET>::matrix_row_view()
:   mp_other(nullptr)
,   m_row(0)
{}

template<class ET> inline 
constexpr
matrix_row_view<ET>::matrix_row_view(engine_type const& eng, index_type row)
:   mp_other(&eng)
,   m_row(row)
{}

template<class ET> inline 
constexpr typename matrix_row_view<ET>::const_reference
matrix_row_view<ET>::operator ()(index_type j) const
{
    return (*mp_other)(m_row, j);
}

template<class ET> inline 
constexpr typename matrix_row_view<ET>::const_iterator
matrix_row_view<ET>::begin() const noexcept
{
    return const_iterator(this, 0, mp_other->columns());
}

template<class ET> inline 
constexpr typename matrix_row_view<ET>::const_iterator
matrix_row_view<ET>::end() const noexcept
{
    return const_iterator(this, mp_other->columns(), mp_other->columns());
}

template<class ET> inline 
constexpr typename matrix_row_view<ET>::size_type
matrix_row_view<ET>::capacity() const noexcept
{
    return mp_other->columns();
}

template<class ET> inline 
constexpr typename matrix_row_view<ET>::size_type
matrix_row_view<ET>::elements() const noexcept
{
    return mp_other->columns();
}

template<class ET> inline 
constexpr void
matrix_row_view<ET>::assign(matrix_row_view const& rhs)
{
    mp_other = rhs.mp_other;
    m_row    = rhs.m_row;
}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_ROW_COLUMN_VIEWS_HPP_DEFINED
