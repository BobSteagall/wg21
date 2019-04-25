//==================================================================================================
//  File:       fixed_size_engines.hpp
//
//  Summary:    This header defines fixed-size vector and matrix engines.  In this context,
//              fixed-size means that the row and column extents of such objects are known at
//              compile-time, and thus the engines can be made constexpr.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_FIXED_SIZE_ENGINES_HPP_DEFINED
#define LINEAR_ALGEBRA_FIXED_SIZE_ENGINES_HPP_DEFINED

namespace STD_LA {
//==================================================================================================
//  Fixed-size, fixed-capacity matrix engine.
//==================================================================================================
//
template<class T, int32_t N>
class fs_vector_engine
{
    static_assert(N >= 1);

  public:
    using engine_category = mutable_vector_engine_tag;
    using element_type    = T;
    using index_type      = int_fast32_t;
    using size_type       = int_fast32_t;

    using is_fixed_size   = true_type;
    using is_resizable    = false_type;

    using is_column_major = true_type;
    using is_dense        = true_type;
    using is_rectangular  = true_type;
    using is_row_major    = true_type;

  public:
    constexpr fs_vector_engine();
    constexpr fs_vector_engine(fs_vector_engine&&) = default;
    constexpr fs_vector_engine(fs_vector_engine const&) = default;

    constexpr fs_vector_engine&     operator =(fs_vector_engine&&) = default;
    constexpr fs_vector_engine&     operator =(fs_vector_engine const&) = default;

    constexpr T         operator ()(index_type i) const;
    constexpr T const*  data() const noexcept;

    constexpr size_type capacity() const noexcept;
    constexpr size_type elements() const noexcept;

    constexpr T&    operator ()(index_type i);
    constexpr T*    data() noexcept;

    constexpr void  swap_elements(index_type i, index_type j);

  private:
    T       ma_elems[N];      //- For exposition; data buffer
};

template<class T, int32_t N> inline
constexpr
fs_vector_engine<T,N>::fs_vector_engine()
{
    if constexpr (is_arithmetic_v<T>)
    {
        for (auto& elem : ma_elems) elem = static_cast<T>(0);
    }
}

template<class T, int32_t N> inline
constexpr T
fs_vector_engine<T,N>::operator ()(index_type i) const
{
    return ma_elems[i];
}

template<class T, int32_t N> inline
constexpr T const*
fs_vector_engine<T,N>::data() const noexcept
{
    return &ma_elems[0];
}

template<class T, int32_t N> inline
constexpr typename fs_vector_engine<T,N>::size_type
fs_vector_engine<T,N>::capacity() const noexcept
{
    return N;
}

template<class T, int32_t N> inline
constexpr typename fs_vector_engine<T,N>::size_type
fs_vector_engine<T,N>::elements() const noexcept
{
    return N;
}

template<class T, int32_t N> inline
constexpr T&
fs_vector_engine<T,N>::operator ()(index_type i)
{
    return ma_elems[i];
}

template<class T, int32_t N> inline
constexpr T*
fs_vector_engine<T,N>::data() noexcept
{
    return &ma_elems[0];
}

template<class T, int32_t N> inline
constexpr void
fs_vector_engine<T,N>::swap_elements(index_type i, index_type j)
{
    std::swap(ma_elems[i], ma_elems[j]);
}


//==================================================================================================
//  Fixed-size, fixed-capacity matrix engine.
//==================================================================================================
//
template<class T, int32_t R, int32_t C>
class fs_matrix_engine
{
    static_assert(R >= 1);
    static_assert(C >= 1);

  public:
    using engine_category = mutable_matrix_engine_tag;
    using element_type    = T;
    using index_type      = int_fast32_t;
    using size_type       = int_fast32_t;
    using size_tuple      = tuple<size_type, size_type>;

    using is_fixed_size   = true_type;
    using is_resizable    = false_type;

    using is_column_major = false_type;
    using is_dense        = true_type;
    using is_rectangular  = true_type;
    using is_row_major    = true_type;

  public:
    constexpr fs_matrix_engine();
    constexpr fs_matrix_engine(fs_matrix_engine&&) = default;
    constexpr fs_matrix_engine(fs_matrix_engine const&) = default;

    constexpr fs_matrix_engine&     operator =(fs_matrix_engine&&) = default;
    constexpr fs_matrix_engine&     operator =(fs_matrix_engine const&) = default;

    constexpr T             operator ()(index_type i, index_type j) const;
    constexpr T const*      data() const noexcept;

