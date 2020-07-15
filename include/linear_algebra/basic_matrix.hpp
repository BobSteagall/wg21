//==================================================================================================
//  File:       matrix.hpp
//
//  Summary:    This header defines the matrix class template, one of the two primary math
//              objects provided by the library.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_BASIC_MATRIX_HPP_DEFINED
#define LINEAR_ALGEBRA_BASIC_MATRIX_HPP_DEFINED

namespace STD_LA {

template<class ET, class OT>
    requires
        default_initializable<ET>   and
        copyable<ET>                and
        detail::readable_matrix_engine<ET>
class basic_matrix
{
  public:
    using engine_type          = ET;
    using owning_engine_type   = detail::determine_owning_engine_type_t<ET>;
    using element_type         = typename engine_type::element_type;
    using value_type           = typename engine_type::value_type;
    using reference            = typename engine_type::reference;
    using const_reference      = typename engine_type::const_reference;
    using index_type           = typename engine_type::index_type;
    using index_tuple_type = tuple<index_type, index_type>;
    using span_type            = detail::engine_span_t<ET>;
    using const_span_type      = detail::engine_const_span_t<ET>;

  public:
    ~basic_matrix() = default;

    //- Construct / assign.
    //
    constexpr basic_matrix() = default;
    constexpr basic_matrix(basic_matrix&&) noexcept = default;
    constexpr basic_matrix(basic_matrix const&) = default;

    basic_matrix&   operator =(basic_matrix&&) noexcept = default;
    basic_matrix&   operator =(basic_matrix const&) = default;

    //- Other constructors.
    //
    inline constexpr
    basic_matrix(index_type rows, index_type cols)
        requires detail::reshapable_matrix_engine<engine_type>
    :   m_engine(rows, cols, rows, cols)
    {}

    inline constexpr
    basic_matrix(index_type rows, index_type cols, index_type rowcap, index_type colcap)
        requires detail::reshapable_matrix_engine<engine_type>
    :   m_engine(rows, cols, rowcap, colcap)
    {}

    template<class ET2> inline constexpr
    basic_matrix(ET2 const& rhs)
        requires detail::readable_matrix_engine<ET2>
    :   m_engine(rhs)
    {}

    template<class T2> inline constexpr
    basic_matrix(initializer_list<initializer_list<T2>> rhs)
        requires detail::initlist_constructible_matrix_engine<engine_type, T2>
    :   m_engine(rhs)
    {}

    template<class T2> inline constexpr
    basic_matrix(initializer_list<T2> rhs)
        requires detail::initlist_constructible_vector_engine<engine_type, T2>
    :   m_engine(rhs)
    {}

    //- Other assignment operators.
    //
    template<class ET2>
    inline constexpr basic_matrix&
    operator =(ET2 const& rhs)
        requires detail::readable_matrix_engine<ET2>
    {
        m_engine = rhs;
        return *this;
    }

    template<class T2>
    inline constexpr basic_matrix&
    operator =(initializer_list<initializer_list<T2>> rhs)
        requires detail::initlist_assignable_matrix_engine<engine_type, T2>
    {
        m_engine = rhs;
        return *this;
    }

    template<class T2>
    inline constexpr basic_matrix&
    operator =(initializer_list<T2> rhs)
        requires detail::initlist_assignable_vector_engine<engine_type, T2>
    {
        m_engine = rhs;
        return *this;
    }

    //- Size and capacity reporting.
    //
    inline constexpr index_type
    elements() const noexcept
    {
        return m_engine.elements();
    }

    inline constexpr index_type
    columns() const noexcept
    {
        return m_engine.columns();
    }

    inline constexpr index_type
    rows() const noexcept
    {
        return m_engine.rows();
    }

    inline constexpr index_type
    size() const noexcept
    {
        return index_tuple_type{m_engine.rows(), m_engine.columns()};
    }

    inline constexpr index_type
    column_capacity() const noexcept
    {
        return m_engine.column_capacity();
    }

