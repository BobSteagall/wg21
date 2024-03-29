//==================================================================================================
//  File:       matrix.hpp
//
//  Summary:    This header defines the matrix class template, one of the two primary
//              math objects provided by the library.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_MATRIX_HPP_DEFINED
#define LINEAR_ALGEBRA_MATRIX_HPP_DEFINED

namespace STD_LA {

template<class ET, class COT = void>
requires
    detail::copyable<ET>
    and
    detail::default_initializable<ET>
    and
    detail::readable_matrix_engine<ET>
class matrix
{
    static constexpr bool   is_writable = detail::writable_matrix_engine<ET>;

    using engine_support              = detail::matrix_engine_support;
    using possibly_writable_identity  = conditional_t<is_writable, matrix_view::identity, matrix_view::const_identity>;
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
    using size_type            = typename engine_type::size_type;

    //- Type aliases pertaining to mdspan.
    //
    using mdspan_type          = detail::get_mdspan_type_t<ET>;
    using const_mdspan_type    = detail::get_const_mdspan_type_t<ET>;

    //- Type aliases pertaining to views.
    //
    using const_negation_type  = matrix<matrix_view_engine<engine_type, matrix_view::const_negation>, COT>;
    using const_conjugate_type = matrix<matrix_view_engine<engine_type, matrix_view::const_conjugate>, COT>;
    using const_hermitian_type = matrix<matrix_view_engine<engine_type, matrix_view::const_hermitian>, COT>;
    using transpose_type       = matrix<matrix_view_engine<engine_type, possibly_writable_transpose>, COT>;
    using const_transpose_type = matrix<matrix_view_engine<engine_type, matrix_view::const_transpose>, COT>;
    using column_type          = matrix<matrix_view_engine<engine_type, possibly_writable_column>, COT>;
    using const_column_type    = matrix<matrix_view_engine<engine_type, matrix_view::const_column>, COT>;
    using row_type             = matrix<matrix_view_engine<engine_type, possibly_writable_row>, COT>;
    using const_row_type       = matrix<matrix_view_engine<engine_type, matrix_view::const_row>, COT>;
    using submatrix_type       = matrix<matrix_view_engine<engine_type, possibly_writable_submatrix>, COT>;
    using const_submatrix_type = matrix<matrix_view_engine<engine_type, matrix_view::const_submatrix>, COT>;

  public:
    ~matrix() = default;

    constexpr matrix() = default;
    constexpr matrix(matrix&&) noexcept = default;
    constexpr matrix(matrix const&) = default;

    matrix&   operator =(matrix&&) noexcept = default;
    matrix&   operator =(matrix const&) = default;

    //----------------------------------------------------------
    //- Other constructors.
    //
    constexpr
    matrix(size_type rows, size_type cols)
    requires
        detail::reshapable_matrix_engine<engine_type>
    :   m_engine(rows, cols, rows, cols)
    {}

    constexpr
    matrix(size_type rows, size_type cols, size_type rowcap, size_type colcap)
    requires
        detail::reshapable_matrix_engine<engine_type>
    :   m_engine(rows, cols, rowcap, colcap)
    {}

    constexpr explicit
    matrix(size_type cols)
    requires
        detail::writable_and_1d_indexable_matrix_engine<engine_type>
        and
        detail::column_reshapable_matrix_engine<engine_type>
    :   m_engine(cols, cols)
    {}

    constexpr explicit
    matrix(size_type rows)
    requires
        detail::writable_and_1d_indexable_matrix_engine<engine_type>
        and
        detail::row_reshapable_matrix_engine<engine_type>
    :   m_engine(rows, rows)
    {}

    //----------------------------------------------------------
    //- Construction from a matrix of different engine type.
    //
    template<class ET2, class COT2>
    constexpr
    matrix(matrix<ET2, COT2> const& rhs)
    requires
        detail::writable_matrix_engine<engine_type>
        and
        detail::constructible_from<engine_type, ET2>
    :   m_engine(rhs.engine())
    {}

    template<class ET2, class COT2>
    constexpr
    matrix(matrix<ET2, COT2> const& rhs)
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
    template<class U, class IT, size_t X0, size_t X1, class ML, class MA>
    constexpr explicit
    matrix(mdspan<U, extents<IT, X0, X1>, ML, MA> const& rhs)
    requires
        detail::writable_matrix_engine<engine_type>
        and
        detail::constructible_from<engine_type, decltype(rhs)>
    :   m_engine(rhs)
    {}

