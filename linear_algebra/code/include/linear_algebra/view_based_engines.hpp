//==================================================================================================
//  File:       view_based_engines.hpp
//
//  Summary:    This header defines engines that act as const views of other engines.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_VIEW_BASED_ENGINES_HPP_DEFINED
#define LINEAR_ALGEBRA_VIEW_BASED_ENGINES_HPP_DEFINED

namespace STD_LA {
//==================================================================================================
//  Matrix transpose engine, meant to act as an rvalue-ish "view" in expressions, in order to
//  prevent unnecessary allocation and element copying.
//==================================================================================================
//
template<class ET>
class tr_matrix_engine
{
  public:
    using engine_type     = ET;
    using engine_category = const_matrix_engine_tag;
    using element_type    = typename engine_type::element_type;
    using index_type      = typename engine_type::index_type;
    using size_type       = typename engine_type::size_type;
    using size_tuple      = typename engine_type::size_tuple;

    using is_fixed_size   = typename engine_type::is_fixed_size;
    using is_resizable    = false_type;

    using is_column_major = typename engine_type::is_row_major;
    using is_dense        = typename engine_type::is_dense;
    using is_rectangular  = typename engine_type::is_rectangular;
    using is_row_major    = typename engine_type::is_column_major;

  public:
    constexpr tr_matrix_engine();
    constexpr tr_matrix_engine(engine_type const& eng);
    constexpr tr_matrix_engine(tr_matrix_engine&&) = default;
    constexpr tr_matrix_engine(tr_matrix_engine const&) = default;

    constexpr tr_matrix_engine&     operator =(tr_matrix_engine&&) = default;
    constexpr tr_matrix_engine&     operator =(tr_matrix_engine const&) = default;

    constexpr element_type          operator ()(index_type i, index_type j) const;
    constexpr element_type const*   data() const noexcept;

    constexpr size_type     columns() const noexcept;
    constexpr size_type     rows() const noexcept;
    constexpr size_tuple    size() const noexcept;

    constexpr size_type     column_capacity() const noexcept;
    constexpr size_type     row_capacity() const noexcept;
    constexpr size_tuple    capacity() const noexcept;

    constexpr void  assign(tr_matrix_engine const& rhs);

  private:
    engine_type* mp_other;  //- For exposition; pointer to actual engine
};

template<class ET> inline constexpr
tr_matrix_engine<ET>::tr_matrix_engine()
:   mp_other(nullptr)
{}

template<class ET> inline constexpr
tr_matrix_engine<ET>::tr_matrix_engine(engine_type const& eng)
:   mp_other(&eng)
{}

template<class ET> inline constexpr
typename tr_matrix_engine<ET>::element_type
tr_matrix_engine<ET>::operator ()(index_type i, index_type j) const
{
    return (*mp_other)(j, i);
}

template<class ET> inline
constexpr typename tr_matrix_engine<ET>::element_type const*
tr_matrix_engine<ET>::data() const noexcept
{
    return mp_other->data();
}

template<class ET> inline
constexpr typename tr_matrix_engine<ET>::size_type
tr_matrix_engine<ET>::columns() const noexcept
{
    return mp_other->rows();
}

template<class ET> inline
constexpr typename tr_matrix_engine<ET>::size_type
tr_matrix_engine<ET>::rows() const noexcept
{
    return mp_other->columns();
}

template<class ET> inline
constexpr typename tr_matrix_engine<ET>::size_tuple
tr_matrix_engine<ET>::size() const noexcept
{
    return size_tuple(mp_other->columns(), mp_other->rows());
}

template<class ET> inline
constexpr typename tr_matrix_engine<ET>::size_type
tr_matrix_engine<ET>::column_capacity() const noexcept
{
    return mp_other->row_capacity();
}

template<class ET> inline
constexpr typename tr_matrix_engine<ET>::size_type
tr_matrix_engine<ET>::row_capacity() const noexcept
{
    return mp_other->column_capacity();
}

template<class ET> inline
constexpr typename tr_matrix_engine<ET>::size_tuple
tr_matrix_engine<ET>::capacity() const noexcept
{
    return size_tuple(mp_other->column_capacity(), mp_other->row_capacity());
}

template<class ET> inline
constexpr void
tr_matrix_engine<ET>::assign(tr_matrix_engine const& rhs)
{
    mp_other == rhs.mp_other;
}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_VIEW_BASED_ENGINES_HPP_DEFINED
