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
template<class T, ptrdiff_t R, ptrdiff_t C>
class fs_matrix_engine
{
    static_assert(R >= 1);
    static_assert(C >= 1);

  public:
    //- Types
    //
    using engine_category = initable_matrix_engine_tag;
    using element_type    = T;
    using value_type      = remove_cv_t<T>;
    using pointer         = element_type*;
    using const_pointer   = element_type const*;
    using reference       = element_type&;
    using const_reference = element_type const&;
    using difference_type = ptrdiff_t;
    using index_type      = ptrdiff_t;
    using index_tuple     = extents<R, C>;
    using span_type       = mdspan<element_type, R, C>;
    using const_span_type = mdspan<element_type const, R, C>;

    //- Construct/copy/destroy
    //
    ~fs_matrix_engine() noexcept = default;

    constexpr fs_matrix_engine();
    constexpr fs_matrix_engine(fs_matrix_engine&&) noexcept = default;
    constexpr fs_matrix_engine(fs_matrix_engine const&) = default;
    template<class T2, ptrdiff_t R2, ptrdiff_t C2>
    constexpr fs_matrix_engine(fs_matrix_engine<T2, R2, C2> const& rhs);
    template<class ET2, detail::enable_if_engine_has_convertible_element<ET2,T> = true>
    constexpr fs_matrix_engine(ET2 const& rhs);
    template<class T2, detail::enable_if_convertible_element<T2,T> = true>
    constexpr fs_matrix_engine(initializer_list<initializer_list<T2>> rhs);

    constexpr fs_matrix_engine&     operator =(fs_matrix_engine&&) noexcept = default;
    constexpr fs_matrix_engine&     operator =(fs_matrix_engine const&) = default;
    template<class T2, ptrdiff_t R2, ptrdiff_t C2>
    constexpr fs_matrix_engine&     operator =(fs_matrix_engine<T2, R2, C2> const& rhs);
    template<class ET2, detail::enable_if_engine_has_convertible_element<ET2,T> = true>
    constexpr fs_matrix_engine&     operator =(ET2 const& rhs);
    template<class T2, detail::enable_if_convertible_element<T2,T> = true>
    constexpr fs_matrix_engine&     operator =(initializer_list<initializer_list<T2>> rhs);

    //- Capacity
    //
    constexpr index_type    columns() const noexcept;
    constexpr index_type    rows() const noexcept;
    constexpr index_tuple   size() const noexcept;

    constexpr index_type    column_capacity() const noexcept;
    constexpr index_type    row_capacity() const noexcept;
    constexpr index_tuple   capacity() const noexcept;

    //- Element access
    //
    constexpr reference         operator ()(index_type i, index_type j);
    constexpr const_reference   operator ()(index_type i, index_type j) const;

    //- Data access
    //
    constexpr span_type         span() noexcept;
    constexpr const_span_type   span() const noexcept;

    //- Modifiers
    //
    constexpr void      swap(fs_matrix_engine& rhs) noexcept;
    constexpr void      swap_columns(index_type j1, index_type j2) noexcept;
    constexpr void      swap_rows(index_type i1, index_type i2) noexcept;

  private:
    template<class T2, ptrdiff_t R2, ptrdiff_t C2> friend class fs_matrix_engine;

    array<T, R*C>   ma_elems;

    template<class T2, ptrdiff_t R2, ptrdiff_t C2>
    constexpr void  assign(fs_matrix_engine<T2, R2, C2> const& rhs);
    template<class ET2>
    constexpr void  assign(ET2 const& rhs);
    template<class T2>
    constexpr void  assign(initializer_list<initializer_list<T2>> rhs);
};

//------------------------
//- Construct/copy/destroy
//
template<class T, ptrdiff_t R, ptrdiff_t C> constexpr
fs_matrix_engine<T,R,C>::fs_matrix_engine()
:   ma_elems()
{}

template<class T, ptrdiff_t R, ptrdiff_t C>
template<class T2, ptrdiff_t R2, ptrdiff_t C2> constexpr
fs_matrix_engine<T,R,C>::fs_matrix_engine(fs_matrix_engine<T2,R2,C2> const& rhs)
:   ma_elems()
{
    assign(rhs);
}

template<class T, ptrdiff_t R, ptrdiff_t C>
template<class ET2, detail::enable_if_engine_has_convertible_element<ET2,T>> constexpr
fs_matrix_engine<T,R,C>::fs_matrix_engine(ET2 const& rhs)
:   ma_elems()
{
    assign(rhs);
}

template<class T, ptrdiff_t R, ptrdiff_t C>
template<class T2, detail::enable_if_convertible_element<T2,T>> constexpr
fs_matrix_engine<T,R,C>::fs_matrix_engine(initializer_list<initializer_list<T2>> rhs)
:   ma_elems()
{
    assign(rhs);
}

template<class T, ptrdiff_t R, ptrdiff_t C>
template<class T2, ptrdiff_t R2, ptrdiff_t C2> constexpr
fs_matrix_engine<T,R,C>&
fs_matrix_engine<T,R,C>::operator =(fs_matrix_engine<T2,R2,C2> const& rhs)
{
    assign(rhs);
    return *this;
}

template<class T, ptrdiff_t R, ptrdiff_t C>
template<class ET2, detail::enable_if_engine_has_convertible_element<ET2,T>> constexpr
fs_matrix_engine<T,R,C>&
fs_matrix_engine<T,R,C>::operator =(ET2 const& rhs)
{
    assign(rhs);
    return *this;
}