    inline constexpr index_type
    row_capacity() const noexcept
    {
        return m_engine.row_capacity();
    }

    inline constexpr index_tuple_type
    capacity() const noexcept
    {
        return index_tuple_type{m_engine.row_capacity(), m_engine.column_capacity()};
    }

    //- Element access
    //
    inline constexpr reference
    operator ()(index_type i, index_type j)
    {
        return m_engine(i, j);
    }

    inline constexpr const_reference
    operator ()(index_type i, index_type j) const
        requires detail::writable_matrix_engine<engine_type>
    {
        return m_engine(i, j);
    }

    inline constexpr reference
    operator ()(index_type i)
        requires  detail::writable_vector_engine<engine_type>
    {
        return m_engine(i);
    }

    inline constexpr const_reference
    operator ()(index_type i) const
        requires  detail::readable_vector_engine<engine_type>
    {
        return m_engine(i);
    }

    inline constexpr span_type
    span() noexcept
        requires detail::spannable_matrix_engine<engine_type>
    {
        return m_engine.span();
    }

    inline constexpr const_span_type
    span() const noexcept
        requires detail::spannable_matrix_engine<engine_type>
    {
        return m_engine.span();
    }

    //- Setting column size and capacity.
    //
    constexpr void
    resize_columns(index_type cols)
        requires detail::column_reshapable_matrix_engine<engine_type>
    {
        m_engine.reshape_columns(cols, m_engine.column_capacity());
    }

    constexpr void
    reserve_columns(index_type colcap)
        requires detail::column_reshapable_matrix_engine<engine_type>
    {
        m_engine.reshape_columns(m_engine.columns(), colcap);
    }

    constexpr void
    reshape_columns(index_type cols, index_type colcap)
        requires detail::column_reshapable_matrix_engine<engine_type>
    {
        m_engine.reshape_columns(cols, colcap);
    }

    //- Setting row size and capacity.
    //
    constexpr void
    resize_rows(index_type rows)
        requires detail::row_reshapable_matrix_engine<engine_type>
    {
        m_engine.reshape_rows(rows, m_engine.row_capacity());
    }

    constexpr void
    reserve_rows(index_type rowcap)
        requires detail::row_reshapable_matrix_engine<engine_type>
    {
        m_engine.reshape_rows(m_engine.rows(), rowcap);
    }

    constexpr void
    reshape_rows(index_type rows, index_type rowcap)
        requires detail::row_reshapable_matrix_engine<engine_type>
    {
        m_engine.reshape_rows(rows, rowcap);
    }

    //- Setting overall size and capacity.
    //
    inline constexpr void
    resize(index_type rows, index_type cols)
        requires detail::reshapable_matrix_engine<engine_type>
    {
        m_engine.reshape(rows, cols, m_engine.row_capacity(), m_engine.column_capacity());
    }

    inline constexpr void
    reserve(index_type rowcap, index_type colcap)
        requires detail::reshapable_matrix_engine<engine_type>
    {
        m_engine.reshape(m_engine.rows(), m_engine.columns(), rowcap, colcap);
    }

    inline constexpr void
    reshape(index_type rows, index_type cols, index_type rowcap, index_type colcap)
        requires detail::reshapable_matrix_engine<engine_type>
    {
        m_engine.reshape(rows, cols, rowcap, colcap);
    }

    //- Modifiers
    //
    inline constexpr void
    swap(basic_matrix& rhs) noexcept
    {
        m_engine.swap(rhs.m_engine);
    }

  private:
    engine_type     m_engine;
};


template<class T, ptrdiff_t R, ptrdiff_t C, class OT = matrix_operation_traits>
using fs_dyn_matrix = basic_matrix<matrix_storage_engine<T, extents<R, C>, allocator<T>, row_major>, OT>;

template<class T, class OT = matrix_operation_traits>
using dr_matrix = basic_matrix<matrix_storage_engine<T, extents<dynamic_extent, dynamic_extent>, allocator<T>, row_major>, OT>;

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_BASIC_MATRIX_HPP_DEFINED
