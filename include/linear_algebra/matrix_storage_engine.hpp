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

template<class T, class X, class A, class L>
class matrix_storage_engine;


template<class T, ptrdiff_t R, ptrdiff_t C, class A, class L>
    requires
        detail::valid_mse_extents<extents<R, C>>  and
        detail::valid_mse_allocator<A, T>         and
        detail::valid_mse_layout<L>
class matrix_storage_engine<T, extents<R, C>, A, L>
{
    using this_type     = matrix_storage_engine;
    using engine_traits = detail::mse_traits<T, extents<R, C>, A, L>;
    using storage_type  = detail::mse_data<T, extents<R, C>, A, L>;

  public:
    using value_type       = T;
    using allocator_type   = A;
    using element_type     = value_type;
    using reference        = element_type&;
    using const_reference  = element_type const&;
    using index_type       = ptrdiff_t;
    using index_tuple_type = tuple<index_type, index_type>;
    using span_type        = typename engine_traits::span_type;
    using const_span_type  = typename engine_traits::const_span_type;

  public:
    inline ~matrix_storage_engine() = default;

    //- Construct / assign.
    //
    constexpr matrix_storage_engine() = default;
    constexpr matrix_storage_engine(matrix_storage_engine&& rhs) noexcept = default;
    constexpr matrix_storage_engine(matrix_storage_engine const& rhs) = default;

    constexpr matrix_storage_engine&    operator =(matrix_storage_engine&& rhs) noexcept = default;
    constexpr matrix_storage_engine&    operator =(matrix_storage_engine const& rhs) = default;

    inline constexpr
    matrix_storage_engine(index_type rows, index_type cols)
        requires engine_traits::is_resizable
    :   m_data()
    {
        reshape(rows, cols, rows, cols);
    }

    inline constexpr
    matrix_storage_engine(index_type rows, index_type cols, index_type rowcap, index_type colcap)
        requires engine_traits::is_resizable
    :   m_data()
    {
        reshape(rows, cols, rowcap, colcap);
    }

    template<class ET2> inline constexpr
    matrix_storage_engine(ET2 const& rhs)
    :   m_data()
    {
        assign(rhs);
    }

    template<class T2> inline constexpr
    matrix_storage_engine(initializer_list<initializer_list<T2>> rhs)
    :   m_data()
    {
        assign(rhs);
    }

    template<class ET2>
    inline constexpr matrix_storage_engine&
    operator =(ET2 const& rhs)
    {
        assign(rhs);
        return *this;
    }

