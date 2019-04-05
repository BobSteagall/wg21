#ifndef LINEAR_ALGEBRA_DYNAMIC_ENGINES_HPP_DEFINED
#define LINEAR_ALGEBRA_DYNAMIC_ENGINES_HPP_DEFINED

#include "element_traits.hpp"

namespace STD_LA {
//=================================================================================================
//  Dynamically-resizable matrix engine.
//=================================================================================================
//
template<class T, class AT>
class dr_vector_engine
{
  public:
    using engine_category = resizable_matrix_engine_tag;
    using element_type    = T;
    using is_dense        = true_type;
    using is_rectangular  = true_type;
    using is_row_major    = true_type;
    using index_type      = ptrdiff_t;
    using size_type       = ptrdiff_t;

  public:
    ~dr_vector_engine();
    dr_vector_engine();
    dr_vector_engine(dr_vector_engine&&);
    dr_vector_engine(dr_vector_engine const&);
    dr_vector_engine(size_type elems);
    dr_vector_engine(size_type elems, size_type elem_cap);

    dr_vector_engine& operator =(dr_vector_engine&&);
    dr_vector_engine& operator =(dr_vector_engine const&);

    T           operator ()(index_type i) const;
    T const*    data() const noexcept;

    size_type   elements() const noexcept;
    size_type   capacity() const noexcept;

    T&      operator ()(index_type i);
    T*      data() noexcept;

    void    reserve(size_type elem_cap);
    void    resize(size_type elems);
    void    resize(size_type elems, size_type elem_cap);

    void    swap_elements(index_type i, index_type j);

  private:
    using pointer = typename allocator_traits<AT>::pointer;

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
dr_vector_engine<T,AT>::dr_vector_engine(dr_vector_engine&&)
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
T
dr_vector_engine<T,AT>::operator ()(index_type i) const
{
    return mp_elems[i];
}

template<class T, class AT> inline
T const*
dr_vector_engine<T,AT>::data() const noexcept
{
    return &mp_elems[0];
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
T&
dr_vector_engine<T,AT>::operator ()(index_type i)
{
    return mp_elems[i];
}

template<class T, class AT> inline
T*
dr_vector_engine<T,AT>::data() noexcept
{
    return &mp_elems[0];
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
dr_vector_engine<T,AT>::swap_elements(index_type, index_type)
{}


//=================================================================================================
//  Dynamically-resizable matrix engine.
//=================================================================================================
//
template<class T, class AT>
class dr_matrix_engine
{
  public:
    using engine_category = resizable_matrix_engine_tag;
    using element_type    = T;
    using is_dense        = true_type;
    using is_rectangular  = true_type;
    using is_row_major    = true_type;
    using index_type      = ptrdiff_t;
    using size_type       = ptrdiff_t;
    using size_tuple      = tuple<size_type, size_type>;

  public:
    ~dr_matrix_engine();
    dr_matrix_engine();
    dr_matrix_engine(dr_matrix_engine&&);
    dr_matrix_engine(dr_matrix_engine const&);
    dr_matrix_engine(size_type rows, size_type cols);
    dr_matrix_engine(size_type rows, size_type cols, size_type rowcap, size_type colcap);

    dr_matrix_engine& operator =(dr_matrix_engine&&);
    dr_matrix_engine& operator =(dr_matrix_engine const&);

    T           operator ()(size_type i, size_type j) const;
    T const*    data() const noexcept;

    size_type   columns() const noexcept;
    size_type   rows() const noexcept;
    size_tuple  size() const noexcept;

    size_type   column_capacity() const noexcept;
    size_type   row_capacity() const noexcept;
    size_tuple  capacity() const noexcept;

    T&      operator ()(size_type i, size_type j);
    T*      data() noexcept;

    void    reserve(size_type rowcap, size_type colcap);
    void    resize(size_type rows, size_type cols);
    void    resize(size_type rows, size_type cols, size_type rowcap, size_type colcap);

    void    swap_columns(index_type i, index_type j);
    void    swap_rows(index_type i, index_type j);

  private:
    using pointer = typename allocator_traits<AT>::pointer;

    pointer     mp_elems;       //- For exposition; data buffer
    size_type   m_rows;
    size_type   m_cols;
    size_type   m_rowcap;
    size_type   m_colcap;
};

template<class T, class AT> inline
dr_matrix_engine<T,AT>::~dr_matrix_engine()
{}

template<class T, class AT> inline
dr_matrix_engine<T,AT>::dr_matrix_engine()
{}

template<class T, class AT> inline
dr_matrix_engine<T,AT>::dr_matrix_engine(dr_matrix_engine&&)
{}

template<class T, class AT> inline
dr_matrix_engine<T,AT>::dr_matrix_engine(dr_matrix_engine const&)
{}

template<class T, class AT> inline
dr_matrix_engine<T,AT>::dr_matrix_engine(size_type, size_type)
{}

template<class T, class AT> inline
dr_matrix_engine<T,AT>::dr_matrix_engine(size_type, size_type, size_type, size_type)
{}

template<class T, class AT> inline
dr_matrix_engine<T,AT>&
dr_matrix_engine<T,AT>::operator =(dr_matrix_engine&&)
{
    return *this;
}

template<class T, class AT> inline
dr_matrix_engine<T,AT>&
dr_matrix_engine<T,AT>::operator =(dr_matrix_engine const&)
{
    return *this;
}

template<class T, class AT> inline
T
dr_matrix_engine<T,AT>::operator ()(size_type i, size_type j) const
{
    return mp_elems[i*m_colcap + j];
}

template<class T, class AT> inline
T const*
dr_matrix_engine<T,AT>::data() const noexcept
{
    return &mp_elems[0];
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
T&
dr_matrix_engine<T,AT>::operator ()(size_type i, size_type j)
{
    return mp_elems[i*m_colcap + j];
}

template<class T, class AT> inline
T*
dr_matrix_engine<T,AT>::data() noexcept
{
    return &mp_elems[0];
}

template<class T, class AT> inline
void
dr_matrix_engine<T,AT>::reserve(index_type, index_type)
{}

template<class T, class AT> inline
void
dr_matrix_engine<T,AT>::resize(index_type, index_type)
{}

template<class T, class AT> inline
void
dr_matrix_engine<T,AT>::resize(size_type, size_type, size_type, size_type)
{}

template<class T, class AT> inline
void
dr_matrix_engine<T,AT>::swap_columns(index_type, index_type)
{}

template<class T, class AT> inline
void
dr_matrix_engine<T,AT>::swap_rows(index_type, index_type)
{}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_DYNAMIC_ENGINES_HPP_DEFINED
