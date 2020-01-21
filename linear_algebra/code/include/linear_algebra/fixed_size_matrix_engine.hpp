//==================================================================================================
//  File:       fixed_size_matrix_engine.hpp
//
//  Summary:    This header defines a fixed-size matrix engine.  In this context, fixed-size
//              means that the row and column extents of such objects are known at compile-time.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_FIXED_SIZE_MATRIX_ENGINE_HPP_DEFINED
#define LINEAR_ALGEBRA_FIXED_SIZE_MATRIX_ENGINE_HPP_DEFINED

namespace STD_LA {
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
    template<class T2, size_t R2, size_t C2>
    constexpr fs_matrix_engine(fs_matrix_engine<T2, R2, C2> const& rhs);
    template<class ET2>
    constexpr fs_matrix_engine(ET2 const& rhs);
    template<class T2>
    constexpr fs_matrix_engine(initializer_list<T2> rhs);

    constexpr fs_matrix_engine&     operator =(fs_matrix_engine&&) noexcept = default;
    constexpr fs_matrix_engine&     operator =(fs_matrix_engine const&) = default;
    template<class T2, size_t R2, size_t C2>
    constexpr fs_matrix_engine&     operator =(fs_matrix_engine<T2, R2, C2> const& rhs);
    template<class ET2>
    constexpr fs_matrix_engine&     operator =(ET2 const& rhs);
    template<class T2>
    constexpr fs_matrix_engine&     operator =(initializer_list<T2> rhs);

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
    template<class T2, size_t R2, size_t C2> friend class fs_matrix_engine;

    array<T, R*C>   ma_elems;

    template<class T2, size_t R2, size_t C2>
    constexpr void  assign(fs_matrix_engine<T2, R2, C2> const& rhs);
    template<class ET2>
    constexpr void  assign(ET2 const& rhs);
    template<class T2>
    constexpr void  assign(initializer_list<T2> rhs);
};

//------------------------
//- Construct/copy/destroy
//
template<class T, size_t R, size_t C> constexpr
fs_matrix_engine<T,R,C>::fs_matrix_engine()
:   ma_elems()
{}

template<class T, size_t R, size_t C>
template<class T2, size_t R2, size_t C2> constexpr
fs_matrix_engine<T,R,C>::fs_matrix_engine(fs_matrix_engine<T2,R2,C2> const& rhs)
:   ma_elems()
{
    assign(rhs);
}

template<class T, size_t R, size_t C>
template<class ET2> constexpr
fs_matrix_engine<T,R,C>::fs_matrix_engine(ET2 const& rhs)
:   ma_elems()
{
    assign(rhs);
}

template<class T, size_t R, size_t C>
template<class T2> constexpr
fs_matrix_engine<T,R,C>::fs_matrix_engine(initializer_list<T2> rhs)
:   ma_elems()
{
    assign(rhs);
}

template<class T, size_t R, size_t C>
template<class T2, size_t R2, size_t C2> constexpr
fs_matrix_engine<T,R,C>&
fs_matrix_engine<T,R,C>::operator =(fs_matrix_engine<T2,R2,C2> const& rhs)
{
    assign(rhs);
    return *this;
}

template<class T, size_t R, size_t C>
template<class ET2> constexpr
fs_matrix_engine<T,R,C>&
fs_matrix_engine<T,R,C>::operator =(ET2 const& rhs)
{
    assign(rhs);
    return *this;
}

template<class T, size_t R, size_t C>
template<class T2> constexpr
fs_matrix_engine<T,R,C>&
fs_matrix_engine<T,R,C>::operator =(initializer_list<T2> rhs)
{
    assign(rhs);
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
    return span_type(ma_elems.data());
}

template<class T, size_t R, size_t C> constexpr
typename fs_matrix_engine<T,R,C>::const_span_type
fs_matrix_engine<T,R,C>::span() const noexcept
{
    return const_span_type(ma_elems.data());
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

//------------------------
//- Private implementation
//
template<class T, size_t R, size_t C>
template<class T2, size_t R2, size_t C2> constexpr
void
fs_matrix_engine<T,R,C>::assign(fs_matrix_engine<T2,R2,C2> const& rhs)
{
    static_assert(R2 == R  &&  C2 == C);

    for (size_type i = 0;  i < (R*C);  ++i)
    {
        ma_elems[i] = static_cast<T>(rhs.ma_elems[i]);
    }
}

template<class T, size_t R, size_t C>
template<class ET2> constexpr
void
fs_matrix_engine<T,R,C>::assign(ET2 const& rhs)
{
    static_assert(is_matrix_engine_v<ET2>);
    using src_size_type = typename ET2::size_type;

    if ((static_cast<size_type>(rhs.rows()) != R) || (static_cast<size_type>(rhs.columns()) != C))
    {
        throw runtime_error("invalid size");
    }

    src_size_type   si = 0, sj = 0;
    size_type       di = 0, dj = 0;

    for (;  di < R;  ++di, ++si)
    {
        for (;  dj < C;  ++dj, ++sj)
        {
            (*this)(di, dj) = rhs(si, sj);
        }
    }
}

template<class T, size_t R, size_t C>
template<class T2> constexpr
void
fs_matrix_engine<T,R,C>::assign(initializer_list<T2> rhs)
{
    if (rhs.size() > (R*C))
    {
        throw runtime_error("invalid size");
    }

    size_type   count = min(R*C, rhs.size());
    size_type   di    = 0;
    auto        iter  = rhs.begin();

    for (;  di < count;  ++di, ++iter)
    {
        ma_elems[di] = static_cast<T>(*iter);
    }

    for (;  di < (R*C);  ++di)
    {
        ma_elems[di] = static_cast<T>(0);
    }
}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_FIXED_SIZE_MATRIX_ENGINE_HPP_DEFINED
