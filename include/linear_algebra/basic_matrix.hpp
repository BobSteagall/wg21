//==================================================================================================
//  File:       basic_matrix.hpp
//
//  Summary:    This header defines the basic_matrix class template, one of the two primary
//              math objects provided by the library.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_BASIC_MATRIX_HPP_DEFINED
#define LINEAR_ALGEBRA_BASIC_MATRIX_HPP_DEFINED

namespace STD_LA {

template<class ET, class OT>
requires
    detail::copyable<ET>  and
    detail::default_initializable<ET>  and
    detail::readable_matrix_engine<ET>
class basic_matrix
{
    static constexpr bool   engine_has_mdspan = detail::spannable_matrix_engine<ET>;

    using engine_support = detail::matrix_engine_support;

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

    //----------------------------------------------------------
    //- Other constructors.
    //
    constexpr
    basic_matrix(index_type rows, index_type cols)
    requires
        detail::reshapable_matrix_engine<engine_type>
    :   m_engine(rows, cols, rows, cols)
    {}

    constexpr
    basic_matrix(index_type rows, index_type cols, index_type rowcap, index_type colcap)
    requires
        detail::reshapable_matrix_engine<engine_type>
    :   m_engine(rows, cols, rowcap, colcap)
    {}

    //----------------------------------------------------------
    //- Construction from a matrix of different engine type.
    //
    template<class ET2, class OT2>
    constexpr explicit
    basic_matrix(basic_matrix<ET2, OT2> const& rhs)
    requires
        detail::writable_matrix_engine<engine_type>
        and
        detail::constructible_from<engine_type, ET2>
    :   m_engine(rhs)
    {}

    template<class ET2, class OT2>
    constexpr explicit
    basic_matrix(basic_matrix<ET2, OT2> const& rhs)
    requires
        detail::writable_matrix_engine<engine_type>
        and
        detail::not_constructible_from<engine_type, ET2>
        and
        detail::convertible_from<element_type, typename ET2::element_type>
    :   m_engine()
    {
        engine_support::assign_from(m_engine, rhs.m_engine);
    }

    //----------------------------------------------------------
    //- Construction from a 2D mdspan.
    //
    template<class U, ptrdiff_t X0, ptrdiff_t X1, class L, class A>
    constexpr explicit
    basic_matrix(basic_mdspan<U, extents<X0, X1>, L, A> const& rhs)
    requires
        detail::writable_matrix_engine<engine_type>
        and
        detail::constructible_from<engine_type, decltype(rhs)>
    :   m_engine(rhs)
    {}

    template<class U, ptrdiff_t X0, ptrdiff_t X1, class L, class A>
    constexpr explicit
    basic_matrix(basic_mdspan<U, extents<X0, X1>, L, A> const& rhs)
    requires
        detail::writable_matrix_engine<engine_type>
        and
        detail::not_constructible_from<engine_type, decltype(rhs)>
        and
        detail::convertible_from<element_type, U>
    :   m_engine()
    {
        engine_support::assign_from(m_engine, rhs);
    }

    //----------------------------------------------------------
    //- Construction from a 2D initialization list.
    //
    template<class U>
    constexpr
    basic_matrix(initializer_list<initializer_list<U>> rhs)
    requires
        detail::writable_matrix_engine<engine_type>
        and
        detail::constructible_from<engine_type, decltype(rhs)>
    :   m_engine(rhs)
    {}

    template<class U>
    constexpr
    basic_matrix(initializer_list<initializer_list<U>> rhs)
    requires
        detail::writable_matrix_engine<engine_type>
        and
        detail::not_constructible_from<engine_type, decltype(rhs)>
        and
        detail::convertible_from<element_type, U>
    :   m_engine()
    {
        engine_support::assign_from(m_engine, rhs);
    }

    //----------------------------------------------------------
    //- Construction from a 1D standard sequence container.
    //
    template<class CT>
    constexpr explicit
    basic_matrix(CT const& rhs)
    requires
        detail::writable_and_1d_indexable_matrix_engine<ET>
        and
        detail::random_access_standard_container<CT>
        and
        detail::constructible_from<engine_type, CT>
        and
        detail::convertible_from<typename ET::element_type, typename CT::value_type>
    :   m_engine(rhs)
    {}

