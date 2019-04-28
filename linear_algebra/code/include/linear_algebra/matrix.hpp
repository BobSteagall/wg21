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
//=================================================================================================
//  A matrix parametrized by an engine type and operator traits.
//=================================================================================================
//
template<class ET, class OT>
class matrix
{
  public:
    using engine_type     = ET;
    using element_type    = typename engine_type::element_type;
    using reference       = typename engine_type::reference;
    using pointer         = typename engine_type::pointer;
    using const_reference = typename engine_type::const_reference;
    using const_pointer   = typename engine_type::const_pointer;
    using index_type      = typename engine_type::index_type;
    using size_type       = typename engine_type::size_type;
    using size_tuple      = typename engine_type::size_tuple;

    using column_type     = vector<matrix_column_view<engine_type>, OT>;
    using row_type        = vector<matrix_row_view<engine_type>, OT>;
    using transpose_type  = matrix<matrix_transpose_view<engine_type>, OT>;
    using hermitian_type  = conditional_t<is_complex_v<element_type>, matrix, transpose_type>;

    using is_column_major = typename engine_type::is_column_major;
    using is_dense        = typename engine_type::is_dense;
    using is_rectangular  = typename engine_type::is_rectangular;
    using is_row_major    = typename engine_type::is_row_major;

    static_assert(is_matrix_element_v<element_type>);

  public:
    ~matrix() = default;
    matrix() = default;
    matrix(matrix&&) noexcept = default;
    matrix(matrix const&) = default;

    template<class ET2, class OT2>
    matrix(matrix<ET2, OT2> const& src);
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    matrix(size_tuple size);
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    matrix(size_type rows, size_type cols);

    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    matrix(size_tuple size, size_tuple cap);
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    matrix(size_type rows, size_type cols, size_type rowcap, size_type colcap);

    matrix&     operator =(matrix&&) noexcept = default;
    matrix&     operator =(matrix const&) = default;
    template<class ET2, class OT2>
    matrix&     operator =(matrix<ET2, OT2> const& rhs);

    //- Const element access.
    //
    const_reference     operator ()(index_type i, index_type j) const;

    //- Accessors.
    //
    size_type   columns() const noexcept;
    size_type   rows() const noexcept;
    constexpr size_tuple    size() const noexcept;

    size_type   column_capacity() const noexcept;
    size_type   row_capacity() const noexcept;
    size_tuple  capacity() const noexcept;

    //- Column view, row view, transpose view, and Hermitian.
    //
    column_type     column(index_type j) const;
    row_type        row(index_type i) const;
    transpose_type  t() const;
    hermitian_type  h() const;

    //- Mutable element access.
    //
    reference   operator ()(index_type i, index_type j);

    //- Assignment.
    //
    void    assign(matrix&& rhs);
    void    assign(matrix const& rhs);
    template<class ET2, class OT2>
    constexpr void  assign(matrix<ET2, OT2> const& rhs);

    //- Change capacity.
    //
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    void    reserve(size_tuple cap);
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    void    reserve(size_type rowcap, size_type colcap);

    //- Change size.
    //
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    void    resize(size_tuple size);
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    void    resize(size_type rows, size_type cols);

    //- Change size and capacity in one shot.
    //
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    void    resize(size_tuple size, size_tuple cap);
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    void    resize(size_type rows, size_type cols, size_type rowcap, size_type colcap);

    //- Swapping operations.
    //

    template<class ET2 = ET, detail::enable_if_mutable<ET, ET2> = true>
    void    swap(matrix& rhs);
    template<class ET2 = ET, detail::enable_if_mutable<ET, ET2> = true>
    void    swap_columns(index_type i, index_type j);
    template<class ET2 = ET, detail::enable_if_mutable<ET, ET2> = true>
    void    swap_rows(index_type i, index_type j);

  private:
    template<class ET2, class OT2> friend class matrix;

  private:
    engine_type     m_engine;

  private:
    matrix(engine_type const& eng);

  public:
    template<class ET2, class OT2>
    constexpr void    copy_elements(matrix<ET2, OT2> const& rhs);
};

template<class ET, class OT>
template<class ET2, class OT2>
matrix<ET,OT>::matrix(matrix<ET2, OT2> const& rhs)
:   m_engine()
{
    assign(rhs);
}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>>
matrix<ET,OT>::matrix(size_tuple size)
:   m_engine(get<0>(size), get<1>(size))
{}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>>
matrix<ET,OT>::matrix(size_type rows, size_type cols)
:   m_engine(rows, cols)
{}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>>
matrix<ET,OT>::matrix(size_tuple size, size_tuple cap)
:   m_engine(get<0>(size), get<1>(size), get<0>(cap), get<1>(cap))
{}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>>
matrix<ET,OT>::matrix(size_type rows, size_type cols, size_type rowcap, size_type colcap)
:   m_engine(rows, cols, rowcap, colcap)
{}

template<class ET, class OT>
matrix<ET,OT>::matrix(engine_type const& eng)
:   m_engine(eng)
{}

template<class ET, class OT>
template<class ET2, class OT2>
matrix<ET,OT>&
matrix<ET,OT>::operator =(matrix<ET2, OT2> const& rhs)
{
    assign(rhs);
    return *this;
}

