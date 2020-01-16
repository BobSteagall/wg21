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
//  Fixed-size, fixed-capacity vector engine.
//==================================================================================================
//
template<class T, size_t N>
class fs_vector_engine
{
    static_assert(N >= 1);

  public:
    using engine_category = writable_vector_engine_tag;
    using element_type    = T;
    using value_type      = remove_cv_t<T>;
    using pointer         = element_type*;
    using const_pointer   = element_type const*;
    using reference       = element_type&;
    using const_reference = element_type const&;
    using difference_type = ptrdiff_t;
    using size_type       = size_t;

#ifdef LA_USE_VECTOR_ENGINE_ITERATORS
    using iterator        = detail::vector_iterator<fs_vector_engine>;
    using const_iterator  = detail::vector_const_iterator<fs_vector_engine>;
#endif

#ifdef LA_USE_MDSPAN
    using span_type       = mdspan<element_type, N>;
    using const_span_type = mdspan<element_type const, N>;
#endif

    //- Construct/copy/destroy
    //
    ~fs_vector_engine() noexcept = default;

    constexpr fs_vector_engine();
    constexpr fs_vector_engine(fs_vector_engine&&) noexcept = default;
    constexpr fs_vector_engine(fs_vector_engine const&) = default;
    template<class U>
    constexpr fs_vector_engine(initializer_list<U> list);

    constexpr fs_vector_engine&     operator =(fs_vector_engine&&) noexcept = default;
    constexpr fs_vector_engine&     operator =(fs_vector_engine const&) = default;

#ifdef LA_USE_VECTOR_ENGINE_ITERATORS
    //- Iterators
    //
    constexpr iterator          begin() noexcept;
    constexpr iterator          end() noexcept;
    constexpr const_iterator    begin() const noexcept;
    constexpr const_iterator    end() const noexcept;
    constexpr const_iterator    cbegin() const noexcept;
    constexpr const_iterator    cend() const noexcept;
#endif

    //- Capacity
    //
    static constexpr size_type  capacity() noexcept;
    static constexpr size_type  elements() noexcept;

    //- Element access
    //
    constexpr reference         operator ()(size_type i);
    constexpr const_reference   operator ()(size_type i) const;

#ifdef LA_USE_MDSPAN
    constexpr span_type         span() noexcept;
    constexpr const_span_type   span() const noexcept;
#endif

    //- Modifiers
    //
    constexpr void  swap(fs_vector_engine& rhs) noexcept;
    constexpr void  swap_elements(size_type i, size_type j) noexcept;

  private:
    T   ma_elems[N];
};

//------------------------
//- Construct/copy/destroy
//
template<class T, size_t N> constexpr
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

template<class T, size_t N>
template<class U> constexpr
fs_vector_engine<T,N>::fs_vector_engine(initializer_list<U> list)
:   ma_elems()
{
    size_type   count = min<size_type>(N, list.size());
    auto        iter  = list.begin();

    for (size_type i = 0;  i < count;  ++i, ++iter)
    {
        ma_elems[i] = static_cast<T>( *iter);
    }

    if constexpr (is_arithmetic_v<T>)
    {
        if (count < N)
        {
            for (size_type i = count;  i < N;  ++i)
            {
                ma_elems[i] = static_cast<T>(0);
            }
        }
    }
}

#ifdef LA_USE_VECTOR_ENGINE_ITERATORS
//-----------
//- Iterators
//
template<class T, size_t N> constexpr
typename fs_vector_engine<T,N>::iterator
fs_vector_engine<T,N>::begin() noexcept
{
    return iterator(this, 0, N);
}

template<class T, size_t N> constexpr
typename fs_vector_engine<T,N>::const_iterator
fs_vector_engine<T,N>::begin() const noexcept
{
    return const_iterator(this, 0, N);
}

template<class T, size_t N> constexpr
typename fs_vector_engine<T,N>::iterator
fs_vector_engine<T,N>::end() noexcept
{
    return iterator(this, N, N);
}

template<class T, size_t N> constexpr
typename fs_vector_engine<T,N>::const_iterator
fs_vector_engine<T,N>::end() const noexcept
{
    return const_iterator(this, N, N);
}

template<class T, size_t N> constexpr
typename fs_vector_engine<T,N>::const_iterator
fs_vector_engine<T,N>::cbegin() const noexcept
{
    return const_iterator(this, 0, N);
}

template<class T, size_t N> constexpr
typename fs_vector_engine<T,N>::const_iterator
fs_vector_engine<T,N>::cend() const noexcept
{
    return const_iterator(this, N, N);
}

