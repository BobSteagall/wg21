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

//template<class T, class DX, class AT, class DL>  class matrix_storage_engine {};

struct engine_attribute
{
    //- Tags describing an engine's representational category.
    //
    struct dense_matrix {};
    struct dense_column_matrix {};
    struct dense_row_matrix {};
    struct dense_vector {};
    struct sparse_matrix {};

    //- Tags describing an engine's interface category.
    //
    struct readable {};
    struct writable {};
    struct initable {};
    struct column_resizable {};
    struct row_resizable {};
    struct resizable {};

    //- Tags describing the layout of an engine's elements.
    //
    struct general_layout {};
    struct row_major : public layout_right {};
    struct column_major : public layout_left {};
};

struct view_function
{
    struct negation {};
    struct hermitian {};
    struct transpose {};
    struct subset {};
    struct row {};
    struct column {};

};

namespace detail {
//- Some simple utility concepts, to be re-evaluated later.
//
template<typename T, typename U>
concept same_types = is_same_v<T, U>;

template<typename T, typename U>
concept returns = is_same_v<T, U>;

template<typename T, typename U>
concept convertible_element = is_convertible_v<T, U>;

template<ptrdiff_t N>
concept non_negative = requires { N > 0; };


//- This concept, and its supporting traits type, is used to validate the second template
//  argument of a specialization of matrix_storage_engine, the extents type.  It must be
//  one- or two-dimensional, and the value of each dimension must have certain values.
//
template<class X>
struct is_valid_matrix_extents : public false_type {};

template<ptrdiff_t N>
struct is_valid_matrix_extents<extents<N>>
{
    static constexpr bool   value = (N == -1 || N > 0);
};

template<ptrdiff_t R, ptrdiff_t C>
struct is_valid_matrix_extents<extents<R,C>>
{
    static constexpr bool   value = (R == -1 || R > 0) && (C == -1 || C > 0);
};

template<typename X>
concept valid_extents = is_valid_matrix_extents<X>::value;


//- The following three concepts are used to help validate the third template argument of
//  a specialization of matrix_storage_engine, the allocator type.  It must be void, or it
//  must be possible to instatiate a specialization of allocator_traits with it that meets
//  certain requirements.
//
template<typename T>
concept no_allocator = requires { requires is_same_v<T, void>; };

#if defined(LA_COMPILER_GCC) && ((LA_GCC_VERSION == 9) || (LA_GCC_VERSION == 10))
    //- Neither GCC 9 nor GCC 10 can parse the type requirement 'AT::template rebind_alloc<T>',
    //  so we add a level of indirection and hoist it up into its own alias template.
    //
    template<class AT, class T>
    using at_rebind_alloc_t = typename AT::template rebind_alloc<T>;

    template<typename AT, typename T>
    concept valid_allocator_traits =
        requires
        {
            typename AT::allocator_type;
            typename AT::value_type;
            typename AT::size_type;
            typename AT::pointer;
            typename at_rebind_alloc_t<AT, T>;
            requires is_same_v<T, typename AT::value_type>;
        }
        and
        requires (typename AT::allocator_type a, typename AT::pointer p, typename AT::size_type n)
        {
            { AT::deallocate(a, p, n) };
            { AT::allocate(a, n) } -> returns<typename AT::pointer>;
            { static_cast<T*>(p) };
    #if (LA_GCC_VERSION == 9)
            requires is_same_v<decltype(*p), T&>;
            requires is_same_v<decltype(p[n]), T&>;
    #else
            { *p   } -> returns<T&>;
            { p[n] } -> returns<T&>;
    #endif
        };
#else
    //- Clang 10 and VC++ accept the following without any problems.
    //
    template<typename AT, typename T>
    concept valid_allocator_traits =
        requires
        {
            typename AT::allocator_type;
            typename AT::value_type;
            typename AT::size_type;
            typename AT::pointer;
            typename AT::template rebind_alloc<T>;
            requires is_same_v<T, typename AT::value_type>;
        }
        and
        requires (typename AT::allocator_type a, typename AT::pointer p, typename AT::size_type n)
        {
            { AT::deallocate(a, p, n) };
            { AT::allocate(a, n) } -> returns<typename AT::pointer>;
            { static_cast<T*>(p) };
            { *p   } -> returns<T&>;
            { p[n] } -> returns<T&>;
        };
#endif

template<typename A, typename T>
concept valid_allocator = no_allocator<A> or valid_allocator_traits<allocator_traits<A>, T>;


//- The following three concepts are used to validate the fourth template argument of a
//  specialization of matrix_storage_engine, the layout.  This must be row-major or
//  column-major.
//
template<typename EL>
concept row_major = is_same_v<EL, engine_attribute::row_major>;

template<typename EL>
concept column_major = is_same_v<EL, engine_attribute::column_major>;

template<typename EL>
concept row_or_column_major = row_major<EL> or column_major<EL>;


//- Concepts pertaining to engines.
//
template<class ET>
concept has_engine_aliases =
    requires
    {
        typename ET::engine_category;
        typename ET::engine_interface;
        typename ET::element_layout;
        typename ET::value_type;
        typename ET::element_type;
        typename ET::index_type;
        typename ET::index_tuple_type;
        typename ET::reference;
        typename ET::const_reference;
    };

template<class ET>
concept readable_matrix_engine =
    has_engine_aliases<ET> and
    requires (ET const& eng, typename ET::index_type i)
    {
        { eng.columns() } -> returns<typename ET::index_type>;
        { eng.rows()    } -> returns<typename ET::index_type>;
        { eng(i, i)     } -> returns<typename ET::const_reference>;
    };

}   //- namespace detail


