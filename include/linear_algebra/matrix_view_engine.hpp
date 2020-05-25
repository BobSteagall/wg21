//==================================================================================================
//  File:       matrix_view_engine.hpp
//
//  Summary:    This header defines an engine that acts as a "view" of a subset of a matrix.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_SUBMATRIX_ENGINE_HPP_DEFINED
#define LINEAR_ALGEBRA_SUBMATRIX_ENGINE_HPP_DEFINED

namespace STD_LA {
#ifdef LA_NEGATION_AS_VIEW
//=================================================================================================
//  Matrix negation engine, meant to act as "view" of the negation of a matrix in expressions
//  so as to help avoid unnecessary allocation and/or element copying.
//==================================================================================================
//
template<class ET>
class matrix_view_engine<ET, readable_matrix_engine_tag, negation_view_tag>
{
    static_assert(is_matrix_engine_v<ET>);

  public:
    //- Types
    //
    using engine_category = readable_matrix_engine_tag;
    using element_type    = typename ET::element_type;
    using value_type      = typename ET::value_type;
    using pointer         = typename ET::const_pointer;
    using const_pointer   = typename ET::const_pointer;
    using reference       = typename ET::element_type;
    using const_reference = typename ET::element_type;
    using difference_type = typename ET::difference_type;
    using index_type      = typename ET::index_type;
    using index_tuple     = typename ET::index_tuple;
    using span_type       = detail::noe_mdspan_negation_t<detail::noe_const_mdspan_t<ET, readable_matrix_engine_tag>>;
    using const_span_type = detail::noe_mdspan_negation_t<detail::noe_const_mdspan_t<ET, readable_matrix_engine_tag>>;

    //- Construct/copy/destroy
    //
    ~matrix_view_engine() noexcept = default;