    template<class CT>
    constexpr explicit
    basic_matrix(CT const& rhs)
    requires
        detail::writable_and_1d_indexable_matrix_engine<ET>
        and
        detail::random_access_standard_container<CT>
        and
        detail::not_constructible_from<engine_type, CT>
        and
        detail::convertible_from<typename ET::element_type, typename CT::value_type>
    :   m_engine()
    {
        engine_support::assign_from(m_engine, rhs);
    }

    //----------------------------------------------------------
    //- Construction from a 1D mdspan.
    //
    template<class U, ptrdiff_t X0, class L, class A>
    constexpr explicit
    basic_matrix(basic_mdspan<U, extents<X0>, L, A> const& rhs)
    requires
        detail::writable_and_1d_indexable_matrix_engine<engine_type>
        and
        detail::constructible_from<engine_type, decltype(rhs)>
    :   m_engine(rhs)
    {}

    template<class U, ptrdiff_t X0, class L, class A>
    constexpr explicit
    basic_matrix(basic_mdspan<U, extents<X0>, L, A> const& rhs)
    requires
        detail::writable_and_1d_indexable_matrix_engine<engine_type>
        and
        detail::not_constructible_from<engine_type, decltype(rhs)>
        and
        detail::convertible_from<element_type, U>
    :   m_engine()
    {
        engine_support::assign_from(m_engine, rhs);
    }

    //----------------------------------------------------------
    //- Construction from a 1D initialization list.
    //
    template<class U>
    constexpr
    basic_matrix(initializer_list<U> rhs)
    requires
        detail::writable_and_1d_indexable_matrix_engine<engine_type>
        and
        detail::constructible_from<engine_type, decltype(rhs)>
    :   m_engine(rhs)
    {}

    template<class U>
    constexpr
    basic_matrix(initializer_list<U> rhs)
    requires
        detail::writable_and_1d_indexable_matrix_engine<engine_type>
        and
        detail::not_constructible_from<engine_type, decltype(rhs)>
        and
        detail::convertible_from<element_type, U>
    :   m_engine()
    {
        engine_support::assign_from(m_engine, rhs);
    }

    //----------------------------------------------------------
    //- Assignment from a different matrix engine type.
    //
    template<class ET2, class OT2>
    constexpr basic_matrix&
    operator =(basic_matrix<ET2, OT2> const& rhs)
    requires
        detail::writable_matrix_engine<engine_type>
        and
        detail::assignable_from<engine_type, ET2>
    {
        m_engine = rhs.m_engine;
        return *this;
    }

    template<class ET2, class OT2>
    constexpr basic_matrix&
    operator =(basic_matrix<ET2, OT2> const& rhs)
    requires
        detail::writable_matrix_engine<engine_type>
        and
        detail::not_assignable_from<engine_type, ET2>
        and
        detail::convertible_from<element_type, typename ET2::element_type>
    {
        engine_support::assign_from(m_engine, rhs.m_engine);
        return *this;
    }

    //----------------------------------------------------------
    //- Assignment from a 2D mdspan.
    //
    template<class U, ptrdiff_t X0, ptrdiff_t X1, class L, class A>
    constexpr basic_matrix&
    operator =(basic_mdspan<U, extents<X0, X1>, L, A> const& rhs)
    requires
        detail::writable_matrix_engine<engine_type>
        and
        detail::assignable_from<engine_type, decltype(rhs)>
    {
        m_engine = rhs;
        return *this;
    }

    template<class U, ptrdiff_t X0, ptrdiff_t X1, class L, class A>
    constexpr basic_matrix&
    operator =(basic_mdspan<U, extents<X0, X1>, L, A> const& rhs)
    requires
        detail::writable_matrix_engine<engine_type>
        and
        detail::not_assignable_from<engine_type, decltype(rhs)>
        and
        detail::convertible_from<element_type, U>
    {
        engine_support::assign_from(m_engine, rhs);
        return *this;
    }

