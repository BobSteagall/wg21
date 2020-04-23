//==================================================================================================
//  File:       dynamic_vector_engine.hpp
//
//  Summary:    This header defines a dynamically-resizable vector engine.  In this context,
//              dynamically-resizable means that the extents of such objects are determined
//              at run-time.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_DYNAMIC_VECTOR_ENGINE_HPP_DEFINED
#define LINEAR_ALGEBRA_DYNAMIC_VECTOR_ENGINE_HPP_DEFINED

namespace STD_LA {
//==================================================================================================
//  Dynamically-resizable vector engine.
//==================================================================================================
//
template<class T, class AT>
class dr_vector_engine
{
  public:
    //- Types
    //
    using engine_category = resizable_vector_engine_tag;
    using element_type    = T;
    using value_type      = remove_cv_t<T>;
    using allocator_type  = AT;
    using pointer         = typename allocator_traits<AT>::pointer;
    using const_pointer   = typename allocator_traits<AT>::const_pointer;
    using reference       = element_type&;
    using const_reference = element_type const&;
    using difference_type = ptrdiff_t;
    using index_type      = ptrdiff_t;

#ifdef LA_USE_MDSPAN
    using span_type       = mdspan<element_type, dynamic_extent>;
    using const_span_type = mdspan<element_type const, dynamic_extent>;
#endif

    //- Construct/copy/destroy
    //
    ~dr_vector_engine() noexcept;

    dr_vector_engine();
    dr_vector_engine(dr_vector_engine&& rhs) noexcept;
    dr_vector_engine(dr_vector_engine const& rhs);
    dr_vector_engine(index_type elems);
    dr_vector_engine(index_type elems, index_type elem_cap);
    template<class ET2, detail::enable_if_engine_has_convertible_element<ET2,T> = true>
    dr_vector_engine(ET2 const& rhs);
    template<class T2, detail::enable_if_convertible_element<T2,T> = true>
    dr_vector_engine(initializer_list<T2> rhs);

    dr_vector_engine&   operator =(dr_vector_engine&& rhs) noexcept;
    dr_vector_engine&   operator =(dr_vector_engine const& rhs);
    template<class ET2, detail::enable_if_engine_has_convertible_element<ET2,T> = true>
    dr_vector_engine&   operator =(ET2 const& rhs);
    template<class T2, detail::enable_if_convertible_element<T2,T> = true>
    dr_vector_engine&   operator =(initializer_list<T2> rhs);

    //- Capacity
    //
    index_type  capacity() const noexcept;
    index_type  size() const noexcept;

    void        reserve(index_type cap);
    void        resize(index_type elems);
    void        resize(index_type elems, index_type cap);

    //- Element access
    //
    reference       operator ()(index_type i);
    const_reference operator ()(index_type i) const;

    //- Data access
    //
#ifdef LA_USE_MDSPAN
    span_type       span() noexcept;
    const_span_type span() const noexcept;
#endif

    //- Modifiers
    //
    void    swap(dr_vector_engine& rhs) noexcept;
    void    swap_elements(index_type i, index_type j) noexcept;

  private:
    pointer         mp_elems;
    index_type       m_elems;
    index_type       m_elemcap;
    allocator_type  m_alloc;

