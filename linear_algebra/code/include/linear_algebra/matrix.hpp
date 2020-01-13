//==================================================================================================
//  File:       matrix.hpp
//
//  Summary:    This header defines the matrix class template, one of the two primary math
//              objects provided by the library.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_MATRIX_HPP_DEFINED
#define LINEAR_ALGEBRA_MATRIX_HPP_DEFINED

#include <linear_algebra.hpp>

namespace STD_LA {
//==================================================================================================
//  A matrix parametrized by an engine type and operator traits.
//==================================================================================================
//
template<class ET, class OT>
class matrix
{
    static_assert(is_matrix_engine_v<ET>);

    using possibly_writable_vector_tag = detail::noe_category_t<ET, writable_vector_engine_tag>;
    using possibly_writable_matrix_tag = detail::noe_category_t<ET, writable_matrix_engine_tag>;

    static constexpr bool   has_cx_elem = detail::is_complex_v<typename ET::value_type>;

  public:
    //- Types
    //
    using engine_type     = ET;
    using element_type    = typename engine_type::element_type;
    using value_type      = typename engine_type::value_type;
    using reference       = typename engine_type::reference;
    using const_reference = typename engine_type::const_reference;
    using difference_type = typename engine_type::difference_type;
    using size_type       = typename engine_type::size_type;
    using size_tuple      = typename engine_type::size_tuple;

    using column_type       = vector<column_engine<engine_type, possibly_writable_vector_tag>, OT>;
    using const_column_type = vector<column_engine<engine_type, readable_vector_engine_tag>, OT>;
    using row_type          = vector<row_engine<engine_type, possibly_writable_vector_tag>, OT>;
    using const_row_type    = vector<row_engine<engine_type, readable_vector_engine_tag>, OT>;

    using submatrix_type       = matrix<submatrix_engine<engine_type, possibly_writable_matrix_tag>, OT>;
    using const_submatrix_type = matrix<submatrix_engine<engine_type, readable_matrix_engine_tag>, OT>;
    using transpose_type       = matrix<transpose_engine<engine_type, possibly_writable_matrix_tag>, OT>;
    using const_transpose_type = matrix<transpose_engine<engine_type, readable_matrix_engine_tag>, OT>;
    using hermitian_type       = conditional_t<has_cx_elem, matrix, transpose_type>;
    using const_hermitian_type = conditional_t<has_cx_elem, matrix, const_transpose_type>;

    //- Construct/copy/destroy
    //
    ~matrix() noexcept = default;

    constexpr matrix() = default;
    constexpr matrix(matrix&&) noexcept = default;
    constexpr matrix(matrix const&) = default;

    template<class U, class ET2 = ET, detail::enable_if_fixed_size<ET, ET2> = true>
    constexpr matrix(initializer_list<U> list);
    template<class ET2, class OT2>
    constexpr matrix(matrix<ET2, OT2> const& src);
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr matrix(size_tuple size);
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr matrix(size_type rows, size_type cols);

    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr matrix(size_tuple size, size_tuple cap);
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr matrix(size_type rows, size_type cols, size_type rowcap, size_type colcap);

    constexpr matrix&   operator =(matrix&&) noexcept = default;
    constexpr matrix&   operator =(matrix const&) = default;
    template<class ET2, class OT2>
    constexpr matrix&   operator =(matrix<ET2, OT2> const& rhs);

    //- Capacity
    //
    constexpr size_type     columns() const noexcept;
    constexpr size_type     rows() const noexcept;
    constexpr size_tuple    size() const noexcept;

    constexpr size_type     column_capacity() const noexcept;
    constexpr size_type     row_capacity() const noexcept;
    constexpr size_tuple    capacity() const noexcept;

    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr void      reserve(size_tuple cap);
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr void      reserve(size_type rowcap, size_type colcap);

    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr void      resize(size_tuple size);
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr void      resize(size_type rows, size_type cols);

    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr void      resize(size_tuple size, size_tuple cap);
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr void      resize(size_type rows, size_type cols, size_type rowcap, size_type colcap);

    //- Element access
    //
    constexpr reference             operator ()(size_type i, size_type j);
    constexpr const_reference       operator ()(size_type i, size_type j) const;

    //- Columns, rows, submatrices, transposes, and the Hermitian
    //
    constexpr column_type           column(size_type j) noexcept;
    constexpr const_column_type     column(size_type j) const noexcept;
    constexpr row_type              row(size_type i) noexcept;
    constexpr const_row_type        row(size_type i) const noexcept;
    constexpr submatrix_type        submatrix(size_type ri, size_type rn, size_type ci, size_type cn) noexcept;
    constexpr const_submatrix_type  submatrix(size_type ri, size_type rn, size_type ci, size_type cn) const noexcept;
    constexpr transpose_type        t() noexcept;
    constexpr const_transpose_type  t() const noexcept;
    constexpr hermitian_type        h();
    constexpr const_hermitian_type  h() const;

