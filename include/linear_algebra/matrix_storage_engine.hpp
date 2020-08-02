//==================================================================================================
//  File:       matrix_storage_engine.hpp
//
//  Summary:    This header defines an owning, dense, rectangular engine that owns and manages
//              the elements of a matrix.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_MATRIX_STORAGE_ENGINE_HPP_DEFINED
#define LINEAR_ALGEBRA_MATRIX_STORAGE_ENGINE_HPP_DEFINED

namespace STD_LA {

//--------------------------------------------------------------------------------------------------
//  Partial Specialization:     matrix_storage_engine<T, extents<N>, A, L>
//
//  This partial specialization of matrix_storage_engine<T,X,A,L> implements an owning engine
//  for use by class template basic_vector<ET, OT>.  Specifically, it models a mathematical
//  vector having N elements, employing allocator A, and having element layout L.
//
//  Size N may be a positive integer, or have the value "dynamic_extent" (defined by the
//  mdspan facilities).  Allocator type A may be void, which indicates internal non-heap
//  element storage, or it may be an allocator type that fulfills all the requirements imposed
//  by std::allocator_traits.  Layout type L must be "unspecified".
//
//  This specialization assumes that all dynamically-allocated memory is default-constructed,
//  with the consequence that elements lying in unused capacity are also constructed.  This
//  assumption makes implementation easy, but may be absent in the final version.
//--------------------------------------------------------------------------------------------------
//
template<class T, ptrdiff_t N, class A, class L>
requires
    detail::valid_engine_extents<extents<N>>  and
    detail::valid_engine_allocator<A, T>  and
    detail::valid_layout_for_1d_storage_engine<L>
class matrix_storage_engine<T, extents<N>, A, L>
{
    using this_type    = matrix_storage_engine;
    using storage_type = detail::mse_data<T, extents<N>, A, L>;
    using support_type = detail::vector_engine_support;

    using fxd_span     = mdspan<T, N>;
    using c_fxd_span   = mdspan<T const, N>;
    using dyn_span     = mdspan<T, dynamic_extent>;
    using c_dyn_span   = mdspan<T const, dynamic_extent>;

    static constexpr bool   is_fixed_size = storage_type::is_fixed_size;
    static constexpr bool   is_reshapable = storage_type::is_reshapable;

  private:
    storage_type    m_data;

  public:
    using value_type      = T;
    using allocator_type  = A;
    using element_type    = value_type;
    using reference       = element_type&;
    using const_reference = element_type const&;
    using index_type      = ptrdiff_t;
    using span_type       = conditional_t<this_type::is_fixed_size, fxd_span, dyn_span>;
    using const_span_type = conditional_t<this_type::is_fixed_size, c_fxd_span, c_dyn_span>;

  public:
    ~matrix_storage_engine() = default;

    //- Construct / assign.
    //
    constexpr matrix_storage_engine() = default;
    constexpr matrix_storage_engine(matrix_storage_engine&&) noexcept = default;
    constexpr matrix_storage_engine(matrix_storage_engine const&) = default;

    constexpr matrix_storage_engine&    operator =(matrix_storage_engine&&) noexcept = default;
    constexpr matrix_storage_engine&    operator =(matrix_storage_engine const&) = default;

    //- Heterogeneous constructors.
    //
    constexpr
    matrix_storage_engine(index_type size)
    requires
        this_type::is_reshapable
    :   m_data()
    {
        do_reshape(size, size);
    }

    constexpr
    matrix_storage_engine(index_type size, index_type cap)
    requires
        this_type::is_reshapable
    :   m_data()
    {
        do_reshape(size, cap);
    }

    template<class ET2>
    constexpr
    matrix_storage_engine(ET2 const& rhs)
    requires
        detail::readable_vector_engine<ET2>
        and
        detail::convertible_from<element_type, typename ET2::element_type>
    :   m_data()
    {
        support_type::assign_from(*this, rhs);
    }

    template<class CT>
    constexpr
    matrix_storage_engine(CT const& rhs)
    requires
        detail::random_access_standard_container<CT>
        and
        detail::convertible_from<element_type, typename CT::value_type>
    :   m_data()
    {
        support_type::assign_from(*this, rhs);
    }

    template<class U, ptrdiff_t X0, class SL, class SA>
    constexpr
    matrix_storage_engine(basic_mdspan<U, extents<X0>, SL, SA> const& rhs)
    requires
        detail::convertible_from<element_type, U>
    :   m_data()
    {
        support_type::assign_from(*this, rhs);
    }