    void    alloc_new(index_type elems, index_type cap);
    void    assign(dr_vector_engine const& rhs);
    template<class ET2>
    void    assign(ET2 const& rhs);
    template<class T2>
    void    assign(initializer_list<T2> rhs);
    void    check_capacity(index_type cap);
    void    check_size(index_type elems);
    void    reshape(index_type elems, index_type cap);
};

//------------------------
//- Construct/copy/destroy
//
template<class T, class AT> inline
dr_vector_engine<T,AT>::~dr_vector_engine() noexcept
{
    detail::deallocate(m_alloc, mp_elems, m_elemcap);
}

template<class T, class AT> inline
dr_vector_engine<T,AT>::dr_vector_engine()
:   mp_elems(nullptr)
,   m_elems(0)
,   m_elemcap(0)
,   m_alloc()
{}

template<class T, class AT> inline
dr_vector_engine<T,AT>::dr_vector_engine(dr_vector_engine&& rhs) noexcept
:   dr_vector_engine()
{
    rhs.swap(*this);
}

template<class T, class AT> inline
dr_vector_engine<T,AT>::dr_vector_engine(dr_vector_engine const& rhs)
:   dr_vector_engine()
{
    assign(rhs);
}

template<class T, class AT> inline
dr_vector_engine<T,AT>::dr_vector_engine(index_type elems)
:   dr_vector_engine()
{
    alloc_new(elems, elems);
}

template<class T, class AT> inline
dr_vector_engine<T,AT>::dr_vector_engine(index_type elems, index_type cap)
:   dr_vector_engine()
{
    alloc_new(elems, cap);
}

template<class T, class AT>
template<class ET2, detail::enable_if_engine_has_convertible_element<ET2,T>> inline
dr_vector_engine<T,AT>::dr_vector_engine(ET2 const& rhs)
:   dr_vector_engine()
{
    assign(rhs);
}

template<class T, class AT>
template<class T2, detail::enable_if_convertible_element<T2,T>>
dr_vector_engine<T,AT>::dr_vector_engine(initializer_list<T2> rhs)
:   dr_vector_engine()
{
    assign(rhs);
}

template<class T, class AT> inline
dr_vector_engine<T,AT>&
dr_vector_engine<T,AT>::operator =(dr_vector_engine&& rhs) noexcept
{
    dr_vector_engine    tmp;
    tmp.swap(rhs);
    tmp.swap(*this);
    return *this;
}

template<class T, class AT> inline
dr_vector_engine<T,AT>&
dr_vector_engine<T,AT>::operator =(dr_vector_engine const& rhs)
{
    assign(rhs);
    return *this;
}

template<class T, class AT>
template<class ET2, detail::enable_if_engine_has_convertible_element<ET2,T>> inline
dr_vector_engine<T,AT>&
dr_vector_engine<T,AT>::operator =(ET2 const& rhs)
{
    assign(rhs);
    return *this;
}

template<class T, class AT>
template<class T2, detail::enable_if_convertible_element<T2,T>> inline
dr_vector_engine<T,AT>&
dr_vector_engine<T,AT>::operator =(initializer_list<T2> rhs)
{
    assign(rhs);
    return *this;
}

//----------
//- Capacity
//
template<class T, class AT> inline
typename dr_vector_engine<T,AT>::index_type
dr_vector_engine<T,AT>::capacity() const noexcept
{
    return m_elemcap;
}

template<class T, class AT> inline
typename dr_vector_engine<T,AT>::index_type
dr_vector_engine<T,AT>::size() const noexcept
{
    return m_elems;
}

template<class T, class AT> inline
void
dr_vector_engine<T,AT>::reserve(index_type cap)
{
    reshape(m_elems, cap);
}

template<class T, class AT> inline
void
dr_vector_engine<T,AT>::resize(index_type elems)
{
    reshape(elems, m_elemcap);
}

template<class T, class AT> inline
void
dr_vector_engine<T,AT>::resize(index_type elems, index_type cap)
{
    reshape(elems, cap);
}

//----------------
//- Element access
//
template<class T, class AT> inline
typename dr_vector_engine<T,AT>::reference
dr_vector_engine<T,AT>::operator ()(index_type i)
{
    return mp_elems[i];
}

template<class T, class AT> inline
typename dr_vector_engine<T,AT>::const_reference
dr_vector_engine<T,AT>::operator ()(index_type i) const
{
    return mp_elems[i];
}

//-------------
//- Data access
//
#ifdef LA_USE_MDSPAN

template<class T, class AT> inline
typename dr_vector_engine<T,AT>::span_type
dr_vector_engine<T,AT>::span() noexcept
{
    return span_type(static_cast<element_type*>(mp_elems),
                     static_cast<ptrdiff_t>(m_elems));
}

template<class T, class AT> inline
typename dr_vector_engine<T,AT>::const_span_type
dr_vector_engine<T,AT>::span() const noexcept
{
    return const_span_type(static_cast<element_type const*>(mp_elems),
                           static_cast<ptrdiff_t>(m_elems));
}

#endif
//-----------
//- Modifiers
//
template<class T, class AT> inline
void
dr_vector_engine<T,AT>::swap(dr_vector_engine& other) noexcept
{
    if (&other != this)
    {
        detail::la_swap(mp_elems,  other.mp_elems);
        detail::la_swap(m_elems,   other.m_elems);
        detail::la_swap(m_elemcap, other.m_elemcap);
    }
}

template<class T, class AT> inline
void
dr_vector_engine<T,AT>::swap_elements(index_type i, index_type j) noexcept
{
    detail::la_swap(mp_elems[i], mp_elems[j]);
}

//------------------------
//- Private implementation
//
template<class T, class AT>
void
dr_vector_engine<T,AT>::alloc_new(index_type new_size, index_type new_cap)
{
    check_size(new_size);
    check_capacity(new_cap);

    new_cap   = max(new_size, new_cap);
    mp_elems  = detail::allocate(m_alloc, new_cap);
    m_elems   = new_size;
    m_elemcap = new_cap;
}

template<class T, class AT>
void
dr_vector_engine<T,AT>::assign(dr_vector_engine const& rhs)
{
    if (&rhs == this) return;

    index_type   old_n = m_elemcap;
    index_type   new_n = (index_type)(rhs.m_elemcap);
    pointer     p_tmp = detail::allocate(m_alloc, new_n, rhs.mp_elems);

    detail::deallocate(m_alloc, mp_elems, old_n);
    mp_elems  = p_tmp;
    m_elems   = rhs.m_elems;
    m_elemcap = rhs.m_elemcap;
}

template<class T, class AT>
template<class ET2>
void
dr_vector_engine<T,AT>::assign(ET2 const& rhs)
{
    static_assert(is_vector_engine_v<ET2>);

    dr_vector_engine    tmp(static_cast<index_type>(rhs.size()));

    detail::assign_from_vector_engine(tmp, rhs);
    tmp.swap(*this);
}

template<class T, class AT>
template<class T2>
void
dr_vector_engine<T,AT>::assign(initializer_list<T2> rhs)
{
    dr_vector_engine    tmp(static_cast<index_type>(rhs.size()));

    detail::assign_from_vector_list(tmp, rhs);
    tmp.swap(*this);
}

template<class T, class AT>
void
dr_vector_engine<T,AT>::check_capacity(index_type cap)
{
    if (cap < 0)
    {
        throw runtime_error("invalid capacity");
    }
}

template<class T, class AT>
void
dr_vector_engine<T,AT>::check_size(index_type elems)
{
    if (elems < 1)
    {
        throw runtime_error("invalid size");
    }
}

template<class T, class AT>
void
dr_vector_engine<T,AT>::reshape(index_type elems, index_type cap)
{
    if (elems > m_elemcap  ||  cap > m_elemcap)
    {
        dr_vector_engine    tmp(elems, cap);
        index_type const     dst_elems = min(elems, m_elems);

        for (index_type i = 0;  i < dst_elems;  ++i)
        {
            tmp.mp_elems[i] = mp_elems[i];
        }
        tmp.swap(*this);
    }
    else
    {
        check_size(elems);
        m_elems = elems;
    }
}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_DYNAMIC_VECTOR_ENGINE_HPP_DEFINED
