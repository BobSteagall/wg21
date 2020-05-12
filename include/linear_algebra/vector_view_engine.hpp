//==================================================================================================
//  File:       subvector_engine.hpp
//
//  Summary:    This header defines engines that act as views of rows and columns.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_SUBVECTOR_ENGINE_HPP_DEFINED
#define LINEAR_ALGEBRA_SUBVECTOR_ENGINE_HPP_DEFINED

namespace STD_LA {
//=================================================================================================
//  Sub-vector engine, meant to act as "view" of a portion of a vector in expressions so as to
//  help avoid unnecessary allocation and element copying.
//==================================================================================================
//
template<class ET, class VCT>
class vector_view_engine<ET, VCT, subvector_view_tag>
{
    static_assert(is_vector_engine_v<ET>);
    static_assert(is_vector_engine_tag<VCT>);

  public:
    //- Types
    //
    using engine_category = VCT;
    using element_type    = typename ET::element_type;
    using value_type      = typename ET::value_type;
    using pointer         = detail::noe_pointer_t<ET, VCT>;
    using const_pointer   = typename ET::const_pointer;
    using reference       = detail::noe_reference_t<ET, VCT>;
    using const_reference = typename ET::const_reference;
    using difference_type = typename ET::difference_type;
    using size_type       = typename ET::size_type;

#ifdef LA_USE_MDSPAN
    using span_type       = detail::noe_mdspan_subvector_t<detail::noe_mdspan_t<ET, VCT>>;
    using const_span_type = detail::noe_mdspan_subvector_t<detail::noe_const_mdspan_t<ET, VCT>>;
#endif

    //- Construct/copy/destroy
    //
    ~vector_view_engine() noexcept = default;

    constexpr vector_view_engine() noexcept;
    constexpr vector_view_engine(vector_view_engine&&) noexcept = default;
    constexpr vector_view_engine(vector_view_engine const&) noexcept = default;

    constexpr vector_view_engine&    operator =(vector_view_engine&&) noexcept = default;
    constexpr vector_view_engine&    operator =(vector_view_engine const&) noexcept = default;

    template<class ET2, detail::enable_if_convertible_engine<ET2, ET> = true>
    constexpr vector_view_engine&    operator =(ET2 const& rhs);
    template<class U, class ET2 = ET, detail::enable_if_writable<ET2, ET> = true>
    constexpr vector_view_engine&    operator =(initializer_list<U> list);

    //- Capacity
    //
    constexpr size_type     capacity() const noexcept;
    constexpr size_type     size() const noexcept;

    //- Element access
    //
    constexpr reference     operator ()(size_type i) const;

    //- Data access
    //
#ifdef LA_USE_MDSPAN
    constexpr span_type     span() const noexcept;
#endif

    //- Modifiers
    //
    constexpr void      swap(vector_view_engine& rhs);

  private:
    template<class ET2, class OT2>  friend class vector;
    using referent_type = detail::noe_referent_t<ET, VCT>;

    referent_type*  mp_other;
    size_type       m_start;
    size_type       m_count;

