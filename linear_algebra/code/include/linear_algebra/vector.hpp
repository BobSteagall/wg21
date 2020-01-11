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

  public:
    //- Types
    //
    using engine_type     = ET;
    using element_type    = typename engine_type::element_type;
    using value_type      = typename engine_type::value_type;
    using difference_type = typename engine_type::difference_type;
    using size_type       = typename engine_type::size_type;
    using const_reference = typename engine_type::const_reference;
    using reference       = typename engine_type::reference;
    using const_iterator  = typename engine_type::const_iterator;
    using iterator        = typename engine_type::iterator;

    using transpose_type       = vector&;
    using const_transpose_type = vector const&;
    using hermitian_type       = conditional_t<detail::is_complex_v<element_type>, vector, transpose_type>;
    using const_hermitian_type = conditional_t<detail::is_complex_v<element_type>, vector, const_transpose_type>;

    //- Construct/copy/destroy
    //
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

    //- Iterators
    //
    constexpr iterator          begin() noexcept;
    constexpr iterator          end() noexcept;
    constexpr const_iterator    begin() const noexcept;
    constexpr const_iterator    end() const noexcept;
    constexpr const_iterator    cbegin() const noexcept;
    constexpr const_iterator    cend() const noexcept;

    //- Capacity
    //
    constexpr size_type     capacity() const noexcept;
    constexpr size_type     elements() const noexcept;
    constexpr size_type     size() const noexcept;

    //- Element access.
    //
    constexpr reference         operator [](size_type i);
    constexpr reference         operator ()(size_type i);
    constexpr const_reference   operator [](size_type i) const;
    constexpr const_reference   operator ()(size_type i) const;

    constexpr transpose_type        t();
    constexpr const_transpose_type  t() const;
    constexpr hermitian_type        h();
    constexpr const_hermitian_type  h() const;

    //- Modifiers
    //
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr void      reserve(size_type elemcap);

    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr void      resize(size_type elems);
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr void      resize(size_type elems, size_type elemcap);

    constexpr void      swap(vector& rhs) noexcept;
    constexpr void      swap_elements(size_type i, size_type j) noexcept;

  private:
    template<class ET2, class OT2> friend class vector;
    template<class ET2, class OT2> friend class matrix;

    engine_type     m_engine;

    template<class ET2>
    constexpr vector(ET2&& eng, size_type idx, detail::row_or_column_tag);
};

//------------------------
//- Construct/copy/destroy
//
template<class ET, class OT> constexpr
vector<ET,OT>::vector()
:   m_engine()
{}

template<class ET, class OT>
template<class ET2, class OT2> constexpr
vector<ET,OT>::vector(vector<ET2, OT2> const& rhs)
:   m_engine(rhs.m_engine)
{}

template<class ET, class OT>
template<class U> constexpr
vector<ET,OT>::vector(initializer_list<U> list)
:   m_engine(forward<initializer_list<U>>(list))
{}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> constexpr
vector<ET,OT>::vector(size_type elems)
:   m_engine(elems)
{}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> constexpr
vector<ET,OT>::vector(size_type elems, size_type cap)
:   m_engine(elems, cap)
{}

template<class ET, class OT>
template<class ET2> constexpr
vector<ET,OT>::vector(ET2&& eng, size_type idx, detail::row_or_column_tag)
:   m_engine(std::forward<ET2>(eng), idx)
{}

template<class ET, class OT>
template<class ET2, class OT2> constexpr
vector<ET,OT>&
vector<ET,OT>::operator =(vector<ET2, OT2> const& rhs)
{
    m_engine = rhs.m_engine;
    return *this;
}

//-----------
//- Iterators
//
template<class ET, class OT> constexpr 
typename vector<ET,OT>::iterator
vector<ET,OT>::begin() noexcept
{
    return m_engine.begin();
}

template<class ET, class OT> constexpr 
typename vector<ET,OT>::iterator
vector<ET,OT>::end() noexcept
{
    return m_engine.end();
}

