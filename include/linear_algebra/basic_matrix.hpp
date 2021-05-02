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

template<class ET, class COT>
requires
    detail::copyable<ET>
    and
    detail::default_initializable<ET>
    and
    detail::readable_matrix_engine<ET>
class basic_matrix
{
    static constexpr bool   is_writable = detail::writable_matrix_engine<ET>;

    using engine_support              = detail::matrix_engine_support;
    using possibly_writable_column    = conditional_t<is_writable, matrix_view::column, matrix_view::const_column>;
    using possibly_writable_row       = conditional_t<is_writable, matrix_view::row, matrix_view::const_row>;
    using possibly_writable_submatrix = conditional_t<is_writable, matrix_view::submatrix, matrix_view::const_submatrix>;
    using possibly_writable_transpose = conditional_t<is_writable, matrix_view::transpose, matrix_view::const_transpose>;

  public:
    //- Fundamental type aliases.
    //
    using engine_type          = ET;
    using owning_engine_type   = detail::get_owning_engine_type_t<ET>;
    using element_type         = typename engine_type::element_type;
    using reference            = typename engine_type::reference;
    using const_reference      = typename engine_type::const_reference;
    using index_type           = typename engine_type::index_type;

    //- Type aliases pertaining to mdspan.
    //
    using span_type            = detail::get_mdspan_type_t<ET>;
    using const_span_type      = detail::get_const_mdspan_type_t<ET>;

    //- Type aliases pertaining to views.
    //
    using const_negation_type  = basic_matrix<matrix_view_engine<engine_type, matrix_view::const_negation>, COT>;
    using const_conjugate_type = basic_matrix<matrix_view_engine<engine_type, matrix_view::const_conjugate>, COT>;
    using const_hermitian_type = basic_matrix<matrix_view_engine<engine_type, matrix_view::const_hermitian>, COT>;
    using transpose_type       = basic_matrix<matrix_view_engine<engine_type, possibly_writable_transpose>, COT>;
    using const_transpose_type = basic_matrix<matrix_view_engine<engine_type, matrix_view::const_transpose>, COT>;
    using column_type          = basic_matrix<matrix_view_engine<engine_type, possibly_writable_column>, COT>;
    using const_column_type    = basic_matrix<matrix_view_engine<engine_type, matrix_view::const_column>, COT>;
    using row_type             = basic_matrix<matrix_view_engine<engine_type, possibly_writable_row>, COT>;
    using const_row_type       = basic_matrix<matrix_view_engine<engine_type, matrix_view::const_row>, COT>;
    using submatrix_type       = basic_matrix<matrix_view_engine<engine_type, possibly_writable_submatrix>, COT>;
    using const_submatrix_type = basic_matrix<matrix_view_engine<engine_type, matrix_view::const_submatrix>, COT>;

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
    template<class ET2, class COT2>
    constexpr
    basic_matrix(basic_matrix<ET2, COT2> const& rhs)
    requires
        detail::writable_matrix_engine<engine_type>
        and
        detail::constructible_from<engine_type, ET2>
    :   m_engine(rhs.engine())
    {}

