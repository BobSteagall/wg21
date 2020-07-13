//==================================================================================================
//  File:       matrix_storage_engine_data.hpp
//
//  Summary:    This header defines several partial specializations of mse_data<T, X, A, L>.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_MATRIX_STORAGE_ENGINE_DATA_HPP_DEFINED
#define LINEAR_ALGEBRA_MATRIX_STORAGE_ENGINE_DATA_HPP_DEFINED

namespace STD_LA {
namespace detail {
//--------------------------------------------------------------------------------------------------
//  Class Template:     mse_data<T, X, A, L>
//
//  Type that contains and manages elements on behalf of matrix_storage_engine<T,X,A,L>.
//
//  Partial specializations of this class template are tailored to specific corresponding partial
//  specializations of matrix_storage_engine.  They provide the special member functions, as well
//  as other manipulation of internal state, that make sense for each set of template arguments.
//
//  The implementation assumes that all dynamically-allocated memory is default-constructed,
//  with the consequence that elements lying in unused capacity are also constructed.  This
//  assumption makes implementation easy, but may be absent in the final version.
//--------------------------------------------------------------------------------------------------
//
//  Partial Specialization:     mse_data<T, extents<N>, void, L>
//
//  Manages elements representing a fixed-size unoriented vector of N elements.  Its elements are
//  implemented as member data in a std::array.
//--------------------------------------------------------------------------------------------------
//
template<class T, ptrdiff_t N, class L>
struct mse_data<T, extents<N>, void, L>
{
    using this_type       = mse_data;
    using array_type      = std::array<T, N>;
    using support         = mse_support<this_type>;
    using span_type       = mdspan<T, N>;
    using const_span_type = mdspan<T const, N>;

    static constexpr bool   is_resizable = false;

    static constexpr ptrdiff_t  m_size = N;
    static constexpr ptrdiff_t  m_cap  = N;

    array_type  m_elems;

    ~mse_data() = default;
    constexpr mse_data()
    :   m_elems()
    {
        if constexpr (!is_class_v<T>)
        {
            m_elems.fill(T{});
        }
    }
    constexpr mse_data(mse_data&&) noexcept = default;
    constexpr mse_data(mse_data const&) = default;
    constexpr mse_data&     operator =(mse_data&&) noexcept = default;
    constexpr mse_data&     operator =(mse_data const&) = default;

    inline constexpr span_type
    span() noexcept
    {
        return span_type(m_elems.data());
    }

    inline constexpr const_span_type
    span() const noexcept
    {
        return const_span_type(m_elems.data());
    }

    template<class U>
    constexpr void
    assign(initializer_list<U> src)
    {
        ptrdiff_t   size = static_cast<ptrdiff_t>(src.size());
        support::verify_size(size, m_size);
        support::copy_list(*this, src);
    }

    template<class ET>
    constexpr void
    assign(ET const& eng)
    {
        support::verify_size(static_cast<ptrdiff_t>(eng.size()), m_size);
        support::copy_engine(*this, eng);
    }

    inline constexpr void
    swap(mse_data& rhs) noexcept
    {
        support::swap(*this, rhs);
    }
};


//--------------------------------------------------------------------------------------------------
//  Specialization:     mse_data<T, extents<N>, A, L>
//
//  Manages elements representing a fixed-size unoriented vector of N elements.  Its elements are
//  implemented as member data in a std::vector.
//--------------------------------------------------------------------------------------------------
//
template<class T, ptrdiff_t N, class A, class L>
struct mse_data<T, extents<N>, A, L>
{
    using this_type       = mse_data;
    using array_type      = std::vector<T, A>;
    using support         = mse_support<this_type>;
    using span_type       = mdspan<T, N>;
    using const_span_type = mdspan<T const, N>;

    static constexpr bool   is_resizable = false;

    static constexpr ptrdiff_t  m_size = N;
    static constexpr ptrdiff_t  m_cap  = N;

    array_type  m_elems;

