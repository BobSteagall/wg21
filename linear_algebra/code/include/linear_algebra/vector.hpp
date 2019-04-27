//==================================================================================================
//  File:       vector.hpp
//
//  Summary:    This header defines the vector class template, one of the two primary math
//              objects provided by the library.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_VECTOR_HPP_DEFINED
#define LINEAR_ALGEBRA_VECTOR_HPP_DEFINED

namespace STD_LA {
//==================================================================================================
//  A vector type parametrized by an engine type and operator traits.
//==================================================================================================
//
template<class ET, class OT>
class vector
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
    using transpose_type  = vector const&;
    using hermitian_type  = conditional_t<is_complex_v<element_type>, vector, transpose_type>;

    using is_fixed_size   = typename engine_type::is_fixed_size;
    using is_resizable    = typename engine_type::is_resizable;

    using is_column_major = typename engine_type::is_column_major;
    using is_dense        = typename engine_type::is_dense;
    using is_rectangular  = typename engine_type::is_rectangular;
    using is_row_major    = typename engine_type::is_row_major;

    static_assert(is_matrix_element_v<element_type>);

  public:
    ~vector() = default;
    vector();
    vector(vector&&) = default;
    vector(vector const&) = default;
    template<class ET2, class OT2>
    vector(vector<ET2, OT2> const& src);

    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    vector(size_type elems);
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    vector(size_type elems, size_type elemcap);

    vector& operator =(vector&&) = default;
    vector& operator =(vector const&) = default;
    template<class ET2, class OT2>
    vector& operator =(vector<ET2, OT2> const& rhs);

    //- Const element access.
    //
    const_reference     operator ()(index_type i) const;
    const_pointer       data() const noexcept;

    //- Accessors.
    //
    size_type   capacity() const noexcept;
    size_type   elements() const noexcept;

    //- Transpose and Hermitian.
    //
    transpose_type  t() const;
    hermitian_type  h() const;

    //- Mutable element access.
    //
    reference   operator ()(index_type i);
    pointer     data() noexcept;

    //- Change capacity.
    //
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    void    reserve(size_type elemcap);

    //- Change size.
    //
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    void    resize(size_type elems);

    //- Change size and capacity in one shot.
    //
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    void    resize(size_type elems, size_type elemcap);

    //- column operations.
    //
    void    swap_elements(index_type i, index_type j);

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
template<class ET2, detail::enable_if_resizable<ET, ET2>> inline
vector<ET,OT>::vector(size_type)
{}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> inline
vector<ET,OT>::vector(size_type, size_type)
{}

template<class ET, class OT> inline
vector<ET,OT>::vector(engine_type const& eng)
:   m_engine(eng)
{}

template<class ET, class OT>
template<class ET2, class OT2> inline
vector<ET,OT>&
vector<ET,OT>::operator =(vector<ET2, OT2> const&)
{
    return *this;
}

template<class ET, class OT> inline
typename vector<ET,OT>::const_reference
vector<ET,OT>::operator ()(index_type i) const
{
    return m_engine(i);
}

template<class ET, class OT> inline
typename vector<ET,OT>::const_pointer
vector<ET,OT>::data() const noexcept
{
    return m_engine.data();
}

template<class ET, class OT> inline
typename vector<ET,OT>::size_type
vector<ET,OT>::capacity() const noexcept
{
    return m_engine.capacity();
}

template<class ET, class OT> inline
typename vector<ET,OT>::size_type
vector<ET,OT>::elements() const noexcept
{
    return m_engine.elements();
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
typename vector<ET,OT>::reference
vector<ET,OT>::operator ()(index_type i)
{
    return m_engine(i);
}

template<class ET, class OT> inline
typename vector<ET,OT>::pointer
vector<ET,OT>::data() noexcept
{
    return m_engine.data();
}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> inline
void
vector<ET,OT>::reserve(size_type)
{}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> inline
void
vector<ET,OT>::resize(size_type)
{}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> inline
void
vector<ET,OT>::resize(size_type, size_type)
{}

template<class ET, class OT> inline
void
vector<ET,OT>::swap_elements(index_type, index_type)
{}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_VECTOR_HPP_DEFINED