    //----------------------------------------------------------
    //- Assignment from a 2D initializtion list.
    //
    template<class U>
    constexpr basic_matrix&
    operator =(initializer_list<initializer_list<U>> rhs)
    requires
        detail::writable_matrix_engine<engine_type>
        and
        detail::assignable_from<engine_type, decltype(rhs)>
    {
        m_engine = rhs;
        return *this;
    }

    template<class U>
    constexpr basic_matrix&
    operator =(initializer_list<initializer_list<U>> rhs)
    requires
        detail::writable_matrix_engine<engine_type>
        and
        detail::not_assignable_from<engine_type, decltype(rhs)>
        and
        detail::convertible_from<element_type, U>
    {
        engine_support::assign_from(m_engine, rhs);
        return *this;
    }

    //----------------------------------------------------------
    //- Assignment from a 1D standard sequence container.
    //
    template<class CT>
    constexpr basic_matrix&
    operator =(CT const& rhs)
    requires
        detail::writable_and_1d_indexable_matrix_engine<engine_type>
        and
        detail::random_access_standard_container<CT>
        and
        detail::assignable_from<engine_type, decltype(rhs)>
        and
        detail::convertible_from<element_type, typename CT::value_type>
    {
        m_engine = rhs;
        return *this;
    }

    template<class CT>
    constexpr basic_matrix&
    operator =(CT const& rhs)
    requires
        detail::writable_and_1d_indexable_matrix_engine<ET>
        and
        detail::random_access_standard_container<CT>
        and
        detail::not_assignable_from<engine_type, decltype(rhs)>
        and
        detail::convertible_from<element_type, typename CT::value_type>
    {
        engine_support::assign_from(m_engine, rhs);
        return *this;
    }

    //----------------------------------------------------------
    //- Assignment from a 1D mdspan.
    //
    template<class U, ptrdiff_t X0, class L, class A>
    constexpr basic_matrix&
    operator =(basic_mdspan<U, extents<X0>, L, A> const& rhs)
    requires
        detail::writable_and_1d_indexable_matrix_engine<engine_type>
        and
        detail::assignable_from<engine_type, decltype(rhs)>
    {
        m_engine = rhs;
        return *this;
    }

    template<class U, ptrdiff_t X0, class L, class A>
    constexpr basic_matrix&
    operator =(basic_mdspan<U, extents<X0>, L, A> const& rhs)
    requires
        detail::writable_and_1d_indexable_matrix_engine<engine_type>
        and
        detail::not_assignable_from<engine_type, decltype(rhs)>
        and
        detail::convertible_from<element_type, U>
    {
        engine_support::assign_from(m_engine, rhs);
        return *this;
    }

    //----------------------------------------------------------
    //- Assignment from a 1D initializtion list.
    //
    template<class U>
    constexpr basic_matrix&
    operator =(initializer_list<U> rhs)
    requires
        detail::writable_and_1d_indexable_matrix_engine<engine_type>
        and
        detail::assignable_from<engine_type, decltype(rhs)>
    {
        m_engine = rhs;
        return *this;
    }

    template<class U>
    constexpr basic_matrix&
    operator =(initializer_list<U> rhs)
    requires
        detail::writable_and_1d_indexable_matrix_engine<engine_type>
        and
        detail::not_assignable_from<engine_type, decltype(rhs)>
        and
        detail::convertible_from<element_type, U>
    {
        engine_support::assign_from(m_engine, rhs);
        return *this;
    }

    //----------------------------------------------------------
    //- Size and capacity reporting.
    //
    constexpr index_type
    elements() const noexcept
    {
        return m_engine.elements();
    }

    constexpr index_type
    columns() const noexcept
    {
        return m_engine.columns();
    }

    constexpr index_type
    rows() const noexcept
    {
        return m_engine.rows();
    }

    constexpr index_type
    size() const noexcept
    {
        return m_engine.size();
    }

    constexpr index_type
    column_capacity() const noexcept
    {
        return m_engine.column_capacity();
    }