    ~mse_data() = default;
    inline mse_data()
    :   m_elems(N)
    {}
    mse_data(mse_data&&) noexcept = default;
    mse_data(mse_data const&) = default;
    mse_data&   operator =(mse_data&&) noexcept = default;
    mse_data&   operator =(mse_data const&) = default;

    inline span_type
    span() noexcept
    {
        return span_type(m_elems.data());
    }

    inline const_span_type
    span() const noexcept
    {
        return const_span_type(m_elems.data());
    }

    template<class U>
    constexpr void
    assign(initializer_list<U> src)
    {
        ptrdiff_t   size = static_cast<ptrdiff_t>(src.size());
        support::verify_size(size, m_size);
        support::copy_list(*this, src);
    }

    template<class ET>
    constexpr void
    assign(ET const& eng)
    {
        support::verify_size(static_cast<ptrdiff_t>(eng.size()), m_size);
        support::copy_engine(*this, eng);
    }

    inline void
    swap(mse_data& rhs) noexcept
    {
        support::swap(*this, rhs);
    }
};


//--------------------------------------------------------------------------------------------------
//  Partial Specialization:     mse_data<T, extents<dynamic_extent>, A, L>
//
//  Manages elements representing a dynamically-resizable unoriented vector.  Its elements are
//  implemented as member data in a std::vector.
//--------------------------------------------------------------------------------------------------
//
template<class T, class A, class L>
struct mse_data<T, extents<dynamic_extent>, A, L>
{
    using this_type       = mse_data;
    using array_type      = std::vector<T, A>;
    using support         = mse_support<this_type>;
    using span_type       = mdspan<T, dynamic_extent>;
    using const_span_type = mdspan<T const, dynamic_extent>;

    static constexpr bool   is_resizable = true;

    array_type  m_elems;
    ptrdiff_t   m_size;
    ptrdiff_t   m_cap;

    ~mse_data() = default;
    inline constexpr mse_data()
    :   m_elems(), m_size(0), m_cap(0)
    {}
    constexpr mse_data(mse_data&&) noexcept = default;
    constexpr mse_data(mse_data const&) = default;
    constexpr mse_data&     operator =(mse_data&&) noexcept = default;
    constexpr mse_data&     operator =(mse_data const&) = default;

    inline constexpr span_type
    span() noexcept
    {
        return span_type(m_elems.data(), m_size);
    }

    inline constexpr const_span_type
    span() const noexcept
    {
        return const_span_type(m_elems.data(), m_size);
    }

    template<class U>
    constexpr void
    assign(initializer_list<U> src)
    {
        reshape(static_cast<ptrdiff_t>(src.size()), m_cap);
        support::copy_list(*this, src);
    }

    template<class ET>
    constexpr void
    assign(ET const& eng)
    {
        reshape(static_cast<ptrdiff_t>(eng.size()), m_cap);
        support::copy_engine(*this, eng);
    }

    void
    reshape(ptrdiff_t newsize, ptrdiff_t newcap)
    {
        if (newsize == m_size) return;

        support::verify_size(newsize);
        support::verify_capacity(newcap);

        //- Only reallocate new storage if we have to.
        //
        if (newsize > m_cap  ||  newcap != m_cap)
        {
            //- Normalize requested new capacity.
            //
            newcap = max(newsize, newcap);

            //- Prepare a temporary engine to receive elements from this one.
            //
            this_type   tmp;
            tmp.m_elems.resize(newcap);
            tmp.m_size = newsize;
            tmp.m_cap  = newcap;

            //- Move the appropriate subset of elements into the temporary engine, then swap.
            //
            ptrdiff_t   dst_size = min(newsize, m_size);
            support::move_elements(tmp, *this, dst_size);
            support::swap(*this, tmp);
        }
        else
        {
            if (newsize < m_size)
            {
                support::fill(*this, newsize, m_size, T{});
            }
            m_size = newsize;
        }
    }

