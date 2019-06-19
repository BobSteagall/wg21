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
    static_assert(detail::is_matrix_engine_v<ET>);

  public:
    using engine_type     = ET;
    using element_type    = typename engine_type::element_type;
    using reference       = typename engine_type::reference;
    using const_reference = typename engine_type::const_reference;
    using index_type      = typename engine_type::index_type;
    using size_type       = typename engine_type::size_type;
    using size_tuple      = typename engine_type::size_tuple;

    using column_type     = vector<matrix_column_view<engine_type>, OT>;
    using row_type        = vector<matrix_row_view<engine_type>, OT>;
    using transpose_type  = matrix<matrix_transpose_view<engine_type>, OT>;
    using hermitian_type  = conditional_t<is_complex_v<element_type>, matrix, transpose_type>;

    using is_fixed_size   = typename engine_type::is_fixed_size;
    using is_resizable    = typename engine_type::is_resizable;

    using is_column_major = typename engine_type::is_column_major;
    using is_dense        = typename engine_type::is_dense;
    using is_rectangular  = typename engine_type::is_rectangular;
    using is_row_major    = typename engine_type::is_row_major;

  public:
    ~matrix() = default;
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

    constexpr matrix&       operator =(matrix&&) noexcept = default;
    constexpr matrix&       operator =(matrix const&) = default;
    template<class ET2, class OT2>
    constexpr matrix&       operator =(matrix<ET2, OT2> const& rhs);

    //- Const element access.
    //
    constexpr const_reference   operator ()(index_type i, index_type j) const;

    //- Accessors.
    //
    constexpr index_type    columns() const noexcept;
    constexpr index_type    rows() const noexcept;
    constexpr size_tuple    size() const noexcept;

    constexpr size_type     column_capacity() const noexcept;
    constexpr size_type     row_capacity() const noexcept;
    constexpr size_tuple    capacity() const noexcept;

    //- Column view, row view, transpose view, and Hermitian.
    //
    constexpr column_type       column(index_type j) const noexcept;
    constexpr row_type          row(index_type i) const noexcept;
    constexpr transpose_type    t() const;
    constexpr hermitian_type    h() const;

    //- Mutable element access.
    //
    constexpr reference     operator ()(index_type i, index_type j);

    //- Assignment.
    //
    constexpr void      assign(matrix const& rhs);
    template<class ET2, class OT2>
    constexpr void      assign(matrix<ET2, OT2> const& rhs);

    //- Change capacity.
    //
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr void      reserve(size_tuple cap);
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr void      reserve(size_type rowcap, size_type colcap);

    //- Change size.
    //
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr void      resize(size_tuple size);
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr void      resize(size_type rows, size_type cols);

    //- Change size and capacity in one shot.
    //
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr void      resize(size_tuple size, size_tuple cap);
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr void      resize(size_type rows, size_type cols, size_type rowcap, size_type colcap);

    //- Swapping operations.
    //
    template<class ET2 = ET, detail::enable_if_mutable<ET, ET2> = true>
    constexpr void      swap(matrix& rhs) noexcept;
    template<class ET2 = ET, detail::enable_if_mutable<ET, ET2> = true>
    constexpr void      swap_columns(index_type i, index_type j) noexcept;
    template<class ET2 = ET, detail::enable_if_mutable<ET, ET2> = true>
    constexpr void      swap_rows(index_type i, index_type j) noexcept;

  private:
    template<class ET2, class OT2> friend class matrix;
    template<class ET2, class OT2> friend class vector;

  private:
    engine_type     m_engine;

  private:
    constexpr matrix(engine_type const& eng);
};

template<class ET, class OT>
template<class U, class ET2, detail::enable_if_fixed_size<ET, ET2>>
constexpr
matrix<ET,OT>::matrix(initializer_list<U> list)
:   m_engine(forward<initializer_list<U>>(list))
{}

template<class ET, class OT>
template<class ET2, class OT2>
constexpr
matrix<ET,OT>::matrix(matrix<ET2, OT2> const& rhs)
:   m_engine()
{
    assign(rhs);
}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>>
constexpr 
matrix<ET,OT>::matrix(size_tuple size)
:   m_engine(get<0>(size), get<1>(size))
{}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>>
constexpr 
matrix<ET,OT>::matrix(size_type rows, size_type cols)
:   m_engine(rows, cols)
{}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>>
constexpr 
matrix<ET,OT>::matrix(size_tuple size, size_tuple cap)
:   m_engine(get<0>(size), get<1>(size), get<0>(cap), get<1>(cap))
{}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>>
constexpr 
matrix<ET,OT>::matrix(size_type rows, size_type cols, size_type rowcap, size_type colcap)
:   m_engine(rows, cols, rowcap, colcap)
{}

template<class ET, class OT>
constexpr 
matrix<ET,OT>::matrix(engine_type const& eng)
:   m_engine(eng)
{}