    template<class U, class IT, size_t X0, size_t X1, class ML, class MA>
    constexpr explicit
    matrix(mdspan<U, extents<IT, X0, X1>, ML, MA> const& rhs)
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
    matrix(initializer_list<initializer_list<U>> rhs)
    requires
        detail::writable_matrix_engine<engine_type>
        and
        detail::constructible_from<engine_type, decltype(rhs)>
    :   m_engine(rhs)
    {}

    template<class U>
    constexpr
    matrix(initializer_list<initializer_list<U>> rhs)
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
    matrix(CT const& rhs)
    requires
        detail::writable_and_1d_indexable_matrix_engine<ET>
        and
        detail::standard_random_access_container<CT>
        and
        detail::constructible_from<engine_type, CT>
    :   m_engine(rhs)
    {}

    template<class CT>
    constexpr explicit
    matrix(CT const& rhs)
    requires
        detail::writable_and_1d_indexable_matrix_engine<ET>
        and
        detail::standard_random_access_container<CT>
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
    template<class U, class IT, size_t X0, class ML, class MA>
    constexpr explicit
    matrix(mdspan<U, extents<IT, X0>, ML, MA> const& rhs)
    requires
        detail::writable_and_1d_indexable_matrix_engine<engine_type>
        and
        detail::constructible_from<engine_type, decltype(rhs)>
    :   m_engine(rhs)
    {}

    template<class U, class IT, size_t X0, class ML, class MA>
    constexpr explicit
    matrix(mdspan<U, extents<IT, X0>, ML, MA> const& rhs)
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
    matrix(initializer_list<U> rhs)
    requires
        detail::writable_and_1d_indexable_matrix_engine<engine_type>
        and
        detail::constructible_from<engine_type, decltype(rhs)>
    :   m_engine(rhs)
    {}

    template<class U>
    constexpr
    matrix(initializer_list<U> rhs)
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
    constexpr matrix&
    operator =(matrix<ET2, COT2> const& rhs)
    requires
        detail::writable_matrix_engine<engine_type>
        and
        detail::assignable_from<engine_type, ET2>
    {
        m_engine = rhs.engine();
        return *this;
    }

    template<class ET2, class COT2>
    constexpr matrix&
    operator =(matrix<ET2, COT2> const& rhs)
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
    template<class U, class IT, size_t X0, size_t X1, class ML, class MA>
    constexpr matrix&
    operator =(mdspan<U, extents<IT, X0, X1>, ML, MA> const& rhs)
    requires
        detail::writable_matrix_engine<engine_type>
        and
        detail::assignable_from<engine_type, decltype(rhs)>
    {
        m_engine = rhs;
        return *this;
    }

    template<class U, class IT, size_t X0, size_t X1, class ML, class MA>
    constexpr matrix&
    operator =(mdspan<U, extents<IT, X0, X1>, ML, MA> const& rhs)
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
    //- Assignment from a 2D initialization list.
    //
    template<class U>
    constexpr matrix&
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
    constexpr matrix&
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
    constexpr matrix&
    operator =(CT const& rhs)
    requires
        detail::writable_and_1d_indexable_matrix_engine<engine_type>
        and
        detail::standard_random_access_container<CT>
        and
        detail::assignable_from<engine_type, decltype(rhs)>
    {
        m_engine = rhs;
        return *this;
    }

    template<class CT>
    constexpr matrix&
    operator =(CT const& rhs)
    requires
        detail::writable_and_1d_indexable_matrix_engine<ET>
        and
        detail::standard_random_access_container<CT>
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
    template<class U, class IT, size_t X0, class ML, class MA>
    constexpr matrix&
    operator =(mdspan<U, extents<IT, X0>, ML, MA> const& rhs)
    requires
        detail::writable_and_1d_indexable_matrix_engine<engine_type>
        and
        detail::assignable_from<engine_type, decltype(rhs)>
    {
        m_engine = rhs;
        return *this;
    }

    template<class U, class IT, size_t X0, class ML, class MA>
    constexpr matrix&
    operator =(mdspan<U, extents<IT, X0>, ML, MA> const& rhs)
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
    //- Assignment from a 1D initialization list.
    //
    template<class U>
    constexpr matrix&
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
    constexpr matrix&
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
    constexpr size_type
    elements() const noexcept
    {
        return m_engine.elements();
    }

