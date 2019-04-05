#ifndef LINEAR_ALGEBRA_MATRIX_HPP_DEFINED
#define LINEAR_ALGEBRA_MATRIX_HPP_DEFINED

#include "vector.hpp"

namespace STD_LA {
//=================================================================================================
//  A matrix parametrized by an engine type and operator traits.
//=================================================================================================
//
template<class ET, class OT>
class matrix
{
  public:
    using engine_type    = ET;
    using element_type   = typename engine_type::element_type;
    using is_dense       = typename engine_type::is_dense;
    using is_rectangular = typename engine_type::is_rectangular;
    using index_type     = typename engine_type::index_type;
    using size_type      = typename engine_type::size_type;
    using size_tuple     = typename engine_type::size_tuple;

    using transpose_type = matrix<matrix_transpose_engine<engine_type>>;
    using hermitian_type = std::conditional_t<is_complex_v<element_type>, matrix, transpose_type>;

    static_assert(is_matrix_element_v<element_type>);

  public:
    ~matrix() = default;
    matrix();
    matrix(matrix&&) = default;
    matrix(matrix const&) = default;
    template<class ET2, class OT2>
    matrix(matrix<ET2, OT2> const& src);

    template<class ET2 = ET, enable_if_resizable<ET, ET2> = true>
    matrix(size_tuple size);
    template<class ET2 = ET, enable_if_resizable<ET, ET2> = true>
    matrix(size_type rows, size_type cols);

    template<class ET2 = ET, enable_if_resizable<ET, ET2> = true>
    matrix(size_tuple size, size_tuple cap);
    template<class ET2 = ET, enable_if_resizable<ET, ET2> = true>
    matrix(size_type rows, size_type cols, size_type rowcap, size_type colcap);

    matrix&     operator =(matrix&&) = default;
    matrix&     operator =(matrix const&) = default;
    template<class ET2, class OT2>
    matrix&     operator =(matrix<ET2, OT2> const& rhs);

    //- Const element access.
    //
    element_type        operator ()(index_type i, index_type j) const;
    element_type const* data() const noexcept;

    //- Accessors.
    //
    size_type   columns() const noexcept;
    size_type   rows() const noexcept;
    size_tuple  size() const noexcept;

    size_type   column_capacity() const noexcept;
    size_type   row_capacity() const noexcept;
    size_tuple  capacity() const noexcept;

    //- Transpose and Hermitian.
    //
    transpose_type  t() const;
    hermitian_type  h() const;

    //- Mutable element access.
    //
    element_type&   operator ()(size_type i, size_type j);
    element_type*   data() noexcept;

    //- Change capacity.
    //
    template<class ET2 = ET, enable_if_resizable<ET, ET2> = true>
    void    reserve(size_tuple cap);
    template<class ET2 = ET, enable_if_resizable<ET, ET2> = true>
    void    reserve(size_type rowcap, size_type colcap);

    //- Change size.
    //
    template<class ET2 = ET, enable_if_resizable<ET, ET2> = true>
    void    resize(size_tuple size);
    template<class ET2 = ET, enable_if_resizable<ET, ET2> = true>
    void    resize(size_type rows, size_type cols);

    //- Change size and capacity in one shot.
    //
    template<class ET2 = ET, enable_if_resizable<ET, ET2> = true>
    void    resize(size_tuple size, size_tuple cap);
    template<class ET2 = ET, enable_if_resizable<ET, ET2> = true>
    void    resize(size_type rows, size_type cols, size_type rowcap, size_type colcap);

    //- Row and column operations.
    //
    void    swap_columns(index_type i, index_type j);

    template<class ET2 = ET, enable_if_engine_is_mutable<ET, ET2> = true>
    void    swap_rows(index_type i, index_type j);

  private:
    template<class ET2, class OT2> friend class matrix;

  private:
    engine_type     m_engine;

