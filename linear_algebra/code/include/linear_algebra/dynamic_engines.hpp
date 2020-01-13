//==================================================================================================
//  File:       dynamic_engines.hpp
//
//  Summary:    This header defines dynamically-resizable vector and matrix engines.  In this
//              context, dynamically-resizable means that the row and column extents of such
//              objects are set at run-time, and thus unknown at compile-time.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_DYNAMIC_ENGINES_HPP_DEFINED
#define LINEAR_ALGEBRA_DYNAMIC_ENGINES_HPP_DEFINED

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
    using size_type       = size_t;
    using iterator        = detail::vector_iterator<dr_vector_engine>;
    using const_iterator  = detail::vector_const_iterator<dr_vector_engine>;

    //- Construct/copy/destroy
    //
    ~dr_vector_engine() noexcept;

    dr_vector_engine();
    dr_vector_engine(dr_vector_engine&&) noexcept;
    dr_vector_engine(dr_vector_engine const&);
    template<class U>
    dr_vector_engine(initializer_list<U> list);
    dr_vector_engine(size_type elems);
    dr_vector_engine(size_type elems, size_type elem_cap);

    dr_vector_engine&   operator =(dr_vector_engine&& rhs) noexcept;
    dr_vector_engine&   operator =(dr_vector_engine const& rhs);
    template<class ET2>
    dr_vector_engine&   operator =(ET2 const& rhs);

    //- Iterators
    //
    iterator        begin() noexcept;
    iterator        end() noexcept;
    const_iterator  begin() const noexcept;
    const_iterator  end() const noexcept;
    const_iterator  cbegin() const noexcept;
    const_iterator  cend() const noexcept;

    //- Capacity
    //
    size_type       capacity() const noexcept;
    size_type       elements() const noexcept;

    void    reserve(size_type cap);
    void    resize(size_type elems);
    void    resize(size_type elems, size_type cap);

    //- Element access
    //
    reference       operator ()(size_type i);
    const_reference operator ()(size_type i) const;

    //- Modifiers
    //
    void    swap(dr_vector_engine& rhs) noexcept;
    void    swap_elements(size_type i, size_type j) noexcept;

  private:
    pointer         mp_elems;
    size_type       m_elems;
    size_type       m_elemcap;
    allocator_type  m_alloc;

    void    alloc_new(size_type elems, size_type cap);
    void    assign(dr_vector_engine const& rhs);
    template<class ET2>
    void    assign(ET2 const& rhs);
    void    check_capacity(size_type cap);
    void    check_size(size_type elems);
    void    reshape(size_type elems, size_type cap);
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
:   mp_elems(nullptr)
,   m_elems(0)
,   m_elemcap(0)
{
    rhs.swap(*this);
}

template<class T, class AT> inline
dr_vector_engine<T,AT>::dr_vector_engine(dr_vector_engine const& rhs)
:   mp_elems(nullptr)
,   m_elems(0)
,   m_elemcap(0)
,   m_alloc()
{
    assign(rhs);
}

template<class T, class AT> 
template<class U> 
dr_vector_engine<T,AT>::dr_vector_engine(initializer_list<U> list)
:   mp_elems(nullptr)
,   m_elems(0)
,   m_elemcap(0)
,   m_alloc()
{
    alloc_new((size_type) list.size(), (size_type) list.size());

    auto    iter = list.begin();

    for (size_t i = 0;  i < list.size();  ++i, ++iter)
    {
        mp_elems[i] = static_cast<T>( *iter);
    }
}

template<class T, class AT> inline
dr_vector_engine<T,AT>::dr_vector_engine(size_type elems)
:   mp_elems(nullptr)
,   m_elems(0)
,   m_elemcap(0)
,   m_alloc()
{
    alloc_new(elems, elems);
}