    constexpr size_type
    columns() const noexcept
    {
        return m_engine.columns();
    }

    constexpr size_type
    rows() const noexcept
    {
        return m_engine.rows();
    }

    constexpr size_type
    size() const noexcept
    {
        return m_engine.size();
    }

    constexpr size_type
    column_capacity() const noexcept
    {
        return m_engine.column_capacity();
    }

    constexpr size_type
    row_capacity() const noexcept
    {
        return m_engine.row_capacity();
    }

    constexpr size_type
    capacity() const noexcept
    {
        return m_engine.capacity();
    }

    //----------------------------------------------------------
    //- Element access.
    //
    constexpr reference
    operator ()(size_type i, size_type j)
    {
        return m_engine(i, j);
    }

    constexpr const_reference
    operator ()(size_type i, size_type j) const
    {
        return m_engine(i, j);
    }

    constexpr reference
    operator ()(size_type i)
    requires
        detail::writable_and_1d_indexable_matrix_engine<engine_type>
    {
        return m_engine(i);
    }

    constexpr const_reference
    operator ()(size_type i) const
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
    column(size_type j) noexcept
    {
        return column_type(detail::special_ctor_tag(), m_engine, j);
    }

    constexpr const_column_type
    column(size_type j) const noexcept
    {
        return const_column_type(detail::special_ctor_tag(), m_engine, j);
    }

    constexpr row_type
    row(size_type i) noexcept
    {
        return row_type(detail::special_ctor_tag(), m_engine, i);
    }

    constexpr const_row_type
    row(size_type i) const noexcept
    {
        return const_row_type(detail::special_ctor_tag(), m_engine, i);
    }

    constexpr submatrix_type
    submatrix(size_type ri, size_type rn, size_type ci, size_type cn) noexcept
    {
        return submatrix_type(detail::special_ctor_tag(), m_engine, ri, rn, ci, cn);
    }

    constexpr const_submatrix_type
    submatrix(size_type ri, size_type rn, size_type ci, size_type cn) const noexcept
    {
        return const_submatrix_type(detail::special_ctor_tag(), m_engine, ri, rn, ci, cn);
    }

    //----------------------------------------------------------
    //- Custom operation injection.
    //
    template<class COT2>
    constexpr matrix<matrix_view_engine<engine_type, possibly_writable_identity>, COT2>
    adopt() noexcept
    {
        return {detail::special_ctor_tag(), m_engine};
    }

    template<class COT2>
    constexpr matrix<matrix_view_engine<engine_type, matrix_view::const_identity>, COT2>
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

    constexpr mdspan_type
    span() noexcept
    requires
        detail::spannable_matrix_engine<engine_type>
    {
        return m_engine.span();
    }

    constexpr const_mdspan_type
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
    resize_columns(size_type cols)
    requires
        detail::column_reshapable_matrix_engine<engine_type>
    {
        m_engine.reshape_columns(cols, m_engine.column_capacity());
    }

    constexpr void
    reserve_columns(size_type colcap)
    requires
        detail::column_reshapable_matrix_engine<engine_type>
    {
        m_engine.reshape_columns(m_engine.columns(), colcap);
    }

    constexpr void
    reshape_columns(size_type cols, size_type colcap)
    requires
        detail::column_reshapable_matrix_engine<engine_type>
    {
        m_engine.reshape_columns(cols, colcap);
    }

    //----------------------------------------------------------
    //- Setting row size and capacity.
    //
    constexpr void
    resize_rows(size_type rows)
    requires
        detail::row_reshapable_matrix_engine<engine_type>
    {
        m_engine.reshape_rows(rows, m_engine.row_capacity());
    }

    constexpr void
    reserve_rows(size_type rowcap)
    requires
        detail::row_reshapable_matrix_engine<engine_type>
    {
        m_engine.reshape_rows(m_engine.rows(), rowcap);
    }

    constexpr void
    reshape_rows(size_type rows, size_type rowcap)
    requires
        detail::row_reshapable_matrix_engine<engine_type>
    {
        m_engine.reshape_rows(rows, rowcap);
    }

    //----------------------------------------------------------
    //- Setting overall size and capacity.
    //
    constexpr void
    resize(size_type rows, size_type cols)
    requires
        detail::reshapable_matrix_engine<engine_type>
    {
        m_engine.reshape(rows, cols, m_engine.row_capacity(), m_engine.column_capacity());
    }