    template<class U>
    constexpr
    matrix_storage_engine(initializer_list<U> rhs)
    requires
        detail::convertible_from<element_type, U>
    :   m_data()
    {
        support_type::assign_from(*this, rhs);
    }

    //- Heterogeneous assignment operators.
    //
    template<class ET2>
    constexpr matrix_storage_engine&
    operator =(ET2 const& rhs)
    requires
        detail::readable_vector_engine<ET2>
        and
        detail::convertible_from<element_type, typename ET2::element_type>
    {
        support_type::assign_from(*this, rhs);
        return *this;
    }

    template<class CT>
    constexpr matrix_storage_engine&
    operator =(CT const& rhs)
    requires
        detail::random_access_standard_container<CT>
        and
        detail::convertible_from<element_type, typename CT::value_type>
    {
        support_type::assign_from(*this, rhs);
        return *this;
    }

    template<class U, ptrdiff_t X0, class SL, class SA>
    constexpr matrix_storage_engine&
    operator =(basic_mdspan<U, extents<X0>, SL, SA> const& rhs)
    requires
        detail::convertible_from<element_type, U>
    {
        support_type::assign_from(*this, rhs);
        return *this;
    }

    template<class U>
    constexpr matrix_storage_engine&
    operator =(initializer_list<U> rhs)
    requires
        detail::convertible_from<element_type, U>
    {
        support_type::assign_from(*this, rhs);
        return *this;
    }

    //- Size and capacity reporting.
    //
    constexpr index_type
    size() const noexcept
    {
        return m_data.m_size;
    }

    constexpr index_type
    capacity() const noexcept
    {
        return m_data.m_cap;
    }

    //- Setting overall size and capacity.
    //
    void
    reshape(index_type newsize, index_type newcap)
    requires
        this_type::is_reshapable
    {
        do_reshape(newsize, newcap);
    }

    //- Element access
    //
    constexpr reference
    operator ()(index_type i)
    {
        return m_data.m_elems[i];
    }

    constexpr const_reference
    operator ()(index_type i) const
    {
        return m_data.m_elems[i];
    }

    constexpr span_type
    span() noexcept
    {
        return make_mdspan<span_type, element_type>(m_data.m_elems.data(), m_data);
    }

    constexpr const_span_type
    span() const noexcept
    {
        return make_mdspan<const_span_type, element_type const>(m_data.m_elems.data(), m_data);
    }

    //- Modifiers
    //
    constexpr void
    swap(matrix_storage_engine& rhs) noexcept
    {
        support_type::swap(m_data, rhs.m_data);
    }

  private:
    template<class ST, class U>
    static constexpr ST
    make_mdspan(U* pdata, storage_type const& rep)
    {
        if constexpr (storage_type::is_fixed_size)
        {
            return ST(pdata);
        }
        else
        {
            return ST(pdata, rep.m_size);
        }
    }

    void
    do_reshape(ptrdiff_t newsize, ptrdiff_t newcap)
    requires
        this_type::is_reshapable
    {
        if (newsize == m_data.m_size) return;

        support_type::verify_size(newsize);
        support_type::verify_capacity(newcap);

        //- Only allocate new storage if it is needed.
        //
        if (newsize > m_data.m_cap  ||  newcap != m_data.m_cap)
        {
            //- Normalize requested new capacity.
            //
            newcap = max(newsize, newcap);

            //- Prepare a temporary engine to receive elements from this one.
            //
            this_type   tmp;
            tmp.m_data.m_elems.resize(newcap);
            tmp.m_data.m_size = newsize;
            tmp.m_data.m_cap  = newcap;

            //- Move the appropriate subset of elements into the temporary engine, then swap.
            //
            ptrdiff_t   dst_size = min(newsize, m_data.m_size);
            support_type::move_elements(tmp, *this, dst_size);
            support_type::swap(m_data, tmp.m_data);
        }
        else
        {
            if (newsize < m_data.m_size)
            {
                support_type::fill(*this, newsize, m_data.m_size, T{});
                m_data.m_size = newsize;
            }
        }
    }
};


//--------------------------------------------------------------------------------------------------
//  Partial Specialization:     matrix_storage_engine<T, extents<R, C>, A, L>
//
//  This partial specialization of matrix_storage_engine<T,X,A,L> implements an owning engine
//  for use by class template basic_matrix<ET, OT>.  Specifically, it models a mathematical
//  matrix with R rows and C columns, employing allocator A, and having element layout L.
//
//  Sizes R and C may be positive integers, or have the value "dynamic_extent" (defined in the
//  <mdspan> header).  Allocator type A may be void, which indicates internal non-heap element
//  storage, or it may be an allocator type that fulfills all the requirements imposed by
//  std::allocator_traits.  Layout type L must be "row_major" or "column_major".
//
//  This specialization assumes that all dynamically-allocated memory is default-constructed,
//  with the consequence that elements lying in unused capacity are also constructed.  This
//  assumption makes implementation easy, but may be absent in the final version.
//--------------------------------------------------------------------------------------------------
//
template<class T, ptrdiff_t R, ptrdiff_t C, class A, class L>
requires
    detail::valid_engine_extents<extents<R, C>>
    and detail::valid_engine_allocator<A, T>
    and detail::valid_layout_for_2d_storage_engine<L>
class matrix_storage_engine<T, extents<R, C>, A, L>
{
    using this_type    = matrix_storage_engine;
    using storage_type = detail::mse_data<T, extents<R, C>, A, L>;
    using support_type = detail::matrix_engine_support;

