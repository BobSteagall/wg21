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

template<class ET, class OT> constexpr
detail::vector_iterator<vector<ET, OT>>
begin(vector<ET, OT>& v) noexcept
{
    return detail::vector_iterator<vector<ET, OT>>(v, 0, v.size());
}

template<class ET, class OT> constexpr
detail::vector_iterator<vector<ET, OT>>
end(vector<ET, OT>& v) noexcept
{
    return detail::vector_iterator<vector<ET, OT>>(v, v.size(), v.size());
}


template<class ET, class OT> constexpr
detail::vector_const_iterator<vector<ET, OT>>
begin(vector<ET, OT> const& v) noexcept
{
    return detail::vector_const_iterator<vector<ET, OT>>(v, 0, v.size());
}

template<class ET, class OT> constexpr
detail::vector_const_iterator<vector<ET, OT>>
end(vector<ET, OT>const & v) noexcept
{
    return detail::vector_const_iterator<vector<ET, OT>>(v, v.size(), v.size());
}


template<class ET, class OT> constexpr
detail::vector_const_iterator<vector<ET, OT>>
cbegin(vector<ET, OT> const& v) noexcept
{
    return detail::vector_const_iterator<vector<ET, OT>>(v, 0, v.size());
}

template<class ET, class OT> constexpr
detail::vector_const_iterator<vector<ET, OT>>
cend(vector<ET, OT>const & v) noexcept
{
    return detail::vector_const_iterator<vector<ET, OT>>(v, v.size(), v.size());
}

//- Reverse iterators.
//
template<class ET, class OT> constexpr
reverse_iterator<detail::vector_iterator<vector<ET, OT>>>
rbegin(vector<ET, OT>& v) noexcept
{
    return reverse_iterator<detail::vector_iterator<vector<ET, OT>>>(v, 0, v.size());
}

template<class ET, class OT> constexpr
reverse_iterator<detail::vector_iterator<vector<ET, OT>>>
rend(vector<ET, OT>& v) noexcept
{
    return reverse_iterator<detail::vector_iterator<vector<ET, OT>>>(v, v.size(), v.size());
}


template<class ET, class OT> constexpr
reverse_iterator<detail::vector_const_iterator<vector<ET, OT>>>
rbegin(vector<ET, OT> const& v) noexcept
{
    return reverse_iterator<detail::vector_const_iterator<vector<ET, OT>>>(v, 0, v.size());
}

template<class ET, class OT> constexpr
reverse_iterator<detail::vector_const_iterator<vector<ET, OT>>>
rend(vector<ET, OT>const & v) noexcept
{
    return reverse_iterator<detail::vector_const_iterator<vector<ET, OT>>>(v, v.size(), v.size());
}


template<class ET, class OT> constexpr
reverse_iterator<detail::vector_const_iterator<vector<ET, OT>>>
crbegin(vector<ET, OT> const& v) noexcept
{
    return reverse_iterator<detail::vector_const_iterator<vector<ET, OT>>>(v, 0, v.size());
}

template<class ET, class OT> constexpr
reverse_iterator<detail::vector_const_iterator<vector<ET, OT>>>
crend(vector<ET, OT>const & v) noexcept
{
    return reverse_iterator<detail::vector_const_iterator<vector<ET, OT>>>(v, v.size(), v.size());
}

//==================================================================================================
//  A vector type parametrized by an engine type and operator traits.
//==================================================================================================
//
template<class ET, class OT>
class vector
{
    static_assert(is_vector_engine_v<ET>);
#ifdef LA_USE_MDSPAN
    static_assert(detail::has_valid_span_alias_form_v<ET>);
#endif
    using possibly_writable_vector_tag = detail::noe_category_t<ET, writable_vector_engine_tag>;
    static constexpr bool   has_cx_elem  = detail::is_complex_v<typename ET::value_type>;

  public:
    //- Types
    //
    using engine_type          = ET;
    using element_type         = typename engine_type::element_type;
    using value_type           = typename engine_type::value_type;
    using difference_type      = typename engine_type::difference_type;
    using size_type            = typename engine_type::size_type;
    using pointer              = typename engine_type::pointer;
    using const_pointer        = typename engine_type::const_pointer;
    using reference            = typename engine_type::reference;
    using const_reference      = typename engine_type::const_reference;
    using subvector_type       = vector<subvector_engine<engine_type, possibly_writable_vector_tag>, OT>;
    using const_subvector_type = vector<subvector_engine<engine_type, readable_vector_engine_tag>, OT>;
    using transpose_type       = vector&;
    using const_transpose_type = vector const&;
    using hermitian_type       = conditional_t<has_cx_elem, vector, transpose_type>;
    using const_hermitian_type = conditional_t<has_cx_elem, vector, const_transpose_type>;

#ifdef LA_USE_MDSPAN
    using span_type            = detail::engine_span_t<ET>;
    using const_span_type      = detail::engine_const_span_t<ET>;
#endif

    //- Construct/copy/destroy
    //
    ~vector() = default;

    constexpr vector();
    constexpr vector(vector&&) noexcept = default;
    constexpr vector(vector const&) = default;

    template<class ET2, class OT2>
    constexpr vector(vector<ET2, OT2> const& src);
    template<class U, class ET2 = ET, detail::enable_if_initable<ET, ET2> = true>
    constexpr vector(initializer_list<U> list);

    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr vector(size_type elems);
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr vector(size_type elems, size_type elemcap);

