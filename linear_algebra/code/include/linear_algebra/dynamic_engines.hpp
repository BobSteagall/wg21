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
//  Dynamically-resizable matrix engine.
//==================================================================================================
//
template<class T, class AT>
class dr_vector_engine
{
  public:
    using engine_category = resizable_vector_engine_tag;
    using element_type    = T;
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
    ~dr_vector_engine();
    dr_vector_engine();
    dr_vector_engine(dr_vector_engine&&) noexcept;
    dr_vector_engine(dr_vector_engine const&);
    dr_vector_engine(size_type elems);
    dr_vector_engine(size_type elems, size_type elem_cap);

    dr_vector_engine& operator =(dr_vector_engine&&);
    dr_vector_engine& operator =(dr_vector_engine const&);

    const_reference operator ()(index_type i) const;
    const_pointer   data() const noexcept;
    const_iterator  begin() const noexcept;
    const_iterator  end() const noexcept;

    size_type       elements() const noexcept;
    size_type       capacity() const noexcept;


    reference   operator ()(index_type i);
    pointer     data() noexcept;
    iterator    begin() noexcept;
    iterator    end() noexcept;

    void        reserve(size_type elem_cap);
    void        resize(size_type elems);
    void        resize(size_type elems, size_type elem_cap);
    void        swap_elements(index_type i, index_type j);

  private:
    pointer     mp_elems;       //- For exposition; data buffer
    size_type   m_elems;
    size_type   m_elemcap;
};

template<class T, class AT> inline
dr_vector_engine<T,AT>::~dr_vector_engine()
{}

template<class T, class AT> inline
dr_vector_engine<T,AT>::dr_vector_engine()
{}

template<class T, class AT> inline
dr_vector_engine<T,AT>::dr_vector_engine(dr_vector_engine&&) noexcept
{}

template<class T, class AT> inline
dr_vector_engine<T,AT>::dr_vector_engine(dr_vector_engine const&)
{}

template<class T, class AT> inline
dr_vector_engine<T,AT>::dr_vector_engine(size_type)
{}

template<class T, class AT> inline
dr_vector_engine<T,AT>::dr_vector_engine(size_type, size_type)
{}

template<class T, class AT> inline
dr_vector_engine<T,AT>&
dr_vector_engine<T,AT>::operator =(dr_vector_engine&&)
{
    return *this;
}

template<class T, class AT> inline
dr_vector_engine<T,AT>&
dr_vector_engine<T,AT>::operator =(dr_vector_engine const&)
{
    return *this;
}

template<class T, class AT> inline
typename dr_vector_engine<T,AT>::const_reference
dr_vector_engine<T,AT>::operator ()(index_type i) const
{
    return mp_elems[i];
}