template<class T, ptrdiff_t R, ptrdiff_t C>
template<class T2, detail::enable_if_convertible_element<T2,T>> constexpr
fs_matrix_engine<T,R,C>&
fs_matrix_engine<T,R,C>::operator =(initializer_list<initializer_list<T2>> rhs)
{
    assign(rhs);
    return *this;
}

//----------
//- Capacity
//
template<class T, ptrdiff_t R, ptrdiff_t C> constexpr
typename fs_matrix_engine<T,R,C>::index_type
fs_matrix_engine<T,R,C>::columns() const noexcept
{
    return C;
}

template<class T, ptrdiff_t R, ptrdiff_t C> constexpr
typename fs_matrix_engine<T,R,C>::index_type
fs_matrix_engine<T,R,C>::rows() const noexcept
{
    return R;
}

template<class T, ptrdiff_t R, ptrdiff_t C> constexpr
typename fs_matrix_engine<T,R,C>::index_tuple
fs_matrix_engine<T,R,C>::size() const noexcept
{
    return index_tuple{};
}

template<class T, ptrdiff_t R, ptrdiff_t C> constexpr
typename fs_matrix_engine<T,R,C>::index_type
fs_matrix_engine<T,R,C>::column_capacity() const noexcept
{
    return C;
}

template<class T, ptrdiff_t R, ptrdiff_t C> constexpr
typename fs_matrix_engine<T,R,C>::index_type
fs_matrix_engine<T,R,C>::row_capacity() const noexcept
{
    return R;
}

template<class T, ptrdiff_t R, ptrdiff_t C> constexpr
typename fs_matrix_engine<T,R,C>::index_tuple
fs_matrix_engine<T,R,C>::capacity() const noexcept
{
    return index_tuple{};
}

//----------------
//- Element access
//
template<class T, ptrdiff_t R, ptrdiff_t C> constexpr
typename fs_matrix_engine<T,R,C>::reference
fs_matrix_engine<T,R,C>::operator ()(index_type i, index_type j)
{
    return ma_elems[i*C + j];
}

template<class T, ptrdiff_t R, ptrdiff_t C> constexpr
typename fs_matrix_engine<T,R,C>::const_reference
fs_matrix_engine<T,R,C>::operator ()(index_type i, index_type j) const
{
    return ma_elems[i*C + j];
}

//-------------
//- Data access
//
template<class T, ptrdiff_t R, ptrdiff_t C> constexpr
typename fs_matrix_engine<T,R,C>::span_type
fs_matrix_engine<T,R,C>::span() noexcept
{
    return span_type(ma_elems.data());
}

template<class T, ptrdiff_t R, ptrdiff_t C> constexpr
typename fs_matrix_engine<T,R,C>::const_span_type
fs_matrix_engine<T,R,C>::span() const noexcept
{
    return const_span_type(ma_elems.data());
}

//-----------
//- Modifiers
//
template<class T, ptrdiff_t R, ptrdiff_t C> constexpr
void
fs_matrix_engine<T,R,C>::swap(fs_matrix_engine& rhs) noexcept
{
    if (&rhs != this)
    {
        for (ptrdiff_t i = 0;  i < R*C;  ++i)
        {
            detail::la_swap(ma_elems[i], rhs.ma_elems[i]);
        }
    }
}

template<class T, ptrdiff_t R, ptrdiff_t C> constexpr
void
fs_matrix_engine<T,R,C>::swap_columns(index_type j1, index_type j2) noexcept
{
    if (j1 != j2)
    {
        for (ptrdiff_t i = 0;  i < R;  ++i)
        {
            detail::la_swap(ma_elems[i*C + j1], ma_elems[i*C + j2]);
        }
    }
}

template<class T, ptrdiff_t R, ptrdiff_t C> constexpr
void
fs_matrix_engine<T,R,C>::swap_rows(index_type i1, index_type i2) noexcept
{
    if (i1 != i2)
    {
        for (ptrdiff_t j = 0;  j < C;  ++j)
        {
            detail::la_swap(ma_elems[i1*C + j], ma_elems[i2*C + j]);
        }
    }
}

//------------------------
//- Private implementation
//
template<class T, ptrdiff_t R, ptrdiff_t C>
template<class T2, ptrdiff_t R2, ptrdiff_t C2> constexpr
void
fs_matrix_engine<T,R,C>::assign(fs_matrix_engine<T2,R2,C2> const& rhs)
{
    static_assert(R2 == R  &&  C2 == C);

    for (index_type i = 0;  i < (R*C);  ++i)
    {
        ma_elems[i] = static_cast<T>(rhs.ma_elems[i]);
    }
}

template<class T, ptrdiff_t R, ptrdiff_t C>
template<class ET2> constexpr
void
fs_matrix_engine<T,R,C>::assign(ET2 const& rhs)
{
    static_assert(is_matrix_engine_v<ET2>);
    detail::check_source_engine_size(rhs, R, C);
    detail::assign_from_matrix_engine(*this, rhs);
}

template<class T, ptrdiff_t R, ptrdiff_t C>
template<class T2> constexpr
void
fs_matrix_engine<T,R,C>::assign(initializer_list<initializer_list<T2>> rhs)
{
    detail::check_source_init_list(rhs, R, C);
    detail::assign_from_matrix_initlist(*this, rhs);
}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_FIXED_SIZE_MATRIX_ENGINE_HPP_DEFINED