  private:
    matrix(engine_type const& eng);
};

template<class ET, class OT> inline
matrix<ET,OT>::matrix()
{}

template<class ET, class OT>
template<class ET2, class OT2> inline
matrix<ET,OT>::matrix(matrix<ET2, OT2> const&)
{}

template<class ET, class OT>
template<class ET2, enable_if_resizable<ET, ET2>> inline
matrix<ET,OT>::matrix(size_tuple)
{}

template<class ET, class OT>
template<class ET2, enable_if_resizable<ET, ET2>>
matrix<ET,OT>::matrix(size_type, size_type)
{}

template<class ET, class OT>
template<class ET2, enable_if_resizable<ET, ET2>> inline
matrix<ET,OT>::matrix(size_tuple, size_tuple)
{}

template<class ET, class OT>
template<class ET2, enable_if_resizable<ET, ET2>>
matrix<ET,OT>::matrix(size_type, size_type, size_type, size_type)
{}

template<class ET, class OT> inline
matrix<ET,OT>::matrix(engine_type const&)
{}

template<class ET, class OT>
template<class ET2, class OT2> inline
matrix<ET,OT>&
matrix<ET,OT>::operator =(matrix<ET2, OT2> const&)
{
    return *this;
}

template<class ET, class OT> inline
typename matrix<ET,OT>::element_type
matrix<ET,OT>::operator ()(index_type i, index_type j) const
{
    return m_engine(i, j);
}

template<class ET, class OT> inline
typename matrix<ET,OT>::element_type const*
matrix<ET,OT>::data() const noexcept
{
    return m_engine.data();
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
typename matrix<ET,OT>::size_tuple
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
typename matrix<ET,OT>::element_type&
matrix<ET,OT>::operator ()(size_type i, size_type j)
{
    return m_engine(i, j);
}

template<class ET, class OT> inline
typename matrix<ET,OT>::element_type*
matrix<ET,OT>::data() noexcept
{
    return m_engine.data();
}

template<class ET, class OT>
template<class ET2, enable_if_resizable<ET, ET2>> inline
void
matrix<ET,OT>::reserve(size_tuple cap)
{
    m_engine.resize(std::get<0>(cap), std::get<1>(cap));
}

template<class ET, class OT>
template<class ET2, enable_if_resizable<ET, ET2>> inline
void
matrix<ET,OT>::reserve(size_type rowcap, size_type colcap)
{
    m_engine.reserve(rowcap, colcap);
}

template<class ET, class OT>
template<class ET2, enable_if_resizable<ET, ET2>> inline
void
matrix<ET,OT>::resize(size_tuple size)
{
    m_engine.resize(std::get<0>(size), std::get<1>(size));
}

template<class ET, class OT>
template<class ET2, enable_if_resizable<ET, ET2>> inline
void
matrix<ET,OT>::resize(size_type rows, size_type cols)
{
    m_engine.resize(rows, cols);
}

template<class ET, class OT>
template<class ET2, enable_if_resizable<ET, ET2>> inline
void
matrix<ET,OT>::resize(size_tuple size, size_tuple cap)
{
    m_engine.resize(std::get<0>(size), std::get<1>(size), std::get<0>(cap), std::get<1>(cap));
}

template<class ET, class OT>
template<class ET2, enable_if_resizable<ET, ET2>> inline
void
matrix<ET,OT>::resize(size_type rows, size_type cols, size_type rowcap, size_type colcap)
{
    m_engine.resize(rows, cols, rowcap, colcap);
}

template<class ET, class OT> inline
void
matrix<ET,OT>::swap_columns(index_type c1, index_type c2)
{
    m_engine.swap_columns(c1, c2);
}

template<class ET, class OT> 
template<class ET2, enable_if_engine_is_mutable<ET, ET2>> inline
void
matrix<ET,OT>::swap_rows(index_type r1, index_type r2)
{
    m_engine.swap_rows(r1, r2);
}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_MATRIX_HPP_DEFINED
