#ifndef TEST_NEW_ENGINE_HPP_DEFINED
#define TEST_NEW_ENGINE_HPP_DEFINED

#include "linear_algebra.hpp"

template<class T, int32_t R, int32_t C>
class fs_matrix_engine_tst
{
    static_assert(R >= 1);
    static_assert(C >= 1);

  public:
    using engine_category = STD_LA::mutable_matrix_engine_tag;
    using element_type    = T;
    using reference       = T&;
    using pointer         = T*;
    using const_reference = T const&;
    using const_pointer   = T const*;
    using index_type      = int_fast32_t;
    using size_type       = int_fast32_t;
    using size_tuple      = std::tuple<size_type, size_type>;

    using is_fixed_size   = std::true_type;
    using is_resizable    = std::false_type;

    using is_column_major = std::false_type;
    using is_dense        = std::true_type;
    using is_rectangular  = std::true_type;
    using is_row_major    = std::true_type;

  public:
    constexpr fs_matrix_engine_tst() = default;
    constexpr fs_matrix_engine_tst(fs_matrix_engine_tst&&) = default;
    constexpr fs_matrix_engine_tst(fs_matrix_engine_tst const&) = default;

    constexpr fs_matrix_engine_tst&     operator =(fs_matrix_engine_tst&&) = default;
    constexpr fs_matrix_engine_tst&     operator =(fs_matrix_engine_tst const&) = default;

    constexpr T const&      operator ()(index_type i, index_type j) const;
    constexpr T const*      data() const noexcept;

    constexpr size_type     columns() const noexcept;
    constexpr size_type     rows() const noexcept;
    constexpr size_tuple    size() const noexcept;

    constexpr size_type     column_capacity() const noexcept;
    constexpr size_type     row_capacity() const noexcept;
    constexpr size_tuple    capacity() const noexcept;

    constexpr T&    operator ()(index_type i, index_type j);
    constexpr T*    data() noexcept;

    constexpr void  swap_columns(index_type i, index_type j);
    constexpr void  swap_rows(index_type i, index_type j);

  private:
    T       ma_elems[R*C];      //- For exposition; data buffer
};

template<class T, int32_t R, int32_t C> inline
constexpr T const&
fs_matrix_engine_tst<T,R,C>::operator ()(index_type i, index_type j) const
{
    return ma_elems[i*C + j];
}

template<class T, int32_t R, int32_t C> inline
constexpr T const*
fs_matrix_engine_tst<T,R,C>::data() const noexcept
{
    return &ma_elems[0];
}

template<class T, int32_t R, int32_t C> inline 
constexpr typename fs_matrix_engine_tst<T,R,C>::size_type
fs_matrix_engine_tst<T,R,C>::columns() const noexcept
{
    return C;
}

template<class T, int32_t R, int32_t C> inline 
constexpr typename fs_matrix_engine_tst<T,R,C>::size_type
fs_matrix_engine_tst<T,R,C>::rows() const noexcept
{
    return R;
}

template<class T, int32_t R, int32_t C> inline 
constexpr typename fs_matrix_engine_tst<T,R,C>::size_tuple
fs_matrix_engine_tst<T,R,C>::size() const noexcept
{
    return size_tuple(R, C);
}

template<class T, int32_t R, int32_t C> inline 
constexpr typename fs_matrix_engine_tst<T,R,C>::size_type
fs_matrix_engine_tst<T,R,C>::column_capacity() const noexcept
{
    return C;
}

template<class T, int32_t R, int32_t C> inline 
constexpr typename fs_matrix_engine_tst<T,R,C>::size_type
fs_matrix_engine_tst<T,R,C>::row_capacity() const noexcept
{
    return R;
}

template<class T, int32_t R, int32_t C> inline  
constexpr typename fs_matrix_engine_tst<T,R,C>::size_tuple
fs_matrix_engine_tst<T,R,C>::capacity() const noexcept
{
    return size_tuple(R, C);
}

template<class T, int32_t R, int32_t C> inline
constexpr T&
fs_matrix_engine_tst<T,R,C>::operator ()(index_type i, index_type j)
{
    return ma_elems[i*C + j];
}

template<class T, int32_t R, int32_t C> inline
constexpr T*
fs_matrix_engine_tst<T,R,C>::data() noexcept
{
    return &ma_elems[0];
}

template<class T, int32_t R, int32_t C> inline
constexpr void
fs_matrix_engine_tst<T,R,C>::swap_columns(index_type, index_type)
{}

template<class T, int32_t R, int32_t C> inline
constexpr void
fs_matrix_engine_tst<T,R,C>::swap_rows(index_type, index_type)
{}

template<class T, int32_t R, int32_t C>
using fs_matrix_tst = STD_LA::matrix<fs_matrix_engine_tst<T, R, C>>;

#endif  //- TEST_NEW_ENGINE_HPP_DEFINED