    inline void
    swap(mse_data& rhs) noexcept
    {
        support::swap(*this, rhs);
    }
};


//--------------------------------------------------------------------------------------------------
//  Partial Specialization:     mse_data<T, extents<R, C>, void, L>
//
//  Manages elements representing a fixed-size matrix of R rows and C columns.  Its elements are
//  implemented as member data in a std::array.
//--------------------------------------------------------------------------------------------------
//
template<class T, ptrdiff_t R, ptrdiff_t C, class L>
struct mse_data<T, extents<R, C>, void, L>
{
    using this_type       = mse_data;
    using array_type      = std::array<T, R*C>;
    using support         = mse_support<this_type>;
    using this_layout     = conditional_t<is_same_v<L, row_major>, layout_right, layout_left>;
    using span_type       = basic_mdspan<T, extents<R, C>, this_layout>;
    using const_span_type = basic_mdspan<T const, extents<R, C>, this_layout>;

    static constexpr bool   is_column_matrix    = (C == 1);
    static constexpr bool   is_row_matrix       = (R == 1);
    static constexpr bool   is_linear_matrix    = (is_column_matrix || is_row_matrix);
    static constexpr bool   is_column_resizable = false;
    static constexpr bool   is_row_resizable    = false;
    static constexpr bool   is_resizable        = false;
    static constexpr bool   is_column_major     = is_same_v<L, column_major>;
    static constexpr bool   is_row_major        = is_same_v<L, row_major>;

    static constexpr ptrdiff_t  m_rows   = R;
    static constexpr ptrdiff_t  m_cols   = C;
    static constexpr ptrdiff_t  m_rowcap = R;
    static constexpr ptrdiff_t  m_colcap = C;

    array_type  m_elems;

    ~mse_data() = default;

    constexpr mse_data()
    :   m_elems()
    {
        if constexpr (!is_class_v<T>)
        {
            m_elems.fill(T{});
        }
    }
    constexpr mse_data(mse_data&&) noexcept = default;
    constexpr mse_data(mse_data const&) = default;
    constexpr mse_data&     operator =(mse_data&&) noexcept = default;
    constexpr mse_data&     operator =(mse_data const&) = default;

    inline span_type
    span() noexcept
    {
        return span_type(m_elems.data());
    }

    inline const_span_type
    span() const noexcept
    {
        return const_span_type(m_elems.data());
    }

    template<class U>
    constexpr void
    assign(initializer_list<U> src)
        requires this_type::is_column_matrix
    {
        ptrdiff_t   rows = static_cast<ptrdiff_t>(src.size());
        support::verify_size(rows, m_rows);
        support::copy_list(*this, src);
    }

    template<class U>
    constexpr void
    assign(initializer_list<U> src)
        requires this_type::is_row_matrix
    {
        ptrdiff_t   cols = static_cast<ptrdiff_t>(src.size());
        support::verify_size(cols, m_cols);
        support::copy_list(*this, src);
    }

    template<class U>
    constexpr void
    assign(initializer_list<initializer_list<U>> src)
    {
        auto    [rows, cols] = support::verify_list(src);
        support::verify_size(cols, m_cols);
        support::verify_size(rows, m_rows);
        support::copy_list(*this, src);
    }

    template<class ET>
    constexpr void
    assign(ET const& eng)
    {
        support::verify_size(static_cast<ptrdiff_t>(eng.rows()), m_rows);
        support::verify_size(static_cast<ptrdiff_t>(eng.columns()), m_cols);
        support::copy_engine(*this, eng);
    }

    inline void
    swap(mse_data& rhs) noexcept
    {
        support::swap(*this, rhs);
    }
};


//--------------------------------------------------------------------------------------------------
//  Partial Specialization:     mse_data<T, extents<R, C>, A, L>
//
//  Manages elements representing a fixed-size matrix of R rows and C columns.  Its elements are
//  implemented as member data in a std::vector.
//--------------------------------------------------------------------------------------------------
//
template<class T, ptrdiff_t R, ptrdiff_t C, class A, class L>
struct mse_data<T, extents<R, C>, A, L>
{
    using this_type       = mse_data;
    using array_type      = std::vector<T, A>;
    using support         = mse_support<this_type>;
    using this_layout     = conditional_t<is_same_v<L, row_major>, layout_right, layout_left>;
    using span_type       = basic_mdspan<T, extents<R, C>, this_layout>;
    using const_span_type = basic_mdspan<T const, extents<R, C>, this_layout>;