    constexpr void
    reserve(size_type rowcap, size_type colcap)
    requires
        detail::reshapable_matrix_engine<engine_type>
    {
        m_engine.reshape(m_engine.rows(), m_engine.columns(), rowcap, colcap);
    }

    constexpr void
    reshape(size_type rows, size_type cols, size_type rowcap, size_type colcap)
    requires
        detail::reshapable_matrix_engine<engine_type>
    {
        m_engine.reshape(rows, cols, rowcap, colcap);
    }

    //----------------------------------------------------------
    //- Other modifiers.
    //
    constexpr void
    swap(matrix& rhs) noexcept
    {
        m_engine.swap(rhs.m_engine);
    }

    constexpr void
    swap_columns(size_type c1, size_type c2) noexcept
    requires
        detail::writable_matrix_engine<engine_type>
    {
        if (c1 != c2)
        {
            for (size_type i = 0;  i < m_engine.rows();  ++i)
            {
                engine_support::swap(m_engine(i, c1), m_engine(i, c2));
            }
        }
    }

    constexpr void
    swap_rows(size_type r1, size_type r2) noexcept
    requires
        detail::writable_matrix_engine<engine_type>
    {
        if (r1 != r2)
        {
            for (size_type j = 0;  j < m_engine.columns();  ++j)
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
    friend class matrix;

    engine_type     m_engine;

    template<class ET2, class... ARGS>

    constexpr
    matrix(detail::special_ctor_tag, ET2&& eng, ARGS&&... args)
    :   m_engine(std::forward<ET2>(eng), std::forward<ARGS>(args)...)
    {}
};


//- Comparison operators for matrix operands.  Using these may or may not make sense,
//  based on the element type.
//
template<class ET1, class COT1, class ET2, class COT2> inline constexpr
bool
operator ==(matrix<ET1, COT1> const& m1, matrix<ET2, COT2> const& m2)
{
    return detail::matrix_engine_support::compare(m1, m2);
}

template<class ET1, class COT1, class ET2, class COT2> inline constexpr
bool
operator !=(matrix<ET1, COT1> const& m1, matrix<ET2, COT2> const& m2)
{
    return !detail::matrix_engine_support::compare(m1, m2);
}


//- Convenience aliases for declaring matrix objects.
//
template<class T, size_t R, size_t C, class COT = void>
requires detail::valid_fixed_engine_size<R, C>
using fixed_size_matrix =
        matrix<matrix_storage_engine<T, R, C, void, matrix_layout::row_major>, COT>;

template<class T, size_t R, class COT = void>
requires detail::valid_fixed_engine_size<R, 1>
using fixed_size_column_vector =
        matrix<matrix_storage_engine<T, R, 1, void, matrix_layout::column_major>, COT>;

template<class T, size_t C, class COT = void>
requires detail::valid_fixed_engine_size<1, C>
using fixed_size_row_vector =
        matrix<matrix_storage_engine<T, 1, C, void, matrix_layout::row_major>, COT>;


template<class T, size_t R, size_t C, class COT = void>
requires detail::valid_fixed_engine_size<R, 1>
using general_matrix =
        matrix<matrix_storage_engine<T, R, C, std::allocator<T>, matrix_layout::row_major>, COT>;

template<class T, size_t R, class COT = void>
requires detail::valid_fixed_engine_size<R, 1>
using general_column_vector =
        matrix<matrix_storage_engine<T, R, 1, std::allocator<T>, matrix_layout::column_major>, COT>;

template<class T, size_t C, class COT = void>
requires detail::valid_fixed_engine_size<1, C>
using general_row_vector =
        matrix<matrix_storage_engine<T, 1, C, std::allocator<T>, matrix_layout::row_major>, COT>;


template<class T, class COT = void>
using dynamic_matrix =
        matrix<matrix_storage_engine<T, std::dynamic_extent, std::dynamic_extent, std::allocator<T>, matrix_layout::row_major>, COT>;

template<class T, class COT = void>
using dynamic_column_vector =
        matrix<matrix_storage_engine<T, std::dynamic_extent, 1, std::allocator<T>, matrix_layout::column_major>, COT>;

template<class T, class COT = void>
using dynamic_row_vector =
        matrix<matrix_storage_engine<T, 1, std::dynamic_extent, std::allocator<T>, matrix_layout::row_major>, COT>;


}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_MATRIX_HPP_DEFINED