    constexpr vector_view_engine(referent_type& eng, size_type start, size_type count);
};

//------------------------
//- Construct/copy/destroy
//
template<class ET, class VCT> constexpr
vector_view_engine<ET, VCT, subvector_view_tag>::vector_view_engine() noexcept
:   mp_other(nullptr)
,   m_start(0)
,   m_count(0)
{}

template<class ET, class VCT>
template<class ET2, detail::enable_if_convertible_engine<ET2, ET>> constexpr
vector_view_engine<ET, VCT, subvector_view_tag>&
vector_view_engine<ET, VCT, subvector_view_tag>::operator =(ET2 const& rhs)
{
    detail::check_source_engine_size(rhs, size());
    detail::assign_from_vector_engine(*this, rhs);
    return *this;
}

template<class ET, class VCT>
template<class U, class ET2, detail::enable_if_writable<ET2, ET>> constexpr
vector_view_engine<ET, VCT, subvector_view_tag>&
vector_view_engine<ET, VCT, subvector_view_tag>::operator =(initializer_list<U> rhs)
{
    detail::check_source_init_list(rhs, size());
    detail::assign_from_vector_list(*this, rhs);
    return *this;
}

//----------
//- Capacity
//
template<class ET, class VCT> constexpr
typename vector_view_engine<ET, VCT, subvector_view_tag>::size_type
vector_view_engine<ET, VCT, subvector_view_tag>::capacity() const noexcept
{
    return m_count;
}

template<class ET, class VCT> constexpr
typename vector_view_engine<ET, VCT, subvector_view_tag>::size_type
vector_view_engine<ET, VCT, subvector_view_tag>::size() const noexcept
{
    return m_count;
}

//----------------
//- Element access
//
template<class ET, class VCT> constexpr
typename vector_view_engine<ET, VCT, subvector_view_tag>::reference
vector_view_engine<ET, VCT, subvector_view_tag>::operator ()(size_type i) const
{
    return (*mp_other)(i + m_start);
}

//-------------
//- Data access
//
#ifdef LA_USE_MDSPAN

template<class ET, class VCT> constexpr
typename vector_view_engine<ET, VCT, subvector_view_tag>::span_type
vector_view_engine<ET, VCT, subvector_view_tag>::span() const noexcept
{
    return detail::noe_mdspan_subvector(mp_other->span(), m_start, m_count);
}

#endif
//-----------
//- Modifiers
//
template<class ET, class VCT> constexpr
void
vector_view_engine<ET, VCT, subvector_view_tag>::swap(vector_view_engine& rhs)
{
    std::swap(mp_other, rhs.mp_other);
    std::swap(m_start, rhs.m_start);
    std::swap(m_count, rhs.m_count);
}

//------------------------
//- Private implementation
//
template<class ET, class VCT> constexpr
vector_view_engine<ET, VCT, subvector_view_tag>::vector_view_engine(referent_type& eng, size_type start, size_type count)
:   mp_other(&eng)
,   m_start(start)
,   m_count(count)
{}


//=================================================================================================
//  Matrix column engine, meant to act as a "view" of a portion of a matrix column in expresssions
//  so as to help avoid unnecessary allocation and element copying.
//==================================================================================================
//
template<class ET, class VCT>
class vector_view_engine<ET, VCT, column_view_tag>
{
    static_assert(is_matrix_engine_v<ET>);
    static_assert(is_vector_engine_tag<VCT>);

  public:
    //- Types
    //
    using engine_category = VCT;
    using element_type    = typename ET::element_type;
    using value_type      = typename ET::value_type;
    using pointer         = detail::noe_pointer_t<ET, VCT>;
    using const_pointer   = typename ET::const_pointer;
    using reference       = detail::noe_reference_t<ET, VCT>;
    using const_reference = typename ET::const_reference;
    using difference_type = typename ET::difference_type;
    using size_type       = typename ET::size_type;

#ifdef LA_USE_MDSPAN
    using span_type       = detail::noe_mdspan_rowcolumn_t<detail::noe_mdspan_t<ET, VCT>>;
    using const_span_type = detail::noe_mdspan_rowcolumn_t<detail::noe_const_mdspan_t<ET, VCT>>;
#endif

    //- Construct/copy/destroy
    //
    ~vector_view_engine() noexcept = default;

    constexpr vector_view_engine() noexcept;
    constexpr vector_view_engine(vector_view_engine&&) noexcept = default;
    constexpr vector_view_engine(vector_view_engine const&) noexcept = default;

    constexpr vector_view_engine&    operator =(vector_view_engine&&) noexcept = default;
    constexpr vector_view_engine&    operator =(vector_view_engine const&) noexcept = default;

    template<class ET2, detail::enable_if_convertible_engine<ET2, ET> = true>
    constexpr vector_view_engine&    operator =(ET2 const& rhs);
    template<class U, class ET2 = ET, detail::enable_if_writable<ET2, ET> = true>
    constexpr vector_view_engine&    operator =(initializer_list<U> list);

    //- Capacity
    //
    constexpr size_type     capacity() const noexcept;
    constexpr size_type     size() const noexcept;

    //- Element access
    //
    constexpr reference     operator ()(size_type i) const;

    //- Data access
    //
#ifdef LA_USE_MDSPAN
    constexpr span_type     span() const noexcept;
#endif

    //- Modifiers
    //
    constexpr void      swap(vector_view_engine& rhs);