    static constexpr bool   is_column_matrix    = (C == 1);
    static constexpr bool   is_row_matrix       = (R == 1);
    static constexpr bool   is_linear_matrix    = (is_column_matrix || is_row_matrix);
    static constexpr bool   is_column_resizable = false;
    static constexpr bool   is_row_resizable    = false;
    static constexpr bool   is_resizable        = false;
    static constexpr bool   is_column_major     = is_same_v<L, column_major>;
    static constexpr bool   is_row_major        = is_same_v<L, row_major>;

    static constexpr ptrdiff_t  m_rows   = R;
    static constexpr ptrdiff_t  m_cols   = C;
    static constexpr ptrdiff_t  m_rowcap = R;
    static constexpr ptrdiff_t  m_colcap = C;

    array_type  m_elems;

    //- Construct/copy/destroy.
    //
    ~mse_data() = default;

    inline constexpr mse_data()
    :   m_elems(static_cast<typename array_type::size_type>(R*C))
    {}
    constexpr mse_data(mse_data&&) noexcept = default;
    constexpr mse_data(mse_data const&) = default;
    constexpr mse_data&     operator =(mse_data&&) noexcept = default;
    constexpr mse_data&     operator =(mse_data const&) = default;

    inline span_type
    span() noexcept
    {
        return span_type(m_elems.data());
    }

    inline const_span_type
    span() const noexcept
    {
        return const_span_type(m_elems.data());
    }

    template<class U>
    constexpr void
    assign(initializer_list<U> src)
        requires this_type::is_column_matrix
    {
        ptrdiff_t   rows = static_cast<ptrdiff_t>(src.size());
        support::verify_size(rows, m_rows);
        support::copy_list(*this, src);
    }

    template<class U>
    constexpr void
    assign(initializer_list<U> src)
        requires this_type::is_row_matrix
    {
        ptrdiff_t   cols = static_cast<ptrdiff_t>(src.size());
        support::verify_size(cols, m_cols);
        support::copy_list(*this, src);
    }

    template<class U>
    void
    assign(initializer_list<initializer_list<U>> src)
    {
        auto    [rows, cols] = support::verify_list(src);
        support::verify_size(cols, m_cols);
        support::verify_size(rows, m_rows);
        support::copy_list(*this, src);
    }

    template<class ET>
    void
    assign(ET const& eng)
    {
        support::verify_size(static_cast<ptrdiff_t>(eng.rows()), m_rows);
        support::verify_size(static_cast<ptrdiff_t>(eng.columns()), m_cols);
        support::copy_engine(*this, eng);
    }

    inline void
    swap(mse_data& rhs) noexcept
    {
        support::swap(*this, rhs);
    }
};


//--------------------------------------------------------------------------------------------------
//  Partial Specialization:     mse_data<T, extents<R, dynamic_extent>, A, L>
//
//  Manages elements representing a matrix having a fixed number of R rows and a dynamically-
//  resizable number of columns.  Its elements are implemented as member data in a std::vector.
//--------------------------------------------------------------------------------------------------
//
template<class T, ptrdiff_t R, class A, class L>
struct mse_data<T, extents<R, dynamic_extent>, A, L>
{
    using this_type       = mse_data;
    using array_type      = std::vector<T, A>;
    using support         = mse_support<this_type>;
    using span_type       = typename support::dyn_span_type;
    using const_span_type = typename support::const_dyn_span_type;