    constexpr size_type     columns() const noexcept;
    constexpr size_type     rows() const noexcept;
    constexpr size_tuple    size() const noexcept;

    constexpr size_type     column_capacity() const noexcept;
    constexpr size_type     row_capacity() const noexcept;
    constexpr size_tuple    capacity() const noexcept;

    constexpr T&    operator ()(index_type i, index_type j);
    constexpr T*    data() noexcept;

    constexpr void  assign(fs_matrix_engine const& rhs);
    constexpr void  swap(fs_matrix_engine& rhs);
    constexpr void  swap_columns(index_type j1, index_type j2);
    constexpr void  swap_rows(index_type i1, index_type i2);

  private:
    T       ma_elems[R*C];      //- For exposition; data buffer
};

template<class T, int32_t R, int32_t C> inline
constexpr
fs_matrix_engine<T,R,C>::fs_matrix_engine()
:   ma_elems()
{
    if constexpr (is_arithmetic_v<T>)
    {
        for (auto& elem : ma_elems) elem = static_cast<T>(0);
    }
}

template<class T, int32_t R, int32_t C> inline
constexpr T
fs_matrix_engine<T,R,C>::operator ()(index_type i, index_type j) const
{
    return ma_elems[i*C + j];
}

template<class T, int32_t R, int32_t C> inline
constexpr T const*
fs_matrix_engine<T,R,C>::data() const noexcept
{
    return &ma_elems[0];
}

template<class T, int32_t R, int32_t C> inline
constexpr typename fs_matrix_engine<T,R,C>::size_type
fs_matrix_engine<T,R,C>::columns() const noexcept
{
    return C;
}

template<class T, int32_t R, int32_t C> inline
constexpr typename fs_matrix_engine<T,R,C>::size_type
fs_matrix_engine<T,R,C>::rows() const noexcept
{
    return R;
}

template<class T, int32_t R, int32_t C> inline
constexpr typename fs_matrix_engine<T,R,C>::size_tuple
fs_matrix_engine<T,R,C>::size() const noexcept
{
    return size_tuple(R, C);
}

template<class T, int32_t R, int32_t C> inline
constexpr typename fs_matrix_engine<T,R,C>::size_type
fs_matrix_engine<T,R,C>::column_capacity() const noexcept
{
    return C;
}

template<class T, int32_t R, int32_t C> inline
constexpr typename fs_matrix_engine<T,R,C>::size_type
fs_matrix_engine<T,R,C>::row_capacity() const noexcept
{
    return R;
}

template<class T, int32_t R, int32_t C> inline
constexpr typename fs_matrix_engine<T,R,C>::size_tuple
fs_matrix_engine<T,R,C>::capacity() const noexcept
{
    return size_tuple(R, C);
}

template<class T, int32_t R, int32_t C> inline
constexpr T&
fs_matrix_engine<T,R,C>::operator ()(index_type i, index_type j)
{
    return ma_elems[i*C + j];
}

template<class T, int32_t R, int32_t C> inline
constexpr T*
fs_matrix_engine<T,R,C>::data() noexcept
{
    return &ma_elems[0];
}

template<class T, int32_t R, int32_t C> inline
constexpr void
fs_matrix_engine<T,R,C>::assign(fs_matrix_engine const& rhs)
{
    if (&rhs != this)
    {
        for (index_type i = 0;  i < R*C;  ++i)
        {
            ma_elems[i] = rhs.ma_elems[i];
        }
    }
}

template<class T, int32_t R, int32_t C> inline
constexpr void
fs_matrix_engine<T,R,C>::swap(fs_matrix_engine& rhs)
{
    if (&rhs != this)
    {
        for (index_type i = 0;  i < R*C;  ++i)
        {
            std::swap(ma_elems[i], rhs.ma_elems[i]);
        }
    }
}

template<class T, int32_t R, int32_t C> inline
constexpr void
fs_matrix_engine<T,R,C>::swap_columns(index_type j1, index_type j2)
{
    if (j1 != j2)
    {
        for (index_type i = 0;  i < R;  ++i)
        {
            std::swap(ma_elems[i*C + j1], ma_elems[i*C + j2]);
        }
    }
}

template<class T, int32_t R, int32_t C> inline
constexpr void
fs_matrix_engine<T,R,C>::swap_rows(index_type i1, index_type i2)
{
    if (i1 != i2)
    {
        for (index_type j = 0;  j < C;  ++j)
        {
            std::swap(ma_elems[i1*C + j], ma_elems[i2*C + j]);
        }
    }
}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_FIXED_SIZE_ENGINES_HPP_DEFINED