#endif
//----------
//- Capacity
//
template<class T, size_t N> constexpr
typename fs_vector_engine<T,N>::size_type
fs_vector_engine<T,N>::capacity() noexcept
{
    return N;
}

template<class T, size_t N> constexpr
typename fs_vector_engine<T,N>::size_type
fs_vector_engine<T,N>::elements() noexcept
{
    return N;
}

//----------------
//- Element access
//
template<class T, size_t N> constexpr
typename fs_vector_engine<T,N>::reference
fs_vector_engine<T,N>::operator ()(size_type i)
{
    return ma_elems[i];
}

template<class T, size_t N> constexpr
typename fs_vector_engine<T,N>::const_reference
fs_vector_engine<T,N>::operator ()(size_type i) const
{
    return ma_elems[i];
}

#ifdef LA_USE_MDSPAN

template<class T, size_t N> constexpr
typename fs_vector_engine<T,N>::span_type
fs_vector_engine<T,N>::span() noexcept
{
    return span_type(&ma_elems[0]);
}

template<class T, size_t N> constexpr
typename fs_vector_engine<T,N>::const_span_type
fs_vector_engine<T,N>::span() const noexcept
{
    return const_span_type(&ma_elems[0]);
}

#endif
//-----------
//- Modifiers
//
template<class T, size_t N> constexpr
void
fs_vector_engine<T,N>::swap(fs_vector_engine& rhs) noexcept
{
    if (&rhs != this)
    {
        for (size_t i = 0;  i < N;  ++i)
        {
            detail::la_swap(ma_elems[i], rhs.ma_elems[i]);
        }
    }
}

template<class T, size_t N> constexpr
void
fs_vector_engine<T,N>::swap_elements(size_type i, size_type j) noexcept
{
    detail::la_swap(ma_elems[i], ma_elems[j]);
}


//==================================================================================================
//  Fixed-size, fixed-capacity matrix engine.
//==================================================================================================
//
template<class T, size_t R, size_t C>
class fs_matrix_engine
{
    static_assert(R >= 1);
    static_assert(C >= 1);

  public:
    //- Types
    //
    using engine_category = writable_matrix_engine_tag;
    using element_type    = T;
    using value_type      = remove_cv_t<T>;
    using pointer         = element_type*;
    using const_pointer   = element_type const*;
    using reference       = element_type&;
    using const_reference = element_type const&;
    using difference_type = ptrdiff_t;
    using size_type       = size_t;
    using size_tuple      = tuple<size_type, size_type>;

#ifdef LA_USE_MDSPAN
    using span_type       = mdspan<element_type, R, C>;
    using const_span_type = mdspan<element_type const, R, C>;
#endif

    //- Construct/copy/destroy
    //
    ~fs_matrix_engine() noexcept = default;

    constexpr fs_matrix_engine();
    constexpr fs_matrix_engine(fs_matrix_engine&&) noexcept = default;
    constexpr fs_matrix_engine(fs_matrix_engine const&) = default;
    template<class U>
    constexpr fs_matrix_engine(initializer_list<U> list);

    constexpr fs_matrix_engine&     operator =(fs_matrix_engine&&) noexcept = default;
    constexpr fs_matrix_engine&     operator =(fs_matrix_engine const&) = default;
    template<class ET2>
    constexpr fs_matrix_engine&     operator =(ET2 const& rhs);

    //- Capacity
    //
    constexpr size_type     columns() const noexcept;
    constexpr size_type     rows() const noexcept;
    constexpr size_tuple    size() const noexcept;

    constexpr size_type     column_capacity() const noexcept;
    constexpr size_type     row_capacity() const noexcept;
    constexpr size_tuple    capacity() const noexcept;

    //- Element access
    //
    constexpr reference         operator ()(size_type i, size_type j);
    constexpr const_reference   operator ()(size_type i, size_type j) const;

#ifdef LA_USE_MDSPAN
    constexpr span_type         span() noexcept;
    constexpr const_span_type   span() const noexcept;
#endif

    //- Modifiers
    //
    constexpr void      swap(fs_matrix_engine& rhs) noexcept;
    constexpr void      swap_columns(size_type j1, size_type j2) noexcept;
    constexpr void      swap_rows(size_type i1, size_type i2) noexcept;

  private:
    T   ma_elems[R*C];
};

//------------------------
//- Construct/copy/destroy
//
template<class T, size_t R, size_t C> constexpr
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