template<class T, class AT> inline
typename dr_vector_engine<T,AT>::const_pointer
dr_vector_engine<T,AT>::data() const noexcept
{
    return &mp_elems[0];
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
typename dr_vector_engine<T,AT>::pointer
dr_vector_engine<T,AT>::data() noexcept
{
    return &mp_elems[0];
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

template<class T, class AT> inline
void
dr_vector_engine<T,AT>::reserve(size_type)
{}

template<class T, class AT> inline
void
dr_vector_engine<T,AT>::resize(size_type)
{}

template<class T, class AT> inline
void
dr_vector_engine<T,AT>::resize(size_type, size_type)
{}

template<class T, class AT> inline
void
dr_vector_engine<T,AT>::swap_elements(index_type i, index_type j)
{
    std::swap(mp_elems[i], mp_elems[j]);
}


//==================================================================================================
//  Dynamically-resizable matrix engine.
//==================================================================================================
//
template<class T, class AT>
class dr_matrix_engine
{
  public:
    using engine_category = resizable_matrix_engine_tag;
    using element_type    = T;
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
    dr_matrix_engine(dr_matrix_engine&& rhs);
    dr_matrix_engine(dr_matrix_engine const& rhs);
    dr_matrix_engine(size_type rows, size_type cols);
    dr_matrix_engine(size_type rows, size_type cols, size_type rowcap, size_type colcap);

    dr_matrix_engine& operator =(dr_matrix_engine&&);
    dr_matrix_engine& operator =(dr_matrix_engine const&);

    const_reference     operator ()(size_type i, size_type j) const;
    const_pointer       data() const noexcept;

    size_type   columns() const noexcept;
    size_type   rows() const noexcept;
    size_tuple  size() const noexcept;

    size_type   column_capacity() const noexcept;
    size_type   row_capacity() const noexcept;
    size_tuple  capacity() const noexcept;

    reference   operator ()(size_type i, size_type j);
    pointer     data() noexcept;

    void    assign(dr_matrix_engine const& rhs);
    void    reserve(size_type rowcap, size_type colcap);
    void    resize(size_type rows, size_type cols);
    void    resize(size_type rows, size_type cols, size_type rowcap, size_type colcap);
    void    swap(dr_matrix_engine& other) noexcept;
    void    swap_columns(index_type c1, index_type c2);
    void    swap_rows(index_type r1, index_type r2);

  private:
    pointer     mp_elems;       //- For exposition; data buffer
    size_type   m_rows;
    size_type   m_cols;
    size_type   m_rowcap;
    size_type   m_colcap;

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
{
    alloc_new(1, 1, 1, 1);
}

template<class T, class AT>
dr_matrix_engine<T,AT>::dr_matrix_engine(dr_matrix_engine&& rhs)
:   mp_elems(nullptr)
{
    alloc_new(1, 1, 1, 1);
    rhs.swap(*this);
}

template<class T, class AT>
dr_matrix_engine<T,AT>::dr_matrix_engine(dr_matrix_engine const& rhs)
:   mp_elems(nullptr)
{
    assign(rhs);
}

template<class T, class AT>
dr_matrix_engine<T,AT>::dr_matrix_engine(size_type rows, size_type cols)
:   mp_elems(nullptr)
{
    alloc_new(rows, cols, rows, cols);
}

template<class T, class AT>
dr_matrix_engine<T,AT>::dr_matrix_engine
(size_type rows, size_type cols, size_type rowcap, size_type colcap)
:   mp_elems(nullptr)
{
    alloc_new(rows, cols, rowcap, colcap);
}

template<class T, class AT>
dr_matrix_engine<T,AT>&
dr_matrix_engine<T,AT>::operator =(dr_matrix_engine&& rhs)
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
dr_matrix_engine<T,AT>::operator ()(size_type i, size_type j) const
{
    return mp_elems[i*m_colcap + j];
}

template<class T, class AT> inline
typename dr_matrix_engine<T,AT>::const_pointer
dr_matrix_engine<T,AT>::data() const noexcept
{
    return static_cast<T const*>(&mp_elems[0]);
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
dr_matrix_engine<T,AT>::operator ()(size_type i, size_type j)
{
    return mp_elems[i*m_colcap + j];
}

template<class T, class AT> inline
typename dr_matrix_engine<T,AT>::pointer
dr_matrix_engine<T,AT>::data() noexcept
{
    return static_cast<T*>(&mp_elems[0]);
}

template<class T, class AT>
void
dr_matrix_engine<T,AT>::assign(dr_matrix_engine const& rhs)
{
    if (&rhs != this)
    {
        dr_matrix_engine    tmp(rhs.m_rows, rhs.m_cols, rhs.m_rowcap, rhs.m_colcap);
        copy(rhs.data(), rhs.data() + rhs.m_rowcap*rhs.m_colcap, tmp.data());
        tmp.swap(*this);
    }
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
        std::swap(mp_elems, other.mp_elems);
        std::swap(m_rows,   other.m_rows);
        std::swap(m_cols,   other.m_cols);
        std::swap(m_rowcap, other.m_rowcap);
        std::swap(m_colcap, other.m_colcap);
    }
}

template<class T, class AT>
void
dr_matrix_engine<T,AT>::swap_columns(index_type c1, index_type c2)
{
    if (c1 != c2)
    {
        for (index_type i = 0;  i < m_rows;  ++i)
        {
            std::swap(mp_elems[i*m_colcap + c1], mp_elems[i*m_colcap + c2]);
        }
    }
}

template<class T, class AT>
void
dr_matrix_engine<T,AT>::swap_rows(index_type r1, index_type r2)
{
    if (r1 != r2)
    {
        for (index_type j = 0;  j < m_cols;  ++j)
        {
            std::swap(mp_elems[r1*m_colcap + j], mp_elems[r2*m_colcap + j]);
        }
    }
}

template<class T, class AT>
void
dr_matrix_engine<T,AT>::alloc_new(size_type rows, size_type cols, size_type rowcap, size_type colcap)
{
    check_sizes(rows, cols);
    check_capacities(rowcap, colcap);

    m_rows   = rows;
    m_cols   = cols;
    m_rowcap = max(rows, rowcap);
    m_colcap = max(cols, colcap);
    mp_elems = new T[m_rowcap*m_colcap]();
}

template<class T, class AT>
void
dr_matrix_engine<T,AT>::check_capacities(size_type rowcap, size_type colcap)
{
    if (rowcap < 1  || colcap < 1)
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
                tmp(i, j) = (*this)(i, j);
            }
        }
        tmp.swap(*this);
    }
    else
    {
        check_sizes(rows, cols);
        check_capacities(rowcap, colcap);
        m_rows   = rows;
        m_cols   = cols;
        m_rowcap = max(m_rowcap, rowcap);
        m_colcap = max(m_colcap, colcap);
    }
}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_DYNAMIC_ENGINES_HPP_DEFINED