    using this_layout = conditional_t<storage_type::is_row_major, layout_right, layout_left>;
    using fxd_span    = basic_mdspan<T, extents<R, C>, this_layout>;
    using c_fxd_span  = basic_mdspan<T const, extents<R, C>, this_layout>;

    using dyn_extents = extents<dynamic_extent, dynamic_extent>;
    using dyn_layout  = layout_stride<dynamic_extent, dynamic_extent>;
    using dyn_strides = array<typename dyn_extents::index_type, 2>;
    using dyn_mapping = typename dyn_layout::template mapping<dyn_extents>;
    using dyn_span    = basic_mdspan<T, dyn_extents, dyn_layout>;
    using c_dyn_span  = basic_mdspan<T const, dyn_extents, dyn_layout>;

    static constexpr bool   is_fixed_size        = storage_type::is_fixed_size;
    static constexpr bool   is_1d_indexable      = storage_type::is_1d_indexable;
    static constexpr bool   is_column_major      = storage_type::is_column_major;
    static constexpr bool   is_row_major         = storage_type::is_row_major;
    static constexpr bool   is_two_d_reshapable  = storage_type::is_two_d_reshapable;
    static constexpr bool   is_column_reshapable = storage_type::is_column_reshapable;
    static constexpr bool   is_row_reshapable    = storage_type::is_row_reshapable;

  private:
    storage_type    m_data;

  public:
    using value_type      = T;
    using allocator_type  = A;
    using element_type    = value_type;
    using reference       = element_type&;
    using const_reference = element_type const&;
    using index_type      = ptrdiff_t;
    using span_type       = conditional_t<this_type::is_fixed_size, fxd_span, dyn_span>;
    using const_span_type = conditional_t<this_type::is_fixed_size, c_fxd_span, c_dyn_span>;

  public:
    ~matrix_storage_engine() = default;

    //- Construct / assign.
    //
    constexpr matrix_storage_engine() = default;
    constexpr matrix_storage_engine(matrix_storage_engine&&) noexcept = default;
    constexpr matrix_storage_engine(matrix_storage_engine const&) = default;

    constexpr matrix_storage_engine&    operator =(matrix_storage_engine&&) noexcept = default;
    constexpr matrix_storage_engine&    operator =(matrix_storage_engine const&) = default;

    //- Rehsaping constructors.
    //
    constexpr
    matrix_storage_engine(index_type rows, index_type cols)
    requires
        this_type::is_two_d_reshapable
    :   m_data()
    {
        do_reshape(rows, cols, rows, cols);
    }

    constexpr
    matrix_storage_engine(index_type rows, index_type cols, index_type rowcap, index_type colcap)
    requires
        this_type::is_two_d_reshapable
    :   m_data()
    {
        do_reshape(rows, cols, rowcap, colcap);
    }

    //- Heterogeneous construction from two-dimensional sources.
    //
    template<class ET2>
    constexpr
    matrix_storage_engine(ET2 const& rhs)
    requires
        detail::readable_matrix_engine<ET2>
        and
        detail::convertible_from<element_type, typename ET2::element_type>
    :   m_data()
    {
        support_type::assign_from(*this, rhs);
    }

