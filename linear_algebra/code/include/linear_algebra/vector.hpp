#ifndef LINEAR_ALGEBRA_VECTOR_HPP_DEFINED
#define LINEAR_ALGEBRA_VECTOR_HPP_DEFINED

#include "helper_traits.hpp"

namespace STD_LA
{
//=================================================================================================
//  A vector type parametrized by an engine type and operator traits.
//=================================================================================================
//
template<class ET, class OT>
class vector
{
    template<class ET1, class ET2>
    using enable_if_resizable_t =
    typename std::enable_if_t<std::is_same_v<ET1, ET2>&& ET1::is_resizable::value, bool>;

  public:
    using engine_type    = ET;
    using element_type   = typename engine_type::element_type;
    using is_dense       = typename engine_type::is_dense;
    using is_rectangular = typename engine_type::is_rectangular;
    using is_resizable   = typename engine_type::is_resizable;
    using size_tuple     = typename engine_type::size_tuple;
    using transpose_type = vector;
    using hermitian_type = std::conditional_t<is_complex_v<element_type>,
                                              vector<ET, OT>,
                                              transpose_type>;

    static_assert(is_matrix_element_v<element_type>);

  public:
    ~vector() = default;
    vector();
    vector(vector&&) = default;
    vector(vector const&) = default;
    template<class ET2, class OT2>
    vector(vector<ET2, OT2> const& src);

    template<class ET2 = ET, enable_if_resizable_t<ET, ET2> = true>
    vector(size_t cols);
    template<class ET2 = ET, enable_if_resizable_t<ET, ET2> = true>
    vector(size_t cols, size_t colcap);

    vector& operator =(vector&&) = default;
    vector& operator =(vector const&) = default;
    template<class ET2, class OT2>
    vector& operator =(vector<ET2, OT2> const& rhs);

    //- Const element access.
    //
    element_type        operator ()(size_t i) const;
    element_type const* data() const noexcept;

    //- Accessors.
    //
    size_t      columns() const noexcept;
    size_t      rows() const noexcept;
    size_tuple  size() const noexcept;

    size_t      column_capacity() const noexcept;
    size_t      row_capacity() const noexcept;
    size_tuple  capacity() const noexcept;

    //- Transpose and Hermitian.
    //
    transpose_type  t() const;
    hermitian_type  h() const;

    //- Mutable element access.
    //
    element_type& operator ()(size_t i);
    element_type* data() noexcept;

    //- Change capacity.
    //
    template<class ET2 = ET, enable_if_resizable_t<ET, ET2> = true>
    void    reserve(size_t colcap);

    //- Change size.
    //
    template<class ET2 = ET, enable_if_resizable_t<ET, ET2> = true>
    void    resize(size_t cols);

    //- Change size and capacity in one shot.
    //
    template<class ET2 = ET, enable_if_resizable_t<ET, ET2> = true>
    void    resize(size_t cols, size_t colcap);

    //- column operations.
    //
    void    swap_columns(size_t i, size_t j);

private:
    template<class ET2, class OT2> friend class column_vector;

private:
    engine_type     m_engine;

private:
    vector(engine_type const& eng);
};

template<class ET, class OT> inline
vector<ET,OT>::vector()
{}

template<class ET, class OT>
template<class ET2, class OT2> inline
vector<ET,OT>::vector(vector<ET2, OT2> const&)
{}

template<class ET, class OT>
template<class ET2, enable_if_resizable_t<ET, ET2>> inline
vector<ET,OT>::vector(size_t)
{}

template<class ET, class OT>
template<class ET2, enable_if_resizable_t<ET, ET2>>
vector<ET,OT>::vector(size_t, size_t)
{}

template<class ET, class OT> inline
vector<ET,OT>::vector(engine_type const&)
{}

template<class ET, class OT>
template<class ET2, class OT2> inline
vector<ET,OT>&
vector<ET,OT>::operator =(vector<ET2, OT2> const&)
{
    return *this;
}

template<class ET, class OT> inline
typename vector<ET,OT>::element_type
vector<ET,OT>::operator ()(size_t i) const
{
    return m_engine(i);
}

template<class ET, class OT> inline
typename vector<ET,OT>::element_type const*
vector<ET,OT>::data() const noexcept
{
    return m_engine.data();
}

template<class ET, class OT> inline
size_t
vector<ET,OT>::columns() const noexcept
{
    return m_engine.columns();
}

template<class ET, class OT> inline
size_t
vector<ET,OT>::rows() const noexcept
{
    return m_engine.rows();
}

template<class ET, class OT> inline
typename vector<ET,OT>::size_tuple
vector<ET,OT>::size() const noexcept
{
    return m_engine.size();
}

template<class ET, class OT> inline
size_t
vector<ET,OT>::column_capacity() const noexcept
{
    return m_engine.column_capacity();
}

template<class ET, class OT> inline
size_t
vector<ET,OT>::row_capacity() const noexcept
{
    return m_engine.row_capacity();
}

template<class ET, class OT> inline
typename vector<ET,OT>::size_tuple
vector<ET,OT>::capacity() const noexcept
{
    return m_engine.capacity();
}

template<class ET, class OT> inline
typename vector<ET,OT>::transpose_type
vector<ET,OT>::t() const
{
    return transpose_type(m_engine);
}

template<class ET, class OT> inline
typename vector<ET,OT>::hermitian_type
vector<ET,OT>::h() const
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
typename vector<ET,OT>::element_type&
vector<ET,OT>::operator ()(size_t i)
{
    return m_engine(i);
}

template<class ET, class OT> inline
typename vector<ET,OT>::element_type*
vector<ET,OT>::data() noexcept
{
    return m_engine.data();
}

template<class ET, class OT>
template<class ET2, enable_if_resizable_t<ET, ET2>> inline
void
vector<ET,OT>::reserve(size_t)
{}

template<class ET, class OT>
template<class ET2, enable_if_resizable_t<ET, ET2>> inline
void
vector<ET,OT>::resize(size_t)
{}

template<class ET, class OT>
template<class ET2, enable_if_resizable_t<ET, ET2>> inline
void
vector<ET,OT>::resize(size_t, size_t)
{}

template<class ET, class OT> inline
void
vector<ET,OT>::swap_columns(size_t, size_t)
{}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_VECTOR_HPP_DEFINED
