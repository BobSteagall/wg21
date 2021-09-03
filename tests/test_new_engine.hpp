#ifndef TEST_NEW_ENGINE_HPP_DEFINED
#define TEST_NEW_ENGINE_HPP_DEFINED

//==================================================================================================
//  Fixed-size, fixed-capacity matrix engine.
//==================================================================================================
//
template<class T, size_t R, size_t C>
class fs_matrix_engine_tst
{
    static_assert(R >= 1);
    static_assert(C >= 1);

  public:
    using element_type     = T;
    using value_type       = T;
    using reference        = T&;
    using const_reference  = T const&;
    using index_type       = uint32_t;
    using index_tuple_type = std::tuple<index_type, index_type>;

#ifdef LA_USE_MDSPAN
//    using span_type       = typename STD_LA::fs_matrix_engine<T, R, C>::span_type;
//    using const_span_type = typename STD_LA::fs_matrix_engine<T, R, C>::const_span_type;
#endif

  public:
    constexpr fs_matrix_engine_tst();
    constexpr fs_matrix_engine_tst(fs_matrix_engine_tst&&) = default;
    constexpr fs_matrix_engine_tst(fs_matrix_engine_tst const&) = default;

    constexpr fs_matrix_engine_tst&     operator =(fs_matrix_engine_tst&&) = default;
    constexpr fs_matrix_engine_tst&     operator =(fs_matrix_engine_tst const&) = default;

    constexpr const_reference   operator ()(index_type i, index_type j) const;

    constexpr index_type    columns() const noexcept;
    constexpr index_type    rows() const noexcept;
    constexpr index_type    size() const noexcept;

    constexpr index_type    column_capacity() const noexcept;
    constexpr index_type    row_capacity() const noexcept;
    constexpr index_type    capacity() const noexcept;

    constexpr reference     operator ()(index_type i, index_type j);

    constexpr void      assign(fs_matrix_engine_tst const& rhs);
    template<class ET2>
    constexpr void      assign(ET2 const& rhs);

    constexpr void      swap(fs_matrix_engine_tst& rhs) noexcept;
    constexpr void      swap_columns(index_type j1, index_type j2);
    constexpr void      swap_rows(index_type i1, index_type i2);

  private:
    T   ma_elems[R*C];
};

template<class T, size_t R, size_t C> inline
constexpr
fs_matrix_engine_tst<T,R,C>::fs_matrix_engine_tst()
:   ma_elems()
{
    if constexpr (std::is_arithmetic_v<T>)
    {
        for (auto& elem : ma_elems) elem = static_cast<T>(0);
    }
}

template<class T, size_t R, size_t C> inline
constexpr typename fs_matrix_engine_tst<T,R,C>::const_reference
fs_matrix_engine_tst<T,R,C>::operator ()(index_type i, index_type j) const
{
    return ma_elems[i*C + j];
}

template<class T, size_t R, size_t C> inline
constexpr typename fs_matrix_engine_tst<T,R,C>::index_type
fs_matrix_engine_tst<T,R,C>::columns() const noexcept
{
    return C;
}

template<class T, size_t R, size_t C> inline
constexpr typename fs_matrix_engine_tst<T,R,C>::index_type
fs_matrix_engine_tst<T,R,C>::rows() const noexcept
{
    return R;
}

template<class T, size_t R, size_t C> inline
constexpr typename fs_matrix_engine_tst<T,R,C>::index_type
fs_matrix_engine_tst<T,R,C>::size() const noexcept
{
    return R*C;
}

template<class T, size_t R, size_t C> inline
constexpr typename fs_matrix_engine_tst<T,R,C>::index_type
fs_matrix_engine_tst<T,R,C>::column_capacity() const noexcept
{
    return C;
}

template<class T, size_t R, size_t C> inline
constexpr typename fs_matrix_engine_tst<T,R,C>::index_type
fs_matrix_engine_tst<T,R,C>::row_capacity() const noexcept
{
    return R;
}

template<class T, size_t R, size_t C> inline
constexpr typename fs_matrix_engine_tst<T,R,C>::index_type
fs_matrix_engine_tst<T,R,C>::capacity() const noexcept
{
    return R*C;
}

template<class T, size_t R, size_t C> inline
constexpr typename fs_matrix_engine_tst<T,R,C>::reference
fs_matrix_engine_tst<T,R,C>::operator ()(index_type i, index_type j)
{
    return ma_elems[i*C + j];
}

template<class T, size_t R, size_t C> inline
constexpr void
fs_matrix_engine_tst<T,R,C>::assign(fs_matrix_engine_tst const& rhs)
{
    if (&rhs != this) return;

    for (index_type i = 0;  i < R*C;  ++i)
    {
        ma_elems[i] = rhs.ma_elems[i];
    }
}

template<class T, size_t R, size_t C>
template<class ET2> inline
constexpr void
fs_matrix_engine_tst<T,R,C>::assign(ET2 const& rhs)
{
    using src_index_type = typename ET2::index_type;

    if (rhs.size() != size())
    {
        throw std::runtime_error("invalid size");
    }

    if constexpr(std::is_same_v<index_type, src_index_type>)
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
        index_type       di = 0;
        index_type       dj = 0;

        for (; di < rows(); ++di, ++si)
        {
            for (; dj < columns(); ++dj, ++sj)
            {
                (*this)(di, dj) = rhs(si, sj);
            }
        }
    }
}

