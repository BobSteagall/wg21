//==================================================================================================
//  File:       dynamic_matrix_engine.hpp
//
//  Summary:    This header defines a dynamically-resizable vector engine.  In this context,
//              dynamically-resizable means that the row and column extents of such objects
//              are determined at run-time.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_DYNAMIC_MATRIX_ENGINE_HPP_DEFINED
#define LINEAR_ALGEBRA_DYNAMIC_MATRIX_ENGINE_HPP_DEFINED

namespace STD_LA {
//==================================================================================================
//  Dynamically-resizable matrix engine.
//==================================================================================================
//
template<class T, class AT>
class dr_matrix_engine
{
  public:
    //- Types
    //
    using engine_category = resizable_matrix_engine_tag;
    using element_type    = T;
    using value_type      = remove_cv_t<T>;
    using allocator_type  = AT;
    using pointer         = typename allocator_traits<AT>::pointer;
    using const_pointer   = typename allocator_traits<AT>::const_pointer;
    using reference       = element_type&;
    using const_reference = element_type const&;
    using difference_type = ptrdiff_t;
    using size_type       = size_t;
    using size_tuple      = tuple<size_type, size_type>;

#ifdef LA_USE_MDSPAN
    using span_type       = basic_mdspan<T, detail::dyn_mat_extents, detail::dyn_mat_layout>;
    using const_span_type = basic_mdspan<T const, detail::dyn_mat_extents, detail::dyn_mat_layout>;
#endif

    //- Construct/copy/destroy
    //
    ~dr_matrix_engine() noexcept;

    dr_matrix_engine();
    dr_matrix_engine(dr_matrix_engine&& rhs) noexcept;
    dr_matrix_engine(dr_matrix_engine const& rhs);
    dr_matrix_engine(size_type rows, size_type cols);
    dr_matrix_engine(size_type rows, size_type cols, size_type rowcap, size_type colcap);
    template<class ET2, detail::enable_if_engine_has_convertible_element<ET2,T> = true>
    dr_matrix_engine(ET2 const& rhs);
    template<class T2, detail::enable_if_convertible_element<T2,T> = true>
    dr_matrix_engine(initializer_list<initializer_list<T2>> rhs);

    dr_matrix_engine&   operator =(dr_matrix_engine&&) noexcept;
    dr_matrix_engine&   operator =(dr_matrix_engine const&);
    template<class ET2, detail::enable_if_engine_has_convertible_element<ET2,T> = true>
    dr_matrix_engine&   operator =(ET2 const& rhs);
    template<class T2, detail::enable_if_convertible_element<T2,T> = true>
    dr_matrix_engine&   operator =(initializer_list<initializer_list<T2>> rhs);

    //- Capacity
    //
    size_type   columns() const noexcept;
    size_type   rows() const noexcept;
    size_tuple  size() const noexcept;

    size_type   column_capacity() const noexcept;
    size_type   row_capacity() const noexcept;
    size_tuple  capacity() const noexcept;

    void        reserve(size_type rowcap, size_type colcap);
    void        resize(size_type rows, size_type cols);
    void        resize(size_type rows, size_type cols, size_type rowcap, size_type colcap);

    //- Element access
    //
    reference       operator ()(size_type i, size_type j);
    const_reference operator ()(size_type i, size_type j) const;

    //- Data access
    //
#ifdef LA_USE_MDSPAN
    span_type       span() noexcept;
    const_span_type span() const noexcept;
#endif

    //- Modifiers
    //
    void    swap(dr_matrix_engine& other) noexcept;
    void    swap_columns(size_type c1, size_type c2) noexcept;
    void    swap_rows(size_type r1, size_type r2) noexcept;

  private:
    pointer         mp_elems;       //- For exposition; data buffer
    size_type       m_rows;
    size_type       m_cols;
    size_type       m_rowcap;
    size_type       m_colcap;
    allocator_type  m_alloc;