    constexpr vector&   operator =(vector&&) noexcept = default;
    constexpr vector&   operator =(vector const&) = default;
    template<class ET2, class OT2>
    constexpr vector&   operator =(vector<ET2, OT2> const& rhs);
    template<class U, class ET2 = ET, detail::enable_if_writable<ET, ET2> = true>
    constexpr vector&   operator =(initializer_list<U> list);

    //- Capacity
    //
    static constexpr bool   is_resizable() noexcept;
    constexpr size_type     capacity() const noexcept;
    constexpr size_type     elements() const noexcept;
    constexpr size_type     size() const noexcept;

    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr void      reserve(size_type elemcap);
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr void      resize(size_type elems);
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr void      resize(size_type elems, size_type elemcap);

    //- Element access
    //
    constexpr reference             operator [](size_type i);
    constexpr reference             operator ()(size_type i);
    constexpr const_reference       operator [](size_type i) const;
    constexpr const_reference       operator ()(size_type i) const;

    constexpr subvector_type        subvector(size_type i, size_type n) noexcept;
    constexpr const_subvector_type  subvector(size_type i, size_type n) const noexcept;
    constexpr transpose_type        t();
    constexpr const_transpose_type  t() const;
    constexpr hermitian_type        h();
    constexpr const_hermitian_type  h() const;

    //- Data access
    //
    constexpr engine_type&          engine() noexcept;
    constexpr engine_type const&    engine() const noexcept;

#ifdef LA_USE_MDSPAN
    template<class ET2 = ET, detail::enable_if_spannable<ET, ET2> = true>
    constexpr span_type             span() noexcept;
    template<class ET2 = ET, detail::enable_if_spannable<ET, ET2> = true>
    constexpr const_span_type       span() const noexcept;
#endif

    //- Modifiers
    //
    constexpr void      swap(vector& rhs) noexcept;
    template<class ET2 = ET, detail::enable_if_writable<ET, ET2> = true>
    constexpr void      swap_elements(size_type i, size_type j) noexcept;

  private:
    template<class ET2, class OT2> friend class vector;
    template<class ET2, class OT2> friend class matrix;

    engine_type     m_engine;

    template<class ET2, class... ARGS>
    constexpr vector(detail::special_ctor_tag, ET2&& eng, ARGS&&... args);
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
template<class U, class ET2, detail::enable_if_initable<ET, ET2>> constexpr
vector<ET,OT>::vector(initializer_list<U> list)
:   m_engine(list)
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
template<class ET2, class ...ARGS> constexpr
vector<ET,OT>::vector(detail::special_ctor_tag, ET2&& eng, ARGS&&... args)
:   m_engine(std::forward<ET2>(eng), std::forward<ARGS>(args)...)
{}

template<class ET, class OT>
template<class ET2, class OT2> constexpr
vector<ET,OT>&
vector<ET,OT>::operator =(vector<ET2, OT2> const& rhs)
{
    m_engine = rhs.m_engine;
    return *this;
}

template<class ET, class OT>
template<class U, class ET2, detail::enable_if_writable<ET, ET2>> constexpr
vector<ET,OT>&
vector<ET,OT>::operator =(initializer_list<U> rhs)
{
    m_engine = rhs;
    return *this;
}

//----------
//- Capacity
//
template<class ET, class OT> constexpr
bool
vector<ET,OT>::is_resizable() noexcept
{
    return is_resizable_engine_v<ET>;
}

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
typename vector<ET,OT>::subvector_type
vector<ET,OT>::subvector(size_type i, size_type n) noexcept
{
    return subvector_type(detail::special_ctor_tag(), m_engine, i, n);
}

template<class ET, class OT> constexpr
typename vector<ET,OT>::const_subvector_type
vector<ET,OT>::subvector(size_type i, size_type n) const noexcept
{
    return const_subvector_type(detail::special_ctor_tag(), m_engine, i, n);
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

//-------------
//- Data access
//
template<class ET, class OT> constexpr
typename vector<ET,OT>::engine_type&
vector<ET,OT>::engine() noexcept
{
    return m_engine;
}

template<class ET, class OT> constexpr
typename vector<ET,OT>::engine_type const&
vector<ET,OT>::engine() const noexcept
{
    return m_engine;
}

#ifdef LA_USE_MDSPAN

template<class ET, class OT>
template<class ET2, detail::enable_if_spannable<ET, ET2>> constexpr
typename vector<ET,OT>::span_type
vector<ET,OT>::span() noexcept
{
    return m_engine.span();
}

template<class ET, class OT>
template<class ET2, detail::enable_if_spannable<ET, ET2>> constexpr
typename vector<ET,OT>::const_span_type
vector<ET,OT>::span() const noexcept
{
    return m_engine.span();
}

#endif

//-----------
//- Modifiers
//
template<class ET, class OT> constexpr
void
vector<ET,OT>::swap(vector& rhs) noexcept
{
    m_engine.swap(rhs);
}

template<class ET, class OT>
template<class ET2, detail::enable_if_writable<ET, ET2>> constexpr
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
    using lhs_size = typename ET1::size_type;
    using rhs_size = typename ET2::size_type;

    lhs_size    i1 = 0;
    lhs_size    e1 = v1.elements();

    rhs_size    i2 = 0;
    rhs_size    e2 = v2.elements();

    if (e1 != static_cast<lhs_size>(e2)) return false;

    for (;  i1 < e1;  ++i1, ++i2)
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