    //- Data access
    //
    constexpr engine_type&          engine() noexcept;
    constexpr engine_type const&    engine() const noexcept;

    //- Modifiers
    //
    template<class ET2 = ET, detail::enable_if_writable<ET, ET2> = true>
    constexpr void      swap(matrix& rhs) noexcept;
    template<class ET2 = ET, detail::enable_if_writable<ET, ET2> = true>
    constexpr void      swap_columns(size_type i, size_type j) noexcept;
    template<class ET2 = ET, detail::enable_if_writable<ET, ET2> = true>
    constexpr void      swap_rows(size_type i, size_type j) noexcept;

  private:
    template<class ET2, class OT2> friend class matrix;
    template<class ET2, class OT2> friend class vector;

  private:
    engine_type     m_engine;

  private:
    constexpr matrix(engine_type const& eng);
};

//------------------------
//- Construct/copy/destroy
//
template<class ET, class OT>
template<class U, class ET2, detail::enable_if_fixed_size<ET, ET2>> constexpr
matrix<ET,OT>::matrix(initializer_list<U> list)
:   m_engine(forward<initializer_list<U>>(list))
{}

template<class ET, class OT>
template<class ET2, class OT2> constexpr
matrix<ET,OT>::matrix(matrix<ET2, OT2> const& rhs)
:   m_engine()
{
    assign(rhs);
}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> constexpr 
matrix<ET,OT>::matrix(size_tuple size)
:   m_engine(get<0>(size), get<1>(size))
{}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> constexpr 
matrix<ET,OT>::matrix(size_type rows, size_type cols)
:   m_engine(rows, cols)
{}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> constexpr 
matrix<ET,OT>::matrix(size_tuple size, size_tuple cap)
:   m_engine(get<0>(size), get<1>(size), get<0>(cap), get<1>(cap))
{}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> constexpr 
matrix<ET,OT>::matrix(size_type rows, size_type cols, size_type rowcap, size_type colcap)
:   m_engine(rows, cols, rowcap, colcap)
{}

template<class ET, class OT> constexpr 
matrix<ET,OT>::matrix(engine_type const& eng)
:   m_engine(eng)
{}

template<class ET, class OT>
template<class ET2, class OT2> constexpr 
matrix<ET,OT>&
matrix<ET,OT>::operator =(matrix<ET2, OT2> const& rhs)
{
    m_engine = rhs.m_engine;
    return *this;
}

//----------
//- Capacity
//
template<class ET, class OT> inline constexpr 
typename matrix<ET,OT>::size_type
matrix<ET,OT>::columns() const noexcept
{
    return m_engine.columns();
}

template<class ET, class OT> inline constexpr 
typename matrix<ET,OT>::size_type
matrix<ET,OT>::rows() const noexcept
{
    return m_engine.rows();
}

template<class ET, class OT> inline constexpr 
typename matrix<ET,OT>::size_tuple
matrix<ET,OT>::size() const noexcept
{
    return size_tuple(m_engine.rows(), m_engine.columns());
}

template<class ET, class OT> inline constexpr 
typename matrix<ET,OT>::size_type
matrix<ET,OT>::column_capacity() const noexcept
{
    return m_engine.column_capacity();
}

template<class ET, class OT> inline constexpr 
typename matrix<ET,OT>::size_type
matrix<ET,OT>::row_capacity() const noexcept
{
    return m_engine.row_capacity();
}

template<class ET, class OT> inline constexpr 
typename matrix<ET,OT>::size_tuple
matrix<ET,OT>::capacity() const noexcept
{
    return size_tuple(m_engine.row_capacity(), m_engine.column_capacity());
}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> inline constexpr 
void
matrix<ET,OT>::reserve(size_tuple cap)
{
    m_engine.resize(get<0>(cap), get<1>(cap));
}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> inline constexpr 
void
matrix<ET,OT>::reserve(size_type rowcap, size_type colcap)
{
    m_engine.reserve(rowcap, colcap);
}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> inline constexpr 
void
matrix<ET,OT>::resize(size_tuple size)
{
    m_engine.resize(get<0>(size), get<1>(size));
}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> inline constexpr 
void
matrix<ET,OT>::resize(size_type rows, size_type cols)
{
    m_engine.resize(rows, cols);
}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> inline constexpr 
void
matrix<ET,OT>::resize(size_tuple size, size_tuple cap)
{
    m_engine.resize(get<0>(size), get<1>(size), get<0>(cap), get<1>(cap));
}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> inline constexpr 
void
matrix<ET,OT>::resize(size_type rows, size_type cols, size_type rowcap, size_type colcap)
{
    m_engine.resize(rows, cols, rowcap, colcap);
}

//----------------
//- Element access
//
template<class ET, class OT> inline constexpr 
typename matrix<ET,OT>::reference
matrix<ET,OT>::operator ()(size_type i, size_type j)
{
    return m_engine(i, j);
}

template<class ET, class OT> inline constexpr 
typename matrix<ET,OT>::const_reference
matrix<ET,OT>::operator ()(size_type i, size_type j) const
{
    return m_engine(i, j);
}

template<class ET, class OT> inline constexpr 
typename matrix<ET,OT>::const_column_type
matrix<ET,OT>::column(size_type j) const noexcept
{
    return const_column_type(m_engine, j, detail::row_or_column_tag());
}

template<class ET, class OT> inline constexpr 
typename matrix<ET,OT>::column_type
matrix<ET,OT>::column(size_type j) noexcept
{
    return column_type(m_engine, j, detail::row_or_column_tag());
}

template<class ET, class OT> inline constexpr 
typename matrix<ET,OT>::row_type
matrix<ET,OT>::row(size_type i) noexcept
{
    return row_type(m_engine, i, detail::row_or_column_tag());
}

template<class ET, class OT> inline constexpr 
typename matrix<ET,OT>::const_row_type
matrix<ET,OT>::row(size_type i) const noexcept
{
    return const_row_type(m_engine, i, detail::row_or_column_tag());
}

template<class ET, class OT> inline constexpr 
typename matrix<ET,OT>::transpose_type
matrix<ET,OT>::t() noexcept
{
    return transpose_type(m_engine);
}

template<class ET, class OT> inline constexpr 
typename matrix<ET,OT>::const_transpose_type
matrix<ET,OT>::t() const noexcept
{
    return const_transpose_type(m_engine);
}

template<class ET, class OT> inline constexpr 
typename matrix<ET,OT>::hermitian_type
matrix<ET,OT>::h() 
{
    if constexpr (detail::is_complex_v<element_type>)
    {
        return hermitian_type();
    }
    else
    {
        return hermitian_type(m_engine);
    }
}

template<class ET, class OT> inline constexpr 
typename matrix<ET,OT>::const_hermitian_type
matrix<ET,OT>::h() const
{
    if constexpr (detail::is_complex_v<element_type>)
    {
        return const_hermitian_type();
    }
    else
    {
        return const_hermitian_type(m_engine);
    }
}

//-------------
//- Data access
//
template<class ET, class OT> constexpr 
typename matrix<ET,OT>::engine_type&
matrix<ET,OT>::engine() noexcept
{
    return m_engine;
}

template<class ET, class OT> constexpr 
typename matrix<ET,OT>::engine_type const&
matrix<ET,OT>::engine() const noexcept
{
    return m_engine;
}

//-----------
//- Modifiers
//
template<class ET, class OT>
template<class ET2, detail::enable_if_writable<ET, ET2>> inline constexpr 
void
matrix<ET,OT>::swap(matrix& rhs) noexcept
{
    m_engine.swap(rhs.m_engine);
}

template<class ET, class OT>
template<class ET2, detail::enable_if_writable<ET, ET2>> inline constexpr 
void
matrix<ET,OT>::swap_columns(size_type c1, size_type c2) noexcept
{
    m_engine.swap_columns(c1, c2);
}

template<class ET, class OT>
template<class ET2, detail::enable_if_writable<ET, ET2>> inline constexpr 
void
matrix<ET,OT>::swap_rows(size_type r1, size_type r2) noexcept
{
    m_engine.swap_rows(r1, r2);
}

//------------
//- Comparison
//
template<class ET1, class OT1, class ET2, class OT2> 
constexpr bool
operator ==(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2)
{
    if (m1.size() != m2.size()) return false;

    for (int i = 0;  i < m1.rows();  ++i)
    {
        for (int j = 0;  j < m2.columns();  ++j)
        {
            if (m1(i, j) != m2(i, j)) return false;
        }
    }
    return true;
}

template<class ET1, class OT1, class ET2, class OT2> 
constexpr bool
operator !=(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2)
{
    return !(m1 == m2);
}


}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_MATRIX_HPP_DEFINED
