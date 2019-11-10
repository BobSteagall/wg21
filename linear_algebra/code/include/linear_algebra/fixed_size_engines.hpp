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
    static_assert(is_matrix_element_v<T>);
    static_assert(N >= 1);

  public:
    using engine_category = mutable_vector_engine_tag;
    using element_type    = T;
    using value_type      = T;
    using reference       = T&;
    using pointer         = T*;
    using const_reference = T const&;
    using const_pointer   = T const*;
    using iterator        = detail::vector_iterator<fs_vector_engine>;
    using const_iterator  = detail::vector_const_iterator<fs_vector_engine>;
    using difference_type = ptrdiff_t;
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
    template<class U>
    constexpr fs_vector_engine(initializer_list<U> list);
    constexpr fs_vector_engine(fs_vector_engine&&) noexcept = default;
    constexpr fs_vector_engine(fs_vector_engine const&) = default;

    constexpr fs_vector_engine&     operator =(fs_vector_engine&&) noexcept = default;
    constexpr fs_vector_engine&     operator =(fs_vector_engine const&) = default;

    constexpr const_reference   operator ()(index_type i) const;
    constexpr const_iterator    begin() const noexcept;
    constexpr const_iterator    end() const noexcept;

    static constexpr size_type  capacity() noexcept;
    static constexpr index_type elements() noexcept;
    static constexpr size_type  size() noexcept;

    constexpr reference     operator ()(index_type i);
    constexpr iterator      begin() noexcept;
    constexpr iterator      end() noexcept;

    constexpr void  swap(fs_vector_engine& rhs) noexcept;
    constexpr void  swap_elements(index_type i, index_type j) noexcept;

  private:
    T   ma_elems[N];
};

template<class T, int32_t N>
constexpr
fs_vector_engine<T,N>::fs_vector_engine()
{
    if constexpr (is_arithmetic_v<T>)
    {
        for (auto& elem : ma_elems) 
        {
            elem = static_cast<T>(0);
        }
    }
}

template<class T, int32_t N> 
template<class U>
constexpr
fs_vector_engine<T,N>::fs_vector_engine(initializer_list<U> list)
:   ma_elems()
{
    size_t  total = (size_t)(N);
    size_t  count = min(total, list.size());
    auto    iter  = list.begin();

    for (size_t i = 0;  i < count;  ++i, ++iter)
    {
        ma_elems[i] = static_cast<T>( *iter);
    }

    if constexpr (is_arithmetic_v<T>)
    {
        if (count < total)
        {
            for (size_t i = count;  i < total;  ++i) 
            {
                ma_elems[i] = static_cast<T>(0);
            }
        }
    }
}

template<class T, int32_t N> inline
constexpr typename fs_vector_engine<T,N>::const_reference
fs_vector_engine<T,N>::operator ()(index_type i) const
{
    return ma_elems[i];
}

template<class T, int32_t N> inline
constexpr typename fs_vector_engine<T,N>::const_iterator
fs_vector_engine<T,N>::begin() const noexcept
{
    return const_iterator(this, 0, N);
}

template<class T, int32_t N> inline
constexpr typename fs_vector_engine<T,N>::const_iterator
fs_vector_engine<T,N>::end() const noexcept
{
    return const_iterator(this, N, N);
}

template<class T, int32_t N> inline
constexpr typename fs_vector_engine<T,N>::size_type
fs_vector_engine<T,N>::capacity() noexcept
{
    return N;
}

template<class T, int32_t N> inline
constexpr typename fs_vector_engine<T,N>::index_type
fs_vector_engine<T,N>::elements() noexcept
{
    return N;
}

template<class T, int32_t N> inline
constexpr typename fs_vector_engine<T,N>::size_type
fs_vector_engine<T,N>::size() noexcept
{
    return N;
}

template<class T, int32_t N> inline
constexpr typename fs_vector_engine<T,N>::reference
fs_vector_engine<T,N>::operator ()(index_type i)
{
    return ma_elems[i];
}