template<class T, class X, class A, class L>    struct mse_traits;
template<class T, class X, class A, class L>    struct mse_data;
template<class T, class X, class A, class L>    struct mse_indexer;


//--------------------------------------
//- Single-element matrix engine (1 x 1)
//
template<class T, class L>
struct mse_traits<T, extents<1, 1>, void, L>
{
};

//---------------------------
//- Row matrix engine (1 x C)
//
template<class T, ptrdiff_t C, class L>
struct mse_traits<T, extents<1, C>, void, L>
{
    using shape_category     = engine_attribute::dense_row_matrix;
    using interface_category = engine_attribute::initable;
    using layout_category    = L;

    using storage_type = mse_data<T, extents<1, C>, void, L>;
    using indexer_type = mse_indexer<T, extents<1, C>, void, L>;
};

template<class T, ptrdiff_t C, class A, class L>
struct mse_traits<T, extents<1, C>, A, L>
{
    using shape_category     = engine_attribute::dense_row_matrix;
    using interface_category = engine_attribute::initable;
    using layout_category    = L;

    using storage_type = mse_data<T, extents<1, C>, A, L>;
    using indexer_type = mse_indexer<T, extents<1, C>, A, L>;
};

template<class T, class A, class L>
struct mse_traits<T, extents<1, dynamic_extent>, A, L>
{
    using shape_category     = engine_attribute::dense_row_matrix;
    using interface_category = engine_attribute::row_resizable;
    using layout_category    = L;

    using storage_type = mse_data<T, extents<1, dynamic_extent>, A, L>;
    using indexer_type = mse_indexer<T, extents<1, dynamic_extent>, A, L>;
};

//------------------------------
//- Column matrix engine (R x 1)
//
template<class T, ptrdiff_t R, class L>
struct mse_traits<T, extents<R, 1>, void, L>
{
    using shape_category     = engine_attribute::dense_column_matrix;
    using interface_category = engine_attribute::initable;
    using layout_category    = L;

    using storage_type = mse_data<T, extents<R, 1>, void, L>;
    using indexer_type = mse_indexer<T, extents<R, 1>, void, L>;
};

template<class T, ptrdiff_t R, class A, class L>
struct mse_traits<T, extents<R, 1>, A, L>
{
    using shape_category     = engine_attribute::dense_column_matrix;
    using interface_category = engine_attribute::initable;
    using layout_category    = L;

    using storage_type = mse_data<T, extents<R, 1>, A, L>;
    using indexer_type = mse_indexer<T, extents<R, 1>, A, L>;
};

template<class T, class A, class L>
struct mse_traits<T, extents<dynamic_extent, 1>, A, L>
{
    using shape_category     = engine_attribute::dense_column_matrix;
    using interface_category = engine_attribute::column_resizable;
    using layout_category    = L;

    using storage_type = mse_data<T, extents<dynamic_extent, 1>, A, L>;
    using indexer_type = mse_indexer<T, extents<dynamic_extent, 1>, A, L>;
};

//------------------------------
//- General matrix engine (R, C)
//
template<class T, ptrdiff_t R, ptrdiff_t C, class L>
struct mse_traits<T, extents<R, C>, void, L>
{
    using shape_category     = engine_attribute::dense_matrix;
    using interface_category = engine_attribute::initable;
    using layout_category    = L;

    using storage_type = mse_data<T, extents<R, C>, void, L>;
    using indexer_type = mse_indexer<T, extents<R, C>, void, L>;
};