    template<class U, ptrdiff_t X0, ptrdiff_t X1, class SL, class SA>
    constexpr
    matrix_storage_engine(basic_mdspan<U, extents<X0, X1>, SL, SA> const& rhs)
    requires
        detail::convertible_from<element_type, U>
    :   m_data()
    {
        support_type::assign_from(*this, rhs);
    }

    template<class U>
    constexpr
    matrix_storage_engine(initializer_list<initializer_list<U>> rhs)
    requires
        detail::convertible_from<T, U>
    :   m_data()
    {
        support_type::assign_from(*this, rhs);
    }

    //- Heterogeneous construction from one-dimensional sources.
    //
    template<class ET2>
    constexpr
    matrix_storage_engine(ET2 const& rhs)
    requires
        this_type::is_1d_indexable
        and
        detail::readable_1d_vector_engine<ET2>
        and
        detail::convertible_from<element_type, typename ET2::element_type>
    :   m_data()
    {
        support_type::assign_from(*this, rhs);
    }

    template<class CT>
    constexpr
    matrix_storage_engine(CT const& rhs)
    requires
        this_type::is_1d_indexable
        and
        detail::random_access_standard_container<CT>
        and
        detail::convertible_from<element_type, typename CT::value_type>
    :   m_data()
    {
        support_type::assign_from(*this, rhs);
    }

    template<class U, ptrdiff_t X0, class SL, class SA>
    constexpr
    matrix_storage_engine(basic_mdspan<U, extents<X0>, SL, SA> const& rhs)
    requires
        this_type::is_1d_indexable
        and
        detail::convertible_from<element_type, U>
    :   m_data()
    {
        support_type::assign_from(*this, rhs);
    }

    template<class U>
    constexpr
    matrix_storage_engine(initializer_list<U> rhs)
    requires
        this_type::is_1d_indexable
        and
        detail::convertible_from<element_type, U>
    :   m_data()
    {
        support_type::assign_from(*this, rhs);
    }

    //- Heterogeneous assignment from two-dimensional sources.
    //
    template<class ET2>
    constexpr matrix_storage_engine&
    operator =(ET2 const& rhs)
    requires
        detail::readable_matrix_engine<ET2>
        and
        detail::convertible_from<element_type, typename ET2::element_type>
    {
        support_type::assign_from(*this, rhs);
        return *this;
    }

    template<class U, ptrdiff_t X0, ptrdiff_t X1, class SL, class SA>
    constexpr matrix_storage_engine&
    operator =(basic_mdspan<U, extents<X0, X1>, SL, SA> const& rhs)
    requires
        detail::convertible_from<element_type, U>
    {
        support_type::assign_from(*this, rhs);
        return *this;
    }

    template<class U>
    constexpr matrix_storage_engine&
    operator =(initializer_list<initializer_list<U>> rhs)
    requires
        detail::convertible_from<element_type, U>
    {
        support_type::assign_from(*this, rhs);
        return *this;
    }

    //- Heterogeneous assignment from one-dimensional sources.
    //
    template<class ET2>
    constexpr matrix_storage_engine&
    operator =(ET2 const& rhs)
    requires
        this_type::is_1d_indexable
        and
        detail::readable_1d_vector_engine<ET2>
        and
        detail::convertible_from<element_type, typename ET2::element_type>
    {
        support_type::assign_from(*this, rhs);
        return *this;
    }

    template<class CT>
    constexpr matrix_storage_engine&
    operator =(CT const& rhs)
    requires
        this_type::is_1d_indexable
        and
        detail::random_access_standard_container<CT>
        and
        detail::convertible_from<element_type, typename CT::value_type>
    {
        support_type::assign_from(*this, rhs);
        return *this;
    }

    template<class U, ptrdiff_t X0, class SL, class SA>
    constexpr matrix_storage_engine&
    operator =(basic_mdspan<U, extents<X0>, SL, SA> const& rhs)
    requires
        this_type::is_1d_indexable
        and
        detail::convertible_from<element_type, U>
    {
        support_type::assign_from(*this, rhs);
        return *this;
    }

    template<class U>
    constexpr matrix_storage_engine&
    operator =(initializer_list<U> rhs)
    requires
        this_type::is_1d_indexable
        and
        detail::convertible_from<element_type, U>
    {
        support_type::assign_from(*this, rhs);
        return *this;
    }

    //- Size and capacity reporting.
    //
    constexpr index_type
    columns() const noexcept
    {
        return m_data.m_cols;
    }

    constexpr index_type
    rows() const noexcept
    {
        return m_data.m_rows;
    }