    static constexpr bool   is_column_matrix    = false;
    static constexpr bool   is_row_matrix       = (R == 1);
    static constexpr bool   is_linear_matrix    = is_row_matrix;
    static constexpr bool   is_column_resizable = true;
    static constexpr bool   is_row_resizable    = false;
    static constexpr bool   is_resizable        = false;
    static constexpr bool   is_column_major     = is_same_v<L, column_major>;
    static constexpr bool   is_row_major        = is_same_v<L, row_major>;

    static constexpr ptrdiff_t  m_rows   = R;
    static constexpr ptrdiff_t  m_rowcap = R;

    array_type  m_elems;
    ptrdiff_t   m_cols;
    ptrdiff_t   m_colcap;

    //- Construct/copy/destroy.
    //
    ~mse_data() = default;

    inline constexpr
    mse_data()
    :   m_elems(), m_cols(0), m_colcap(0)
    {}
    constexpr mse_data(mse_data&&) noexcept = default;
    constexpr mse_data(mse_data const&) = default;
    constexpr mse_data&     operator =(mse_data&&) noexcept = default;
    constexpr mse_data&     operator =(mse_data const&) = default;

    inline span_type
    span() noexcept
    {
        return support::make_dyn_mdspan(*this);
    }

    inline const_span_type
    span() const noexcept
    {
        return support::make_const_dyn_mdspan(*this);
    }

    template<class U>
    constexpr void
    assign(initializer_list<U> src)
        requires this_type::is_row_matrix
    {
        ptrdiff_t   cols = static_cast<ptrdiff_t>(src.size());
        reshape_columns(cols, m_colcap);
        support::copy_list(*this, src);
    }

    template<class U>
    void
    assign(initializer_list<initializer_list<U>> src)
    {
        auto    [rows, cols] = support::verify_list(src);
        support::verify_size(rows, m_rows);
        reshape_columns(cols, m_colcap);
        support::copy_list(*this, src);
    }

    template<class ET>
    void
    assign(ET const& eng)
    {
        support::verify_size(static_cast<ptrdiff_t>(eng.rows()), m_rows);
        reshape_columns(static_cast<ptrdiff_t>(eng.columns()), m_colcap);
        support::copy_engine(*this, eng);
    }

    void
    reshape_columns(ptrdiff_t cols, ptrdiff_t colcap)
    {
        support::verify_size(cols);
        support::verify_capacity(colcap);

        //- Only reallocate new storage if we have to.
        //
        if (cols > m_colcap  ||  colcap != m_colcap)
        {
            //- Normalize requested new capacity.
            //
            colcap = max(cols, colcap);

            //- Prepare a temporary object to receive elements from this one.
            //
            this_type   tmp;
            tmp.m_elems.resize(m_rowcap*colcap);
            tmp.m_cols   = cols;
            tmp.m_colcap = colcap;

            //- Move the appropriate subset of elements into the temporary engine and swap.
            //
            ptrdiff_t   dst_cols = min(cols, m_cols);
            support::move_elements(tmp, *this, m_rows, dst_cols);
            support::swap(*this, tmp);
        }
        else
        {
            if (cols < m_cols)
            {
                support::fill_columns(*this, cols, m_cols, T{});
            }
            m_cols = cols;
        }
    }

    inline void
    swap(mse_data& rhs) noexcept
    {
        support::swap(*this, rhs);
    }
};


//--------------------------------------------------------------------------------------------------
//  Partial Specialization:     mse_data<T, extents<dynamic_extent, C>, A, L>
//
//  Manages elements representing a matrix having a dynamically-resizable number of rows and a
//  fixed number of C columns.  Its elements are implemented as member data in a std::vector.
//--------------------------------------------------------------------------------------------------
//
template<class T, ptrdiff_t C, class A, class L>
struct mse_data<T, extents<dynamic_extent, C>, A, L>
{
    using this_type       = mse_data;
    using array_type      = std::vector<T, A>;
    using support         = mse_support<this_type>;
    using span_type       = typename support::dyn_span_type;
    using const_span_type = typename support::const_dyn_span_type;

