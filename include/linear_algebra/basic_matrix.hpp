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
    static constexpr bool   engine_has_mdspan = detail::spannable_matrix_engine<ET>;

    using engine_support = detail::engine_support;

  public:
    using engine_type          = ET;
    using owning_engine_type   = detail::determine_owning_engine_type_t<ET>;
    using element_type         = typename engine_type::element_type;
    using value_type           = typename engine_type::value_type;
    using reference            = typename engine_type::reference;
    using const_reference      = typename engine_type::const_reference;
    using index_type           = typename engine_type::index_type;
    using span_type            = conditional_t<engine_has_mdspan, detail::engine_mdspan_t<ET>, void>;
    using const_span_type      = conditional_t<engine_has_mdspan, detail::engine_const_mdspan_t<ET>, void>;

  public:
    ~basic_matrix() = default;

    constexpr basic_matrix() = default;
    constexpr basic_matrix(basic_matrix&&) noexcept = default;
    constexpr basic_matrix(basic_matrix const&) = default;

    basic_matrix&   operator =(basic_matrix&&) noexcept = default;
    basic_matrix&   operator =(basic_matrix const&) = default;

    //---------------------
    //- Other constructors.
    //
    inline constexpr
    basic_matrix(index_type rows, index_type cols)
        requires
            detail::reshapable_matrix_engine<engine_type>
    :   m_engine(rows, cols, rows, cols)
    {}

    inline constexpr
    basic_matrix(index_type rows, index_type cols, index_type rowcap, index_type colcap)
        requires
            detail::reshapable_matrix_engine<engine_type>
    :   m_engine(rows, cols, rowcap, colcap)
    {}

    //- Construction from a different matrix engine type.
    //
    template<class ET2, class OT2>
    inline constexpr
    basic_matrix(basic_matrix<ET2, OT2> const& rhs)
        requires
            detail::writable_matrix_engine<engine_type>                 and
            detail::constructible_from_engine<engine_type, ET2>
    :   m_engine(rhs)
    {}

    template<class ET2, class OT2>
    inline constexpr
    basic_matrix(basic_matrix<ET2, OT2> const& rhs)
        requires
            detail::writable_matrix_engine<engine_type>                 and
            (not detail::constructible_from_engine<engine_type, ET2>)   and
            detail::convertible_from<element_type, typename ET2::element_type>
    :   m_engine()
    {
        engine_support::assign_from_matrix_engine(m_engine, rhs.m_engine);
    }

    //- Construction from a 2D initializtion list.
    //
    template<class T2>
    inline constexpr
    basic_matrix(initializer_list<initializer_list<T2>> rhs)
        requires
            detail::writable_matrix_engine<engine_type>                 and
            detail::constructible_from_2d_list<engine_type, T2>
    :   m_engine(rhs)
    {}

    template<class T2>
    inline constexpr
    basic_matrix(initializer_list<initializer_list<T2>> rhs)
        requires
            detail::writable_matrix_engine<engine_type>                 and
            (not detail::constructible_from_2d_list<engine_type, T2>)   and
            detail::convertible_from<element_type, T2>
    :   m_engine()
    {
        engine_support::assign_from_matrix_initlist(m_engine, rhs);
    }

    //- Construction from a 1D initializtion list.
    //
    template<class T2>
    inline constexpr
    basic_matrix(initializer_list<T2> rhs)
        requires
            detail::writable_vector_engine<engine_type>                 and
            detail::constructible_from_1d_list<engine_type, T2>
    :   m_engine(rhs)
    {}

    template<class T2>
    inline constexpr
    basic_matrix(initializer_list<T2> rhs)
        requires
            detail::writable_vector_engine<engine_type>                 and
            (not detail::constructible_from_1d_list<engine_type, T2>)   and
            detail::convertible_from<element_type, T2>
    :   m_engine()
    {
        engine_support::assign_from_vector_initlist(m_engine, rhs);
    }

    //-------------------------------------------------
    //- Assignment from a different matrix engine type.
    //
    template<class ET2, class OT2>
    inline constexpr basic_matrix&
    operator =(basic_matrix<ET2, OT2> const& rhs)
        requires
            detail::writable_matrix_engine<engine_type>                 and
            detail::assignable_from_engine<engine_type, ET2>
    {
        m_engine = rhs.m_engine;
        return *this;
    }

    template<class ET2, class OT2>
    inline constexpr basic_matrix&
    operator =(basic_matrix<ET2, OT2> const& rhs)
        requires
            detail::writable_matrix_engine<engine_type>                 and
            (not detail::assignable_from_engine<engine_type, ET2>)      and
            detail::convertible_from<element_type, typename ET2::element_type>
    {
        engine_support::assign_from_matrix_engine(m_engine, rhs.m_engine);
        return *this;
    }

    //- Assignment from a 2D initializtion list.
    //
    template<class T2>
    inline constexpr basic_matrix&
    operator =(initializer_list<initializer_list<T2>> rhs)
        requires
            detail::writable_matrix_engine<engine_type>                 and
            detail::assignable_from_2d_list<engine_type, T2>
    {
        m_engine = rhs;
        return *this;
    }

    template<class T2>
    inline constexpr basic_matrix&
    operator =(initializer_list<initializer_list<T2>> rhs)
        requires
            detail::writable_matrix_engine<engine_type>                 and
            (not detail::assignable_from_2d_list<engine_type, T2>)      and
            detail::convertible_from<element_type, T2>
    {
        engine_support::assign_from_matrix_initlist(m_engine, rhs);
        return *this;
    }

    //- Assignment from a 1D initializtion list.
    //
    template<class T2>
    inline constexpr basic_matrix&
    operator =(initializer_list<T2> rhs)
        requires
            detail::writable_vector_engine<engine_type>                 and
            detail::assignable_from_1d_list<engine_type, T2>
    {
        m_engine = rhs;
        return *this;
    }

    template<class T2>
    inline constexpr basic_matrix&
    operator =(initializer_list<T2> rhs)
        requires
            detail::writable_vector_engine<engine_type>                 and
            (not detail::assignable_from_1d_list<engine_type, T2>)      and
            detail::convertible_from<element_type, T2>
    {
        engine_support::assign_from_vector_initlist(m_engine, rhs);
        return *this;
    }

    //------------------------------
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
        return m_engine.size();
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

    inline constexpr index_type
    capacity() const noexcept
    {
        return m_engine.capacity();
    }

    //-----------------
    //- Element access.
    //
    inline constexpr reference
    operator ()(index_type i, index_type j)
    {
        return m_engine(i, j);
    }

    inline constexpr const_reference
    operator ()(index_type i, index_type j) const
    {
        return m_engine(i, j);
    }

    inline constexpr reference
    operator ()(index_type i)
        requires
            detail::readable_vector_engine<engine_type>
    {
        return m_engine(i);
    }

    inline constexpr const_reference
    operator ()(index_type i) const
        requires
            detail::readable_vector_engine<engine_type>
    {
        return m_engine(i);
    }

    //--------------
    //- Data access.
    //
    inline constexpr engine_type&
    engine() noexcept
    {
        return m_engine;
    }

    inline constexpr engine_type const&
    engine() const noexcept
    {
        return m_engine;
    }

    inline constexpr span_type
    span() noexcept
        requires
            detail::spannable_matrix_engine<engine_type>
    {
        return m_engine.span();
    }

    inline constexpr const_span_type
    span() const noexcept
        requires
            detail::spannable_matrix_engine<engine_type>
    {
        return m_engine.span();
    }

    //-----------------------------------
    //- Setting column size and capacity.
    //
    inline constexpr void
    resize_columns(index_type cols)
        requires
            detail::column_reshapable_matrix_engine<engine_type>
    {
        m_engine.reshape_columns(cols, m_engine.column_capacity());
    }

    inline constexpr void
    reserve_columns(index_type colcap)
        requires
            detail::column_reshapable_matrix_engine<engine_type>
    {
        m_engine.reshape_columns(m_engine.columns(), colcap);
    }

    inline constexpr void
    reshape_columns(index_type cols, index_type colcap)
        requires
            detail::column_reshapable_matrix_engine<engine_type>
    {
        m_engine.reshape_columns(cols, colcap);
    }

    //--------------------------------
    //- Setting row size and capacity.
    //
    inline constexpr void
    resize_rows(index_type rows)
        requires
            detail::row_reshapable_matrix_engine<engine_type>
    {
        m_engine.reshape_rows(rows, m_engine.row_capacity());
    }

    inline constexpr void
    reserve_rows(index_type rowcap)
        requires
            detail::row_reshapable_matrix_engine<engine_type>
    {
        m_engine.reshape_rows(m_engine.rows(), rowcap);
    }

    inline constexpr void
    reshape_rows(index_type rows, index_type rowcap)
        requires
            detail::row_reshapable_matrix_engine<engine_type>
    {
        m_engine.reshape_rows(rows, rowcap);
    }

    //------------------------------------
    //- Setting overall size and capacity.
    //
    inline constexpr void
    resize(index_type rows, index_type cols)
        requires
            detail::reshapable_matrix_engine<engine_type>
    {
        m_engine.reshape(rows, cols, m_engine.row_capacity(), m_engine.column_capacity());
    }

    inline constexpr void
    reserve(index_type rowcap, index_type colcap)
        requires
            detail::reshapable_matrix_engine<engine_type>
    {
        m_engine.reshape(m_engine.rows(), m_engine.columns(), rowcap, colcap);
    }

    inline constexpr void
    reshape(index_type rows, index_type cols, index_type rowcap, index_type colcap)
        requires
            detail::reshapable_matrix_engine<engine_type>
    {
        m_engine.reshape(rows, cols, rowcap, colcap);
    }

    //------------------
    //- Other modifiers.
    //
    inline constexpr void
    swap(basic_matrix& rhs) noexcept
    {
        m_engine.swap(rhs.m_engine);
    }

    constexpr void
    swap_columns(index_type c1, index_type c2) noexcept
        requires
            detail::writable_matrix_engine<engine_type>
    {
        if (c1 != c2)
        {
            for (index_type i = 0;  i < m_engine.rows();  ++i)
            {
                detail::la_swap(m_engine(i, c1), m_engine(i, c2));
            }
        }
    }

    constexpr void
    swap_rows(index_type r1, index_type r2) noexcept
        requires
            detail::writable_matrix_engine<engine_type>
    {
        if (r1 != r2)
        {
            for (index_type j = 0;  j < m_engine.columns();  ++j)
            {
                detail::la_swap(m_engine(r1, j), m_engine(r2, j));
            }
        }
    }

  private:
    template<class ET2, class OT2>
        requires
            default_initializable<ET2>  and
            copyable<ET2>               and
            detail::readable_matrix_engine<ET2>
    friend class basic_matrix;

    //template<class ET2, class OT2> friend class basic_vector;

    engine_type     m_engine;

    template<class T2>
    constexpr void
    assign_from(initializer_list<initializer_list<T2>> list)
    {
        index_type  di = 0;
        auto        rp = list.begin();

        for (;  di < m_engine.rows();  ++di, ++rp)
        {
            index_type  dj = 0;
            auto        cp = rp->begin();

            for (;  dj < m_engine.columns();  ++dj, ++cp)
            {
                m_engine(di, dj) = static_cast<element_type>(*cp);
            }
        }
    }

    template<class T2>
    constexpr void
    assign_from(initializer_list<T2> list)
    {
        index_type  di = 0;
        auto        ep = list.begin();

        for (;  di < m_engine.size();  ++di)
        {
            m_engine(di) = static_cast<element_type>(*ep);
        }
    }
};


template<class T, ptrdiff_t R, ptrdiff_t C, class OT = matrix_operation_traits>
using fs_dyn_matrix = basic_matrix<matrix_storage_engine<T, extents<R, C>, allocator<T>, row_major>, OT>;

template<class T, class OT = matrix_operation_traits>
using dr_matrix = basic_matrix<matrix_storage_engine<T, extents<dynamic_extent, dynamic_extent>, allocator<T>, row_major>, OT>;

template<class T, class OT = matrix_operation_traits>
using dyn_row_vector = basic_matrix<matrix_storage_engine<T, extents<1, dynamic_extent>, allocator<T>, row_major>, OT>;

template<class T, class OT = matrix_operation_traits>
using dyn_col_vector = basic_matrix<matrix_storage_engine<T, extents<dynamic_extent, 1>, allocator<T>, column_major>, OT>;



}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_BASIC_MATRIX_HPP_DEFINED