    constexpr index_type
    size() const noexcept
    {
        return m_data.m_rows * m_data.m_cols;
    }

    constexpr index_type
    column_capacity() const noexcept
    {
        return m_data.m_colcap;
    }

    constexpr index_type
    row_capacity() const noexcept
    {
        return m_data.m_rowcap;
    }

    constexpr index_type
    capacity() const noexcept
    {
        return m_data.m_rowcap * m_data.m_colcap;
    }

    //- Element access
    //
    constexpr reference
    operator ()(index_type i)
    requires
        this_type::is_1d_indexable
    {
        return m_data.m_elems[i];
    }

    constexpr const_reference
    operator ()(index_type i) const
    requires
        this_type::is_1d_indexable
    {
        return m_data.m_elems[i];
    }

    constexpr reference
    operator ()(index_type i, index_type j)
    requires
        this_type::is_row_major
    {
        return m_data.m_elems[(i * m_data.m_colcap) + j];
    }

    constexpr reference
    operator ()(index_type i, index_type j)
    requires
        this_type::is_column_major
    {
         return m_data.m_elems[i + (j * m_data.m_rowcap)];
    }

    constexpr const_reference
    operator ()(index_type i, index_type j) const
    requires
        this_type::is_row_major
    {
        return m_data.m_elems[(i * m_data.m_colcap) + j];
    }

    constexpr const_reference
    operator ()(index_type i, index_type j) const
    requires
        this_type::is_column_major
    {
         return m_data.m_elems[i + (j * m_data.m_rowcap)];
    }

    constexpr span_type
    span() noexcept
    {
        return make_mdspan<span_type, element_type>(m_data.m_elems.data(), m_data);
    }

    constexpr const_span_type
    span() const noexcept
    {
        return make_mdspan<const_span_type, element_type const>(m_data.m_elems.data(), m_data);
    }

    //- Setting overall size and capacity.
    //
    constexpr void
    reshape(index_type rows, index_type cols, index_type rowcap, index_type colcap)
    requires
        this_type::is_two_d_reshapable
    {
        do_reshape(rows, cols, rowcap, colcap);
    }

    //- Setting column size and capacity.
    //
    constexpr void
    reshape_columns(index_type cols, index_type colcap)
    requires
        this_type::is_column_reshapable
    {
        do_reshape_columns(cols, colcap);
    }

    constexpr void
    reshape_columns(index_type cols, index_type colcap)
    requires
        this_type::is_two_d_reshapable
    {
        do_reshape(m_data.m_rows, cols, m_data.m_rowcap, colcap);
    }

    //- Setting row size and capacity.
    //
    constexpr void
    reshape_rows(index_type rows, index_type rowcap)
    requires
        this_type::is_row_reshapable
    {
        do_reshape_rows(rows, rowcap);
    }

    constexpr void
    reshape_rows(index_type rows, index_type rowcap)
    requires
        this_type::is_two_d_reshapable
    {
        do_reshape(rows, m_data.m_cols, rowcap, m_data.m_colcap);
    }

    //- Other modifiers.
    //
    constexpr void
    swap(matrix_storage_engine& rhs) noexcept
    {
        support_type::swap(m_data, rhs.m_data);
    }

  private:
    template<class ST, class U>
    static constexpr ST
    make_mdspan(U* pdata, storage_type const& rep)
    {
        if constexpr (this_type::is_fixed_size)
        {
            return ST(pdata);
        }
        else
        {
            if constexpr (this_type::is_row_major)
            {
                dyn_extents     extents(rep.m_rows, rep.m_cols);
                dyn_strides     strides{rep.m_colcap, 1};
                dyn_mapping     mapping(extents, strides);

                return ST(pdata, mapping);
            }
            else
            {
                dyn_extents     extents(rep.m_rows, rep.m_cols);
                dyn_strides     strides{1, rep.m_rowcap};
                dyn_mapping     mapping(extents, strides);

                return ST(pdata, mapping);
            }
        }
    }