    template<class T2>
    inline constexpr matrix_storage_engine&
    operator =(initializer_list<initializer_list<T2>> rhs)
    {
        assign(rhs);
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

    inline constexpr index_tuple_type
    size() const noexcept
    {
        return index_tuple_type(m_data.m_rows, m_data.m_cols);
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

    inline constexpr index_tuple_type
    capacity() const noexcept
    {
        return index_tuple_type(m_data.m_rowcap, m_data.m_colcap);
    }

    //- Setting column size and capacity.
    //
    inline void
    resize_columns(index_type cols)
        requires engine_traits::is_column_resizable
    {
        reshape_columns(cols, m_data.m_colcap);
    }

    inline void
    reserve_columns(index_type colcap)
        requires engine_traits::is_column_resizable
    {
        reshape_columns(m_data.m_cols, colcap);
    }

    void
    reshape_columns(index_type cols, index_type colcap)
        requires engine_traits::is_column_resizable
    {
        //- Only reallocate new storage if we have to.
        //
        if (cols > m_data.m_colcap  ||  colcap != m_data.m_colcap)
        {
            //- Prepare a temporary engine to receive elements from this one.
            //
            this_type   tmp;

            tmp.m_data.m_elems.resize(m_data.m_rowcap*colcap);
            tmp.m_data.m_cols   = cols;
            tmp.m_data.m_colcap = colcap;

            //- Move the appropriate subset of elements into the temporary engine, then swap.
            //
            index_type  dst_rows = m_data.m_rows;
            index_type  dst_cols = min(cols, m_data.m_cols);

            move_from(0, 0, dst_rows, dst_cols, *this);
            detail::la_swap(m_data, tmp.m_data);
        }
        else
        {
            if (cols < m_data.m_cols)
            {
                fill(0, cols, m_data.m_rows, min(cols, m_data.m_cols), T{});
            }
            m_data.m_cols = cols;
        }
    }

    //- Setting row size and capacity.
    //
    inline void
    resize_rows(index_type rows)
        requires engine_traits::is_row_resizable
    {
        reshape_rows(rows, m_data.m_rowcap);
    }

    inline void
    reserve_rows(index_type rowcap)
        requires engine_traits::is_row_resizable
    {
        reshape_rows(m_data.m_rows, rowcap);
    }

    void
    reshape_rows(index_type rows, index_type rowcap)
        requires engine_traits::is_row_resizable
    {
        //- Only reallocate new storage if we have to.
        //
        if (rows > m_data.m_rowcap  ||  rowcap != m_data.m_rowcap)
        {
            //- Prepare a temporary engine to receive elements from this one.
            //
            this_type   tmp;

            tmp.m_data.m_elems.resize(rowcap*m_data.m_colcap);
            tmp.m_data.m_rows   = rows;
            tmp.m_data.m_rowcap = rowcap;

            //- Move the appropriate subset of elements into the temporary engine, then swap.
            //
            index_type  dst_rows = min(rows, m_data.m_rows);
            index_type  dst_cols = m_data.m_cols;

            tmp.move_from(0, 0, dst_rows, dst_cols, *this);
            detail::la_swap(m_data, tmp.m_data);
        }
        else
        {
            if (rows < m_data.m_rows)
            {
                fill(rows, 0, m_data.m_rows, m_data.m_cols, T{});
            }
            m_data.m_rows = rows;
        }
    }

    //- Setting overall size and capacity.
    //
    inline void
    resize(index_type rows, index_type cols)
        requires engine_traits::is_resizable
    {
        reshape(rows, cols, m_data.m_rowcap, m_data.m_colcap);
    }

    inline void
    reserve(index_type rowcap, index_type colcap)
        requires engine_traits::is_resizable
    {
        reshape(m_data.m_rows, m_data.m_cols, rowcap, colcap);
    }

    void
    reshape(index_type rows, index_type cols, index_type rowcap, index_type colcap)
        requires engine_traits::is_resizable
    {
        detail::check_size(rows);
        detail::check_size(cols);
        detail::check_capacity(rowcap);
        detail::check_capacity(colcap);

        //- Only reallocate new storage if we have to.
        //
        if (rows   >  m_data.m_rowcap  ||  cols   >  m_data.m_colcap  ||
            rowcap != m_data.m_rowcap  ||  colcap != m_data.m_colcap)
        {
            rowcap = max(rows, rowcap);
            colcap = max(cols, colcap);

            //- Prepare a temporary engine to receive elements from this one.
            //
            this_type   tmp;

            tmp.m_data.m_elems.resize(rowcap*colcap);
            tmp.m_data.m_rows   = rows;
            tmp.m_data.m_cols   = cols;
            tmp.m_data.m_rowcap = rowcap;
            tmp.m_data.m_colcap = colcap;

            //- Move the appropriate subset of elements into the temporary engine, then swap.
            //
            index_type  dst_rows = min(rows, m_data.m_rows);
            index_type  dst_cols = min(cols, m_data.m_cols);

            tmp.move_from(0, 0, dst_rows, dst_cols, *this);
            detail::la_swap(m_data, tmp.m_data);
        }
        else
        {
            if (rows < m_data.m_rows)
            {
                fill(rows, 0, m_data.m_rows, m_data.m_cols, T{});
            }
            if (cols < m_data.m_cols)
            {
                fill(0, cols, min(rows, m_data.m_rows), m_data.m_cols, T{});
            }
            m_data.m_rows = rows;
            m_data.m_cols = cols;
        }
    }

    //- Element access
    //
    inline constexpr reference
    operator ()(index_type i)
        requires (engine_traits::is_column_matrix || engine_traits::is_row_matrix)
    {
        return m_data.m_elems[i];
    }

    inline constexpr const_reference
    operator ()(index_type i) const
        requires (engine_traits::is_column_matrix || engine_traits::is_row_matrix)
    {
        return m_data.m_elems[i];
    }

    inline constexpr reference
    operator ()(index_type i, index_type j)
        requires engine_traits::is_row_major
    {
        return m_data.m_elems[i*m_data.m_colcap + j];
    }

    inline constexpr reference
    operator ()(index_type i, index_type j)
        requires engine_traits::is_column_major
    {
        return m_data.m_elems[i + j*m_data.m_rowcap];
    }

    inline constexpr const_reference
    operator ()(index_type i, index_type j) const
        requires engine_traits::is_row_major
    {
        return m_data.m_elems[i*m_data.m_colcap + j];
    }

    inline constexpr const_reference
    operator ()(index_type i, index_type j) const
        requires engine_traits::is_column_major
    {
        return m_data.m_elems[i + j*m_data.m_rowcap];
    }

    inline constexpr span_type
    span() noexcept
    {
        return detail::make_matrix_mdspan<span_type, engine_traits>(m_data);
    }

    inline constexpr const_span_type
    span() const noexcept
    {
        return detail::make_matrix_mdspan<const_span_type, engine_traits>(m_data);
    }

    //- Modifiers
    //
    inline constexpr void
    swap(matrix_storage_engine& rhs) noexcept
    {
        if (&rhs != this)
        {
            detail::la_swap(m_data, rhs.m_data);
        }
    }

    constexpr void
    swap_columns(index_type c1, index_type c2) noexcept
    {
        if (c1 != c2)
        {
            for (index_type i = 0;  i < m_data.m_rows;  ++i)
            {
                detail::la_swap((*this)(i, c1), (*this)(i, c2));
            }
        }
    }

    constexpr void
    swap_rows(index_type r1, index_type r2) noexcept
    {
        if (r1 != r2)
        {
            for (index_type j = 0;  j < m_data.m_cols;  ++j)
            {
                detail::la_swap((*this)(r1, j), (*this)(r2, j));
            }
        }
    }

  private:
    storage_type    m_data;

    template<class FN>
    constexpr void
    apply(index_type i0, index_type j0, index_type i1, index_type j1, FN fn)
    {
        if constexpr (engine_traits::is_row_major)
        {
            for (index_type i = i0;  i < i1;  ++i)
            {
                for (index_type j = j0;  j < j1;  ++j)
                {
                    (*this)(i, j) = fn(i, j);
                }
            }
        }
        else
        {
            for (index_type j = j0;  j < j1;  ++j)
            {
                for (index_type i = i0;  i < i1;  ++i)
                {
                    (*this)(i, j) = fn(i, j);
                }
            }
        }
    }


    template<class ET2>
    inline constexpr void
    assign_from(index_type i0, index_type j0, index_type i1, index_type j1, ET2 const& rhs)
    {
        apply(i0, j0, i1, j1, [&rhs](index_type i, index_type j){ return rhs(i, j); });
    }

    inline constexpr void
    move_from(index_type i0, index_type j0, index_type i1, index_type j1, this_type const& rhs)
    {
        apply(i0, j0, i1, j1, [&rhs](index_type i, index_type j){ return std::move(rhs(i, j)); });
    }

    inline constexpr void
    fill(index_type i0, index_type j0, index_type i1, index_type j1, value_type const& t)
    {
        apply(i0, j0, i1, j1, [&t](index_type, index_type){ return t; });
    }

    template<class ET2>
    constexpr void
    assign(ET2 const& rhs)
        requires (!engine_traits::is_column_resizable  &&  !engine_traits::is_row_resizable)
    {
        detail::check_source_engine_size(rhs, m_data.m_rows, m_data.m_cols);
        detail::assign_from_matrix_engine(*this, rhs);
    }

    template<class ET2>
    constexpr void
    assign(ET2 const& rhs)
        requires (engine_traits::is_column_resizable  &&  engine_traits::is_row_resizable)
    {
        index_type  rows = static_cast<index_type>(rhs.rows());
        index_type  cols = static_cast<index_type>(rhs.columns());

        reshape(rows, cols, m_data.m_rowcap, m_data.m_colcap);
        assign_from(0, 0, rows, cols, rhs);
    }

    template<class T2>
    constexpr void
    assign(initializer_list<initializer_list<T2>> rhs)
        requires (!engine_traits::is_column_resizable && !engine_traits::is_row_resizable)
    {
        detail::check_source_init_list(rhs, m_data.m_rows, m_data.m_cols);
        detail::assign_from_matrix_initlist(*this, rhs);
    }

    template<class T2>
    constexpr void
    assign(initializer_list<initializer_list<T2>> rhs)
        requires (engine_traits::is_column_resizable && !engine_traits::is_row_resizable)
    {
        detail::check_source_init_list(rhs);
        static_assert(m_data.m_rows == static_cast<index_type>(rhs.size()));

        this_type     tmp;
        index_type    cols = static_cast<index_type>(rhs.begin()->size());

        tmp.resize_columns(cols);
        detail::assign_from_matrix_initlist(tmp, rhs);
        tmp.swap(*this);
    }

    template<class T2>
    constexpr void
    assign(initializer_list<initializer_list<T2>> rhs)
        requires (!engine_traits::is_column_resizable && engine_traits::is_row_resizable)
    {
        detail::check_source_init_list(rhs);
        static_assert(m_data.m_cols == static_cast<index_type>(rhs.begin()->size()));

        this_type     tmp;
        index_type    rows = static_cast<index_type>(rhs.size());

        tmp.resize_rows(rows);
        detail::assign_from_matrix_initlist(tmp, rhs);
        tmp.swap(*this);
    }

    template<class T2>
    constexpr void
    assign(initializer_list<initializer_list<T2>> rhs)
        requires (engine_traits::is_column_resizable && engine_traits::is_row_resizable)
    {
        detail::check_source_init_list(rhs);

        this_type     tmp;
        index_type    rows = static_cast<index_type>(rhs.size());
        index_type    cols = static_cast<index_type>(rhs.begin()->size());

        tmp.resize(rows, cols);
        detail::assign_from_matrix_initlist(tmp, rhs);
        tmp.swap(*this);
    }
};


template<class T, ptrdiff_t N, class A, class L>
    requires
        detail::valid_mse_extents<extents<N>>  and
        detail::valid_mse_allocator<A, T>      and
        detail::valid_mse_layout<L>
class matrix_storage_engine<T, extents<N>, A, L>
{
    using this_type     = matrix_storage_engine;
    using engine_traits = detail::mse_traits<T, extents<N>, A, L>;
    using storage_type  = detail::mse_data<T, extents<N>, A, L>;

  public:
    using value_type       = T;
    using allocator_type   = A;
    using element_type     = value_type;
    using reference        = element_type&;
    using const_reference  = element_type const&;
    using index_type       = ptrdiff_t;
    using span_type        = typename engine_traits::span_type;
    using const_span_type  = typename engine_traits::const_span_type;

  public:
    inline ~matrix_storage_engine() = default;

    //- Construct / assign.
    //
    constexpr matrix_storage_engine() = default;
    constexpr matrix_storage_engine(matrix_storage_engine&& rhs) noexcept = default;
    constexpr matrix_storage_engine(matrix_storage_engine const& rhs) = default;

    constexpr matrix_storage_engine&    operator =(matrix_storage_engine&& rhs) noexcept = default;
    constexpr matrix_storage_engine&    operator =(matrix_storage_engine const& rhs) = default;

    inline constexpr
    matrix_storage_engine(index_type rows, index_type cols)
        requires engine_traits::is_resizable
    :   m_data()
    {
        reshape(rows, cols, rows, cols);
    }

    template<class ET2> inline constexpr
    matrix_storage_engine(ET2 const& rhs)
    :   m_data()
    {
        assign(rhs);
    }

    template<class T2> inline constexpr
    matrix_storage_engine(initializer_list<initializer_list<T2>> rhs)
    :   m_data()
    {
        assign(rhs);
    }

    template<class ET2>
    inline constexpr matrix_storage_engine&
    operator =(ET2 const& rhs)
    {
        assign(rhs);
    }

    template<class T2>
    inline constexpr matrix_storage_engine&
    operator =(initializer_list<initializer_list<T2>> rhs)
    {
        assign(rhs);
    }

    //- Size and capacity reporting.
    //
    inline constexpr index_type
    size() const noexcept
    {
        return index_tuple_type(m_data.m_rows, m_data.m_cols);
    }

    inline constexpr index_type
    capacity() const noexcept
    {
        return index_tuple_type(m_data.m_rowcap, m_data.m_colcap);
    }

    //- Setting overall size and capacity.
    //
    inline void
    resize(index_type size)
        requires engine_traits::is_resizable
    {
        reshape(size, m_data.m_cap);
    }

    inline void
    reserve(index_type cap)
        requires engine_traits::is_resizable
    {
        reshape(m_data.m_size, cap);
    }

    void
    reshape(index_type newsize, index_type newcap)
        requires engine_traits::is_resizable
    {

        detail::check_size(newsize);
        detail::check_capacity(newcap);

        //- Only reallocate new storage if we have to.
        //
        if (newsize > m_data.m_cap  ||  newcap != m_data.m_cap)
        {
            //- Prepare a temporary engine to receive elements from this one.
            //
            this_type   tmp;

            tmp.m_data.m_elems.resize(newcap);
            tmp.m_data.m_size = newsize;
            tmp.m_data.m_cap  = newcap;

            //- Move the appropriate subset of elements into the temporary engine, then swap.
            //
            index_type  dst_count = min(newsize, m_data.m_size);

            move_elements(tmp, *this, 0, dst_count);
            detail::la_swap(m_data, tmp.m_data);
        }
        else
        {
            if (newsize < m_data.m_size)
            {
                zero(*this, newsize, m_data.m_size);
            }
            m_data.m_size = newsize;
        }
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
        return detail::make_matrix_mdspan<span_type, engine_traits>(m_data);
    }

    inline constexpr const_span_type
    span() const noexcept
    {
        return detail::make_matrix_mdspan<const_span_type, engine_traits>(m_data);
    }

    //- Modifiers
    //
    inline constexpr void
    swap(matrix_storage_engine& rhs) noexcept
    {
        if (&rhs != this)
        {
            detail::la_swap(m_data, rhs.m_data);
        }
    }

  private:
    storage_type    m_data;

    template<class ET2>
    constexpr void  assign(ET2 const& rhs);
    template<class T2>
    constexpr void  assign(initializer_list<initializer_list<T2>> rhs);
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