    constexpr matrix_view_engine() noexcept;
    constexpr matrix_view_engine(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine(matrix_view_engine const&) noexcept = default;

    constexpr matrix_view_engine&    operator =(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine&    operator =(matrix_view_engine const&) noexcept = default;

    //- Capacity
    //
    constexpr index_type    columns() const noexcept;
    constexpr index_type    rows() const noexcept;
    constexpr index_tuple   size() const noexcept;

    constexpr index_type    column_capacity() const noexcept;
    constexpr index_type    row_capacity() const noexcept;
    constexpr index_tuple   capacity() const noexcept;

    //- Element access
    //
    constexpr reference     operator ()(index_type i, index_type j) const;

    //- Data access
    //
    constexpr span_type     span() const noexcept;

    //- Modifiers
    //
    constexpr void      swap(matrix_view_engine& rhs) noexcept;

  private:
    template<class ET2, class OT2>  friend class matrix;

    using referent_type = detail::noe_referent_t<ET, readable_matrix_engine_tag>;

    referent_type*  mp_other;

    constexpr matrix_view_engine(referent_type& eng);
};

//------------------------
//- Construct/copy/destroy
//
template<class ET> constexpr
matrix_view_engine<ET, readable_matrix_engine_tag, negation_view_tag>::matrix_view_engine() noexcept
:   mp_other(nullptr)
{}

//----------
//- Capacity
//
template<class ET> constexpr
typename matrix_view_engine<ET, readable_matrix_engine_tag, negation_view_tag>::index_type
matrix_view_engine<ET, readable_matrix_engine_tag, negation_view_tag>::columns() const noexcept
{
    return mp_other->columns();
}

template<class ET> constexpr
typename matrix_view_engine<ET, readable_matrix_engine_tag, negation_view_tag>::index_type
matrix_view_engine<ET, readable_matrix_engine_tag, negation_view_tag>::rows() const noexcept
{
    return mp_other->rows();
}

template<class ET> constexpr
typename matrix_view_engine<ET, readable_matrix_engine_tag, negation_view_tag>::index_tuple
matrix_view_engine<ET, readable_matrix_engine_tag, negation_view_tag>::size() const noexcept
{
    return mp_other->size();
}

template<class ET> constexpr
typename matrix_view_engine<ET, readable_matrix_engine_tag, negation_view_tag>::index_type
matrix_view_engine<ET, readable_matrix_engine_tag, negation_view_tag>::column_capacity() const noexcept
{
    return mp_other->columns();
}

template<class ET> constexpr
typename matrix_view_engine<ET, readable_matrix_engine_tag, negation_view_tag>::index_type
matrix_view_engine<ET, readable_matrix_engine_tag, negation_view_tag>::row_capacity() const noexcept
{
    return mp_other->rows();
}

template<class ET> constexpr
typename matrix_view_engine<ET, readable_matrix_engine_tag, negation_view_tag>::index_tuple
matrix_view_engine<ET, readable_matrix_engine_tag, negation_view_tag>::capacity() const noexcept
{
    return mp_other->size();
}

//----------------
//- Element access
//
template<class ET> constexpr
typename matrix_view_engine<ET, readable_matrix_engine_tag, negation_view_tag>::reference
matrix_view_engine<ET, readable_matrix_engine_tag, negation_view_tag>::operator ()(index_type i, index_type j) const
{
    return -(*mp_other)(i, j);
}

//-------------
//- Data access
//
template<class ET> constexpr
typename matrix_view_engine<ET, readable_matrix_engine_tag, negation_view_tag>::span_type
matrix_view_engine<ET, readable_matrix_engine_tag, negation_view_tag>::span() const noexcept
{
    return detail::noe_mdspan_negation(mp_other->span());
}

//-----------
//- Modifiers
//
template<class ET> constexpr
void
matrix_view_engine<ET, readable_matrix_engine_tag, negation_view_tag>::swap(matrix_view_engine& rhs) noexcept
{
    std::swap(mp_other, rhs.mp_other);
}

//------------------------
//- Private implementation
//
template<class ET> constexpr
matrix_view_engine<ET, readable_matrix_engine_tag, negation_view_tag>::matrix_view_engine(referent_type& eng)
:   mp_other(&eng)
{}

#endif
//==================================================================================================
//  Sub-matrix engine, meant to act as a "view" of a subset of a matrix in expressions, so as
//  to help avoid unnecessary allocation and/or element copying.
//==================================================================================================
//
template<class ET, class MCT>
class matrix_view_engine<ET, MCT, subset_view_tag>
{
    static_assert(is_matrix_engine_v<ET>);
    static_assert(is_matrix_engine_tag_v<MCT>);

  public:
    //- Types
    //
    using engine_category = MCT;
    using element_type    = typename ET::element_type;
    using value_type      = typename ET::value_type;
    using pointer         = detail::noe_pointer_t<ET, MCT>;
    using const_pointer   = typename ET::const_pointer;
    using reference       = detail::noe_reference_t<ET, MCT>;
    using const_reference = typename ET::const_reference;
    using difference_type = typename ET::difference_type;
    using index_type      = typename ET::index_type;
    using index_tuple     = typename ET::index_tuple;
    using span_type       = detail::noe_mdspan_submatrix_t<detail::noe_mdspan_t<ET, MCT>>;
    using const_span_type = detail::noe_mdspan_submatrix_t<detail::noe_const_mdspan_t<ET, MCT>>;

    //- Construct/copy/destroy
    //
    ~matrix_view_engine() noexcept = default;

    constexpr matrix_view_engine();
    constexpr matrix_view_engine(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine(matrix_view_engine const&) noexcept = default;

    constexpr matrix_view_engine&     operator =(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine&     operator =(matrix_view_engine const&) noexcept = default;

    template<class ET2, detail::enable_if_convertible_engine<ET2, ET> = true>
    constexpr matrix_view_engine&     operator =(ET2 const& rhs);
    template<class U, detail::enable_if_writable<ET, ET> = true>
    constexpr matrix_view_engine&     operator =(initializer_list<initializer_list<U>> list);

    //- Capacity
    //
    constexpr index_type    columns() const noexcept;
    constexpr index_type    rows() const noexcept;
    constexpr index_tuple   size() const noexcept;

    constexpr index_type    column_capacity() const noexcept;
    constexpr index_type    row_capacity() const noexcept;
    constexpr index_tuple   capacity() const noexcept;

    //- Element access
    //
    constexpr reference     operator ()(index_type i, index_type j) const;

    //- Data access
    //
    constexpr span_type     span() const noexcept;

    //- Modifiers
    //
    constexpr void          swap(matrix_view_engine& rhs) noexcept;

  private:
    template<class ET2, class OT2>  friend class matrix;

    using referent_type = detail::noe_referent_t<ET, MCT>;

    referent_type*  mp_other;
    index_type      m_row_start;
    index_type      m_row_count;
    index_type      m_col_start;
    index_type      m_col_count;

    constexpr matrix_view_engine(referent_type& eng, index_type ri, index_type rn,
                                                     index_type ci, index_type cn);
};

//------------------------
//- Construct/copy/destroy
//
template<class ET, class MCT> constexpr
matrix_view_engine<ET, MCT, subset_view_tag>::matrix_view_engine()
:   mp_other(nullptr)
,   m_row_start(0)
,   m_row_count(0)
,   m_col_start(0)
,   m_col_count(0)
{}

template<class ET, class MCT>
template<class ET2, detail::enable_if_convertible_engine<ET2, ET>> constexpr
matrix_view_engine<ET, MCT, subset_view_tag>&
matrix_view_engine<ET, MCT, subset_view_tag>::operator =(ET2 const& rhs)
{
    detail::check_source_engine_size(rhs, rows(), columns());
    detail::assign_from_matrix_engine(*this, rhs);
    return *this;
}

template<class ET, class MCT>
template<class U, detail::enable_if_writable<ET, ET>> constexpr
matrix_view_engine<ET, MCT, subset_view_tag>&
matrix_view_engine<ET, MCT, subset_view_tag>::operator =(initializer_list<initializer_list<U>> rhs)
{
    detail::check_source_init_list(rhs, m_row_count, m_col_count);
    detail::assign_from_matrix_initlist(*this, rhs);
    return *this;
}

//----------
//- Capacity
//
template<class ET, class MCT> constexpr
typename matrix_view_engine<ET, MCT, subset_view_tag>::index_type
matrix_view_engine<ET, MCT, subset_view_tag>::columns() const noexcept
{
    return m_col_count;
}

template<class ET, class MCT> constexpr
typename matrix_view_engine<ET, MCT, subset_view_tag>::index_type
matrix_view_engine<ET, MCT, subset_view_tag>::rows() const noexcept
{
    return m_row_count;
}

template<class ET, class MCT> constexpr
typename matrix_view_engine<ET, MCT, subset_view_tag>::index_tuple
matrix_view_engine<ET, MCT, subset_view_tag>::size() const noexcept
{
    return index_tuple(m_row_count, m_col_count);
}

template<class ET, class MCT> constexpr
typename matrix_view_engine<ET, MCT, subset_view_tag>::index_type
matrix_view_engine<ET, MCT, subset_view_tag>::column_capacity() const noexcept
{
    return m_col_count;
}

template<class ET, class MCT> constexpr
typename matrix_view_engine<ET, MCT, subset_view_tag>::index_type
matrix_view_engine<ET, MCT, subset_view_tag>::row_capacity() const noexcept
{
    return m_row_count;
}

template<class ET, class MCT> constexpr
typename matrix_view_engine<ET, MCT, subset_view_tag>::index_tuple
matrix_view_engine<ET, MCT, subset_view_tag>::capacity() const noexcept
{
    return index_tuple(m_row_count, m_col_count);
}

//----------------
//- Element access
//
template<class ET, class MCT> constexpr
typename matrix_view_engine<ET, MCT, subset_view_tag>::reference
matrix_view_engine<ET, MCT, subset_view_tag>::operator ()(index_type i, index_type j) const
{
    return (*mp_other)(i + m_row_start, j + m_col_start);
}

//-------------
//- Data access
//
template<class ET, class MCT> constexpr
typename matrix_view_engine<ET, MCT, subset_view_tag>::span_type
matrix_view_engine<ET, MCT, subset_view_tag>::span() const noexcept
{
    return detail::noe_mdspan_submatrix(mp_other->span(), m_row_start, m_row_count,
                                                          m_col_start, m_col_count);
}

//-----------
//- Modifiers
//
template<class ET, class MCT> constexpr
void
matrix_view_engine<ET, MCT, subset_view_tag>::swap(matrix_view_engine& rhs) noexcept
{
    if (&rhs != this)
    {
        std::swap(mp_other, rhs.mp_other);
        std::swap(m_row_start, rhs.m_row_start);
        std::swap(m_row_count, rhs.m_row_count);
        std::swap(m_col_start, rhs.m_col_start);
        std::swap(m_col_count, rhs.m_col_count);
    }
}

template<class ET, class MCT> constexpr
matrix_view_engine<ET, MCT, subset_view_tag>::matrix_view_engine
(referent_type& eng, index_type ri, index_type rn, index_type ci, index_type cn)
:   mp_other(&eng)
,   m_row_start(ri)
,   m_row_count(rn)
,   m_col_start(ci)
,   m_col_count(cn)
{}


//==================================================================================================
//  Matrix transpose engine, meant to act as a "view" of a matrix transpose in expressions,
//  to help avoid unnecessary allocation and/or element copying.
//==================================================================================================
//
template<class ET, class MCT>
class matrix_view_engine<ET, MCT, transpose_view_tag>
{
    static_assert(is_matrix_engine_v<ET>);
    static_assert(is_matrix_engine_tag_v<MCT>);

  public:
    //- Types
    //
    using engine_category = MCT;
    using element_type    = typename ET::element_type;
    using value_type      = typename ET::value_type;
    using pointer         = detail::noe_pointer_t<ET, MCT>;
    using const_pointer   = typename ET::const_pointer;
    using reference       = detail::noe_reference_t<ET, MCT>;
    using const_reference = typename ET::const_reference;
    using difference_type = typename ET::difference_type;
    using index_type      = typename ET::index_type;
    using index_tuple     = extents<dynamic_extent, dynamic_extent>;
    using span_type       = detail::noe_mdspan_transpose_t<detail::noe_mdspan_t<ET, MCT>>;
    using const_span_type = detail::noe_mdspan_transpose_t<detail::noe_const_mdspan_t<ET, MCT>>;

    //- Construct/copy/destroy
    //
    ~matrix_view_engine() noexcept = default;

    constexpr matrix_view_engine();
    constexpr matrix_view_engine(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine(matrix_view_engine const&) = default;

    constexpr matrix_view_engine&     operator =(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine&     operator =(matrix_view_engine const&) = default;

    template<class ET2, detail::enable_if_convertible_engine<ET2, ET> = true>
    constexpr matrix_view_engine&     operator =(ET2 const& rhs);
    template<class U, detail::enable_if_writable<ET, ET> = true>
    constexpr matrix_view_engine&     operator =(initializer_list<initializer_list<U>> list);

    //- Capacity
    //
    constexpr index_type    columns() const noexcept;
    constexpr index_type    rows() const noexcept;
    constexpr index_tuple   size() const noexcept;

    constexpr index_type    column_capacity() const noexcept;
    constexpr index_type    row_capacity() const noexcept;
    constexpr index_tuple   capacity() const noexcept;

    //- Element access
    //
    constexpr reference     operator ()(index_type i, index_type j) const;

    //- Data access
    //
    constexpr span_type     span() const noexcept;

    //- Modifiers
    //
    constexpr void          swap(matrix_view_engine& rhs) noexcept;

  private:
    template<class ET2, class OT2>  friend class matrix;

    using referent_type = detail::noe_referent_t<ET, MCT>;

    referent_type*  mp_other;

    constexpr matrix_view_engine(referent_type& eng);
};

//------------------------
//- Construct/copy/destroy
//
template<class ET, class MCT> constexpr
matrix_view_engine<ET, MCT, transpose_view_tag>::matrix_view_engine()
:   mp_other(nullptr)
{}

template<class ET, class MCT>
template<class ET2, detail::enable_if_convertible_engine<ET2, ET>> constexpr
matrix_view_engine<ET, MCT, transpose_view_tag>&
matrix_view_engine<ET, MCT, transpose_view_tag>::operator =(ET2 const& rhs)
{
    detail::check_source_engine_size(rhs, rows(), columns());
    detail::assign_from_matrix_engine(*this, rhs);
    return *this;
}

template<class ET, class MCT>
template<class U, detail::enable_if_writable<ET, ET>> constexpr
matrix_view_engine<ET, MCT, transpose_view_tag>&
matrix_view_engine<ET, MCT, transpose_view_tag>::operator =(initializer_list<initializer_list<U>> rhs)
{
    detail::check_source_init_list(rhs, rows(), columns());
    detail::assign_from_matrix_initlist(*this, rhs);
    return *this;
}

//----------
//- Capacity
//
template<class ET, class MCT> constexpr
typename matrix_view_engine<ET, MCT, transpose_view_tag>::index_type
matrix_view_engine<ET, MCT, transpose_view_tag>::columns() const noexcept
{
    return mp_other->rows();
}

template<class ET, class MCT> constexpr
typename matrix_view_engine<ET, MCT, transpose_view_tag>::index_type
matrix_view_engine<ET, MCT, transpose_view_tag>::rows() const noexcept
{
    return mp_other->columns();
}

template<class ET, class MCT> constexpr
typename matrix_view_engine<ET, MCT, transpose_view_tag>::index_tuple
matrix_view_engine<ET, MCT, transpose_view_tag>::size() const noexcept
{
    return index_tuple(mp_other->columns(), mp_other->rows());
}

template<class ET, class MCT> constexpr
typename matrix_view_engine<ET, MCT, transpose_view_tag>::index_type
matrix_view_engine<ET, MCT, transpose_view_tag>::column_capacity() const noexcept
{
    return mp_other->rows();
}

template<class ET, class MCT> constexpr
typename matrix_view_engine<ET, MCT, transpose_view_tag>::index_type
matrix_view_engine<ET, MCT, transpose_view_tag>::row_capacity() const noexcept
{
    return mp_other->columns();
}

template<class ET, class MCT> constexpr
typename matrix_view_engine<ET, MCT, transpose_view_tag>::index_tuple
matrix_view_engine<ET, MCT, transpose_view_tag>::capacity() const noexcept
{
    return index_tuple(mp_other->columns(), mp_other->rows());
}

//----------------
//- Element access
//
template<class ET, class MCT> constexpr
typename matrix_view_engine<ET, MCT, transpose_view_tag>::reference
matrix_view_engine<ET, MCT, transpose_view_tag>::operator ()(index_type i, index_type j) const
{
    return (*mp_other)(j, i);
}

//-------------
//- Data access
//
template<class ET, class MCT> constexpr
typename matrix_view_engine<ET, MCT, transpose_view_tag>::span_type
matrix_view_engine<ET, MCT, transpose_view_tag>::span() const noexcept
{
    return detail::noe_mdspan_transpose(mp_other->span());
}

//-----------
//- Modifiers
//
template<class ET, class MCT> constexpr
void
matrix_view_engine<ET, MCT, transpose_view_tag>::swap(matrix_view_engine& rhs) noexcept
{
    if (&rhs != this)
    {
        std::swap(mp_other, rhs.mp_other);
    }
}

template<class ET, class MCT> constexpr
matrix_view_engine<ET, MCT, transpose_view_tag>::matrix_view_engine(referent_type& eng)
:   mp_other(&eng)
{}


//==================================================================================================
//  Matrix hermitian engine, meant to act as a "view" of a matrix conjugate transpose in
//  expressions, to help avoid unnecessary allocation and/or element copying.
//==================================================================================================
//
template<class ET>
class matrix_view_engine<ET, readable_matrix_engine_tag, hermitian_view_tag>
{
    static_assert(is_matrix_engine_v<ET>);

  public:
    //- Types
    //
    using engine_category = readable_matrix_engine_tag;
    using element_type    = typename ET::element_type;
    using value_type      = typename ET::value_type;
    using pointer         = detail::noe_pointer_t<ET, readable_matrix_engine_tag>;
    using const_pointer   = typename ET::const_pointer;
    using reference       = detail::noe_reference_t<ET, readable_matrix_engine_tag>;
    using const_reference = typename ET::const_reference;
    using difference_type = typename ET::difference_type;
    using index_type      = typename ET::index_type;
    using index_tuple     = extents<dynamic_extent, dynamic_extent>;
    using span_type       = detail::noe_mdspan_transpose_t<detail::noe_const_mdspan_t<ET, readable_matrix_engine_tag>>;
    using const_span_type = detail::noe_mdspan_transpose_t<detail::noe_const_mdspan_t<ET, readable_matrix_engine_tag>>;

    //- Construct/copy/destroy
    //
    ~matrix_view_engine() noexcept = default;

    constexpr matrix_view_engine();
    constexpr matrix_view_engine(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine(matrix_view_engine const&) = default;

    constexpr matrix_view_engine&     operator =(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine&     operator =(matrix_view_engine const&) = default;

    //- Capacity
    //
    constexpr index_type    columns() const noexcept;
    constexpr index_type    rows() const noexcept;
    constexpr index_tuple   size() const noexcept;

    constexpr index_type    column_capacity() const noexcept;
    constexpr index_type    row_capacity() const noexcept;
    constexpr index_tuple   capacity() const noexcept;

    //- Element access
    //
    constexpr reference     operator ()(index_type i, index_type j) const;

    //- Data access
    //
    constexpr span_type     span() const noexcept;

    //- Modifiers
    //
    constexpr void          swap(matrix_view_engine& rhs) noexcept;

  private:
    template<class ET2, class OT2>  friend class matrix;

    using referent_type = detail::noe_referent_t<ET, readable_matrix_engine_tag>;

    referent_type*  mp_other;

    constexpr matrix_view_engine(referent_type& eng);
};

//------------------------
//- Construct/copy/destroy
//
template<class ET> constexpr
matrix_view_engine<ET, readable_matrix_engine_tag, hermitian_view_tag>::matrix_view_engine()
:   mp_other(nullptr)
{}

//----------
//- Capacity
//
template<class ET> constexpr
typename matrix_view_engine<ET, readable_matrix_engine_tag, hermitian_view_tag>::index_type
matrix_view_engine<ET, readable_matrix_engine_tag, hermitian_view_tag>::columns() const noexcept
{
    return mp_other->rows();
}

template<class ET> constexpr
typename matrix_view_engine<ET, readable_matrix_engine_tag, hermitian_view_tag>::index_type
matrix_view_engine<ET, readable_matrix_engine_tag, hermitian_view_tag>::rows() const noexcept
{
    return mp_other->columns();
}

template<class ET> constexpr
typename matrix_view_engine<ET, readable_matrix_engine_tag, hermitian_view_tag>::index_tuple
matrix_view_engine<ET, readable_matrix_engine_tag, hermitian_view_tag>::size() const noexcept
{
    return index_tuple(mp_other->columns(), mp_other->rows());
}

template<class ET> constexpr
typename matrix_view_engine<ET, readable_matrix_engine_tag, hermitian_view_tag>::index_type
matrix_view_engine<ET, readable_matrix_engine_tag, hermitian_view_tag>::column_capacity() const noexcept
{
    return mp_other->rows();
}

template<class ET> constexpr
typename matrix_view_engine<ET, readable_matrix_engine_tag, hermitian_view_tag>::index_type
matrix_view_engine<ET, readable_matrix_engine_tag, hermitian_view_tag>::row_capacity() const noexcept
{
    return mp_other->columns();
}

template<class ET> constexpr
typename matrix_view_engine<ET, readable_matrix_engine_tag, hermitian_view_tag>::index_tuple
matrix_view_engine<ET, readable_matrix_engine_tag, hermitian_view_tag>::capacity() const noexcept
{
    return index_tuple(mp_other->columns(), mp_other->rows());
}

//----------------
//- Element access
//
template<class ET> constexpr
typename matrix_view_engine<ET, readable_matrix_engine_tag, hermitian_view_tag>::reference
matrix_view_engine<ET, readable_matrix_engine_tag, hermitian_view_tag>::operator ()(index_type i, index_type j) const
{
    return (*mp_other)(j, i);
}

//-------------
//- Data access
//
template<class ET> constexpr
typename matrix_view_engine<ET, readable_matrix_engine_tag, hermitian_view_tag>::span_type
matrix_view_engine<ET, readable_matrix_engine_tag, hermitian_view_tag>::span() const noexcept
{
    return detail::noe_mdspan_transpose(mp_other->span());
}

//-----------
//- Modifiers
//
template<class ET> constexpr
void
matrix_view_engine<ET, readable_matrix_engine_tag, hermitian_view_tag>::swap(matrix_view_engine& rhs) noexcept
{
    if (&rhs != this)
    {
        std::swap(mp_other, rhs.mp_other);
    }
}

template<class ET> constexpr
matrix_view_engine<ET, readable_matrix_engine_tag, hermitian_view_tag>::matrix_view_engine(referent_type& eng)
:   mp_other(&eng)
{}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_SUBMATRIX_ENGINE_HPP_DEFINED
