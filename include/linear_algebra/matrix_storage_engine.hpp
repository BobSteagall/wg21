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
    detail::valid_mse_extents<extents<N>>   and
    detail::valid_mse_allocator<A, T>       and
    detail::valid_mse_vector_layout<L>
class matrix_storage_engine<T, extents<N>, A, L>
{
    using this_type    = matrix_storage_engine;
    using storage_type = detail::mse_data<T, extents<N>, A, L>;
    using support_type = detail::vector_engine_support;

    using fxd_span     = mdspan<T, N>;
    using c_fxd_span   = mdspan<T const, N>;
    using dyn_span     = mdspan<T, dynamic_extent>;
    using c_dyn_span   = mdspan<T const, dynamic_extent>;

  public:
    using value_type      = T;
    using allocator_type  = A;
    using element_type    = value_type;
    using reference       = element_type&;
    using const_reference = element_type const&;
    using index_type      = ptrdiff_t;
    using span_type       = conditional_t<storage_type::is_fixed_size, fxd_span, dyn_span>;
    using const_span_type = conditional_t<storage_type::is_fixed_size, c_fxd_span, c_dyn_span>;

  public:
    ~matrix_storage_engine() = default;

    //- Construct / assign.
    //
    constexpr matrix_storage_engine() = default;
    constexpr matrix_storage_engine(matrix_storage_engine&&) noexcept = default;
    constexpr matrix_storage_engine(matrix_storage_engine const&) = default;

    constexpr matrix_storage_engine&    operator =(matrix_storage_engine&&) noexcept = default;
    constexpr matrix_storage_engine&    operator =(matrix_storage_engine const&) = default;

    inline constexpr
    matrix_storage_engine(index_type size)
    requires
        detail::reshapable_msd<storage_type>
    :   m_data()
    {
        do_reshape(size, size);
    }

    template<class ET2> inline constexpr
    matrix_storage_engine(ET2 const& rhs)
    requires
        detail::readable_vector_engine<ET2>
    :   m_data()
    {
        support_type::vector_assign_from(*this, rhs);
    }

    template<class T2> inline constexpr
    matrix_storage_engine(initializer_list<T2> rhs)
    requires
        detail::convertible_from<T, T2>
    :   m_data()
    {
        support_type::vector_assign_from(*this, rhs);
    }

    template<class ET2>
    inline constexpr matrix_storage_engine&
    operator =(ET2 const& rhs)
    requires
        detail::readable_vector_engine<ET2>
    {
        support_type::vector_assign_from(*this, rhs);
        return *this;
    }

    template<class T2>
    inline constexpr matrix_storage_engine&
    operator =(initializer_list<T2> rhs)
    requires
        detail::convertible_from<T, T2>
    {
        support_type::vector_assign_from(*this, rhs);
        return *this;
    }

    //- Size and capacity reporting.
    //
    inline constexpr index_type
    size() const noexcept
    {
        return m_data.m_size;
    }

    inline constexpr index_type
    capacity() const noexcept
    {
        return m_data.m_cap;
    }

    //- Setting overall size and capacity.
    //
    void
    reshape(index_type newsize, index_type newcap)
    requires
        detail::reshapable_msd<storage_type>
    {
        do_reshape(newsize, newcap);
    }

    //- Element access
    //
    inline constexpr reference
    operator ()(index_type i)
    {
        return m_data.m_elems[i];
    }

    inline constexpr const_reference
    operator ()(index_type i) const
    {
        return m_data.m_elems[i];
    }


    inline constexpr span_type
    span() noexcept
    {
        return make_mdspan<span_type, element_type>(m_data.m_elems.data(), m_data);
    }

    inline constexpr const_span_type
    span() const noexcept
    {
        return make_mdspan<const_span_type, element_type const>(m_data.m_elems.data(), m_data);
    }

    //- Modifiers
    //
    inline constexpr void
    swap(matrix_storage_engine& rhs) noexcept
    {
        support_type::swap(m_data, rhs.m_data);
    }

  private:
    storage_type    m_data;