template<class T, class AT> inline
dr_vector_engine<T,AT>::dr_vector_engine(size_type elems, size_type cap)
:   mp_elems(nullptr)
,   m_elems(0)
,   m_elemcap(0)
,   m_alloc()
{
    alloc_new(elems, cap);
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
template<class ET2> inline
dr_vector_engine<T,AT>&
dr_vector_engine<T,AT>::operator =(ET2 const& rhs)
{
    assign(rhs);
    return *this;
}

//-----------
//- Iterators
//
template<class T, class AT> inline
typename dr_vector_engine<T,AT>::iterator
dr_vector_engine<T,AT>::begin() noexcept
{
    return iterator(this, 0, m_elemcap);
}

template<class T, class AT> inline
typename dr_vector_engine<T,AT>::iterator
dr_vector_engine<T,AT>::end() noexcept
{
    return iterator(this, m_elemcap, m_elemcap);
}

template<class T, class AT> inline
typename dr_vector_engine<T,AT>::const_iterator
dr_vector_engine<T,AT>::begin() const noexcept
{
    return const_iterator(this, 0, m_elemcap);
}

template<class T, class AT> inline
typename dr_vector_engine<T,AT>::const_iterator
dr_vector_engine<T,AT>::end() const noexcept
{
    return const_iterator(this, m_elemcap, m_elemcap);
}

template<class T, class AT> inline
typename dr_vector_engine<T,AT>::const_iterator
dr_vector_engine<T,AT>::cbegin() const noexcept
{
    return const_iterator(this, 0, m_elemcap);
}

template<class T, class AT> inline
typename dr_vector_engine<T,AT>::const_iterator
dr_vector_engine<T,AT>::cend() const noexcept
{
    return const_iterator(this, m_elemcap, m_elemcap);
}

//----------
//- Capacity
//
template<class T, class AT> inline
typename dr_vector_engine<T,AT>::size_type
dr_vector_engine<T,AT>::capacity() const noexcept
{
    return m_elemcap;
}

template<class T, class AT> inline
typename dr_vector_engine<T,AT>::size_type
dr_vector_engine<T,AT>::elements() const noexcept
{
    return m_elems;
}

template<class T, class AT> inline
void
dr_vector_engine<T,AT>::reserve(size_type cap)
{
    reshape(m_elems, cap);
}

template<class T, class AT> inline
void
dr_vector_engine<T,AT>::resize(size_type elems)
{
    reshape(elems, m_elemcap);
}

template<class T, class AT> inline
void
dr_vector_engine<T,AT>::resize(size_type elems, size_type cap)
{
    reshape(elems, cap);
}

//----------------
//- Element access
//
template<class T, class AT> inline
typename dr_vector_engine<T,AT>::reference
dr_vector_engine<T,AT>::operator ()(size_type i)
{
    return mp_elems[i];
}

template<class T, class AT> inline
typename dr_vector_engine<T,AT>::const_reference
dr_vector_engine<T,AT>::operator ()(size_type i) const
{
    return mp_elems[i];
}

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
dr_vector_engine<T,AT>::swap_elements(size_type i, size_type j) noexcept
{
    detail::la_swap(mp_elems[i], mp_elems[j]);
}

//------------------------
//- Private implementation
//
template<class T, class AT>
void
dr_vector_engine<T,AT>::alloc_new(size_type new_size, size_type new_cap)
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

    size_type   old_n = (size_type)(m_elemcap);
    size_type   new_n = (size_type)(rhs.m_elemcap);
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
    using src_size_type = typename ET2::size_type;

    size_type           elems = (size_type) rhs.elements();
    dr_vector_engine    tmp(elems);

    if constexpr(is_same_v<size_type, src_size_type>)
    {
        for (size_type i = 0;  i < elems;  ++i)
        {
            tmp(i) = rhs(i);
        }
    }
    else
    {
        src_size_type  si;
        size_type       di;

        for (di = 0, si = 0;  di < elems;  ++di, ++si)
        {
            tmp(di) = rhs(si);
        }
    }

    tmp.swap(*this);
}

template<class T, class AT>
void
dr_vector_engine<T,AT>::check_capacity(size_type cap)
{
    if (cap < 0)
    {
        throw runtime_error("invalid capacity");
    }
}

template<class T, class AT>
void
dr_vector_engine<T,AT>::check_size(size_type elems)
{
    if (elems < 1)
    {
        throw runtime_error("invalid size");
    }
}