template<class T, size_t R, size_t C>
template<class U> constexpr
fs_matrix_engine<T,R,C>::fs_matrix_engine(initializer_list<U> list)
:   ma_elems()
{
    size_t const    total = R*C;
    size_t const    count = std::min(total, (size_t) list.size());
    auto            iter  = list.begin();

    for (size_t i = 0;  i < count;  ++i, ++iter)
    {
        ma_elems[i] = static_cast<T>(*iter);
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

template<class T, size_t R, size_t C>
template<class ET2> constexpr
fs_matrix_engine<T,R,C>&
fs_matrix_engine<T,R,C>::operator =(ET2 const& rhs)
{
    using src_size_type = typename ET2::size_type;

    if (rhs.size() != size())
    {
        throw runtime_error("invalid size");
    }

    src_size_type   si = 0, sj = 0;
    size_type       di = 0, dj = 0;

    for (;  di < rows();  ++di, ++si)
    {
        for (;  dj < columns();  ++dj, ++sj)
        {
            (*this)(di, dj) = rhs(si, sj);
        }
    }

    return *this;
}

//----------
//- Capacity
//
template<class T, size_t R, size_t C> constexpr
typename fs_matrix_engine<T,R,C>::size_type
fs_matrix_engine<T,R,C>::columns() const noexcept
{
    return C;
}

template<class T, size_t R, size_t C> constexpr
typename fs_matrix_engine<T,R,C>::size_type
fs_matrix_engine<T,R,C>::rows() const noexcept
{
    return R;
}

template<class T, size_t R, size_t C> constexpr
typename fs_matrix_engine<T,R,C>::size_tuple
fs_matrix_engine<T,R,C>::size() const noexcept
{
    return size_tuple(R, C);
}

template<class T, size_t R, size_t C> constexpr
typename fs_matrix_engine<T,R,C>::size_type
fs_matrix_engine<T,R,C>::column_capacity() const noexcept
{
    return C;
}

template<class T, size_t R, size_t C> constexpr
typename fs_matrix_engine<T,R,C>::size_type
fs_matrix_engine<T,R,C>::row_capacity() const noexcept
{
    return R;
}

template<class T, size_t R, size_t C> constexpr
typename fs_matrix_engine<T,R,C>::size_tuple
fs_matrix_engine<T,R,C>::capacity() const noexcept
{
    return size_tuple(R, C);
}

//----------------
//- Element access
//
template<class T, size_t R, size_t C> constexpr
typename fs_matrix_engine<T,R,C>::reference
fs_matrix_engine<T,R,C>::operator ()(size_type i, size_type j)
{
    return ma_elems[i*C + j];
}

template<class T, size_t R, size_t C> constexpr
typename fs_matrix_engine<T,R,C>::const_reference
fs_matrix_engine<T,R,C>::operator ()(size_type i, size_type j) const
{
    return ma_elems[i*C + j];
}

#ifdef LA_USE_MDSPAN

template<class T, size_t R, size_t C> constexpr
typename fs_matrix_engine<T,R,C>::span_type
fs_matrix_engine<T,R,C>::span() noexcept
{
    return span_type(&ma_elems[0]);
}

template<class T, size_t R, size_t C> constexpr
typename fs_matrix_engine<T,R,C>::const_span_type
fs_matrix_engine<T,R,C>::span() const noexcept
{
    return const_span_type(&ma_elems[0]);
}

#endif
//-----------
//- Modifiers
//
template<class T, size_t R, size_t C> constexpr
void
fs_matrix_engine<T,R,C>::swap(fs_matrix_engine& rhs) noexcept
{
    if (&rhs != this)
    {
        for (size_t i = 0;  i < R*C;  ++i)
        {
            detail::la_swap(ma_elems[i], rhs.ma_elems[i]);
        }
    }
}

template<class T, size_t R, size_t C> constexpr
void
fs_matrix_engine<T,R,C>::swap_columns(size_type j1, size_type j2) noexcept
{
    if (j1 != j2)
    {
        for (size_t i = 0;  i < R;  ++i)
        {
            detail::la_swap(ma_elems[i*C + j1], ma_elems[i*C + j2]);
        }
    }
}

template<class T, size_t R, size_t C> constexpr
void
fs_matrix_engine<T,R,C>::swap_rows(size_type i1, size_type i2) noexcept
{
    if (i1 != i2)
    {
        for (size_t j = 0;  j < C;  ++j)
        {
            detail::la_swap(ma_elems[i1*C + j], ma_elems[i2*C + j]);
        }
    }
}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_FIXED_SIZE_ENGINES_HPP_DEFINED
