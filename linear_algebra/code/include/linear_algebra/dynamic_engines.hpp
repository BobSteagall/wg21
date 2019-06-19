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
namespace detail {
//--------------------------------------------------------------------------------------------------
//  Some private helper functions for allocating/deallocating the memory used by the dynamic
//  vector and matrix engines defined below.  Note that all memory thus allocated is default-
//  constructed.  This means that elements lying in (currently) unused capacity are also
//  initialized.
//--------------------------------------------------------------------------------------------------
//
template<class AT>
typename allocator_traits<AT>::pointer
allocate(AT& alloc, size_t n)
{
    auto    p_dst = allocator_traits<AT>::allocate(alloc, n);

    try
    {
        uninitialized_value_construct_n(p_dst, n);
    }
    catch (...)
    {
        allocator_traits<AT>::deallocate(alloc, p_dst, n);
        throw;
    }
    return p_dst;
}

//------
//
template<class AT>
typename allocator_traits<AT>::pointer
allocate(AT& alloc, size_t n, typename allocator_traits<AT>::const_pointer p_src)
{
    auto    p_dst = allocator_traits<AT>::allocate(alloc, n);

    try
    {
        uninitialized_copy_n(p_src, n, p_dst);
    }
    catch (...)
    {
        allocator_traits<AT>::deallocate(alloc, p_dst, n);
        throw;
    }
    return p_dst;
}

//------
//
template<class AT>
void
deallocate(AT& alloc, typename allocator_traits<AT>::pointer p_dst, size_t n) noexcept
{
    if (p_dst != nullptr)
    {
        destroy_n(p_dst, n);
        allocator_traits<AT>::deallocate(alloc, p_dst, n);
    }
}

}   //- detail namespace
//==================================================================================================
//  Dynamically-resizable matrix engine.
//==================================================================================================
//
template<class T, class AT>
class dr_vector_engine
{
    static_assert(is_matrix_element_v<T>);

  public:
    using engine_category = resizable_vector_engine_tag;
    using element_type    = T;
    using value_type      = T;
    using alloc_type      = AT;
    using reference       = T&;
    using pointer         = typename allocator_traits<AT>::pointer;
    using const_reference = T const&;
    using const_pointer   = typename allocator_traits<AT>::const_pointer;
    using iterator        = detail::vector_iterator<dr_vector_engine>;
    using const_iterator  = detail::vector_const_iterator<dr_vector_engine>;
    using difference_type = ptrdiff_t;
    using index_type      = ptrdiff_t;
    using size_type       = ptrdiff_t;

    using is_fixed_size   = false_type;
    using is_resizable    = true_type;

    using is_column_major = true_type;
    using is_dense        = true_type;
    using is_rectangular  = true_type;
    using is_row_major    = true_type;

  public:
    ~dr_vector_engine() noexcept;
    dr_vector_engine();
    template<class U>
    constexpr dr_vector_engine(initializer_list<U> list);
    dr_vector_engine(dr_vector_engine&&) noexcept;
    dr_vector_engine(dr_vector_engine const&);
    dr_vector_engine(size_type elems);
    dr_vector_engine(size_type elems, size_type elem_cap);

    dr_vector_engine& operator =(dr_vector_engine&&) noexcept;
    dr_vector_engine& operator =(dr_vector_engine const&);

    const_reference operator ()(index_type i) const;
    const_iterator  begin() const noexcept;
    const_iterator  end() const noexcept;

    size_type       elements() const noexcept;
    size_type       capacity() const noexcept;

    reference   operator ()(index_type i);
    iterator    begin() noexcept;
    iterator    end() noexcept;

    void    assign(dr_vector_engine const& rhs);
    template<class ET2>
    void    assign(ET2 const& rhs);

    void    reserve(size_type cap);
    void    resize(size_type elems);
    void    resize(size_type elems, size_type cap);
    void    swap(dr_vector_engine& rhs) noexcept;
    void    swap_elements(index_type i, index_type j) noexcept;

  private:
    pointer     mp_elems;
    size_type   m_elems;
    size_type   m_elemcap;
    alloc_type  m_alloc;

    void    alloc_new(size_type elems, size_type cap);
    void    check_capacity(size_type cap);
    void    check_size(size_type elems);
    void    reshape(size_type elems, size_type cap);
};

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

template<class T, class AT> 
template<class U>
constexpr
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