template<class T, int32_t N> inline
constexpr typename fs_vector_engine<T,N>::iterator
fs_vector_engine<T,N>::begin() noexcept
{
    return iterator(this, 0, N);
}

template<class T, int32_t N> inline
constexpr typename fs_vector_engine<T,N>::iterator
fs_vector_engine<T,N>::end() noexcept
{
    return iterator(this, N, N);
}

template<class T, int32_t N> inline
constexpr void
fs_vector_engine<T,N>::swap(fs_vector_engine& rhs) noexcept
{
    if (&rhs != this)
    {
        for (index_type i = 0;  i < N;  ++i)
        {
            detail::la_swap(ma_elems[i], rhs.ma_elems[i]);
        }
    }
}

template<class T, int32_t N> inline
constexpr void
fs_vector_engine<T,N>::swap_elements(index_type i, index_type j) noexcept
{
    detail::la_swap(ma_elems[i], ma_elems[j]);
}


//==================================================================================================
//  Fixed-size, fixed-capacity matrix engine.
//==================================================================================================
//
template<class T, int32_t R, int32_t C>
class fs_matrix_engine
{
    static_assert(is_matrix_element_v<T>);
    static_assert(R >= 1);
    static_assert(C >= 1);

  public:
    using engine_category = mutable_matrix_engine_tag;
    using element_type    = T;
    using value_type      = T;
    using reference       = T&;
    using pointer         = T*;
    using const_reference = T const&;
    using const_pointer   = T const*;
    using difference_type = ptrdiff_t;
    using index_type      = int_fast32_t;
    using size_type       = int_fast32_t;
    using size_tuple      = tuple<size_type, size_type>;

    using is_fixed_size   = true_type;
    using is_resizable    = false_type;

    using is_column_major = false_type;
    using is_dense        = true_type;
    using is_rectangular  = true_type;
    using is_row_major    = true_type;

    using column_view_type    = matrix_column_view<fs_matrix_engine>;
    using row_view_type       = matrix_row_view<fs_matrix_engine>;
    using transpose_view_type = matrix_transpose_view<fs_matrix_engine>;

  public:
    constexpr fs_matrix_engine();
    template<class U>
    constexpr fs_matrix_engine(initializer_list<U> list);
    constexpr fs_matrix_engine(fs_matrix_engine&&) noexcept = default;
    constexpr fs_matrix_engine(fs_matrix_engine const&) = default;

    constexpr fs_matrix_engine&     operator =(fs_matrix_engine&&) noexcept = default;
    constexpr fs_matrix_engine&     operator =(fs_matrix_engine const&) = default;

    constexpr const_reference   operator ()(index_type i, index_type j) const;

    constexpr index_type    columns() const noexcept;
    constexpr index_type    rows() const noexcept;
    constexpr size_tuple    size() const noexcept;

    constexpr size_type     column_capacity() const noexcept;
    constexpr size_type     row_capacity() const noexcept;
    constexpr size_tuple    capacity() const noexcept;

    constexpr reference     operator ()(index_type i, index_type j);

    constexpr void      assign(fs_matrix_engine const& rhs);
    template<class ET2>
    constexpr void      assign(ET2 const& rhs);

    constexpr void      swap(fs_matrix_engine& rhs) noexcept;
    constexpr void      swap_columns(index_type j1, index_type j2) noexcept;
    constexpr void      swap_rows(index_type i1, index_type i2) noexcept;

  private:
    T   ma_elems[R*C];
};

template<class T, int32_t R, int32_t C> inline
constexpr
fs_matrix_engine<T,R,C>::fs_matrix_engine()
:   ma_elems()
{
    if constexpr (is_arithmetic_v<T>)
    {
        for (auto& elem : ma_elems) 
        {
            elem = static_cast<T>(0);
        }
    }
}

