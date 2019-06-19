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
    static_assert(detail::is_vector_engine_v<ET>);
    static_assert(is_matrix_element_v<typename ET::element_type>);

  public:
    using engine_type     = ET;
    using element_type    = typename engine_type::element_type;
    using reference       = typename engine_type::reference;
    using pointer         = typename engine_type::pointer;
    using const_reference = typename engine_type::const_reference;
    using const_pointer   = typename engine_type::const_pointer;
    using iterator        = typename engine_type::iterator;
    using const_iterator  = typename engine_type::const_iterator;
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

  public:
    ~vector() = default;

    constexpr vector();
    constexpr vector(vector&&) noexcept = default;
    constexpr vector(vector const&) = default;
    template<class ET2, class OT2>
    constexpr vector(vector<ET2, OT2> const& src);

    template<class U>
    constexpr vector(initializer_list<U> list);
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr vector(size_type elems);
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr vector(size_type elems, size_type elemcap);

    constexpr vector& operator =(vector&&) noexcept = default;
    constexpr vector& operator =(vector const&) = default;
    template<class ET2, class OT2>
    constexpr vector& operator =(vector<ET2, OT2> const& rhs);

    //- Const element access.
    //
    constexpr const_reference   operator ()(index_type i) const;
    constexpr const_iterator    begin() const noexcept;
    constexpr const_iterator    end() const noexcept;

    //- Accessors.
    //
    constexpr size_type     capacity() const noexcept;
    constexpr index_type    elements() const noexcept;
    constexpr size_type     size() const noexcept;

    //- Transpose and Hermitian.
    //
    constexpr transpose_type    t() const;
    constexpr hermitian_type    h() const;

    //- Mutable element access.
    //
    constexpr reference     operator ()(index_type i);
    constexpr iterator      begin() noexcept;
    constexpr iterator      end() noexcept;

    //- Assignment.
    //
    constexpr void      assign(vector const& rhs);
    template<class ET2, class OT2>
    constexpr void      assign(vector<ET2, OT2> const& rhs);

    //- Change capacity.
    //
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr void      reserve(size_type elemcap);

    //- Change size.
    //
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr void      resize(size_type elems);

    //- Change size and capacity in one shot.
    //
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr void      resize(size_type elems, size_type elemcap);

    //- Element operations.
    //
    constexpr void      swap(vector& rhs) noexcept;
    constexpr void      swap_elements(index_type i, index_type j) noexcept;

  private:
    template<class ET2, class OT2> friend class vector;
    template<class ET2, class OT2> friend class matrix;

  private:
    engine_type     m_engine;

  private:
    template<class ET2>
    constexpr vector(ET2 const& eng, index_type idx, detail::row_column_tag);
};

template<class ET, class OT> inline
constexpr
vector<ET,OT>::vector()
:   m_engine()
{}

template<class ET, class OT>
template<class U>
constexpr
vector<ET,OT>::vector(initializer_list<U> list)
:   m_engine(forward<initializer_list<U>>(list))
{}

template<class ET, class OT>
template<class ET2, class OT2> inline
constexpr
vector<ET,OT>::vector(vector<ET2, OT2> const&)
{}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> inline
constexpr
vector<ET,OT>::vector(size_type elems)
:   m_engine(elems)
{}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> inline
constexpr
vector<ET,OT>::vector(size_type elems, size_type cap)
:   m_engine(elems, cap)
{}

template<class ET, class OT>
template<class ET2> inline
constexpr
vector<ET,OT>::vector(ET2 const& eng, index_type idx, detail::row_column_tag)
:   m_engine(eng, idx)
{}

template<class ET, class OT>
template<class ET2, class OT2> inline
constexpr
vector<ET,OT>&
vector<ET,OT>::operator =(vector<ET2, OT2> const& rhs)
{
    m_engine.assign(rhs.m_engine);
    return *this;
}

template<class ET, class OT> inline
constexpr typename vector<ET,OT>::const_reference
vector<ET,OT>::operator ()(index_type i) const
{
    return m_engine(i);
}

template<class ET, class OT> inline
constexpr typename vector<ET,OT>::const_iterator
vector<ET,OT>::begin() const noexcept
{
    return m_engine.begin();
}

template<class ET, class OT> inline
constexpr typename vector<ET,OT>::const_iterator
vector<ET,OT>::end() const noexcept
{
    return m_engine.end();
}

template<class ET, class OT> inline
constexpr typename vector<ET,OT>::size_type
vector<ET,OT>::capacity() const noexcept
{
    return m_engine.capacity();
}

template<class ET, class OT> inline
constexpr typename vector<ET,OT>::index_type
vector<ET,OT>::elements() const noexcept
{
    return m_engine.elements();
}

template<class ET, class OT> inline
constexpr typename vector<ET,OT>::size_type
vector<ET,OT>::size() const noexcept
{
    return m_engine.elements();
}

template<class ET, class OT> inline
constexpr typename vector<ET,OT>::transpose_type
vector<ET,OT>::t() const
{
    return transpose_type(m_engine);
}

template<class ET, class OT> inline
constexpr typename vector<ET,OT>::hermitian_type
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
constexpr typename vector<ET,OT>::reference
vector<ET,OT>::operator ()(index_type i)
{
    return m_engine(i);
}

template<class ET, class OT> inline
constexpr typename vector<ET,OT>::iterator
vector<ET,OT>::begin() noexcept
{
    return m_engine.begin();
}

template<class ET, class OT> inline
constexpr typename vector<ET,OT>::iterator
vector<ET,OT>::end() noexcept
{
    return m_engine.end();
}

template<class ET, class OT>
constexpr void
vector<ET,OT>::assign(vector const& rhs)
{
    m_engine.assign(rhs.m_engine);
}

template<class ET, class OT>
template<class ET2, class OT2>
constexpr void
vector<ET,OT>::assign(vector<ET2, OT2> const& rhs)
{
    m_engine.assign(rhs);
}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> inline
constexpr void
vector<ET,OT>::reserve(size_type cap)
{
    m_engine.reserve(cap);
}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> inline
constexpr void
vector<ET,OT>::resize(size_type elems)
{
    m_engine.resize(elems);
}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> inline
constexpr void
vector<ET,OT>::resize(size_type elems, size_type cap)
{
    m_engine.resize(elems, cap);
}

template<class ET, class OT> inline
constexpr void
vector<ET,OT>::swap(vector& rhs) noexcept
{
    m_engine.swap(rhs);
}

template<class ET, class OT> inline
constexpr void
vector<ET,OT>::swap_elements(index_type i, index_type j) noexcept
{
    m_engine.swap_elements(i, j);
}


template<class ET1, class OT1, class ET2, class OT2>
constexpr bool
operator ==(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2)
{
    if (v1.size() != v2.size()) return false;

    for (int i = 0;  i < v1.elements();  ++i)
    {
        if (v1(i) != v2(i)) return false;
    }
    return true;
}

template<class ET1, class OT1, class ET2, class OT2>
constexpr bool
operator !=(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2)
{
    return !(v1 == v2);
}


}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_VECTOR_HPP_DEFINED