template<class T, class AT> inline
typename dr_vector_engine<T,AT>::const_reference
dr_vector_engine<T,AT>::operator ()(index_type i) const
{
    return mp_elems[i];
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
typename dr_vector_engine<T,AT>::size_type
dr_vector_engine<T,AT>::elements() const noexcept
{
    return m_elems;
}

template<class T, class AT> inline
typename dr_vector_engine<T,AT>::size_type
dr_vector_engine<T,AT>::capacity() const noexcept
{
    return m_elemcap;
}

template<class T, class AT> inline
typename dr_vector_engine<T,AT>::reference
dr_vector_engine<T,AT>::operator ()(index_type i)
{
    return mp_elems[i];
}

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

template<class T, class AT>
void
dr_vector_engine<T,AT>::assign(dr_vector_engine const& rhs)
{
    if (&rhs == this) return;

    size_t      old_n = (size_t)(m_elemcap);
    size_t      new_n = (size_t)(rhs.m_elemcap);
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
    static_assert(detail::is_vector_engine_v<ET2>);
    using src_index_type = typename ET2::index_type;

    index_type          elems = (index_type) rhs.elements();
    dr_vector_engine    tmp(elems);

    if constexpr(is_same_v<index_type, src_index_type>)
    {
        for (index_type i = 0;  i < elems;  ++i)
        {
            tmp(i) = rhs(i);
        }
    }
    else
    {
        src_index_type  si;
        index_type      di;

        for (di = 0, si = 0;  di < elems;  ++di, ++si)
        {
            tmp(di) = rhs(si);
        }
    }

    tmp.swap(*this);
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
        index_type const    dst_elems = min(elems, m_elems);

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


//==================================================================================================
//  Dynamically-resizable matrix engine.
//==================================================================================================
//
template<class T, class AT>
class dr_matrix_engine
{
    static_assert(is_matrix_element_v<T>);

  public:
    using engine_category = resizable_matrix_engine_tag;
    using element_type    = T;
    using value_type      = T;
    using allocator_type  = AT;
    using reference       = T&;
    using pointer         = typename allocator_traits<AT>::pointer;
    using const_reference = T const&;
    using const_pointer   = typename allocator_traits<AT>::const_pointer;
    using difference_type = ptrdiff_t;
    using index_type      = ptrdiff_t;
    using size_type       = ptrdiff_t;
    using size_tuple      = tuple<size_type, size_type>;

    using is_fixed_size   = false_type;
    using is_resizable    = true_type;

    using is_column_major = false_type;
    using is_dense        = true_type;
    using is_rectangular  = true_type;
    using is_row_major    = true_type;

    using column_view_type    = matrix_column_view<dr_matrix_engine>;
    using row_view_type       = matrix_row_view<dr_matrix_engine>;
    using transpose_view_type = matrix_transpose_view<dr_matrix_engine>;

  public:
    ~dr_matrix_engine();
    dr_matrix_engine();
    dr_matrix_engine(dr_matrix_engine&& rhs) noexcept;
    dr_matrix_engine(dr_matrix_engine const& rhs);
    dr_matrix_engine(size_type rows, size_type cols);
    dr_matrix_engine(size_type rows, size_type cols, size_type rowcap, size_type colcap);

    dr_matrix_engine& operator =(dr_matrix_engine&&) noexcept;
    dr_matrix_engine& operator =(dr_matrix_engine const&);

    const_reference     operator ()(index_type i, index_type j) const;

    size_type   columns() const noexcept;
    size_type   rows() const noexcept;
    size_tuple  size() const noexcept;

    size_type   column_capacity() const noexcept;
    size_type   row_capacity() const noexcept;
    size_tuple  capacity() const noexcept;

    reference   operator ()(index_type i, index_type j);

    void    assign(dr_matrix_engine const& rhs);
    template<class ET2>
    void    assign(ET2 const& rhs);

    void    reserve(size_type rowcap, size_type colcap);
    void    resize(size_type rows, size_type cols);
    void    resize(size_type rows, size_type cols, size_type rowcap, size_type colcap);
    void    swap(dr_matrix_engine& other) noexcept;
    void    swap_columns(index_type c1, index_type c2) noexcept;
    void    swap_rows(index_type r1, index_type r2) noexcept;

  private:
    pointer         mp_elems;       //- For exposition; data buffer
    size_type       m_rows;
    size_type       m_cols;
    size_type       m_rowcap;
    size_type       m_colcap;
    allocator_type  m_alloc;

    void    alloc_new(size_type rows, size_type cols, size_type rowcap, size_type colcap);
    void    check_capacities(size_type rowcap, size_type colcap);
    void    check_sizes(size_type rows, size_type cols);
    void    reshape(size_type rows, size_type cols, size_type rowcap, size_type colcap);
};

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

template<class T, class AT> inline
typename dr_matrix_engine<T,AT>::const_reference
dr_matrix_engine<T,AT>::operator ()(index_type i, index_type j) const
{
    return mp_elems[i*m_colcap + j];
}

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

template<class T, class AT> inline
typename dr_matrix_engine<T,AT>::reference
dr_matrix_engine<T,AT>::operator ()(index_type i, index_type j)
{
    return mp_elems[i*m_colcap + j];
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
    static_assert(detail::is_matrix_engine_v<ET2>);
    using src_index_type = typename ET2::index_type;

    index_type          rows = (index_type) rhs.rows();
    index_type          cols = (index_type) rhs.columns();
    dr_matrix_engine    tmp(rows, cols);

    if constexpr(is_same_v<index_type, src_index_type>)
    {
        for (index_type i = 0;  i < rows;  ++i)
        {
            for (index_type j = 0;  j < cols;  ++j)
            {
                tmp(i, j) = rhs(i, j);
            }
        }
    }
    else
    {
        src_index_type  si, sj;
        index_type      di, dj;

        for (di = 0, si = 0;  di < rows;  ++di, ++si)
        {
            for (dj = 0, sj = 0;  dj < cols;  ++dj, ++sj)
            {
                tmp(di, dj) = rhs(si, sj);
            }
        }
    }
    tmp.swap(*this);
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
        index_type const    dst_rows = min(rows, m_rows);
        index_type const    dst_cols = min(cols, m_cols);

        for (index_type i = 0;  i < dst_rows;  ++i)
        {
            for (index_type j = 0;  j < dst_cols;  ++j)
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
