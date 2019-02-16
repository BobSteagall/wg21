#ifndef TEST_NEW_ENGINE_HPP_DEFINED
#define TEST_NEW_ENGINE_HPP_DEFINED

#include "linear_algebra.hpp"

template<class T, size_t R, size_t C>
class fs_matrix_engine_tst
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
    fs_matrix_engine_tst() = default;
    fs_matrix_engine_tst(fs_matrix_engine_tst&&) = default;
    fs_matrix_engine_tst(fs_matrix_engine_tst const&) = default;

    fs_matrix_engine_tst&   operator =(fs_matrix_engine_tst&&) = default;
    fs_matrix_engine_tst&   operator =(fs_matrix_engine_tst const&) = default;

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
    T*      mp_bias;            //- For exposition; bias pointer for 1-based indexing
};

template<class T, size_t R, size_t C>
inline T
fs_matrix_engine_tst<T,R,C>::operator ()(size_t i) const
{
    return ma_elems[i];
}

template<class T, size_t R, size_t C>
inline T
fs_matrix_engine_tst<T,R,C>::operator ()(size_t i, size_t j) const
{
    return ma_elems[i*C + j];
}

template<class T, size_t R, size_t C>
inline T const*
fs_matrix_engine_tst<T,R,C>::data() const noexcept
{
    return &ma_elems[0];
}

template<class T, size_t R, size_t C>
inline size_t
fs_matrix_engine_tst<T,R,C>::columns() const noexcept
{
    return C;
}

template<class T, size_t R, size_t C>
inline size_t
fs_matrix_engine_tst<T,R,C>::rows() const noexcept
{
    return R;
}

template<class T, size_t R, size_t C>
inline typename fs_matrix_engine_tst<T,R,C>::size_tuple
fs_matrix_engine_tst<T,R,C>::size() const noexcept
{
    return size_tuple(R, C);
}

template<class T, size_t R, size_t C>
inline size_t
fs_matrix_engine_tst<T,R,C>::column_capacity() const noexcept
{
    return C;
}

template<class T, size_t R, size_t C>
inline size_t
fs_matrix_engine_tst<T,R,C>::row_capacity() const noexcept
{
    return R;
}

template<class T, size_t R, size_t C>
inline typename fs_matrix_engine_tst<T,R,C>::size_tuple
fs_matrix_engine_tst<T,R,C>::capacity() const noexcept
{
    return size_tuple(R, C);
}

template<class T, size_t R, size_t C>
inline T&
fs_matrix_engine_tst<T,R,C>::operator ()(size_t i)
{
    return ma_elems[i];
}

template<class T, size_t R, size_t C>
inline T&
fs_matrix_engine_tst<T,R,C>::operator ()(size_t i, size_t j)
{
    return ma_elems[i*C + j];
}

template<class T, size_t R, size_t C>
inline T*
fs_matrix_engine_tst<T,R,C>::data() noexcept
{
    return &ma_elems[0];
}

template<class T, size_t R, size_t C>
inline void
fs_matrix_engine_tst<T,R,C>::swap_columns(size_t, size_t)
{}

template<class T, size_t R, size_t C>
inline void
fs_matrix_engine_tst<T,R,C>::swap_rows(size_t, size_t)
{}

template<class T, size_t R>
using fs_column_vector_tst = STD_LA::column_vector<fs_matrix_engine_tst<T, R, 1>>;

template<class T, size_t R>
using fs_col_vector_tst = fs_column_vector_tst<T, R>;

template<class T, size_t C>
using fs_row_vector_tst = STD_LA::row_vector<fs_matrix_engine_tst<T, 1, C>>;

template<class T, size_t R, size_t C>
using fs_matrix_tst = STD_LA::matrix<fs_matrix_engine_tst<T, R, C>>;

template<class T1, size_t R1, size_t C1, class T2, size_t R2, size_t C2>
struct STD_LA::matrix_addition_engine_promotion<fs_matrix_engine_tst<T1, R1, C1>, 
                                                fs_matrix_engine_tst<T2, R2, C2>>
{
    static_assert(R1 == R2);
    static_assert(C1 == C2);
    using element_type = matrix_element_promotion_t<T1, T2>;
    using engine_type  = fs_matrix_engine_tst<element_type, R1, C1>;
};

#endif  //- TEST_NEW_ENGINE_HPP_DEFINED