template<class T, size_t R, size_t C> inline
constexpr void
fs_matrix_engine_tst<T,R,C>::swap(fs_matrix_engine_tst& rhs) noexcept
{
    if (&rhs != this)
    {
        for (index_type i = 0;  i < R*C;  ++i)
        {
            STD_LA::detail::matrix_engine_support::swap(ma_elems[i], rhs.ma_elems[i]);
        }
    }
}

template<class T, size_t R, size_t C> inline
constexpr void
fs_matrix_engine_tst<T,R,C>::swap_columns(index_type j1, index_type j2)
{
    if (j1 != j2)
    {
        for (index_type i = 0;  i < R;  ++i)
        {
            STD_LA::detail::matrix_engine_support::swap(ma_elems[i*C + j1], ma_elems[i*C + j2]);
        }
    }
}

template<class T, size_t R, size_t C> inline
constexpr void
fs_matrix_engine_tst<T,R,C>::swap_rows(index_type i1, index_type i2)
{
    if (i1 != i2)
    {
        for (index_type j = 0;  j < C;  ++j)
        {
            STD_LA::detail::matrix_engine_support::swap(ma_elems[i1*C + j], ma_elems[i2*C + j]);
        }
    }
}

//==================================================================================================
//  Fixed-size, fixed-capacity matrix engine.
//==================================================================================================
//
template<class T, size_t R, size_t C>
class test_fs_matrix_engine
{
    static_assert(R >= 1);
    static_assert(C >= 1);

  public:
    using element_type     = T;
    using value_type       = T;
    using reference        = T&;
    using const_reference  = T const&;
    using difference_type  = std::int32_t;
    using index_type       = std::uint32_t;

#ifdef LA_USE_MDSPAN
//    using span_type       = typename STD_LA::fs_matrix_engine<T, R, C>::span_type;
//    using const_span_type = typename STD_LA::fs_matrix_engine<T, R, C>::const_span_type;
#endif

    using direct_engine_type = test_fs_matrix_engine;
    using owning_engine_type = test_fs_matrix_engine;

  public:
    constexpr test_fs_matrix_engine();
    constexpr test_fs_matrix_engine(test_fs_matrix_engine&&) = default;
    constexpr test_fs_matrix_engine(test_fs_matrix_engine const&) = default;

    constexpr test_fs_matrix_engine&     operator =(test_fs_matrix_engine&&) = default;
    constexpr test_fs_matrix_engine&     operator =(test_fs_matrix_engine const&) = default;

    constexpr reference         operator ()(index_type i, index_type j);
    constexpr const_reference   operator ()(index_type i, index_type j) const;

    constexpr index_type    columns() const noexcept;
    constexpr index_type    rows() const noexcept;
    constexpr index_type    size() const noexcept;

    constexpr index_type    column_capacity() const noexcept;
    constexpr index_type    row_capacity() const noexcept;
    constexpr index_type    capacity() const noexcept;

    constexpr void      swap(test_fs_matrix_engine& rhs) noexcept;

  private:
    std::array<T, R*C>  ma_elems;
};

template<class T, size_t R, size_t C> inline
constexpr
test_fs_matrix_engine<T,R,C>::test_fs_matrix_engine()
:   ma_elems()
{
    if constexpr (std::is_arithmetic_v<T>)
    {
        for (auto& elem : ma_elems) elem = static_cast<T>(0);
    }
}

template<class T, size_t R, size_t C> inline
constexpr typename test_fs_matrix_engine<T,R,C>::reference
test_fs_matrix_engine<T,R,C>::operator ()(index_type i, index_type j)
{
    return ma_elems[i*C + j];
}

template<class T, size_t R, size_t C> inline
constexpr typename test_fs_matrix_engine<T,R,C>::const_reference
test_fs_matrix_engine<T,R,C>::operator ()(index_type i, index_type j) const
{
    return ma_elems[i*C + j];
}

template<class T, size_t R, size_t C> inline
constexpr typename test_fs_matrix_engine<T,R,C>::index_type
test_fs_matrix_engine<T,R,C>::columns() const noexcept
{
    return C;
}

template<class T, size_t R, size_t C> inline
constexpr typename test_fs_matrix_engine<T,R,C>::index_type
test_fs_matrix_engine<T,R,C>::rows() const noexcept
{
    return R;
}

template<class T, size_t R, size_t C> inline
constexpr typename test_fs_matrix_engine<T,R,C>::index_type
test_fs_matrix_engine<T,R,C>::size() const noexcept
{
    return R*C;
}

template<class T, size_t R, size_t C> inline
constexpr typename test_fs_matrix_engine<T,R,C>::index_type
test_fs_matrix_engine<T,R,C>::column_capacity() const noexcept
{
    return C;
}

template<class T, size_t R, size_t C> inline
constexpr typename test_fs_matrix_engine<T,R,C>::index_type
test_fs_matrix_engine<T,R,C>::row_capacity() const noexcept
{
    return R;
}

template<class T, size_t R, size_t C> inline
constexpr typename test_fs_matrix_engine<T,R,C>::index_type
test_fs_matrix_engine<T,R,C>::capacity() const noexcept
{
    return R*C;
}

template<class T, size_t R, size_t C> inline
constexpr void
test_fs_matrix_engine<T,R,C>::swap(test_fs_matrix_engine& rhs) noexcept
{
    if (&rhs != this)
    {
        for (index_type i = 0;  i < R*C;  ++i)
        {
            STD_LA::detail::matrix_engine_support::swap(ma_elems[i], rhs.ma_elems[i]);
        }
    }
}

#endif  //- TEST_NEW_ENGINE_HPP_DEFINED