template<class T, class AT>
void
dr_vector_engine<T,AT>::reshape(size_type elems, size_type cap)
{
    if (elems > m_elemcap  ||  cap > m_elemcap)
    {
        dr_vector_engine    tmp(elems, cap);
        size_type const    dst_elems = min(elems, m_elems);

        for (size_type i = 0;  i < dst_elems;  ++i)
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

    //- Construct/copy/destroy
    //
    ~dr_matrix_engine() noexcept;

    dr_matrix_engine();
    dr_matrix_engine(dr_matrix_engine&& rhs) noexcept;
    dr_matrix_engine(dr_matrix_engine const& rhs);
    dr_matrix_engine(size_type rows, size_type cols);
    dr_matrix_engine(size_type rows, size_type cols, size_type rowcap, size_type colcap);

    dr_matrix_engine&   operator =(dr_matrix_engine&&) noexcept;
    dr_matrix_engine&   operator =(dr_matrix_engine const&);
    template<class ET2>
    dr_matrix_engine&   operator =(ET2 const& rhs);

    //- Capacity
    //
    size_type   columns() const noexcept;
    size_type   rows() const noexcept;
    size_tuple  size() const noexcept;

    size_type   column_capacity() const noexcept;
    size_type   row_capacity() const noexcept;
    size_tuple  capacity() const noexcept;

    void    reserve(size_type rowcap, size_type colcap);
    void    resize(size_type rows, size_type cols);
    void    resize(size_type rows, size_type cols, size_type rowcap, size_type colcap);

    //- Element access
    //
    reference           operator ()(size_type i, size_type j);
    const_reference     operator ()(size_type i, size_type j) const;

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
    void    check_capacities(size_type rowcap, size_type colcap);
    void    check_sizes(size_type rows, size_type cols);
    void    reshape(size_type rows, size_type cols, size_type rowcap, size_type colcap);
};

//------------------------
//- Construct/copy/destroy
//
template<class T, class AT>
dr_matrix_engine<T,AT>::~dr_matrix_engine()
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
:   mp_elems(nullptr)
,   m_rows(0)
,   m_cols(0)
,   m_rowcap(0)
,   m_colcap(0)
,   m_alloc()
{
    rhs.swap(*this);
}

template<class T, class AT>
dr_matrix_engine<T,AT>::dr_matrix_engine(dr_matrix_engine const& rhs)
:   mp_elems(nullptr)
,   m_rows(0)
,   m_cols(0)
,   m_rowcap(0)
,   m_colcap(0)
,   m_alloc()
{
    assign(rhs);
}

template<class T, class AT>
dr_matrix_engine<T,AT>::dr_matrix_engine(size_type rows, size_type cols)
:   mp_elems(nullptr)
,   m_rows(0)
,   m_cols(0)
,   m_rowcap(0)
,   m_colcap(0)
,   m_alloc()
{
    alloc_new(rows, cols, rows, cols);
}

template<class T, class AT>
dr_matrix_engine<T,AT>::dr_matrix_engine
(size_type rows, size_type cols, size_type rowcap, size_type colcap)
:   mp_elems(nullptr)
,   m_rows(0)
,   m_cols(0)
,   m_rowcap(0)
,   m_colcap(0)
,   m_alloc()
{
    alloc_new(rows, cols, rowcap, colcap);
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

template<class T, class AT>
dr_matrix_engine<T,AT>&
dr_matrix_engine<T,AT>::operator =(dr_matrix_engine const& rhs)
{
    assign(rhs);
    return *this;
}

template<class T, class AT>
template<class ET2>
dr_matrix_engine<T,AT>&
dr_matrix_engine<T,AT>::operator =(ET2 const& rhs)
{
    static_assert(is_matrix_engine_v<ET2>);
    using src_size_type = typename ET2::size_type;

    size_type           rows = (size_type) rhs.rows();
    size_type           cols = (size_type) rhs.columns();
    dr_matrix_engine    tmp(rows, cols);

    src_size_type   si, sj;
    size_type       di, dj;

    for (di = 0, si = 0;  di < rows;  ++di, ++si)
    {
        for (dj = 0, sj = 0;  dj < cols;  ++dj, ++sj)
        {
            tmp(di, dj) = rhs(si, sj);
        }
    }

    tmp.swap(*this);

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
        dr_matrix_engine    tmp(rows, cols, rowcap, colcap);
        size_type const    dst_rows = min(rows, m_rows);
        size_type const    dst_cols = min(cols, m_cols);

        for (size_type i = 0;  i < dst_rows;  ++i)
        {
            for (size_type j = 0;  j < dst_cols;  ++j)
            {
                tmp.mp_elems[i*m_colcap + j] = mp_elems[i*m_colcap + j];
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
#endif  //- LINEAR_ALGEBRA_DYNAMIC_ENGINES_HPP_DEFINED