  private:
    template<class ET2, class OT2>  friend class vector;
    using referent_type = detail::noe_referent_t<ET, VCT>;

    referent_type*  mp_other;
    size_type       m_column;

    constexpr vector_view_engine(referent_type& eng, size_type col);
};

//------------------------
//- Construct/copy/destroy
//
template<class ET, class VCT> constexpr
vector_view_engine<ET, VCT, column_view_tag>::vector_view_engine() noexcept
:   mp_other(nullptr)
,   m_column(0)
{}

template<class ET, class VCT>
template<class ET2, detail::enable_if_convertible_engine<ET2, ET>> constexpr
vector_view_engine<ET, VCT, column_view_tag>&
vector_view_engine<ET, VCT, column_view_tag>::operator =(ET2 const& rhs)
{
    detail::check_source_engine_size(rhs, size());
    detail::assign_from_vector_engine(*this, rhs);
    return *this;
}

template<class ET, class VCT>
template<class U, class ET2, detail::enable_if_writable<ET2, ET>> constexpr
vector_view_engine<ET, VCT, column_view_tag>&
vector_view_engine<ET, VCT, column_view_tag>::operator =(initializer_list<U> rhs)
{
    detail::check_source_init_list(rhs, size());
    detail::assign_from_vector_list(*this, rhs);
    return *this;
}

//----------
//- Capacity
//
template<class ET, class VCT> constexpr
typename vector_view_engine<ET, VCT, column_view_tag>::size_type
vector_view_engine<ET, VCT, column_view_tag>::capacity() const noexcept
{
    return mp_other->rows();
}

template<class ET, class VCT> constexpr
typename vector_view_engine<ET, VCT, column_view_tag>::size_type
vector_view_engine<ET, VCT, column_view_tag>::size() const noexcept
{
    return mp_other->rows();
}

//----------------
//- Element access
//
template<class ET, class VCT> constexpr
typename vector_view_engine<ET, VCT, column_view_tag>::reference
vector_view_engine<ET, VCT, column_view_tag>::operator ()(size_type i) const
{
    return (*mp_other)(i, m_column);
}

//-------------
//- Data access
//
#ifdef LA_USE_MDSPAN

template<class ET, class VCT> constexpr
typename vector_view_engine<ET, VCT, column_view_tag>::span_type
vector_view_engine<ET, VCT, column_view_tag>::span() const noexcept
{
    return detail::noe_mdspan_column(mp_other->span(), m_column);
}

#endif
//-----------
//- Modifiers
//
template<class ET, class VCT> constexpr
void
vector_view_engine<ET, VCT, column_view_tag>::swap(vector_view_engine& rhs)
{
    std::swap(mp_other, rhs.mp_other);
    std::swap(m_column, rhs.m_column);
}

//------------------------
//- Private implementation
//
template<class ET, class VCT> constexpr
vector_view_engine<ET, VCT, column_view_tag>::vector_view_engine(referent_type& eng, size_type col)
:   mp_other(&eng)
,   m_column(col)
{}


//=================================================================================================
//  Matrix row engine, meant to act as a "view" of a portion of a matrix rwo in expresssions
//  so as to help avoid unnecessary allocation and element copying.
//==================================================================================================
//
template<class ET, class VCT>
class vector_view_engine<ET, VCT, row_view_tag>
{
    static_assert(is_matrix_engine_v<ET>);
    static_assert(is_vector_engine_tag<VCT>);

  public:
    //- Types
    //
    using engine_category = VCT;
    using element_type    = typename ET::element_type;
    using value_type      = typename ET::value_type;
    using pointer         = detail::noe_pointer_t<ET, VCT>;
    using const_pointer   = typename ET::const_pointer;
    using reference       = detail::noe_reference_t<ET, VCT>;
    using const_reference = typename ET::const_reference;
    using difference_type = typename ET::difference_type;
    using size_type       = typename ET::size_type;

#ifdef LA_USE_MDSPAN
    using span_type       = detail::noe_mdspan_rowcolumn_t<detail::noe_mdspan_t<ET, VCT>>;
    using const_span_type = detail::noe_mdspan_rowcolumn_t<detail::noe_const_mdspan_t<ET, VCT>>;
#endif

    //- Construct/copy/destroy
    //
    ~vector_view_engine() noexcept = default;