template<class T, ptrdiff_t R, ptrdiff_t C, class A, class L>
struct mse_traits<T, extents<R, C>, A, L>
{
    using shape_category     = engine_attribute::dense_matrix;
    using interface_category = engine_attribute::initable;
    using layout_category    = L;

    using storage_type = mse_data<T, extents<R, C>, A, L>;
    using indexer_type = mse_indexer<T, extents<R, C>, A, L>;
};

template<class T, ptrdiff_t C, class A, class L>
struct mse_traits<T, extents<dynamic_extent, C>, A, L>
{
    using shape_category     = engine_attribute::dense_matrix;
    using interface_category = engine_attribute::row_resizable;
    using layout_category    = L;

    using storage_type = mse_data<T, extents<dynamic_extent, C>, A, L>;
    using indexer_type = mse_indexer<T, extents<dynamic_extent, C>, A, L>;
};

template<class T, ptrdiff_t R, class A, class L>
struct mse_traits<T, extents<R, dynamic_extent>, A, L>
{
    using shape_category     = engine_attribute::dense_matrix;
    using interface_category = engine_attribute::column_resizable;
    using layout_category    = L;

    using storage_type = mse_data<T, extents<R, dynamic_extent>, A, L>;
    using indexer_type = mse_indexer<T, extents<R, dynamic_extent>, A, L>;
};

template<class T, class A, class L>
struct mse_traits<T, extents<dynamic_extent, dynamic_extent>, A, L>
{
    using shape_category     = engine_attribute::dense_matrix;
    using interface_category = engine_attribute::resizable;
    using layout_category    = L;

    using storage_type = mse_data<T, extents<dynamic_extent, dynamic_extent>, A, L>;
    using indexer_type = mse_indexer<T, extents<dynamic_extent, dynamic_extent>, A, L>;
};




template<class T, ptrdiff_t R, ptrdiff_t C, class L>
struct mse_data<T, extents<R, C>, void, L>
{
    array<T, R*C>   ma_elems;
};

template<class T, ptrdiff_t R, ptrdiff_t C, class A, class L>
struct mse_data<T, extents<R, C>, A, L>
{};

template<class T, ptrdiff_t R, class A, class L>
struct mse_data<T, extents<R, dynamic_extent>, A, L>
{};

template<class T, ptrdiff_t C, class A, class L>
struct mse_data<T, extents<dynamic_extent, C>, A, L>
{};

template<class T, class A, class L>
struct mse_data<T, extents<dynamic_extent, dynamic_extent>, A, L>
{
    using pointer = typename allocator_traits<A>::pointer;

    pointer         mp_elems;
    index_type      m_rows;
    index_type      m_cols;
    index_type      m_rowcap;
    index_type      m_colcap;
    allocator_type  m_alloc;
};





template<class T, ptrdiff_t R, ptrdiff_t C, class A>
struct mse_indexer<T, extents<R, C>, A, engine_attribute::row_major>
{
    using storage_type = mse_traits<T, extents<R, C>, A, engine_attribute::row_major>;

    static inline constexpr T&     
    offset(storage_type& s, ptrdiff_t i, ptrdiff_t r)
    {
        return s.ma_data[i*C + j];
    }
};

template<class T, ptrdiff_t R, ptrdiff_t C, class A>
struct mse_indexer<T, extents<R, C>, A, engine_attribute::column_major>
{
    using storage_type = mse_traits<T, extents<R, C>, A, engine_attribute::column_major>;

    static inline constexpr T&     
    offset(storage_type& s, ptrdiff_t i, ptrdiff_t r)
    {
        return s.ma_data[i + j*R];
    }
};

template<class T, ptrdiff_t R, class A>
struct mse_indexer<T, extents<R, dynamic_extent>, A, engine_attribute::row_major>
{};

template<class T, ptrdiff_t R, class A>
struct mse_indexer<T, extents<R, dynamic_extent>, A, engine_attribute::column_major>
{};

template<class T, ptrdiff_t C, class A>
struct mse_indexer<T, extents<dynamic_extent, C>, A, engine_attribute::row_major>
{};

template<class T, ptrdiff_t C, class A>
struct mse_indexer<T, extents<dynamic_extent, C>, A, engine_attribute::column_major>
{};

template<class T, class A>
struct mse_indexer<T, extents<dynamic_extent, dynamic_extent>, A, engine_attribute::row_major>
{};

template<class T, class A>
struct mse_indexer<T, extents<dynamic_extent, dynamic_extent>, A, engine_attribute::column_major>
{};




template<class T, class X, class A, class L>
    requires
        detail::valid_extents<X>        and
        detail::valid_allocator<A, T>   and
        detail::row_or_column_major<L>
