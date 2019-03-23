#ifndef LINEAR_ALGEBRA_FIXED_SIZE_ENGINES_HPP_DEFINED
#define LINEAR_ALGEBRA_FIXED_SIZE_ENGINES_HPP_DEFINED

#include "element_promotion_traits.hpp"

//- Experimental namespace for test implementation
//
namespace STD_LA {
//=================================================================================================
//  Fixed-size, fixed-capacity matrix engine.
//=================================================================================================
//
template<class T, size_t R, size_t C>
class fs_matrix_engine
{
    static_assert(R >= 1);
    static_assert(C >= 1);

  public:
    using element_type   = T;
    using is_dense       = std::true_type;
    using is_rectangular = std::true_type;
    using is_resizable   = std::false_type;
    using is_row_major   = std::true_type;
    using size_tuple     = std::tuple<size_t, size_t>;

  public:
    fs_matrix_engine() = default;
    fs_matrix_engine(fs_matrix_engine&&) = default;
    fs_matrix_engine(fs_matrix_engine const&) = default;

    fs_matrix_engine&   operator =(fs_matrix_engine&&) = default;
    fs_matrix_engine&   operator =(fs_matrix_engine const&) = default;

    T           operator ()(size_t i) const;
    T           operator ()(size_t i, size_t j) const;
    T const*    data() const noexcept;

    size_t      columns() const noexcept;
    size_t      rows() const noexcept;
    size_tuple  size() const noexcept;

    size_t      column_capacity() const noexcept;
    size_t      row_capacity() const noexcept;
    size_tuple  capacity() const noexcept;

    T&      operator ()(size_t i);
    T&      operator ()(size_t i, size_t j);
    T*      data() noexcept;

    void    swap_columns(size_t i, size_t j);
    void    swap_rows(size_t i, size_t j);

  private:
    T       ma_elems[R*C];      //- For exposition; data buffer
};

template<class T, size_t R, size_t C>
inline T
fs_matrix_engine<T,R,C>::operator ()(size_t i) const
{
    return ma_elems[i];
}

template<class T, size_t R, size_t C>
inline T
fs_matrix_engine<T,R,C>::operator ()(size_t i, size_t j) const
{
    return ma_elems[i*C + j];
}

template<class T, size_t R, size_t C>
inline T const*
fs_matrix_engine<T,R,C>::data() const noexcept
{
    return &ma_elems[0];
}

template<class T, size_t R, size_t C>
inline size_t
fs_matrix_engine<T,R,C>::columns() const noexcept
{
    return C;
}

template<class T, size_t R, size_t C>
inline size_t
fs_matrix_engine<T,R,C>::rows() const noexcept
{
    return R;
}

template<class T, size_t R, size_t C>
inline typename fs_matrix_engine<T,R,C>::size_tuple
fs_matrix_engine<T,R,C>::size() const noexcept
{
    return size_tuple(R, C);
}

template<class T, size_t R, size_t C>
inline size_t
fs_matrix_engine<T,R,C>::column_capacity() const noexcept
{
    return C;
}

template<class T, size_t R, size_t C>
inline size_t
fs_matrix_engine<T,R,C>::row_capacity() const noexcept
{
    return R;
}

template<class T, size_t R, size_t C>
inline typename fs_matrix_engine<T,R,C>::size_tuple
fs_matrix_engine<T,R,C>::capacity() const noexcept
{
    return size_tuple(R, C);
}

template<class T, size_t R, size_t C>
inline T&
fs_matrix_engine<T,R,C>::operator ()(size_t i)
{
    return ma_elems[i];
}

template<class T, size_t R, size_t C>
inline T&
fs_matrix_engine<T,R,C>::operator ()(size_t i, size_t j)
{
    return ma_elems[i*C + j];
}

template<class T, size_t R, size_t C>
inline T*
fs_matrix_engine<T,R,C>::data() noexcept
{
    return &ma_elems[0];
}

template<class T, size_t R, size_t C>
inline void
fs_matrix_engine<T,R,C>::swap_columns(size_t, size_t)
{}

template<class T, size_t R, size_t C>
inline void
fs_matrix_engine<T,R,C>::swap_rows(size_t, size_t)
{}


}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_FIXED_SIZE_ENGINES_HPP_DEFINED