template<class T, int32_t R, int32_t C> 
template<class U> inline
constexpr
fs_matrix_engine<T,R,C>::fs_matrix_engine(initializer_list<U> list)
:   ma_elems()
{
    size_t  total = (size_t)(R*C);
    size_t  count = min(total, list.size());
    auto    iter  = list.begin();

    for (size_t i = 0;  i < count;  ++i, ++iter)
    {
        ma_elems[i] = static_cast<T>( *iter);
    }

    if constexpr (is_arithmetic_v<T>)
    {
        if (count < total)
        {
            for (size_t i = count;  i < total;  ++i) 
            {
                ma_elems[i] = static_cast<T>(0);
            }
        }
    }
}

template<class T, int32_t R, int32_t C> inline
constexpr typename fs_matrix_engine<T,R,C>::const_reference
fs_matrix_engine<T,R,C>::operator ()(index_type i, index_type j) const
{
    return ma_elems[i*C + j];
}

template<class T, int32_t R, int32_t C> inline
constexpr typename fs_matrix_engine<T,R,C>::index_type
fs_matrix_engine<T,R,C>::columns() const noexcept
{
    return C;
}

template<class T, int32_t R, int32_t C> inline
constexpr typename fs_matrix_engine<T,R,C>::index_type
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
constexpr typename fs_matrix_engine<T,R,C>::reference
fs_matrix_engine<T,R,C>::operator ()(index_type i, index_type j)
{
    return ma_elems[i*C + j];
}

template<class T, int32_t R, int32_t C> inline
constexpr void
fs_matrix_engine<T,R,C>::assign(fs_matrix_engine const& rhs)
{
    if (&rhs != this) return;

    for (index_type i = 0;  i < R*C;  ++i)
    {
        ma_elems[i] = rhs.ma_elems[i];
    }
}

template<class T, int32_t R, int32_t C> 
template<class ET2> inline
constexpr void
fs_matrix_engine<T,R,C>::assign(ET2 const& rhs)
{
    using src_index_type = typename ET2::index_type;

    if (rhs.size() != size()) 
    {
        throw runtime_error("invalid size");
    }

    if constexpr(is_same_v<index_type, src_index_type>)
    {
        for (index_type i = 0;  i < rows();  ++i)
        {
            for (index_type j = 0;  j < columns();  ++j)
            {
                (*this)(i, j) = rhs(i, j);
            }
        }
    }
    else
    {
        src_index_type  si = 0;
        src_index_type  sj = 0;
        index_type      di = 0;
        index_type      dj = 0;

        for (;  di < rows();  ++di, ++si)
        {
            for (;  dj < columns();  ++dj, ++sj)
            {
                (*this)(di, dj) = rhs(si, sj);
            }
        }
    }
}

template<class T, int32_t R, int32_t C> inline
constexpr void
fs_matrix_engine<T,R,C>::swap(fs_matrix_engine& rhs) noexcept
{
    if (&rhs != this)
    {
        for (index_type i = 0;  i < R*C;  ++i)
        {
            detail::la_swap(ma_elems[i], rhs.ma_elems[i]);
        }
    }
}

template<class T, int32_t R, int32_t C> inline
constexpr void
fs_matrix_engine<T,R,C>::swap_columns(index_type j1, index_type j2) noexcept
{
    if (j1 != j2)
    {
        for (index_type i = 0;  i < R;  ++i)
        {
            detail::la_swap(ma_elems[i*C + j1], ma_elems[i*C + j2]);
        }
    }
}

template<class T, int32_t R, int32_t C> inline
constexpr void
fs_matrix_engine<T,R,C>::swap_rows(index_type i1, index_type i2) noexcept
{
    if (i1 != i2)
    {
        for (index_type j = 0;  j < C;  ++j)
        {
            detail::la_swap(ma_elems[i1*C + j], ma_elems[i2*C + j]);
        }
    }
}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_FIXED_SIZE_ENGINES_HPP_DEFINED