    void
    do_reshape(ptrdiff_t rows, ptrdiff_t cols, ptrdiff_t rowcap, ptrdiff_t colcap)
    requires
        this_type::is_two_d_reshapable
    {
        support_type::verify_size(rows);
        support_type::verify_size(cols);
        support_type::verify_capacity(rowcap);
        support_type::verify_capacity(colcap);

        //- Only allocate new storage if we need to.
        //
        if (rows > m_data.m_rowcap  ||  rowcap != m_data.m_rowcap  ||
            cols > m_data.m_colcap  ||  colcap != m_data.m_colcap)
        {
            //- Normalize requested new capacities.
            //
            rowcap = max(rows, rowcap);
            colcap = max(cols, colcap);

            //- Prepare a temporary object to receive elements from this one.
            //
            this_type   tmp;
            tmp.m_data.m_elems.resize(rowcap * colcap);
            tmp.m_data.m_rows   = rows;
            tmp.m_data.m_cols   = cols;
            tmp.m_data.m_rowcap = rowcap;
            tmp.m_data.m_colcap = colcap;

            //- Move the appropriate subset of elements into the temporary engine and swap.
            //
            ptrdiff_t   dst_rows = min(rows, m_data.m_rows);
            ptrdiff_t   dst_cols = min(cols, m_data.m_cols);
            support_type::move_elements(tmp, *this, dst_rows, dst_cols);
            support_type::swap(m_data, tmp.m_data);
        }
        else
        {
            if (rows < m_data.m_rows)
            {
                support_type::fill_rows(*this, rows, m_data.m_rows, T{});
                m_data.m_rows = rows;
            }
            if (cols < m_data.m_cols)
            {
                support_type::fill_columns(*this, cols, m_data.m_cols, T{});
                m_data.m_cols = cols;
            }
        }
    }

    void
    do_reshape_columns(ptrdiff_t cols, ptrdiff_t colcap)
    requires
        this_type::is_column_reshapable
    {
        support_type::verify_size(cols);
        support_type::verify_capacity(colcap);

        //- Only allocate new storage if we need to.
        //
        if (cols > m_data.m_colcap  ||  colcap != m_data.m_colcap)
        {
            //- Normalize requested new capacity.
            //
            colcap = max(cols, colcap);

            //- Prepare a temporary object to receive elements from this one.
            //
            this_type   tmp;
            tmp.m_data.m_elems.resize(m_data.m_rowcap * colcap);
            tmp.m_data.m_cols   = cols;
            tmp.m_data.m_colcap = colcap;

            //- Move the appropriate subset of elements into the temporary engine and swap.
            //
            ptrdiff_t   dst_cols = min(cols, m_data.m_cols);
            support_type::move_elements(tmp, *this, m_data.m_rows, dst_cols);
            support_type::swap(*this, tmp);
        }
        else
        {
            if (cols < m_data.m_cols)
            {
                support_type::fill_columns(*this, cols, m_data.m_cols, T{});
                m_data.m_cols = cols;
            }
        }
    }

    void
    do_reshape_rows(ptrdiff_t rows, ptrdiff_t rowcap)
    requires
        this_type::is_row_reshapable
    {
        support_type::verify_size(rows);
        support_type::verify_capacity(rowcap);

        //- Only allocate new storage if we need to.
        //
        if (rows > m_data.m_rowcap  ||  rowcap != m_data.m_rowcap)
        {
            //- Normalize requested new capacity.
            //
            rowcap = max(rows, rowcap);

            //- Prepare a temporary object to receive elements from this one.
            //
            this_type   tmp;
            tmp.m_data.m_elems.resize(rowcap * m_data.m_colcap);
            tmp.m_data.m_rows   = rows;
            tmp.m_data.m_rowcap = rowcap;

            //- Move the appropriate subset of elements into the temporary engine and swap.
            //
            ptrdiff_t   dst_rows = min(rows, m_data.m_rows);
            support_type::move_elements(tmp, *this, dst_rows, m_data.m_cols);
            support_type::swap(*this, tmp);
        }
        else
        {
            if (rows < m_data.m_rows)
            {
                support_type::fill_rows(*this, rows, m_data.m_rows, T{});
                m_data.m_rows = rows;
            }
        }
    }
};

template<class OT,
         class T1, ptrdiff_t R1, ptrdiff_t C1, class AT1, class EL1,
         class T2, ptrdiff_t R2, ptrdiff_t C2, class AT2, class EL2>
struct matrix_multiplication_engine_traits<OT,
                                           matrix_storage_engine<T1, extents<R1, C1>, AT1, EL1>,
                                           matrix_storage_engine<T2, extents<R2, C2>, AT2, EL2>>
{
    using element_type = select_matrix_multiplication_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<AT1, element_type>;
    using engine_type  = matrix_storage_engine<element_type, extents<R1, C2>, alloc_type, EL1>;
};

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_MATRIX_STORAGE_ENGINE_HPP_DEFINED