class matrix_storage_engine
{
};

//- Matrix engines.
//
/*
template<class T, class AT, class EL>
class matrix_storage_engine<T, extents<dynamic_extent, dynamic_extent>, AT, EL>
{
    static_assert(detail::valid_allocator<AT, T>);
    static_assert(detail::row_or_column_major<EL>);

  public:
    using engine_category  = engine_attribute::dense_matrix;
    using engine_interface = engine_attribute::resizable;
    using element_layout   = EL;
    using value_type       = T;
    using allocator_type   = AT;
    using element_type     = value_type;
    using pointer          = typename allocator_traits<AT>::pointer;
    using const_pointer    = typename allocator_traits<AT>::const_pointer;
    using reference        = element_type&;
    using const_reference  = element_type const&;
    using difference_type  = ptrdiff_t;
    using index_type       = ptrdiff_t;
    using index_tuple_type = tuple<index_type, index_type>;
    using span_type        = basic_mdspan<T, detail::dyn_mat_extents, detail::dyn_mat_layout>;
    using const_span_type  = basic_mdspan<T const, detail::dyn_mat_extents, detail::dyn_mat_layout>;

    //- Construct/copy/destroy
    //
    ~matrix_storage_engine() noexcept
    {
        detail::deallocate(m_alloc, mp_elems, m_rowcap*m_colcap);
    };

    matrix_storage_engine()
    :   mp_elems(nullptr)
    ,   m_rows(0)
    ,   m_cols(0)
    ,   m_rowcap(0)
    ,   m_colcap(0)
    ,   m_alloc()
    {}

    matrix_storage_engine(matrix_storage_engine&& rhs) noexcept
    :   matrix_storage_engine()
    {
        this->swap(rhs);
    }

    matrix_storage_engine(matrix_storage_engine const& rhs)
    :   matrix_storage_engine()
    {
        assign(rhs);
    }

    matrix_storage_engine(index_type rows, index_type cols)
        requires
            detail::valid_allocator<AT, T>
    :   matrix_storage_engine()
    {
        alloc_new(rows, cols, rows, cols);
    }

    matrix_storage_engine(index_type rows, index_type cols, index_type rowcap, index_type colcap)
    :   matrix_storage_engine()
    {
        alloc_new(rows, cols, rowcap, colcap);
    }

    template<class ET2>
        requires
            detail::readable_matrix_engine<ET2> and
            detail::convertible_element<typename ET2::element_type, T>
    matrix_storage_engine(ET2 const& rhs)
    :   matrix_storage_engine()
    {
        assign(rhs);
    }

    template<class T2>
        requires
            detail::convertible_element<T2, T>
    matrix_storage_engine(initializer_list<initializer_list<T2>> rhs)
    :   matrix_storage_engine()
    {
        assign(rhs);
    }

    inline matrix_storage_engine&
    operator =(matrix_storage_engine&& rhs) noexcept
    {
        matrix_storage_engine   tmp;
        tmp.swap(rhs);
        tmp.swap(*this);
        return *this;
    }

    inline matrix_storage_engine&
    operator =(matrix_storage_engine const& rhs)
    {
        assign(rhs);
        return *this;
    }

    template<class ET2>
        requires
            detail::readable_matrix_engine<ET2> and
            detail::convertible_element<typename ET2::element_type, T>
    inline matrix_storage_engine&
    operator =(ET2 const& rhs)
    {
        assign(rhs);
        return *this;
    }

    template<class T2>
        requires
            detail::convertible_element<T2, T>
    inline matrix_storage_engine&
    operator =(initializer_list<initializer_list<T2>> rhs)
    {
        assign(rhs);
        return *this;
    }

    //- Capacity
    //
    inline index_type
    columns() const noexcept
    {
        return m_cols;
    }

    inline index_type
    rows() const noexcept
    {
        return m_rows;
    }

    inline index_tuple_type
    size() const noexcept
    {
        return index_tuple_type(m_rows, m_cols);
    }

    inline index_type
    column_capacity() const noexcept
    {
        return m_colcap;
    }

    inline index_type
    row_capacity() const noexcept
    {
        return m_rowcap;
    }

    inline index_tuple_type
    capacity() const noexcept
    {
        return index_tuple_type(m_rowcap, m_colcap);
    }

    //- Reserve and resize
    //
    inline void
    reserve(index_type rowcap, index_type colcap)
    {
        reshape(m_rows, m_cols, rowcap, colcap);
    }

    inline void
    resize(index_type rows, index_type cols)
    {
        reshape(rows, cols, m_rowcap, m_colcap);
    }

    inline void
    resize(index_type rows, index_type cols, index_type rowcap, index_type colcap)
    {
        reshape(rows, cols, rowcap, colcap);
    }

    //- Element access
    //
    inline reference
    operator ()(index_type i, index_type j)
    {
        if constexpr (is_same_v<element_layout, engine_attribute::row_major>)
        {
            return mp_elems[i*m_colcap + j];
        }
        else
        {
            return mp_elems[i + j*m_rowcap];
        }
    }

    inline const_reference
    operator ()(index_type i, index_type j) const
    {
        if constexpr (is_same_v<element_layout, engine_attribute::row_major>)
        {
            return mp_elems[i*m_colcap + j];
        }
        else
        {
            return mp_elems[i + j*m_rowcap];
        }
    }

    inline span_type
    span() noexcept
    {
        return detail::make_dyn_span(static_cast<element_type*>(mp_elems), m_rows, m_cols, m_colcap);
    }

    inline const_span_type
    span() const noexcept
    {
        return detail::make_dyn_span(static_cast<element_type const*>(mp_elems), m_rows, m_cols, m_colcap);
    }

    //- Modifiers
    //
    void
    swap(matrix_storage_engine& rhs) noexcept
    {
        if (&rhs != this)
        {
            detail::la_swap(mp_elems, rhs.mp_elems);
            detail::la_swap(m_rows,   rhs.m_rows);
            detail::la_swap(m_cols,   rhs.m_cols);
            detail::la_swap(m_rowcap, rhs.m_rowcap);
            detail::la_swap(m_colcap, rhs.m_colcap);
            detail::la_swap(m_alloc,  rhs.m_alloc);
        }
    }

    void
    swap_columns(index_type c1, index_type c2) noexcept
    {
        if (c1 != c2)
        {
            for (index_type i = 0;  i < m_rows;  ++i)
            {
                detail::la_swap((*this)(i, c1), (*this)(i, c2));
            }
        }
    }

    void
    swap_rows(index_type r1, index_type r2) noexcept
    {
        if (r1 != r2)
        {
            for (index_type j = 0;  j < m_cols;  ++j)
            {
                detail::la_swap((*this)(r1, j), (*this)(r2, j));
            }
        }
    }

  private:
    pointer         mp_elems;
    index_type      m_rows;
    index_type      m_cols;
    index_type      m_rowcap;
    index_type      m_colcap;
    allocator_type  m_alloc;

  private:
    static inline void
    check_capacities(index_type rowcap, index_type colcap)
    {
        if (rowcap < 0  || colcap < 0)
        {
            throw runtime_error("invalid capacity");
        }
    }

    static inline void
    check_sizes(index_type rows, index_type cols)
    {
        if (rows < 1  || cols < 1)
        {
            throw runtime_error("invalid size");
        }
    }

    void
    alloc_new(index_type rows, index_type cols, index_type rowcap, index_type colcap)
    {
        check_sizes(rows, cols);
        check_capacities(rowcap, colcap);
        rowcap = max(rows, rowcap);
        colcap = max(cols, colcap);

        mp_elems = detail::allocate(m_alloc, (size_t)(rowcap*colcap));
        m_rows   = rows;
        m_cols   = cols;
        m_rowcap = rowcap;
        m_colcap = colcap;
    }

    void
    assign(matrix_storage_engine const& rhs)
    {
        if (&rhs == this) return;

        size_t      old_n = static_cast<size_t>(m_rowcap*m_colcap);
        size_t      new_n = static_cast<size_t>(rhs.m_rowcap*rhs.m_colcap);
        pointer     p_tmp = detail::allocate(m_alloc, new_n, rhs.mp_elems);

        detail::deallocate(m_alloc, mp_elems, old_n);
        mp_elems = p_tmp;
        m_rows   = rhs.m_rows;
        m_cols   = rhs.m_cols;
        m_rowcap = rhs.m_rowcap;
        m_colcap = rhs.m_colcap;
    }

    template<class ET2>
    void
    assign(ET2 const& rhs)
    {
        typename ET2::index_type    rows = static_cast<index_type>(rhs.rows());
        typename ET2::index_type    cols = static_cast<index_type>(rhs.columns());
        matrix_storage_engine       tmp(rows, cols);

        detail::assign_from_matrix_engine(tmp, rhs);
        tmp.swap(*this);
    }

    template<class T2>
    void
    assign(initializer_list<initializer_list<T2>> rhs)
    {
        detail::check_source_init_list(rhs);

        index_type const        rows = static_cast<index_type>(rhs.size());
        index_type const        cols = static_cast<index_type>(rhs.begin()->size());
        matrix_storage_engine   tmp(rows, cols);

        detail::assign_from_matrix_initlist(tmp, rhs);
        tmp.swap(*this);
    }

    void
    reshape(index_type rows, index_type cols, index_type rowcap, index_type colcap)
    {
        if (rows > m_rowcap  ||  cols > m_colcap   ||  rowcap > m_rowcap  ||  colcap > m_colcap)
        {
            matrix_storage_engine   tmp(rows, cols, rowcap, colcap);
            index_type const        dst_rows = min(rows, m_rows);
            index_type const        dst_cols = min(cols, m_cols);

            for (index_type i = 0;  i < dst_rows;  ++i)
            {
                for (index_type j = 0;  j < dst_cols;  ++j)
                {
                    tmp(i, j) = (*this)(i, j);
                }
            }
            tmp.swap(*this);
        }
        else
        {
            check_sizes(rows, cols);
            check_capacities(rowcap, colcap);
            m_rows = rows;
            m_cols = cols;
        }
    }
};

template<class T, ptrdiff_t C, class AT, class EL>
    requires
        detail::non_negative<C>         and
        detail::valid_allocator<AT, T>  and
        detail::row_or_column_major<EL>
class matrix_storage_engine<T, extents<dynamic_extent, C>, AT, EL>
{};

template<class T, ptrdiff_t R, class AT, class EL>
    requires
        detail::non_negative<R>         and
        detail::valid_allocator<AT, T>  and
        detail::row_or_column_major<EL>
class matrix_storage_engine<T, extents<R, dynamic_extent>, AT, EL>
{};

template<class T, ptrdiff_t R, ptrdiff_t C, class AT, class EL>
    requires
        detail::non_negative<R>         and
        detail::non_negative<C>         and
        detail::valid_allocator<AT, T>  and
        detail::row_or_column_major<EL>
class matrix_storage_engine<T, extents<R, C>, AT, EL>
{
    static_assert(detail::valid_allocator<AT, T>);

  public:
    //- Types
    //
    using engine_category  = engine_attribute::dense_matrix;
    using engine_interface = engine_attribute::initable;
    using element_layout   = EL;
    using value_type       = T;
    using allocator_type   = AT;
    using element_type     = value_type;
    using pointer          = typename allocator_traits<AT>::pointer;
    using const_pointer    = typename allocator_traits<AT>::const_pointer;
    using reference        = element_type&;
    using const_reference  = element_type const&;
    using difference_type  = ptrdiff_t;
    using index_type       = ptrdiff_t;
    using index_tuple_type = tuple<index_type, index_type>;
    using span_type        = mdspan<element_type, R, C>;
    using const_span_type  = mdspan<element_type const, R, C>;

    //- Construct/copy/destroy
    //
    ~matrix_storage_engine() noexcept = default;
    constexpr matrix_storage_engine(matrix_storage_engine&&) noexcept = delete;
    constexpr matrix_storage_engine&    operator =(matrix_storage_engine&&) noexcept = delete;

    constexpr matrix_storage_engine(matrix_storage_engine const&) = default;

    constexpr matrix_storage_engine&    operator =(matrix_storage_engine const&) = default;

    constexpr matrix_storage_engine()
    :   ma_elems()
    {}

    template<class ET2>
        requires
            detail::readable_matrix_engine<ET2> and
            detail::convertible_element<typename ET2::element_type, T>
    constexpr matrix_storage_engine(ET2 const& rhs)
    :   ma_elems()
    {
        assign(rhs);
    }

    template<class T2>
        requires
            detail::convertible_element<T2, T>
    constexpr matrix_storage_engine(initializer_list<initializer_list<T2>> rhs)
    :   ma_elems()
    {
        assign(rhs);
    }

    template<class ET2>
        requires
            detail::readable_matrix_engine<ET2> and
            detail::convertible_element<typename ET2::element_type, T>
    constexpr matrix_storage_engine&
    operator =(ET2 const& rhs)
    {
        assign(rhs);
        return *this;
    }

    template<class T2>
        requires
            detail::convertible_element<T2, T>
    constexpr matrix_storage_engine&
    operator =(initializer_list<initializer_list<T2>> rhs)
    {
        assign(rhs);
        return *this;
    }

    //- Size and capacity
    //
    static inline constexpr index_type
    columns() noexcept
    {
        return C;
    }

    static inline constexpr index_type
    rows() noexcept
    {
        return R;
    }

    static inline constexpr index_tuple_type
    size() noexcept
    {
        return index_tuple_type{R, C};
    }

    static inline constexpr index_type
    column_capacity() noexcept
    {
        return C;
    }

    static inline constexpr index_type
    row_capacity() noexcept
    {
        return R;
    }

    static inline constexpr index_tuple_type
    capacity() noexcept
    {
        return index_tuple_type{R, C};
    }

    //- Element access
    //
    inline constexpr reference
    operator ()(index_type i, index_type j)
    {
        if constexpr (is_same_v<element_layout, engine_attribute::row_major>)
        {
            return ma_elems[i*C + j];
        }
        else
        {
            return ma_elems[i + j*R];
        }
    }

    inline constexpr const_reference
    operator ()(index_type i, index_type j) const
    {
        if constexpr (is_same_v<element_layout, engine_attribute::row_major>)
        {
            return ma_elems[i*C + j];
        }
        else
        {
            return ma_elems[i + j*R];
        }
    }

    inline constexpr span_type
    span() noexcept
    {
        return span_type(ma_elems.data());
    }

    inline constexpr const_span_type
    span() const noexcept
    {
        return const_span_type(ma_elems.data());
    }

    //- Modifiers
    //
    constexpr void
    swap(matrix_storage_engine& rhs) noexcept
    {
        if (&rhs != this)
        {
            for (index_type i = 0;  i < R*C;  ++i)
            {
                detail::la_swap(ma_elems[i], rhs.ma_elems[i]);
            }
        }
    }

    constexpr void
    swap_columns(index_type j1, index_type j2) noexcept
    {
        if (j1 != j2)
        {
            for (index_type i = 0;  i < R;  ++i)
            {
                detail::la_swap((*this)(i, j1), (*this)(i, j1));
            }
        }
    }

    constexpr void
    swap_rows(index_type i1, index_type i2) noexcept
    {
        if (i1 != i2)
        {
            for (index_type j = 0;  j < C;  ++j)
            {
                detail::la_swap((*this)(i1, j) (*this)(i2, j));
            }
        }
    }

  private:
    array<T, R*C>   ma_elems;
    pointer         mp_elems;


    template<class ET2>
    constexpr void  assign(ET2 const& rhs)
    {
        detail::check_source_engine_size(rhs, R, C);
        detail::assign_from_matrix_engine(*this, rhs);
    }

    template<class T2>
    constexpr void  assign(initializer_list<initializer_list<T2>> rhs)
    {
        detail::check_source_init_list(rhs, R, C);
        detail::assign_from_matrix_initlist(*this, rhs);
    }
};

template<class T, ptrdiff_t R, ptrdiff_t C, class EL>
    requires
        detail::non_negative<R>         and
        detail::non_negative<C>         and
        detail::row_or_column_major<EL>
class matrix_storage_engine<T, extents<R, C>, void, EL>
{
  public:
    //- Types
    //
    using engine_category  = engine_attribute::dense_matrix;
    using engine_interface = engine_attribute::initable;
    using element_layout   = EL;
    using value_type       = T;
    using element_type     = value_type;
    using pointer          = element_type*;
    using const_pointer    = element_type const*;
    using reference        = element_type&;
    using const_reference  = element_type const&;
    using difference_type  = ptrdiff_t;
    using index_type       = ptrdiff_t;
    using index_tuple_type = tuple<index_type, index_type>;
    using span_type        = mdspan<element_type, R, C>;
    using const_span_type  = mdspan<element_type const, R, C>;

    //- Construct/copy/destroy
    //
    ~matrix_storage_engine() noexcept = default;

    constexpr matrix_storage_engine(matrix_storage_engine&&) noexcept = default;
    constexpr matrix_storage_engine(matrix_storage_engine const&) = default;

    constexpr matrix_storage_engine&    operator =(matrix_storage_engine&&) noexcept = default;
    constexpr matrix_storage_engine&    operator =(matrix_storage_engine const&) = default;

    constexpr matrix_storage_engine()
    :   ma_elems()
    {}

    template<class ET2>
        requires
            detail::readable_matrix_engine<ET2> and
            detail::convertible_element<typename ET2::element_type, T>
    constexpr matrix_storage_engine(ET2 const& rhs)
    :   ma_elems()
    {
        assign(rhs);
    }

    template<class T2>
        requires
            detail::convertible_element<T2, T>
    constexpr matrix_storage_engine(initializer_list<initializer_list<T2>> rhs)
    :   ma_elems()
    {
        assign(rhs);
    }

    template<class ET2>
        requires
            detail::readable_matrix_engine<ET2> and
            detail::convertible_element<typename ET2::element_type, T>
    constexpr matrix_storage_engine&
    operator =(ET2 const& rhs)
    {
        assign(rhs);
        return *this;
    }

    template<class T2>
        requires
            detail::convertible_element<T2, T>
    constexpr matrix_storage_engine&
    operator =(initializer_list<initializer_list<T2>> rhs)
    {
        assign(rhs);
        return *this;
    }

    //- Size and capacity
    //
    static inline constexpr index_type
    columns() noexcept
    {
        return C;
    }

    static inline constexpr index_type
    rows() noexcept
    {
        return R;
    }

    static inline constexpr index_tuple_type
    size() noexcept
    {
        return index_tuple_type{R, C};
    }

    static inline constexpr index_type
    column_capacity() noexcept
    {
        return C;
    }

    static inline constexpr index_type
    row_capacity() noexcept
    {
        return R;
    }

    static inline constexpr index_tuple_type
    capacity() noexcept
    {
        return index_tuple_type{R, C};
    }

    //- Element access
    //
    inline constexpr reference
    operator ()(index_type i, index_type j)
    {
        if constexpr (is_same_v<element_layout, engine_attribute::row_major>)
        {
            return ma_elems[i*C + j];
        }
        else
        {
            return ma_elems[i + j*R];
        }
    }

    inline constexpr const_reference
    operator ()(index_type i, index_type j) const
    {
        if constexpr (is_same_v<element_layout, engine_attribute::row_major>)
        {
            return ma_elems[i*C + j];
        }
        else
        {
            return ma_elems[i + j*R];
        }
    }

    inline constexpr span_type
    span() noexcept
    {
        return span_type(ma_elems.data());
    }

    inline constexpr const_span_type
    span() const noexcept
    {
        return const_span_type(ma_elems.data());
    }

    //- Modifiers
    //
    constexpr void
    swap(matrix_storage_engine& rhs) noexcept
    {
        if (&rhs != this)
        {
            for (index_type i = 0;  i < R*C;  ++i)
            {
                detail::la_swap(ma_elems[i], rhs.ma_elems[i]);
            }
        }
    }

    constexpr void
    swap_columns(index_type j1, index_type j2) noexcept
    {
        if (j1 != j2)
        {
            for (index_type i = 0;  i < R;  ++i)
            {
                detail::la_swap((*this)(i, j1), (*this)(i, j1));
            }
        }
    }

    constexpr void
    swap_rows(index_type i1, index_type i2) noexcept
    {
        if (i1 != i2)
        {
            for (index_type j = 0;  j < C;  ++j)
            {
                detail::la_swap((*this)(i1, j) (*this)(i2, j));
            }
        }
    }

  private:
    array<T, R*C>   ma_elems;

    template<class ET2>
    constexpr void  assign(ET2 const& rhs)
    {
        detail::check_source_engine_size(rhs, R, C);
        detail::assign_from_matrix_engine(*this, rhs);
    }

    template<class T2>
    constexpr void  assign(initializer_list<initializer_list<T2>> rhs)
    {
        detail::check_source_init_list(rhs, R, C);
        detail::assign_from_matrix_initlist(*this, rhs);
    }
};


template<class T, class EL>
    requires
        detail::row_or_column_major<EL>
class matrix_storage_engine<T, extents<1, 1>, void, EL>
{};

//- Row vector engines
//
template<class T, class AT>
    requires
        detail::valid_allocator<AT, T>
class matrix_storage_engine<T, extents<1, dynamic_extent>, AT, engine_attribute::row_major>
{};

template<class T, ptrdiff_t C, class AT>
    requires
        detail::non_negative<C>         and
        detail::valid_allocator<AT, T>
class matrix_storage_engine<T, extents<1, C>, AT, engine_attribute::row_major>
{};

template<class T, ptrdiff_t C>
    requires
        detail::non_negative<C>
class matrix_storage_engine<T, extents<1, C>, void, engine_attribute::row_major>
{};

//- Column vector engines
//
template<class T, class AT>
    requires
        detail::valid_allocator<AT, T>
class matrix_storage_engine<T, extents<dynamic_extent, 1>, AT, engine_attribute::column_major>
{};

template<class T, ptrdiff_t R, class AT>
    requires
        detail::non_negative<R>         and
        detail::valid_allocator<AT, T>
class matrix_storage_engine<T, extents<R, 1>, AT, engine_attribute::column_major>
{};

template<class T, ptrdiff_t R>
    requires
        detail::non_negative<R>
class matrix_storage_engine<T, extents<R, 1>, void, engine_attribute::column_major>
{};
*/

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