    constexpr index_type
    row_capacity() const noexcept
    {
        return m_engine.row_capacity();
    }

    constexpr index_type
    capacity() const noexcept
    {
        return m_engine.capacity();
    }

    //----------------------------------------------------------
    //- Element access.
    //
    constexpr reference
    operator ()(index_type i, index_type j)
    {
        return m_engine(i, j);
    }

    constexpr const_reference
    operator ()(index_type i, index_type j) const
    {
        return m_engine(i, j);
    }

    constexpr reference
    operator ()(index_type i)
    requires
        detail::readable_vector_engine<engine_type>
    {
        return m_engine(i);
    }

    constexpr const_reference
    operator ()(index_type i) const
    requires
        detail::readable_vector_engine<engine_type>
    {
        return m_engine(i);
    }

    //----------------------------------------------------------
    //- Data access.
    //
    constexpr engine_type&
    engine() noexcept
    {
        return m_engine;
    }

    constexpr engine_type const&
    engine() const noexcept
    {
        return m_engine;
    }

    constexpr span_type
    span() noexcept
    requires
        detail::spannable_matrix_engine<engine_type>
    {
        return m_engine.span();
    }

    constexpr const_span_type
    span() const noexcept
    requires
        detail::spannable_matrix_engine<engine_type>
    {
        return m_engine.span();
    }

    //----------------------------------------------------------
    //- Setting column size and capacity.
    //
    constexpr void
    resize_columns(index_type cols)
    requires
        detail::column_reshapable_matrix_engine<engine_type>
    {
        m_engine.reshape_columns(cols, m_engine.column_capacity());
    }

    constexpr void
    reserve_columns(index_type colcap)
    requires
        detail::column_reshapable_matrix_engine<engine_type>
    {
        m_engine.reshape_columns(m_engine.columns(), colcap);
    }

    constexpr void
    reshape_columns(index_type cols, index_type colcap)
    requires
        detail::column_reshapable_matrix_engine<engine_type>
    {
        m_engine.reshape_columns(cols, colcap);
    }

    //----------------------------------------------------------
    //- Setting row size and capacity.
    //
    constexpr void
    resize_rows(index_type rows)
    requires
        detail::row_reshapable_matrix_engine<engine_type>
    {
        m_engine.reshape_rows(rows, m_engine.row_capacity());
    }

    constexpr void
    reserve_rows(index_type rowcap)
    requires
        detail::row_reshapable_matrix_engine<engine_type>
    {
        m_engine.reshape_rows(m_engine.rows(), rowcap);
    }

    constexpr void
    reshape_rows(index_type rows, index_type rowcap)
    requires
        detail::row_reshapable_matrix_engine<engine_type>
    {
        m_engine.reshape_rows(rows, rowcap);
    }

    //----------------------------------------------------------
    //- Setting overall size and capacity.
    //
    constexpr void
    resize(index_type rows, index_type cols)
    requires
        detail::reshapable_matrix_engine<engine_type>
    {
        m_engine.reshape(rows, cols, m_engine.row_capacity(), m_engine.column_capacity());
    }

    constexpr void
    reserve(index_type rowcap, index_type colcap)
    requires
        detail::reshapable_matrix_engine<engine_type>
    {
        m_engine.reshape(m_engine.rows(), m_engine.columns(), rowcap, colcap);
    }

    constexpr void
    reshape(index_type rows, index_type cols, index_type rowcap, index_type colcap)
    requires
        detail::reshapable_matrix_engine<engine_type>
    {
        m_engine.reshape(rows, cols, rowcap, colcap);
    }

    //----------------------------------------------------------
    //- Other modifiers.
    //
    constexpr void
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
                engine_support::swap(m_engine(i, c1), m_engine(i, c2));
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
                engine_support::swap(m_engine(r1, j), m_engine(r2, j));
            }
        }
    }

  private:
    template<class ET2, class OT2>
    requires
        detail::copyable<ET2>  and
        detail::default_initializable<ET2>  and
        detail::readable_matrix_engine<ET2>
    friend class basic_matrix;

    engine_type     m_engine;
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
