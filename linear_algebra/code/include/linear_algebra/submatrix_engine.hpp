//==================================================================================================
//  File:       submatrix_engine.hpp
//
//  Summary:    This header defines an engine that acts as a "view" of a subset of a matrix.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_SUBMATRIX_ENGINE_HPP_DEFINED
#define LINEAR_ALGEBRA_SUBMATRIX_ENGINE_HPP_DEFINED

namespace STD_LA {
//==================================================================================================
//  Sub-matrix engine, meant to act as a "view" of a portion of a matrix in expressions, in
//  order to help avoid unnecessary allocation and element copying.
//==================================================================================================
//
template<class ET, class MCT>
class submatrix_engine
{
    static_assert(is_matrix_engine_v<ET>);
    static_assert(is_matrix_engine_tag<MCT>);

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
    using size_type       = typename ET::size_type;
    using size_tuple      = typename ET::size_tuple;

    //- Construct/copy/destroy
    //
    ~submatrix_engine() noexcept = default;

    constexpr submatrix_engine();
    constexpr submatrix_engine(submatrix_engine&&) noexcept = default;
    constexpr submatrix_engine(submatrix_engine const&) = default;

    constexpr submatrix_engine&     operator =(submatrix_engine&&) noexcept = default;
    constexpr submatrix_engine&     operator =(submatrix_engine const&) = default;

    //- Capacity
    //
    constexpr size_type     columns() const noexcept;
    constexpr size_type     rows() const noexcept;
    constexpr size_tuple    size() const noexcept;

    constexpr size_type     column_capacity() const noexcept;
    constexpr size_type     row_capacity() const noexcept;
    constexpr size_tuple    capacity() const noexcept;

    //- Element access
    //
    constexpr reference     operator ()(size_type i, size_type j) const;

    //- Modifiers
    //
    constexpr void      swap(submatrix_engine& rhs);

  private:
    template<class ET2, class OT2>  friend class matrix;
    using referent_type = detail::noe_referent_t<ET, MCT>;

    referent_type*  mp_other;
    size_type       m_row_start;
    size_type       m_row_count;
    size_type       m_col_start;
    size_type       m_col_count;

    constexpr submatrix_engine(referent_type& eng, size_type ri, size_type rn,
                                                   size_type ci, size_type cn);
};

//------------------------
//- Construct/copy/destroy
//
template<class ET, class MCT> constexpr
submatrix_engine<ET, MCT>::submatrix_engine()
:   mp_other(nullptr)
,   m_row_start(0)
,   m_row_count(0)
,   m_col_start(0)
,   m_col_count(0)
{}

//----------
//- Capacity
//
template<class ET, class MCT> constexpr
typename submatrix_engine<ET, MCT>::size_type
submatrix_engine<ET, MCT>::columns() const noexcept
{
    return m_col_count;
}

template<class ET, class MCT> constexpr
typename submatrix_engine<ET, MCT>::size_type
submatrix_engine<ET, MCT>::rows() const noexcept
{
    return m_row_count;
}

template<class ET, class MCT> constexpr
typename submatrix_engine<ET, MCT>::size_tuple
submatrix_engine<ET, MCT>::size() const noexcept
{
    return size_tuple(m_row_count, m_col_count);
}

template<class ET, class MCT> constexpr
typename submatrix_engine<ET, MCT>::size_type
submatrix_engine<ET, MCT>::column_capacity() const noexcept
{
    return m_col_count;
}

template<class ET, class MCT> constexpr
typename submatrix_engine<ET, MCT>::size_type
submatrix_engine<ET, MCT>::row_capacity() const noexcept
{
    return m_row_count;
}

template<class ET, class MCT> constexpr
typename submatrix_engine<ET, MCT>::size_tuple
submatrix_engine<ET, MCT>::capacity() const noexcept
{
    return size_tuple(m_row_count, m_col_count);
}

//----------------
//- Element access
//
template<class ET, class MCT> constexpr
typename submatrix_engine<ET, MCT>::reference
submatrix_engine<ET, MCT>::operator ()(size_type i, size_type j) const
{
    return (*mp_other)(i + m_row_start, j + m_col_start);
}

//-----------
//- Modifiers
//
template<class ET, class MCT> constexpr
void
submatrix_engine<ET, MCT>::swap(submatrix_engine& rhs)
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
submatrix_engine<ET, MCT>::submatrix_engine
(referent_type& eng, size_type ri, size_type rn, size_type ci, size_type cn)
:   mp_other(&eng)
,   m_row_start(ri)
,   m_row_count(rn)
,   m_col_start(ci)
,   m_col_count(cn)
{}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_SUBMATRIX_ENGINE_HPP_DEFINED