template<class ET, class OT>
template<class ET2, class OT2>
constexpr matrix<ET,OT>&
matrix<ET,OT>::operator =(matrix<ET2, OT2> const& rhs)
{
    m_engine.assign(rhs.m_engine);
    return *this;
}

template<class ET, class OT> inline
constexpr typename matrix<ET,OT>::const_reference
matrix<ET,OT>::operator ()(index_type i, index_type j) const
{
    return m_engine(i, j);
}

template<class ET, class OT> inline
constexpr typename matrix<ET,OT>::index_type
matrix<ET,OT>::columns() const noexcept
{
    return m_engine.columns();
}

template<class ET, class OT> inline
constexpr typename matrix<ET,OT>::index_type
matrix<ET,OT>::rows() const noexcept
{
    return m_engine.rows();
}

template<class ET, class OT> inline
constexpr typename matrix<ET,OT>::size_tuple
matrix<ET,OT>::size() const noexcept
{
    return size_tuple(m_engine.rows(), m_engine.columns());
}

template<class ET, class OT> inline
constexpr typename matrix<ET,OT>::size_type
matrix<ET,OT>::column_capacity() const noexcept
{
    return m_engine.column_capacity();
}

template<class ET, class OT> inline
constexpr typename matrix<ET,OT>::size_type
matrix<ET,OT>::row_capacity() const noexcept
{
    return m_engine.row_capacity();
}

template<class ET, class OT> inline
constexpr typename matrix<ET,OT>::size_tuple
matrix<ET,OT>::capacity() const noexcept
{
    return size_tuple(m_engine.row_capacity(), m_engine.column_capacity());
}

template<class ET, class OT> inline
constexpr typename matrix<ET,OT>::column_type
matrix<ET,OT>::column(index_type j) const noexcept
{
    return column_type(m_engine, j, detail::row_column_tag());
}

template<class ET, class OT> inline
constexpr typename matrix<ET,OT>::row_type
matrix<ET,OT>::row(index_type i) const noexcept
{
    return row_type(m_engine, i, detail::row_column_tag());
}

template<class ET, class OT> inline
constexpr typename matrix<ET,OT>::transpose_type
matrix<ET,OT>::t() const
{
    return transpose_type(m_engine);
}

template<class ET, class OT> inline
constexpr typename matrix<ET,OT>::hermitian_type
matrix<ET,OT>::h() const
{
    if constexpr (is_complex_v<element_type>)
    {
        return hermitian_type();
    }
    else
    {
        return hermitian_type(m_engine);
    }
}

template<class ET, class OT> inline
constexpr typename matrix<ET,OT>::reference
matrix<ET,OT>::operator ()(index_type i, index_type j)
{
    return m_engine(i, j);
}

template<class ET, class OT> inline
constexpr void
matrix<ET,OT>::assign(matrix const& rhs)
{
    m_engine.assign(rhs.m_engine);
}

template<class ET, class OT>
template<class ET2, class OT2> inline
constexpr void
matrix<ET,OT>::assign(matrix<ET2, OT2> const& rhs)
{
    m_engine.assign(rhs.m_engine);
}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> inline
constexpr void
matrix<ET,OT>::reserve(size_tuple cap)
{
    m_engine.resize(get<0>(cap), get<1>(cap));
}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> inline
constexpr void
matrix<ET,OT>::reserve(size_type rowcap, size_type colcap)
{
    m_engine.reserve(rowcap, colcap);
}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> inline
constexpr void
matrix<ET,OT>::resize(size_tuple size)
{
    m_engine.resize(get<0>(size), get<1>(size));
}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> inline
constexpr void
matrix<ET,OT>::resize(size_type rows, size_type cols)
{
    m_engine.resize(rows, cols);
}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> inline
constexpr void
matrix<ET,OT>::resize(size_tuple size, size_tuple cap)
{
    m_engine.resize(get<0>(size), get<1>(size), get<0>(cap), get<1>(cap));
}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> inline
constexpr void
matrix<ET,OT>::resize(size_type rows, size_type cols, size_type rowcap, size_type colcap)
{
    m_engine.resize(rows, cols, rowcap, colcap);
}

template<class ET, class OT>
template<class ET2, detail::enable_if_mutable<ET, ET2>> inline
constexpr void
matrix<ET,OT>::swap(matrix& rhs) noexcept
{
    m_engine.swap(rhs.m_engine);
}

template<class ET, class OT>
template<class ET2, detail::enable_if_mutable<ET, ET2>> inline
constexpr void
matrix<ET,OT>::swap_columns(index_type c1, index_type c2) noexcept
{
    m_engine.swap_columns(c1, c2);
}

template<class ET, class OT>
template<class ET2, detail::enable_if_mutable<ET, ET2>> inline
constexpr void
matrix<ET,OT>::swap_rows(index_type r1, index_type r2) noexcept
{
    m_engine.swap_rows(r1, r2);
}

//--------
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