    void    alloc_new(size_type rows, size_type cols, size_type rowcap, size_type colcap);
    void    assign(dr_matrix_engine const& rhs);
    template<class ET2>
    void    assign(ET2 const& rhs);
    template<class T2>
    void    assign(initializer_list<initializer_list<T2>> rhs);
    void    check_capacities(size_type rowcap, size_type colcap);
    void    check_sizes(size_type rows, size_type cols);
    void    reshape(size_type rows, size_type cols, size_type rowcap, size_type colcap);
};

//------------------------
//- Construct/copy/destroy
//
template<class T, class AT> inline
dr_matrix_engine<T,AT>::~dr_matrix_engine() noexcept
{
    delete [] mp_elems;
}

template<class T, class AT>
dr_matrix_engine<T,AT>::dr_matrix_engine()
:   mp_elems(nullptr)
,   m_rows(0)
,   m_cols(0)
,   m_rowcap(0)
,   m_colcap(0)
,   m_alloc()
{}

template<class T, class AT>
dr_matrix_engine<T,AT>::dr_matrix_engine(dr_matrix_engine&& rhs) noexcept
:   dr_matrix_engine()
{
    rhs.swap(*this);
}

template<class T, class AT>
dr_matrix_engine<T,AT>::dr_matrix_engine(dr_matrix_engine const& rhs)
:   dr_matrix_engine()
{
    assign(rhs);
}

template<class T, class AT>
dr_matrix_engine<T,AT>::dr_matrix_engine(size_type rows, size_type cols)
:   dr_matrix_engine()
{
    alloc_new(rows, cols, rows, cols);
}

template<class T, class AT>
dr_matrix_engine<T,AT>::dr_matrix_engine
(size_type rows, size_type cols, size_type rowcap, size_type colcap)
:   dr_matrix_engine()
{
    alloc_new(rows, cols, rowcap, colcap);
}

template<class T, class AT>
template<class ET2, detail::enable_if_engine_has_convertible_element<ET2,T>>
dr_matrix_engine<T,AT>::dr_matrix_engine(ET2 const& rhs)
:   dr_matrix_engine()
{
    assign(rhs);
}

template<class T, class AT>
template<class T2, detail::enable_if_convertible_element<T2,T>> inline
dr_matrix_engine<T,AT>::dr_matrix_engine(initializer_list<initializer_list<T2>> rhs)
:   dr_matrix_engine()
{
    assign(rhs);
}

template<class T, class AT>
dr_matrix_engine<T,AT>&
dr_matrix_engine<T,AT>::operator =(dr_matrix_engine&& rhs) noexcept
{
    dr_matrix_engine    tmp;
    tmp.swap(rhs);
    tmp.swap(*this);
    return *this;
}

template<class T, class AT> inline
dr_matrix_engine<T,AT>&
dr_matrix_engine<T,AT>::operator =(dr_matrix_engine const& rhs)
{
    assign(rhs);
    return *this;
}

template<class T, class AT>
template<class ET2, detail::enable_if_engine_has_convertible_element<ET2,T>> inline
dr_matrix_engine<T,AT>&
dr_matrix_engine<T,AT>::operator =(ET2 const& rhs)
{
    assign(rhs);
    return *this;
}

template<class T, class AT>
template<class T2, detail::enable_if_convertible_element<T2,T>> inline
dr_matrix_engine<T,AT>&
dr_matrix_engine<T,AT>::operator =(initializer_list<initializer_list<T2>> rhs)
{
    assign(rhs);
    return *this;
}

//----------
//- Capacity
//
template<class T, class AT> inline
typename dr_matrix_engine<T,AT>::size_type
dr_matrix_engine<T,AT>::columns() const noexcept
{
    return m_cols;
}

template<class T, class AT> inline
typename dr_matrix_engine<T,AT>::size_type
dr_matrix_engine<T,AT>::rows() const noexcept
{
    return m_rows;
}

template<class T, class AT> inline
typename dr_matrix_engine<T,AT>::size_tuple
dr_matrix_engine<T,AT>::size() const noexcept
{
    return size_tuple(m_rows, m_cols);
}

template<class T, class AT> inline
typename dr_matrix_engine<T,AT>::size_type
dr_matrix_engine<T,AT>::column_capacity() const noexcept
{
    return m_colcap;
}

template<class T, class AT> inline
typename dr_matrix_engine<T,AT>::size_type
dr_matrix_engine<T,AT>::row_capacity() const noexcept
{
    return m_rowcap;
}

template<class T, class AT> inline
typename dr_matrix_engine<T,AT>::size_tuple
dr_matrix_engine<T,AT>::capacity() const noexcept
{
    return size_tuple(m_rowcap, m_colcap);
}

template<class T, class AT>
void
dr_matrix_engine<T,AT>::reserve(size_type rowcap, size_type colcap)
{
    reshape(m_rows, m_cols, rowcap, colcap);
}

template<class T, class AT>
void
dr_matrix_engine<T,AT>::resize(size_type rows, size_type cols)
{
    reshape(rows, cols, m_rowcap, m_colcap);
}

template<class T, class AT>
void
dr_matrix_engine<T,AT>::resize(size_type rows, size_type cols, size_type rowcap, size_type colcap)
{
    reshape(rows, cols, rowcap, colcap);
}

//----------------
//- Element access
//
template<class T, class AT> inline
typename dr_matrix_engine<T,AT>::reference
dr_matrix_engine<T,AT>::operator ()(size_type i, size_type j)
{
    return mp_elems[i*m_colcap + j];
}

template<class T, class AT> inline
typename dr_matrix_engine<T,AT>::const_reference
dr_matrix_engine<T,AT>::operator ()(size_type i, size_type j) const
{
    return mp_elems[i*m_colcap + j];
}

//-------------
//- Data access
//
#ifdef LA_USE_MDSPAN

template<class T, class AT> inline
typename dr_matrix_engine<T,AT>::span_type
dr_matrix_engine<T,AT>::span() noexcept
{
    return detail::make_dyn_span(static_cast<element_type*>(mp_elems), m_rows, m_cols, m_colcap);
}

template<class T, class AT> inline
typename dr_matrix_engine<T,AT>::const_span_type
dr_matrix_engine<T,AT>::span() const noexcept
{
    return detail::make_dyn_span(static_cast<element_type const*>(mp_elems), m_rows, m_cols, m_colcap);
}

#endif
//-----------
//- Modifiers
//
template<class T, class AT>
void
dr_matrix_engine<T,AT>::swap(dr_matrix_engine& other) noexcept
{
    if (&other != this)
    {
        detail::la_swap(mp_elems, other.mp_elems);
        detail::la_swap(m_rows,   other.m_rows);
        detail::la_swap(m_cols,   other.m_cols);
        detail::la_swap(m_rowcap, other.m_rowcap);
        detail::la_swap(m_colcap, other.m_colcap);
        detail::la_swap(m_alloc,  other.m_alloc);
    }
}

template<class T, class AT>
void
dr_matrix_engine<T,AT>::swap_columns(size_type c1, size_type c2) noexcept
{
    if (c1 != c2)
    {
        for (size_type i = 0;  i < m_rows;  ++i)
        {
            detail::la_swap(mp_elems[i*m_colcap + c1], mp_elems[i*m_colcap + c2]);
        }
    }
}

template<class T, class AT>
void
dr_matrix_engine<T,AT>::swap_rows(size_type r1, size_type r2) noexcept
{
    if (r1 != r2)
    {
        for (size_type j = 0;  j < m_cols;  ++j)
        {
            detail::la_swap(mp_elems[r1*m_colcap + j], mp_elems[r2*m_colcap + j]);
        }
    }
}

//------------------------
//- Private implementation
//
template<class T, class AT>
void
dr_matrix_engine<T,AT>::alloc_new(size_type rows, size_type cols, size_type rowcap, size_type colcap)
{
    check_sizes(rows, cols);
    check_capacities(rowcap, colcap);
    rowcap = max(rows, rowcap);
    colcap = max(cols, colcap);

    mp_elems = detail::allocate(m_alloc, (size_t)(rowcap*colcap));
    m_rows   = rows;
    m_cols   = cols;
    m_rowcap = rowcap;
    m_colcap = colcap;
}

template<class T, class AT>
void
dr_matrix_engine<T,AT>::assign(dr_matrix_engine const& rhs)
{
    if (&rhs == this) return;

    size_t      old_n = (size_t)(m_rowcap*m_colcap);
    size_t      new_n = (size_t)(rhs.m_rowcap*rhs.m_colcap);
    pointer     p_tmp = detail::allocate(m_alloc, new_n, rhs.mp_elems);

    detail::deallocate(m_alloc, mp_elems, old_n);
    mp_elems = p_tmp;
    m_rows   = rhs.m_rows;
    m_cols   = rhs.m_cols;
    m_rowcap = rhs.m_rowcap;
    m_colcap = rhs.m_colcap;
}

template<class T, class AT>
template<class ET2>
void
dr_matrix_engine<T,AT>::assign(ET2 const& rhs)
{
    static_assert(is_matrix_engine_v<ET2>);

    typename ET2::size_type     rows = static_cast<size_type>(rhs.rows());
    typename ET2::size_type     cols = static_cast<size_type>(rhs.columns());
    dr_matrix_engine            tmp(rows, cols);

    detail::assign_from_matrix_engine(tmp, rhs);
    tmp.swap(*this);
}

template<class T, class AT>
template<class T2>
void
dr_matrix_engine<T,AT>::assign(initializer_list<initializer_list<T2>> rhs)
{
    detail::check_source_init_list(rhs);

    size_type const     rows = static_cast<size_type>(rhs.size());
    size_type const     cols = static_cast<size_type>(rhs.begin()->size());
    dr_matrix_engine    tmp(rows, cols);

    detail::assign_from_matrix_list(tmp, rhs);
    tmp.swap(*this);
}

template<class T, class AT>
void
dr_matrix_engine<T,AT>::check_capacities(size_type rowcap, size_type colcap)
{
    if (rowcap < 0  || colcap < 0)
    {
        throw runtime_error("invalid capacity");
    }
}

template<class T, class AT>
void
dr_matrix_engine<T,AT>::check_sizes(size_type rows, size_type cols)
{
    if (rows < 1  || cols < 1)
    {
        throw runtime_error("invalid size");
    }
}

template<class T, class AT>
void
dr_matrix_engine<T,AT>::reshape(size_type rows, size_type cols, size_type rowcap, size_type colcap)
{
    if (rows > m_rowcap  ||  cols > m_colcap   ||  rowcap > m_rowcap  ||  colcap > m_colcap)
    {
        dr_matrix_engine   tmp(rows, cols, rowcap, colcap);
        size_type const    dst_rows = min(rows, m_rows);
        size_type const    dst_cols = min(cols, m_cols);

        for (size_type i = 0;  i < dst_rows;  ++i)
        {
            for (size_type j = 0;  j < dst_cols;  ++j)
            {
                tmp.mp_elems[i*tmp.m_colcap + j] = mp_elems[i*m_colcap + j];
            }
        }
        tmp.swap(*this);
    }
    else
    {
        check_sizes(rows, cols);
        check_capacities(rowcap, colcap);
        m_rows = rows;
        m_cols = cols;
    }
}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_DYNAMIC_MATRIX_ENGINE_HPP_DEFINED