    static constexpr bool   is_column_matrix    = (C == 1);
    static constexpr bool   is_row_matrix       = false;
    static constexpr bool   is_linear_matrix    = is_column_matrix;
    static constexpr bool   is_column_resizable = false;
    static constexpr bool   is_row_resizable    = true;
    static constexpr bool   is_resizable        = false;
    static constexpr bool   is_column_major     = is_same_v<L, column_major>;
    static constexpr bool   is_row_major        = is_same_v<L, row_major>;

    static constexpr ptrdiff_t  m_cols   = C;
    static constexpr ptrdiff_t  m_colcap = C;

    array_type  m_elems;
    ptrdiff_t   m_rows;
    ptrdiff_t   m_rowcap;

    //- Construct/copy/destroy.
    //
    ~mse_data() = default;

    inline constexpr
    mse_data()
    :   m_elems(), m_rows(0), m_rowcap(0)
    {}
    constexpr mse_data(mse_data&&) noexcept = default;
    constexpr mse_data(mse_data const&) = default;
    constexpr mse_data&     operator =(mse_data&&) noexcept = default;
    constexpr mse_data&     operator =(mse_data const&) = default;

    inline span_type
    span() noexcept
    {
        return support::make_dyn_mdspan(*this);
    }

    inline const_span_type
    span() const noexcept
    {
        return support::make_const_dyn_mdspan(*this);
    }

    template<class U>
    constexpr void
    assign(initializer_list<U> src)
        requires this_type::is_column_matrix
    {
        ptrdiff_t   rows = static_cast<ptrdiff_t>(src.size());
        reshape_rows(rows, m_rowcap);
        support::copy_list(*this, src);
    }

    template<class U>
    void
    assign(initializer_list<initializer_list<U>> src)
    {
        auto    [rows, cols] = support::verify_list(src);
        support::verify_size(cols, m_cols);
        reshape_rows(rows, m_rowcap);
        support::copy_list(*this, src);
    }

    template<class ET>
    void
    assign(ET const& eng)
    {
        support::verify_size(static_cast<ptrdiff_t>(eng.columns()), m_cols);
        reshape_rows(static_cast<ptrdiff_t>(eng.rows()), m_rowcap);
        support::copy_engine(*this, eng);
    }

    void
    reshape_rows(ptrdiff_t rows, ptrdiff_t rowcap)
    {
        support::verify_size(rows);
        support::verify_capacity(rowcap);

        //- Only reallocate new storage if we have to.
        //
        if (rows > m_rowcap  ||  rowcap != m_rowcap)
        {
            //- Normalize requested new capacity.
            //
            rowcap = max(rows, rowcap);

            //- Prepare a temporary object to receive elements from this one.
            //
            this_type   tmp;
            tmp.m_elems.resize(rowcap*m_colcap);
            tmp.m_rows   = rows;
            tmp.m_rowcap = rowcap;

            //- Move the appropriate subset of elements into the temporary engine and swap.
            //
            ptrdiff_t   dst_rows = min(rows, m_rows);
            support::move_elements(tmp, *this, dst_rows, m_cols);
            support::swap(*this, tmp);
        }
        else
        {
            if (rows < m_rows)
            {
                support::fill_rows(*this, rows, m_rows, T{});
            }
            m_rows = rows;
        }
    }

    inline void
    swap(mse_data& rhs) noexcept
    {
        support::swap(*this, rhs);
    }
};


//--------------------------------------------------------------------------------------------------
//  Partial Specialization:     mse_data<T, extents<dynamic_extent, dynamic_extent>, A, L>
//
//  Manages elements representing a matrix having a dynamically-resizable number of rows and a
//  dynamically-resizable number of columns.  Its elements are implemented as member data in a
//  std::vector.
//--------------------------------------------------------------------------------------------------
//
template<class T, class A, class L>
struct mse_data<T, extents<dynamic_extent, dynamic_extent>, A, L>
{
    using this_type       = mse_data;
    using array_type      = std::vector<T, A>;
    using support         = mse_support<this_type>;
    using span_type       = typename support::dyn_span_type;
    using const_span_type = typename support::const_dyn_span_type;