template<class ET, class OT> constexpr 
typename vector<ET,OT>::const_iterator
vector<ET,OT>::begin() const noexcept
{
    return m_engine.cbegin();
}

template<class ET, class OT> constexpr 
typename vector<ET,OT>::const_iterator
vector<ET,OT>::end() const noexcept
{
    return m_engine.cend();
}

template<class ET, class OT> constexpr 
typename vector<ET,OT>::const_iterator
vector<ET,OT>::cbegin() const noexcept
{
    return m_engine.cbegin();
}

template<class ET, class OT> constexpr 
typename vector<ET,OT>::const_iterator
vector<ET,OT>::cend() const noexcept
{
    return m_engine.cend();
}

//----------
//- Capacity
//
template<class ET, class OT> constexpr 
typename vector<ET,OT>::size_type
vector<ET,OT>::capacity() const noexcept
{
    return m_engine.capacity();
}

template<class ET, class OT> constexpr 
typename vector<ET,OT>::size_type
vector<ET,OT>::elements() const noexcept
{
    return m_engine.elements();
}

template<class ET, class OT> constexpr 
typename vector<ET,OT>::size_type
vector<ET,OT>::size() const noexcept
{
    return m_engine.elements();
}

//----------------
//- Element access
//
template<class ET, class OT> constexpr 
typename vector<ET,OT>::reference
vector<ET,OT>::operator [](size_type i)
{
    return m_engine(i);
}

template<class ET, class OT> constexpr 
typename vector<ET,OT>::reference
vector<ET,OT>::operator ()(size_type i)
{
    return m_engine(i);
}

template<class ET, class OT> constexpr 
typename vector<ET,OT>::const_reference
vector<ET,OT>::operator [](size_type i) const
{
    return m_engine(i);
}

template<class ET, class OT> constexpr 
typename vector<ET,OT>::const_reference
vector<ET,OT>::operator ()(size_type i) const
{
    return m_engine(i);
}

template<class ET, class OT> constexpr 
typename vector<ET,OT>::transpose_type
vector<ET,OT>::t()
{
    return *this;
}

template<class ET, class OT> constexpr 
typename vector<ET,OT>::const_transpose_type
vector<ET,OT>::t() const
{
    return *this;
}

template<class ET, class OT> constexpr 
typename vector<ET,OT>::hermitian_type
vector<ET,OT>::h()
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

template<class ET, class OT> constexpr 
typename vector<ET,OT>::const_hermitian_type
vector<ET,OT>::h() const
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

//-----------
//- Modifiers
//
template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> constexpr 
void
vector<ET,OT>::reserve(size_type cap)
{
    m_engine.reserve(cap);
}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> constexpr 
void
vector<ET,OT>::resize(size_type elems)
{
    m_engine.resize(elems);
}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> constexpr 
void
vector<ET,OT>::resize(size_type elems, size_type cap)
{
    m_engine.resize(elems, cap);
}

template<class ET, class OT> constexpr 
void
vector<ET,OT>::swap(vector& rhs) noexcept
{
    m_engine.swap(rhs);
}

template<class ET, class OT> constexpr 
void
vector<ET,OT>::swap_elements(size_type i, size_type j) noexcept
{
    m_engine.swap_elements(i, j);
}

//------------
//- Comparison
//
template<class ET1, class OT1, class ET2, class OT2> constexpr 
bool
operator ==(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2)
{
    using size_type_1 = typename vector<ET1, OT1>::size_type;
    using size_type_2 = typename vector<ET2, OT2>::size_type;

    if (v1.size() != (size_type_1) v2.size()) return false;

    size_type_1     i1;
    size_type_2     i2;

    for (i1 = 0, i2 = 0;  i1 < v1.size();  ++i1, ++i2)
    {
        if (v1(i1) != v2(i2)) return false;
    }
    return true;
}

template<class ET1, class OT1, class ET2, class OT2> constexpr 
bool
operator !=(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2)
{
    return !(v1 == v2);
}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_VECTOR_HPP_DEFINED
