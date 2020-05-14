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
    using index_type      = ptrdiff_t;
    using index_tuple     = extents<dynamic_extent, dynamic_extent>;
    using span_type       = basic_mdspan<T, detail::dyn_mat_extents, detail::dyn_mat_layout>;
    using const_span_type = basic_mdspan<T const, detail::dyn_mat_extents, detail::dyn_mat_layout>;

    //- Construct/copy/destroy
    //
    ~dr_matrix_engine() noexcept;

    dr_matrix_engine();
    dr_matrix_engine(dr_matrix_engine&& rhs) noexcept;
    dr_matrix_engine(dr_matrix_engine const& rhs);
    dr_matrix_engine(index_type rows, index_type cols);
    dr_matrix_engine(index_type rows, index_type cols, index_type rowcap, index_type colcap);
    template<class ET2, detail::enable_if_engine_has_convertible_element<ET2,T> = true>
    dr_matrix_engine(ET2 const& rhs);
    template<class T2, detail::enable_if_convertible_element<T2,T> = true>
    dr_matrix_engine(initializer_list<initializer_list<T2>> rhs);

    dr_matrix_engine&   operator =(dr_matrix_engine&& rhs) noexcept;
    dr_matrix_engine&   operator =(dr_matrix_engine const& rhs);
    template<class ET2, detail::enable_if_engine_has_convertible_element<ET2,T> = true>
    dr_matrix_engine&   operator =(ET2 const& rhs);
    template<class T2, detail::enable_if_convertible_element<T2,T> = true>
    dr_matrix_engine&   operator =(initializer_list<initializer_list<T2>> rhs);

    //- Capacity
    //
    index_type  columns() const noexcept;
    index_type  rows() const noexcept;
    index_tuple size() const noexcept;

    index_type  column_capacity() const noexcept;
    index_type  row_capacity() const noexcept;
    index_tuple capacity() const noexcept;

    void        reserve(index_type rowcap, index_type colcap);
    void        resize(index_type rows, index_type cols);
    void        resize(index_type rows, index_type cols, index_type rowcap, index_type colcap);

    //- Element access
    //
    reference       operator ()(index_type i, index_type j);
    const_reference operator ()(index_type i, index_type j) const;

    //- Data access
    //
    span_type       span() noexcept;
    const_span_type span() const noexcept;

    //- Modifiers
    //
    void    swap(dr_matrix_engine& other) noexcept;
    void    swap_columns(index_type c1, index_type c2) noexcept;
    void    swap_rows(index_type r1, index_type r2) noexcept;

  private:
    pointer         mp_elems;
    index_type      m_rows;
    index_type      m_cols;
    index_type      m_rowcap;
    index_type      m_colcap;
    allocator_type  m_alloc;

    void    alloc_new(index_type rows, index_type cols, index_type rowcap, index_type colcap);
    void    assign(dr_matrix_engine const& rhs);
    template<class ET2>
    void    assign(ET2 const& rhs);
    template<class T2>
    void    assign(initializer_list<initializer_list<T2>> rhs);
    void    check_capacities(index_type rowcap, index_type colcap);
    void    check_sizes(index_type rows, index_type cols);
    void    reshape(index_type rows, index_type cols, index_type rowcap, index_type colcap);
};