    template<class ST, class U>
    static inline constexpr ST
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
    {
        if (newsize == m_data.m_size) return;

        support_type::verify_size(newsize);
        support_type::verify_capacity(newcap);

        //- Only reallocate new storage if we have to.
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
//  Sizes R and C may be positive integers, or have the value "dynamic_extent" (defined by the
//  mdspan facilities).  Allocator type A may be void, which indicates internal non-heap element
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
    detail::valid_mse_extents<extents<R, C>>    and
    detail::valid_mse_allocator<A, T>           and
    detail::valid_mse_matrix_layout<L>
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

    storage_type    m_data;

  public:
    using value_type       = T;
    using allocator_type   = A;
    using element_type     = value_type;
    using reference        = element_type&;
    using const_reference  = element_type const&;
    using index_type       = ptrdiff_t;
    using span_type        = conditional_t<storage_type::is_fixed_size, fxd_span, dyn_span>;
    using const_span_type  = conditional_t<storage_type::is_fixed_size, c_fxd_span, c_dyn_span>;

  public:
    ~matrix_storage_engine() = default;

    //- Construct / assign.
    //
    constexpr matrix_storage_engine() = default;
    constexpr matrix_storage_engine(matrix_storage_engine&&) noexcept = default;
    constexpr matrix_storage_engine(matrix_storage_engine const&) = default;

    constexpr matrix_storage_engine&    operator =(matrix_storage_engine&&) noexcept = default;
    constexpr matrix_storage_engine&    operator =(matrix_storage_engine const&) = default;

    //- Other constructors.
    //
    inline constexpr
    matrix_storage_engine(index_type rows, index_type cols)
    requires
        detail::reshapable_msd<storage_type>
    :   m_data()
    {
        do_reshape(rows, cols, rows, cols);
    }

    inline constexpr
    matrix_storage_engine(index_type rows, index_type cols, index_type rowcap, index_type colcap)
    requires
        detail::reshapable_msd<storage_type>
    :   m_data()
    {
        do_reshape(rows, cols, rowcap, colcap);
    }

    template<class ET2> inline constexpr
    matrix_storage_engine(ET2 const& rhs)
    requires
        detail::readable_matrix_engine<ET2>
    :   m_data()
    {
        support_type::matrix_assign_from(*this, rhs);
    }

    template<class U>
    inline constexpr
    matrix_storage_engine(initializer_list<initializer_list<U>> rhs)
    requires
        detail::convertible_from<T, U>
    :   m_data()
    {
        support_type::matrix_assign_from(*this, rhs);
    }

    template<class U>
    inline constexpr
    matrix_storage_engine(initializer_list<U> rhs)
    requires
        detail::convertible_from<T, U> and
        detail::linearly_indexable_msd<storage_type>
    :   m_data()
    {
        support_type::matrix_assign_from(*this, rhs);
    }

    //- Other assignment operators.
    //
    template<class ET2>
    inline constexpr matrix_storage_engine&
    operator =(ET2 const& rhs)
    requires
        detail::readable_matrix_engine<ET2>
    {
        support_type::matrix_assign_from(*this, rhs);
        return *this;
    }

    template<class U>
    inline constexpr matrix_storage_engine&
    operator =(initializer_list<initializer_list<U>> rhs)
    requires
        detail::convertible_from<T, U>
    {
        support_type::matrix_assign_from(*this, rhs);
        return *this;
    }

    template<class U>
    inline constexpr matrix_storage_engine&
    operator =(initializer_list<U> rhs)
    requires
        detail::convertible_from<T, U>     and
        detail::linearly_indexable_msd<storage_type>
    {
        support_type::matrix_assign_from(*this, rhs);
        return *this;
    }

    //- Size and capacity reporting.
    //
    inline constexpr index_type
    columns() const noexcept
    {
        return m_data.m_cols;
    }

    inline constexpr index_type
    rows() const noexcept
    {
        return m_data.m_rows;
    }

    inline constexpr index_type
    size() const noexcept
    {
        return m_data.m_rows * m_data.m_cols;
    }

    inline constexpr index_type
    column_capacity() const noexcept
    {
        return m_data.m_colcap;
    }

    inline constexpr index_type
    row_capacity() const noexcept
    {
        return m_data.m_rowcap;
    }

    inline constexpr index_type
    capacity() const noexcept
    {
        return m_data.m_rowcap * m_data.m_colcap;
    }

    //- Element access
    //
    inline constexpr reference
    operator ()(index_type i)
    requires
        detail::linearly_indexable_msd<storage_type>
    {
        return m_data.m_elems[i];
    }

    inline constexpr const_reference
    operator ()(index_type i) const
    requires
        detail::linearly_indexable_msd<storage_type>
    {
        return m_data.m_elems[i];
    }

    inline constexpr reference
    operator ()(index_type i, index_type j)
    {
        if constexpr (storage_type::is_row_major)
            return m_data.m_elems[(i * m_data.m_colcap) + j];
        else
            return m_data.m_elems[i + (j * m_data.m_rowcap)];
    }

    inline constexpr const_reference
    operator ()(index_type i, index_type j) const
    {
        if constexpr (storage_type::is_row_major)
            return m_data.m_elems[(i * m_data.m_colcap) + j];
        else
            return m_data.m_elems[i + (j * m_data.m_rowcap)];
    }

    inline constexpr span_type
    span() noexcept
    {
        return make_mdspan<span_type, element_type>(m_data.m_elems.data(), m_data);
    }

    inline constexpr const_span_type
    span() const noexcept
    {
        return make_mdspan<const_span_type, element_type const>(m_data.m_elems.data(), m_data);
    }

    //- Setting overall size and capacity.
    //
    inline constexpr void
    reshape(index_type rows, index_type cols, index_type rowcap, index_type colcap)
    requires
        detail::reshapable_msd<storage_type>
    {
        do_reshape(rows, cols, rowcap, colcap);
    }

    //- Setting column size and capacity.
    //
    inline constexpr void
    reshape_columns(index_type cols, index_type colcap)
    requires
        detail::column_reshapable_msd<storage_type>
    {
        if constexpr (detail::reshapable_msd<storage_type>)
            do_reshape(m_data.m_rows, cols, m_data.m_rowcap, colcap);
        else
            do_reshape_columns(cols, colcap);
    }

    //- Setting row size and capacity.
    //
    inline constexpr void
    reshape_rows(index_type rows, index_type rowcap)
    requires
        detail::row_reshapable_msd<storage_type>
    {
        do_reshape_rows(rows, rowcap);
    }

    inline constexpr void
    reshape_rows(index_type rows, index_type rowcap)
    requires
        detail::row_reshapable_msd<storage_type>    and
        detail::reshapable_msd<storage_type>
    {
        do_reshape(rows, m_data.m_cols, rowcap, m_data.m_colcap);
    }

    //- Other modifiers
    //
    inline constexpr void
    swap(matrix_storage_engine& rhs) noexcept
    {
        support_type::swap(m_data, rhs.m_data);
    }

  private:
    template<class ST, class U>
    static inline constexpr ST
    make_mdspan(U* pdata, storage_type const& rep)
    {
        if constexpr (storage_type::is_fixed_size)
        {
            return ST(pdata);
        }
        else
        {
            if constexpr (storage_type::is_row_major)
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
        detail::reshapable_msd<storage_type>
    {
        support_type::verify_size(rows);
        support_type::verify_size(cols);
        support_type::verify_capacity(rowcap);
        support_type::verify_capacity(colcap);

        //- Only reallocate new storage if we have to.
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
        detail::column_reshapable_msd<storage_type>     and
        (not detail::reshapable_msd<storage_type>)
    {
        support_type::verify_size(cols);
        support_type::verify_capacity(colcap);

        //- Only reallocate new storage if we have to.
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
        detail::row_reshapable_msd<storage_type>     and
        (not detail::reshapable_msd<storage_type>)
    {
        support_type::verify_size(rows);
        support_type::verify_capacity(rowcap);

        //- Only reallocate new storage if we have to.
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