    template<class ET2, class COT2>
    constexpr
    basic_matrix(basic_matrix<ET2, COT2> const& rhs)
    requires
        detail::writable_matrix_engine<engine_type>
        and
        detail::not_constructible_from<engine_type, ET2>
        and
        detail::convertible_from<element_type, typename ET2::element_type>
    :   m_engine()
    {
        engine_support::assign_from(m_engine, rhs.engine());
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
    //- Construction from a standard random-access container.
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
    template<class ET2, class COT2>
    constexpr basic_matrix&
    operator =(basic_matrix<ET2, COT2> const& rhs)
    requires
        detail::writable_matrix_engine<engine_type>
        and
        detail::assignable_from<engine_type, ET2>
    {
        m_engine = rhs.engine();
        return *this;
    }

    template<class ET2, class COT2>
    constexpr basic_matrix&
    operator =(basic_matrix<ET2, COT2> const& rhs)
    requires
        detail::writable_matrix_engine<engine_type>
        and
        detail::not_assignable_from<engine_type, ET2>
        and
        detail::convertible_from<element_type, typename ET2::element_type>
    {
        engine_support::assign_from(m_engine, rhs.engine());
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
    //- Assignment from a standard random-access container.
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
        detail::writable_and_1d_indexable_matrix_engine<engine_type>
    {
        return m_engine(i);
    }

    constexpr const_reference
    operator ()(index_type i) const
    requires
        detail::readable_and_1d_indexable_matrix_engine<engine_type>
    {
        return m_engine(i);
    }

    constexpr const_negation_type
    operator -() const noexcept
    {
        return const_negation_type(detail::special_ctor_tag(), m_engine);
    }

    constexpr const_conjugate_type
    conj() const noexcept
    {
        return const_conjugate_type(detail::special_ctor_tag(), m_engine);
    }

    constexpr const_hermitian_type
    h() const noexcept
    {
        return const_hermitian_type(detail::special_ctor_tag(), m_engine);
    }

    constexpr transpose_type
    t() noexcept
    {
        return transpose_type(detail::special_ctor_tag(), m_engine);
    }

    constexpr const_transpose_type
    t() const noexcept
    {
        return const_transpose_type(detail::special_ctor_tag(), m_engine);
    }

    constexpr column_type
    column(index_type j) noexcept
    {
        return column_type(detail::special_ctor_tag(), m_engine, j);
    }

    constexpr const_column_type
    column(index_type j) const noexcept
    {
        return const_column_type(detail::special_ctor_tag(), m_engine, j);
    }

    constexpr row_type
    row(index_type i) noexcept
    {
        return row_type(detail::special_ctor_tag(), m_engine, i);
    }

    constexpr const_row_type
    row(index_type i) const noexcept
    {
        return const_row_type(detail::special_ctor_tag(), m_engine, i);
    }

    constexpr submatrix_type
    submatrix(index_type ri, index_type rn, index_type ci, index_type cn) noexcept
    {
        return submatrix_type(detail::special_ctor_tag(), m_engine, ri, rn, ci, cn);
    }

    constexpr const_submatrix_type
    submatrix(index_type ri, index_type rn, index_type ci, index_type cn) const noexcept
    {
        return const_submatrix_type(detail::special_ctor_tag(), m_engine, ri, rn, ci, cn);
    }

    //----------------------------------------------------------
    //- Custom operation injection.
    //
    template<class COT2>
    constexpr basic_matrix<matrix_view_engine<engine_type, matrix_view::identity>, COT2>
    adopt() noexcept
    {
        return {detail::special_ctor_tag(), m_engine};
    }

    template<class COT2>
    constexpr basic_matrix<matrix_view_engine<engine_type, matrix_view::const_identity>, COT2>
    adopt() const noexcept
    {
        return {detail::special_ctor_tag(), m_engine};
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
    template<class ET2, class COT2>
    requires
        detail::copyable<ET2>
        and
        detail::default_initializable<ET2>
        and
        detail::readable_matrix_engine<ET2>
    friend class basic_matrix;

    engine_type     m_engine;

    template<class ET2, class... ARGS>
    constexpr
    basic_matrix(detail::special_ctor_tag, ET2&& eng, ARGS&&... args)
    :   m_engine(std::forward<ET2>(eng), std::forward<ARGS>(args)...)
    {}
};


template<class T, ptrdiff_t R, ptrdiff_t C, class COT = void>
using fixed_size_matrix =
        basic_matrix<matrix_storage_engine<T, extents<R, C>, void, matrix_layout::row_major>, COT>;

template<class T, ptrdiff_t R, class COT = void>
using fixed_size_column_vector =
        basic_matrix<matrix_storage_engine<T, extents<R, 1>, void, matrix_layout::column_major>, COT>;

template<class T, ptrdiff_t C, class COT = void>
using fixed_size_row_vector =
        basic_matrix<matrix_storage_engine<T, extents<1, C>, void, matrix_layout::row_major>, COT>;


template<class T, ptrdiff_t R, ptrdiff_t C, class A = std::allocator<T>, class COT = void>
using general_matrix =
        basic_matrix<matrix_storage_engine<T, extents<R, C>, A, matrix_layout::row_major>, COT>;

template<class T, ptrdiff_t R, class A = std::allocator<T>, class COT = void>
using general_column_vector =
        basic_matrix<matrix_storage_engine<T, extents<R, 1>, A, matrix_layout::column_major>, COT>;

template<class T, ptrdiff_t C, class A = std::allocator<T>, class COT = void>
using general_row_vector =
        basic_matrix<matrix_storage_engine<T, extents<1, C>, A, matrix_layout::row_major>, COT>;


template<class T, class COT = void>
using dynamic_matrix =
        basic_matrix<matrix_storage_engine<T, extents<dynamic_extent, dynamic_extent>, std::allocator<T>, matrix_layout::row_major>, COT>;

template<class T, class COT = void>
using dynamic_column_vector =
        basic_matrix<matrix_storage_engine<T, extents<dynamic_extent, 1>, std::allocator<T>, matrix_layout::column_major>, COT>;

template<class T, class COT = void>
using dynamic_row_vector =
        basic_matrix<matrix_storage_engine<T, extents<1, dynamic_extent>, std::allocator<T>, matrix_layout::row_major>, COT>;


template<class ET1, class COT1, class ET2, class COT2> inline constexpr
bool
operator ==(basic_matrix<ET1, COT1> const& m1, basic_matrix<ET2, COT2> const& m2)
{
    return detail::matrix_engine_support::compare(m1, m2);
}

template<class ET1, class COT1, class ET2, class COT2> inline constexpr
bool
operator !=(basic_matrix<ET1, COT1> const& m1, basic_matrix<ET2, COT2> const& m2)
{
    return !detail::matrix_engine_support::compare(m1, m2);
}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_BASIC_MATRIX_HPP_DEFINED
