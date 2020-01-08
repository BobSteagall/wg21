//==================================================================================================
//  File:       transpose_views.hpp
//
//  Summary:    This header defines engines that act as const views of other engines.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_TRANSPOSE_VIEWS_HPP_DEFINED
#define LINEAR_ALGEBRA_TRANSPOSE_VIEWS_HPP_DEFINED

namespace STD_LA {
//==================================================================================================
//  Matrix transpose engine, meant to act as an rvalue-ish "view" in expressions, in order to
//  prevent unnecessary allocation and element copying.
//==================================================================================================
//
template<class ET>
class matrix_transpose_view
{
    static_assert(detail::is_matrix_engine_v<ET>);

  public:
    using engine_type     = ET;
    using engine_category = const_matrix_engine_tag;
    using element_type    = typename engine_type::element_type;
    using value_type      = typename engine_type::value_type;
    using difference_type = typename engine_type::difference_type;
    using size_type       = typename engine_type::size_type;
    using size_tuple      = typename engine_type::size_tuple;
    using const_reference = typename engine_type::const_reference;
    using reference       = typename engine_type::const_reference;
    using const_pointer   = typename engine_type::const_pointer;
    using pointer         = typename engine_type::const_pointer;

    using column_view_type    = matrix_column_view<matrix_transpose_view>;
    using row_view_type       = matrix_row_view<matrix_transpose_view>;
    using transpose_view_type = matrix_transpose_view<matrix_transpose_view>;

  public:
    constexpr matrix_transpose_view();
    constexpr matrix_transpose_view(engine_type const& eng);
    constexpr matrix_transpose_view(matrix_transpose_view&&) noexcept = default;
    constexpr matrix_transpose_view(matrix_transpose_view const&) = default;

    constexpr matrix_transpose_view&    operator =(matrix_transpose_view&&) noexcept = default;
    constexpr matrix_transpose_view&    operator =(matrix_transpose_view const&) = default;

    constexpr const_reference   operator ()(size_type i, size_type j) const;

    constexpr size_type     columns() const noexcept;
    constexpr size_type     rows() const noexcept;
    constexpr size_tuple    size() const noexcept;

    constexpr size_type     column_capacity() const noexcept;
    constexpr size_type     row_capacity() const noexcept;
    constexpr size_tuple    capacity() const noexcept;

    constexpr void      assign(matrix_transpose_view const& rhs);
    constexpr void      swap(matrix_transpose_view& rhs);

  private:
    engine_type const*  mp_other;
};

template<class ET> constexpr 
matrix_transpose_view<ET>::matrix_transpose_view()
:   mp_other(nullptr)
{}

template<class ET> constexpr
matrix_transpose_view<ET>::matrix_transpose_view(engine_type const& eng)
:   mp_other(&eng)
{}

template<class ET> constexpr 
typename matrix_transpose_view<ET>::const_reference
matrix_transpose_view<ET>::operator ()(size_type i, size_type j) const
{
    return (*mp_other)(j, i);
}

template<class ET> constexpr 
typename matrix_transpose_view<ET>::size_type
matrix_transpose_view<ET>::columns() const noexcept
{
    return mp_other->rows();
}

template<class ET> constexpr 
typename matrix_transpose_view<ET>::size_type
matrix_transpose_view<ET>::rows() const noexcept
{
    return mp_other->columns();
}

template<class ET> constexpr 
typename matrix_transpose_view<ET>::size_tuple
matrix_transpose_view<ET>::size() const noexcept
{
    return size_tuple(mp_other->columns(), mp_other->rows());
}

template<class ET> constexpr 
typename matrix_transpose_view<ET>::size_type
matrix_transpose_view<ET>::column_capacity() const noexcept
{
    return mp_other->row_capacity();
}

template<class ET> constexpr 
typename matrix_transpose_view<ET>::size_type
matrix_transpose_view<ET>::row_capacity() const noexcept
{
    return mp_other->column_capacity();
}

template<class ET> constexpr 
typename matrix_transpose_view<ET>::size_tuple
matrix_transpose_view<ET>::capacity() const noexcept
{
    return size_tuple(mp_other->column_capacity(), mp_other->row_capacity());
}

template<class ET> constexpr 
void
matrix_transpose_view<ET>::assign(matrix_transpose_view const& rhs)
{
    mp_other == rhs.mp_other;
}

template<class ET> constexpr 
void
matrix_transpose_view<ET>::swap(matrix_transpose_view& rhs)
{
    std::swap(mp_other, rhs.mp_other);
}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_TRANSPOSE_VIEWS_HPP_DEFINED