    static constexpr bool   is_linear_matrix    = false;
    static constexpr bool   is_column_matrix    = false;
    static constexpr bool   is_row_matrix       = false;
    static constexpr bool   is_column_resizable = true;
    static constexpr bool   is_row_resizable    = true;
    static constexpr bool   is_resizable        = true;
    static constexpr bool   is_column_major     = is_same_v<L, column_major>;
    static constexpr bool   is_row_major        = is_same_v<L, row_major>;

    array_type  m_elems;
    ptrdiff_t   m_rows;
    ptrdiff_t   m_cols;
    ptrdiff_t   m_rowcap;
    ptrdiff_t   m_colcap;

    //- Construct/copy/destroy.
    //
    ~mse_data() = default;

    inline
    mse_data()
    :   m_elems(), m_rows(0), m_cols(0), m_rowcap(0), m_colcap(0)
    {}
    mse_data(mse_data&&) noexcept = default;
    mse_data(mse_data const&) = default;
    mse_data&   operator =(mse_data&&) noexcept = default;
    mse_data&   operator =(mse_data const&) = default;

    inline span_type
    span() noexcept
    {
        return support::make_dyn_mdspan(*this);
    }

    inline const_span_type
    span() const noexcept
    {
        return support::make_const_dyn_mdspan(*this);
    }

    template<class U>
    void
    assign(initializer_list<initializer_list<U>> src)
    {
        auto    [rows, cols] = support::verify_list(src);
        reshape(rows, cols, m_rowcap, m_colcap);
        support::copy_list(*this, src);
    }

    template<class ET>
    void
    assign(ET const& eng)
    {
        reshape(eng.rows(), eng.columns(), m_rowcap, m_colcap);
        support::copy_engine(*this, eng);
    }

    inline void
    reshape_columns(ptrdiff_t cols, ptrdiff_t colcap)
    {
        reshape(m_rows, cols, m_rowcap, colcap);
    }

    inline void
    reshape_rows(ptrdiff_t rows, ptrdiff_t rowcap)
    {
        reshape(rows, m_cols, rowcap, m_colcap);
    }

    void
    reshape(ptrdiff_t rows, ptrdiff_t cols, ptrdiff_t rowcap, ptrdiff_t colcap)
    {
        support::verify_size(rows);
        support::verify_size(cols);
        support::verify_capacity(rowcap);
        support::verify_capacity(colcap);

        //- Only reallocate new storage if we have to.
        //
        if (rows > m_rowcap  ||  cols > m_colcap  ||  rowcap != m_rowcap  ||  colcap != m_colcap)
        {
            //- Normalize requested new capacities.
            //
            rowcap = max(rows, rowcap);
            colcap = max(cols, colcap);

            //- Prepare a temporary object to receive elements from this one.
            //
            this_type   tmp;
            tmp.m_elems.resize(rowcap*colcap);
            tmp.m_rows   = rows;
            tmp.m_cols   = cols;
            tmp.m_rowcap = rowcap;
            tmp.m_colcap = colcap;

            //- Move the appropriate subset of elements into the temporary engine and swap.
            //
            ptrdiff_t   dst_rows = min(rows, this->m_rows);
            ptrdiff_t   dst_cols = min(cols, this->m_cols);
            support::move_elements(tmp, *this, dst_rows, dst_cols);
            support::swap(*this, tmp);
        }
        else
        {
            if (rows < m_rows)
            {
                support::fill_rows(*this, rows, m_rows, T{});
            }
            if (cols < m_cols)
            {
                support::fill_columns(*this, cols, m_cols, T{});
            }
            m_rows = rows;
            m_cols = cols;
        }
    }

    inline void
    swap(mse_data& rhs) noexcept
    {
        support::swap(*this, rhs);
    }
};

}       //- detail namespace
}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_MATRIX_STORAGE_ENGINE_DATA_HPP_DEFINED