template<class ET, class OT> inline
typename matrix<ET,OT>::const_reference
matrix<ET,OT>::operator ()(index_type i, index_type j) const
{
    return m_engine(i, j);
}

template<class ET, class OT> inline
typename matrix<ET,OT>::size_type
matrix<ET,OT>::columns() const noexcept
{
    return m_engine.columns();
}

template<class ET, class OT> inline
typename matrix<ET,OT>::size_type
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
typename matrix<ET,OT>::size_type
matrix<ET,OT>::column_capacity() const noexcept
{
    return m_engine.column_capacity();
}

template<class ET, class OT> inline
typename matrix<ET,OT>::size_type
matrix<ET,OT>::row_capacity() const noexcept
{
    return m_engine.row_capacity();
}

template<class ET, class OT> inline
typename matrix<ET,OT>::size_tuple
matrix<ET,OT>::capacity() const noexcept
{
    return size_tuple(m_engine.row_capacity(), m_engine.column_capacity());
}

template<class ET, class OT> inline
typename matrix<ET,OT>::column_type
matrix<ET,OT>::column(index_type j) const
{
    return column_type(m_engine, j, detail::row_column_tag());
}

template<class ET, class OT> inline
typename matrix<ET,OT>::row_type
matrix<ET,OT>::row(index_type i) const
{
    return row_type(m_engine, i, detail::row_column_tag());
}

template<class ET, class OT> inline
typename matrix<ET,OT>::transpose_type
matrix<ET,OT>::t() const
{
    return transpose_type(m_engine);
}

template<class ET, class OT> inline
typename matrix<ET,OT>::hermitian_type
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
typename matrix<ET,OT>::reference
matrix<ET,OT>::operator ()(index_type i, index_type j)
{
    return m_engine(i, j);
}

template<class ET, class OT>
void
matrix<ET,OT>::assign(matrix&& rhs)
{
    m_engine.assign(std::move(rhs.m_engine));
}

template<class ET, class OT>
void
matrix<ET,OT>::assign(matrix const& rhs)
{
    m_engine.assign(rhs.m_engine);
}

template<class ET, class OT>
template<class ET2, class OT2>
constexpr void
matrix<ET,OT>::assign(matrix<ET2, OT2> const& rhs)
{
    if constexpr (detail::is_fixed_size_engine_v<ET>)
    {
        if constexpr (detail::is_fixed_size_engine_v<ET2>)
        {
            static_assert(detail::engine_size_v<ET> == detail::engine_size_v<ET2>);
        }
        else
        {
            if (size() != rhs.size())
            {
                throw runtime_error("size mismatch on assignment to fixed-size matrix");
            }
        }
        copy_elements(rhs);
    }
    else
    {
        matrix  tmp(rhs.size());
        tmp.copy_elements(rhs);
        tmp.swap(*this);
    }
}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> inline
void
matrix<ET,OT>::reserve(size_tuple cap)
{
    m_engine.resize(get<0>(cap), get<1>(cap));
}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> inline
void
matrix<ET,OT>::reserve(size_type rowcap, size_type colcap)
{
    m_engine.reserve(rowcap, colcap);
}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> inline
void
matrix<ET,OT>::resize(size_tuple size)
{
    m_engine.resize(get<0>(size), get<1>(size));
}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> inline
void
matrix<ET,OT>::resize(size_type rows, size_type cols)
{
    m_engine.resize(rows, cols);
}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> inline
void
matrix<ET,OT>::resize(size_tuple size, size_tuple cap)
{
    m_engine.resize(get<0>(size), get<1>(size), get<0>(cap), get<1>(cap));
}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> inline
void
matrix<ET,OT>::resize(size_type rows, size_type cols, size_type rowcap, size_type colcap)
{
    m_engine.resize(rows, cols, rowcap, colcap);
}

template<class ET, class OT>
template<class ET2, detail::enable_if_mutable<ET, ET2>> inline
void
matrix<ET,OT>::swap(matrix& rhs)
{
    m_engine.swap(rhs.m_engine);
}

template<class ET, class OT>
template<class ET2, detail::enable_if_mutable<ET, ET2>> inline
void
matrix<ET,OT>::swap_columns(index_type c1, index_type c2)
{
    m_engine.swap_columns(c1, c2);
}

template<class ET, class OT>
template<class ET2, detail::enable_if_mutable<ET, ET2>> inline
void
matrix<ET,OT>::swap_rows(index_type r1, index_type r2)
{
    m_engine.swap_rows(r1, r2);
}

template<class ET, class OT>
template<class ET2, class OT2>
constexpr void
matrix<ET,OT>::copy_elements(matrix<ET2,OT2> const& rhs)
{
    using src_index_type = typename matrix<ET2, OT2>::index_type;
    using dst_index_type = index_type;

    dst_index_type  di, dj;
    src_index_type  si, sj;

    dst_index_type const    nrows = rows();
    dst_index_type const    ncols = columns();

    for (di = 0, si = 0;  di < nrows;  ++di, ++si)
    {
        for (dj = 0, sj = 0;  dj < ncols;  ++dj, ++sj)
        {
            m_engine(di, dj) = rhs.m_engine(si, sj);
        }
    }
}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_MATRIX_HPP_DEFINED