//------------------------
//- Construct/copy/destroy
//
template<class T, class AT> inline
dr_matrix_engine<T,AT>::~dr_matrix_engine() noexcept
{
    detail::deallocate(m_alloc, mp_elems, m_rowcap*m_colcap);
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
dr_matrix_engine<T,AT>::dr_matrix_engine(index_type rows, index_type cols)
:   dr_matrix_engine()
{
    alloc_new(rows, cols, rows, cols);
}

template<class T, class AT>
dr_matrix_engine<T,AT>::dr_matrix_engine
(index_type rows, index_type cols, index_type rowcap, index_type colcap)
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
typename dr_matrix_engine<T,AT>::index_type
dr_matrix_engine<T,AT>::columns() const noexcept
{
    return m_cols;
}

template<class T, class AT> inline
typename dr_matrix_engine<T,AT>::index_type
dr_matrix_engine<T,AT>::rows() const noexcept
{
    return m_rows;
}

template<class T, class AT> inline
typename dr_matrix_engine<T,AT>::index_tuple
dr_matrix_engine<T,AT>::size() const noexcept
{
    return index_tuple(m_rows, m_cols);
}

template<class T, class AT> inline
typename dr_matrix_engine<T,AT>::index_type
dr_matrix_engine<T,AT>::column_capacity() const noexcept
{
    return m_colcap;
}

template<class T, class AT> inline
typename dr_matrix_engine<T,AT>::index_type
dr_matrix_engine<T,AT>::row_capacity() const noexcept
{
    return m_rowcap;
}

template<class T, class AT> inline
typename dr_matrix_engine<T,AT>::index_tuple
dr_matrix_engine<T,AT>::capacity() const noexcept
{
    return index_tuple(m_rowcap, m_colcap);
}

template<class T, class AT> inline
void
dr_matrix_engine<T,AT>::reserve(index_type rowcap, index_type colcap)
{
    reshape(m_rows, m_cols, rowcap, colcap);
}

template<class T, class AT> inline
void
dr_matrix_engine<T,AT>::resize(index_type rows, index_type cols)
{
    reshape(rows, cols, m_rowcap, m_colcap);
}

template<class T, class AT> inline
void
dr_matrix_engine<T,AT>::resize(index_type rows, index_type cols, index_type rowcap, index_type colcap)
{
    reshape(rows, cols, rowcap, colcap);
}

//----------------
//- Element access
//
template<class T, class AT> inline
typename dr_matrix_engine<T,AT>::reference
dr_matrix_engine<T,AT>::operator ()(index_type i, index_type j)
{
    return mp_elems[i*m_colcap + j];
}

template<class T, class AT> inline
typename dr_matrix_engine<T,AT>::const_reference
dr_matrix_engine<T,AT>::operator ()(index_type i, index_type j) const
{
    return mp_elems[i*m_colcap + j];
}

//-------------
//- Data access
//
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
dr_matrix_engine<T,AT>::swap_columns(index_type c1, index_type c2) noexcept
{
    if (c1 != c2)
    {
        for (index_type i = 0;  i < m_rows;  ++i)
        {
            detail::la_swap(mp_elems[i*m_colcap + c1], mp_elems[i*m_colcap + c2]);
        }
    }
}

template<class T, class AT>
void
dr_matrix_engine<T,AT>::swap_rows(index_type r1, index_type r2) noexcept
{
    if (r1 != r2)
    {
        for (index_type j = 0;  j < m_cols;  ++j)
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
dr_matrix_engine<T,AT>::alloc_new(index_type rows, index_type cols, index_type rowcap, index_type colcap)
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

    size_t      old_n = static_cast<size_t>(m_rowcap*m_colcap);
    size_t      new_n = static_cast<size_t>(rhs.m_rowcap*rhs.m_colcap);
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

    typename ET2::index_type    rows = static_cast<index_type>(rhs.rows());
    typename ET2::index_type    cols = static_cast<index_type>(rhs.columns());
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

    index_type const    rows = static_cast<index_type>(rhs.size());
    index_type const    cols = static_cast<index_type>(rhs.begin()->size());
    dr_matrix_engine    tmp(rows, cols);

    detail::assign_from_matrix_initlist(tmp, rhs);
    tmp.swap(*this);
}

template<class T, class AT>
void
dr_matrix_engine<T,AT>::check_capacities(index_type rowcap, index_type colcap)
{
    if (rowcap < 0  || colcap < 0)
    {
        throw runtime_error("invalid capacity");
    }
}

template<class T, class AT>
void
dr_matrix_engine<T,AT>::check_sizes(index_type rows, index_type cols)
{
    if (rows < 1  || cols < 1)
    {
        throw runtime_error("invalid size");
    }
}

template<class T, class AT>
void
dr_matrix_engine<T,AT>::reshape(index_type rows, index_type cols, index_type rowcap, index_type colcap)
{
    if (rows > m_rowcap  ||  cols > m_colcap   ||  rowcap > m_rowcap  ||  colcap > m_colcap)
    {
        dr_matrix_engine   tmp(rows, cols, rowcap, colcap);
        index_type const    dst_rows = min(rows, m_rows);
        index_type const    dst_cols = min(cols, m_cols);

        for (index_type i = 0;  i < dst_rows;  ++i)
        {
            for (index_type j = 0;  j < dst_cols;  ++j)
            {
                tmp(i, j) = (*this)(i, j);
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
