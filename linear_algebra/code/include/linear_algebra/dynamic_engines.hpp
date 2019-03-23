#ifndef LINEAR_ALGEBRA_DYNAMIC_ENGINES_HPP_DEFINED
#define LINEAR_ALGEBRA_DYNAMIC_ENGINES_HPP_DEFINED

#include "element_promotion_traits.hpp"

//- Experimental namespace for test implementation
//
namespace STD_LA
{
//=================================================================================================
//  Dynamically-resizable matrix engine.
//=================================================================================================
//
template<class T, class AT>
class dr_matrix_engine
{
  public:
    using element_type   = T;
    using is_dense       = std::true_type;
    using is_rectangular = std::true_type;
    using is_resizable   = std::true_type;
    using is_row_major   = std::true_type;
    using size_tuple     = std::tuple<size_t, size_t>;

  public:
    dr_matrix_engine();
    dr_matrix_engine(dr_matrix_engine&&);
    dr_matrix_engine(dr_matrix_engine const&);
    dr_matrix_engine(size_tuple size);
    dr_matrix_engine(size_t rows, size_t cols);
    dr_matrix_engine(size_tuple size, size_tuple cap);
    dr_matrix_engine(size_t rows, size_t cols, size_t rowcap, size_t colcap);

    dr_matrix_engine& operator =(dr_matrix_engine&&);
    dr_matrix_engine& operator =(dr_matrix_engine const&);

    T           operator ()(size_t i) const;
    T           operator ()(size_t i, size_t j) const;
    T const* data() const noexcept;

    size_t      columns() const noexcept;
    size_t      rows() const noexcept;
    size_tuple  size() const noexcept;

    size_t      column_capacity() const noexcept;
    size_t      row_capacity() const noexcept;
    size_tuple  capacity() const noexcept;

    T& operator ()(size_t i);
    T& operator ()(size_t i, size_t j);
    T* data() noexcept;

    void    reserve(size_tuple cap);
    void    reserve(size_t rowcap, size_t colcap);

    void    resize(size_tuple size);
    void    resize(size_t rows, size_t cols);
    void    resize(size_tuple size, size_tuple cap);
    void    resize(size_t rows, size_t cols, size_t rowcap, size_t colcap);

    void    swap_columns(size_t i, size_t j);
    void    swap_rows(size_t i, size_t j);

  private:
    using pointer = typename std::allocator_traits<AT>::pointer;

    pointer     mp_elems;       //- For exposition; data buffer
    size_t      m_rows;
    size_t      m_cols;
    size_t      m_rowcap;
    size_t      m_colcap;
};

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
dr_matrix_engine<T,AT>::dr_matrix_engine(size_tuple)
{}

template<class T, class AT> inline
dr_matrix_engine<T,AT>::dr_matrix_engine(size_t, size_t)
{}

template<class T, class AT> inline
dr_matrix_engine<T,AT>::dr_matrix_engine(size_tuple, size_tuple)
{}

template<class T, class AT> inline
dr_matrix_engine<T,AT>::dr_matrix_engine(size_t, size_t, size_t, size_t)
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

template<class T, class AT>
inline T
dr_matrix_engine<T,AT>::operator ()(size_t i) const
{
    return mp_elems[i];
}

template<class T, class AT>
inline T
dr_matrix_engine<T,AT>::operator ()(size_t i, size_t j) const
{
    return mp_elems[i*m_colcap + j];
}

template<class T, class AT>
inline T const*
dr_matrix_engine<T,AT>::data() const noexcept
{
    return &mp_elems[0];
}

template<class T, class AT>
inline size_t
dr_matrix_engine<T,AT>::columns() const noexcept
{
    return m_cols;
}

template<class T, class AT>
inline size_t
dr_matrix_engine<T,AT>::rows() const noexcept
{
    return m_rows;
}

template<class T, class AT>
inline typename dr_matrix_engine<T,AT>::size_tuple
dr_matrix_engine<T,AT>::size() const noexcept
{
    return size_tuple(m_rows, m_cols);
}

template<class T, class AT>
inline size_t
dr_matrix_engine<T,AT>::column_capacity() const noexcept
{
    return m_colcap;
}

template<class T, class AT>
inline size_t
dr_matrix_engine<T,AT>::row_capacity() const noexcept
{
    return m_rowcap;
}

template<class T, class AT>
inline typename dr_matrix_engine<T,AT>::size_tuple
dr_matrix_engine<T,AT>::capacity() const noexcept
{
    return size_tuple(m_rowcap, m_colcap);
}

template<class T, class AT>
inline T&
dr_matrix_engine<T,AT>::operator ()(size_t i)
{
    return mp_elems[i];
}

template<class T, class AT>
inline T&
dr_matrix_engine<T,AT>::operator ()(size_t i, size_t j)
{
    return mp_elems[i*m_colcap + j];
}

template<class T, class AT>
inline T*
dr_matrix_engine<T,AT>::data() noexcept
{
    return &mp_elems[0];
}

template<class T, class AT>
inline void
dr_matrix_engine<T,AT>::reserve(size_tuple)
{}

template<class T, class AT>
inline void
dr_matrix_engine<T,AT>::reserve(size_t, size_t)
{}

template<class T, class AT>
inline void
dr_matrix_engine<T,AT>::resize(size_tuple)
{}

template<class T, class AT>
inline void
dr_matrix_engine<T,AT>::resize(size_t, size_t)
{}

template<class T, class AT>
inline void
dr_matrix_engine<T,AT>::resize(size_tuple, size_tuple)
{}

template<class T, class AT>
inline void
dr_matrix_engine<T,AT>::resize(size_t, size_t, size_t, size_t)
{}

template<class T, class AT>
inline void
dr_matrix_engine<T,AT>::swap_columns(size_t, size_t)
{}

template<class T, class AT>
inline void
dr_matrix_engine<T,AT>::swap_rows(size_t, size_t)
{}


}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_DYNAMIC_ENGINES_HPP_DEFINED