    constexpr vector_view_engine() noexcept;
    constexpr vector_view_engine(vector_view_engine&&) noexcept = default;
    constexpr vector_view_engine(vector_view_engine const&) noexcept = default;

    constexpr vector_view_engine&    operator =(vector_view_engine&&) noexcept = default;
    constexpr vector_view_engine&    operator =(vector_view_engine const&) noexcept = default;

    template<class ET2, detail::enable_if_convertible_engine<ET2, ET> = true>
    constexpr vector_view_engine&    operator =(ET2 const& rhs);
    template<class U, class ET2 = ET, detail::enable_if_writable<ET2, ET> = true>
    constexpr vector_view_engine&    operator =(initializer_list<U> list);

    //- Capacity
    //
    constexpr size_type     capacity() const noexcept;
    constexpr size_type     size() const noexcept;

    //- Element access
    //
    constexpr reference     operator ()(size_type i) const;

    //- Data access
    //
#ifdef LA_USE_MDSPAN
    constexpr span_type     span() const noexcept;
#endif

    //- Modifiers
    //
    constexpr void      swap(vector_view_engine& rhs);

  private:
    template<class ET2, class OT2>  friend class vector;
    using referent_type = detail::noe_referent_t<ET, VCT>;

    referent_type*  mp_other;
    size_type       m_row;

    constexpr vector_view_engine(referent_type& eng, size_type row);
};

//------------------------
//- Construct/copy/destroy
//
template<class ET, class VCT> constexpr
vector_view_engine<ET, VCT, row_view_tag>::vector_view_engine() noexcept
:   mp_other(nullptr)
,   m_row(0)
{}

template<class ET, class VCT>
template<class ET2, detail::enable_if_convertible_engine<ET2, ET>> constexpr
vector_view_engine<ET, VCT, row_view_tag>&
vector_view_engine<ET, VCT, row_view_tag>::operator =(ET2 const& rhs)
{
    detail::check_source_engine_size(rhs, size());
    detail::assign_from_vector_engine(*this, rhs);
    return *this;
}

template<class ET, class VCT>
template<class U, class ET2, detail::enable_if_writable<ET2, ET>> constexpr
vector_view_engine<ET, VCT, row_view_tag>&
vector_view_engine<ET, VCT, row_view_tag>::operator =(initializer_list<U> rhs)
{
    detail::check_source_init_list(rhs, size());
    detail::assign_from_vector_list(*this, rhs);
    return *this;
}

//----------
//- Capacity
//
template<class ET, class VCT> constexpr
typename vector_view_engine<ET, VCT, row_view_tag>::size_type
vector_view_engine<ET, VCT, row_view_tag>::capacity() const noexcept
{
    return mp_other->columns();
}

template<class ET, class VCT> constexpr
typename vector_view_engine<ET, VCT, row_view_tag>::size_type
vector_view_engine<ET, VCT, row_view_tag>::size() const noexcept
{
    return mp_other->columns();
}

//----------------
//- Element access
//
template<class ET, class VCT> constexpr
typename vector_view_engine<ET, VCT, row_view_tag>::reference
vector_view_engine<ET, VCT, row_view_tag>::operator ()(size_type i) const
{
    return (*mp_other)(m_row, i);
}

//-------------
//- Data access
//
#ifdef LA_USE_MDSPAN

template<class ET, class VCT> constexpr
typename vector_view_engine<ET, VCT, row_view_tag>::span_type
vector_view_engine<ET, VCT, row_view_tag>::span() const noexcept
{
    return detail::noe_mdspan_row(mp_other->span(), m_row);
}

#endif
//-----------
//- Modifiers
//
template<class ET, class VCT> constexpr
void
vector_view_engine<ET, VCT, row_view_tag>::swap(vector_view_engine& rhs)
{
    std::swap(mp_other, rhs.mp_other);
    std::swap(m_row, rhs.m_row);
}

//------------------------
//- Private implementation
//
template<class ET, class VCT> constexpr
vector_view_engine<ET, VCT, row_view_tag>::vector_view_engine(referent_type& eng, size_type row)
:   mp_other(&eng)
,   m_row(row)
{}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_SUBVECTOR_ENGINE_HPP_DEFINED
