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


template<typename EC>
concept resizable_columns =
    same_types<EC, engine_attribute::column_resizable> or
    same_types<EC, engine_attribute::resizable>;

template<typename EC>
concept resizable_rows =
    same_types<EC, engine_attribute::row_resizable> or
    same_types<EC, engine_attribute::resizable>;

template<typename EC>
concept resizable_rows_and_columns = same_types<EC, engine_attribute::resizable>;

template<typename EC>
concept resizable_columns_and_rows = same_types<EC, engine_attribute::resizable>;


template<typename EC>
concept column_or_row_matrix =
    same_types<EC, engine_attribute::dense_column_matrix> or
    same_types<EC, engine_attribute::dense_row_matrix>;


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

inline void
check_sizes(ptrdiff_t rows, ptrdiff_t cols)
{
    if (rows < 1  || cols < 1)
    {
        throw runtime_error("invalid size");
    }
}

inline void
check_capacities(ptrdiff_t rowcap, ptrdiff_t colcap)
{
    if (rowcap < 0  || colcap < 0)
    {
        throw runtime_error("invalid capacity");
    }
}

}   //- namespace detail


//-
template<class T, class X, class A, class L>    struct mse_traits;


//---------------------------
//- Vector engine (1 x N)
//
template<class T, ptrdiff_t N, class L>
struct mse_traits<T, extents<N>, void, L>
{
    using engine_category  = engine_attribute::dense_vector;
    using engine_interface = engine_attribute::initable;
    using element_layout   = engine_attribute::general_layout;
};

template<class T, ptrdiff_t N, class A, class L>
struct mse_traits<T, extents<N>, A, L>
{
    using engine_category  = engine_attribute::dense_vector;
    using engine_interface = engine_attribute::initable;
    using element_layout   = engine_attribute::general_layout;
};

template<class T, class A, class L>
struct mse_traits<T, extents<dynamic_extent>, A, L>
{
    using engine_category  = engine_attribute::dense_vector;
    using engine_interface = engine_attribute::resizable;
    using element_layout   = engine_attribute::general_layout;
};

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
    using engine_category  = engine_attribute::dense_row_matrix;
    using engine_interface = engine_attribute::initable;
    using element_layout   = L;
};

template<class T, ptrdiff_t C, class A, class L>
struct mse_traits<T, extents<1, C>, A, L>
{
    using engine_category  = engine_attribute::dense_row_matrix;
    using engine_interface = engine_attribute::initable;
    using element_layout   = L;
};

template<class T, class A, class L>
struct mse_traits<T, extents<1, dynamic_extent>, A, L>
{
    using engine_category  = engine_attribute::dense_row_matrix;
    using engine_interface = engine_attribute::row_resizable;
    using element_layout   = L;
};

//------------------------------
//- Column matrix engine (R x 1)
//
template<class T, ptrdiff_t R, class L>
struct mse_traits<T, extents<R, 1>, void, L>
{
    using engine_category  = engine_attribute::dense_column_matrix;
    using engine_interface = engine_attribute::initable;
    using element_layout   = L;
};

template<class T, ptrdiff_t R, class A, class L>
struct mse_traits<T, extents<R, 1>, A, L>
{
    using engine_category  = engine_attribute::dense_column_matrix;
    using engine_interface = engine_attribute::initable;
    using element_layout   = L;
};

template<class T, class A, class L>
struct mse_traits<T, extents<dynamic_extent, 1>, A, L>
{
    using engine_category  = engine_attribute::dense_column_matrix;
    using engine_interface = engine_attribute::column_resizable;
    using element_layout   = L;
};

//------------------------------
//- General matrix engine (R, C)
//
template<class T, ptrdiff_t R, ptrdiff_t C, class L>
struct mse_traits<T, extents<R, C>, void, L>
{
    using engine_category  = engine_attribute::dense_matrix;
    using engine_interface = engine_attribute::initable;
    using element_layout   = L;
};

template<class T, ptrdiff_t R, ptrdiff_t C, class A, class L>
struct mse_traits<T, extents<R, C>, A, L>
{
    using engine_category  = engine_attribute::dense_matrix;
    using engine_interface = engine_attribute::initable;
    using element_layout   = L;
};

template<class T, ptrdiff_t C, class A, class L>
struct mse_traits<T, extents<dynamic_extent, C>, A, L>
{
    using engine_category  = engine_attribute::dense_matrix;
    using engine_interface = engine_attribute::row_resizable;
    using element_layout   = L;
};

template<class T, ptrdiff_t R, class A, class L>
struct mse_traits<T, extents<R, dynamic_extent>, A, L>
{
    using engine_category  = engine_attribute::dense_matrix;
    using engine_interface = engine_attribute::column_resizable;
    using element_layout   = L;
};

template<class T, class A, class L>
struct mse_traits<T, extents<dynamic_extent, dynamic_extent>, A, L>
{
    using engine_category  = engine_attribute::dense_matrix;
    using engine_interface = engine_attribute::resizable;
    using element_layout   = L;
};


//==================================================================================================
//  Partial specializations of class template mse_data contain and manage elements on behalf
//  of matrix_storage_engine.
//
//  Note that, in this implementation, all dynamically-allocated memory is default-constructed.
//  This means that elements lying in (currently) unused capacity are also initialized.
//
//  This state of affairs might not be present in the final version.
//==================================================================================================
//
template<class T, class X, class A, class L>    struct mse_data;


//---------------------------------------------------------------------
//- Fixed elements.  Elements contained as member data in a std::array.
//
template<class T, ptrdiff_t N, class L>
struct mse_data<T, extents<N>, void, L>
{
    using array_type = std::array<T, N>;

    static constexpr ptrdiff_t  m_size = N;
    static constexpr ptrdiff_t  m_cap  = N;

    array_type  m_elems;

    ~mse_data() = default;
    constexpr mse_data() = default;
    constexpr mse_data(mse_data&&) noexcept = default;
    constexpr mse_data(mse_data const&) = default;
    constexpr mse_data&     operator =(mse_data&&) noexcept = default;
    constexpr mse_data&     operator =(mse_data const&) = default;
};


//----------------------------------------------------------------------
//- Fixed elements.  Elements contained as member data in a std::vector.
//
template<class T, ptrdiff_t N, class A, class L>
struct mse_data<T, extents<N>, A, L>
{
    using array_type = std::vector<T, A>;

    static constexpr ptrdiff_t  m_size = N;
    static constexpr ptrdiff_t  m_cap  = N;

    array_type  m_elems;

    ~mse_data() = default;
    inline constexpr mse_data()
    :   m_elems(N)
    {}
    constexpr mse_data(mse_data&&) noexcept = default;
    constexpr mse_data(mse_data const&) = default;
    constexpr mse_data&     operator =(mse_data&&) noexcept = default;
    constexpr mse_data&     operator =(mse_data const&) = default;
};


//------------------------------------------------------------------------
//- Dynamic elements.  Elements contained as member data in a std::vector.
//
template<class T, class A, class L>
struct mse_data<T, extents<dynamic_extent>, A, L>
{
    using array_type = std::vector<T, A>;

    array_type  m_elems;
    ptrdiff_t   m_size;
    ptrdiff_t   m_cap;

    ~mse_data() = default;
    inline constexpr mse_data()
    :   m_elems()
    ,   m_size(0)
    ,   m_cap(0)
    {}
    constexpr mse_data(mse_data&&) noexcept = default;
    constexpr mse_data(mse_data const&) = default;
    constexpr mse_data&     operator =(mse_data&&) noexcept = default;
    constexpr mse_data&     operator =(mse_data const&) = default;
};


//---------------------------------------------------------------------------------
//- Fixed rows / fixed columns.  Elements contained as member data in a std::array.
//
template<class T, ptrdiff_t R, ptrdiff_t C, class L>
struct mse_data<T, extents<R, C>, void, L>
{
    using array_type = std::array<T, R*C>;

    static constexpr ptrdiff_t  m_rows   = R;
    static constexpr ptrdiff_t  m_cols   = C;
    static constexpr ptrdiff_t  m_rowcap = R;
    static constexpr ptrdiff_t  m_colcap = C;

    array_type  m_elems;

    ~mse_data() = default;
    constexpr mse_data() = default;
    constexpr mse_data(mse_data&&) noexcept = default;
    constexpr mse_data(mse_data const&) = default;
    constexpr mse_data&     operator =(mse_data&&) noexcept = default;
    constexpr mse_data&     operator =(mse_data const&) = default;
};


//-------------------------------------------------------------------------------
//- Fixed rows / fixed columns.  Elements dynamically allocated in a std::vector.
//
template<class T, ptrdiff_t R, ptrdiff_t C, class A, class L>
struct mse_data<T, extents<R, C>, A, L>
{
    using array_type = std::vector<T, A>;

    static constexpr ptrdiff_t  m_rows   = R;
    static constexpr ptrdiff_t  m_cols   = C;
    static constexpr ptrdiff_t  m_rowcap = R;
    static constexpr ptrdiff_t  m_colcap = C;

    array_type  m_elems;

    //- Construct/copy/destroy.
    //
    ~mse_data() = default;

    inline constexpr mse_data()
    :   m_elems(R*C)
    {}
    constexpr mse_data(mse_data&&) noexcept = default;
    constexpr mse_data(mse_data const&) = default;
    constexpr mse_data&     operator =(mse_data&&) noexcept = default;
    constexpr mse_data&     operator =(mse_data const&) = default;
};


//---------------------------------------------------------------------------------
//- Fixed rows / dynamic columns.  Elements dynamically allocated in a std::vector.
//
template<class T, ptrdiff_t R, class A, class L>
struct mse_data<T, extents<R, dynamic_extent>, A, L>
{
    using array_type = std::vector<T, A>;

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
    :   m_elems()
    ,   m_cols(0)
    ,   m_colcap(0)
    {}
    constexpr mse_data(mse_data&&) noexcept = default;
    constexpr mse_data(mse_data const&) = default;
    constexpr mse_data&     operator =(mse_data&&) noexcept = default;
    constexpr mse_data&     operator =(mse_data const&) = default;
};

//---------------------------------------------------------------------------------
//- Dynamic rows / fixed columns.  Elements dynamically allocated in a std::vector.
//
template<class T, ptrdiff_t C, class A, class L>
struct mse_data<T, extents<dynamic_extent, C>, A, L>
{
    using array_type = std::vector<T, A>;

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
    :   m_elems()
    ,   m_rows(0)
    ,   m_rowcap(0)
    {}
    constexpr mse_data(mse_data&&) noexcept = default;
    constexpr mse_data(mse_data const&) = default;
    constexpr mse_data&     operator =(mse_data&&) noexcept = default;
    constexpr mse_data&     operator =(mse_data const&) = default;
};

//-----------------------------------------------------------------------------------
//- Dynamic rows / dynamic columns.  Elements dynamically allocated in a std::vector.
//
template<class T, class A, class L>
struct mse_data<T, extents<dynamic_extent, dynamic_extent>, A, L>
{
    using array_type = std::vector<T, A>;

    array_type  m_elems;
    ptrdiff_t   m_rows;
    ptrdiff_t   m_cols;
    ptrdiff_t   m_rowcap;
    ptrdiff_t   m_colcap;

    //- Construct/copy/destroy.
    //
    inline ~mse_data() = default;

    inline constexpr
    mse_data()
    :   m_elems()
    ,   m_rows(0)
    ,   m_cols(0)
    ,   m_rowcap(0)
    ,   m_colcap(0)
    {}
    constexpr mse_data(mse_data&&) noexcept = default;
    constexpr mse_data(mse_data const&) = default;
    constexpr mse_data&     operator =(mse_data&&) noexcept = default;
    constexpr mse_data&     operator =(mse_data const&) = default;
};





template<class T, class X, class A, class L>
    requires
        detail::valid_extents<X>        and
        detail::valid_allocator<A, T>   and
        detail::row_or_column_major<L>
class matrix_storage_engine
{
    using this_type     = matrix_storage_engine;
    using engine_traits = mse_traits<T, X, A, L>;
    using storage_type  = mse_data<T, X, A, L>;

  public:
    using engine_category  = typename engine_traits::engine_category;
    using engine_interface = typename engine_traits::engine_interface;
    using element_layout   = typename engine_traits::element_layout;
    using value_type       = T;
    using allocator_type   = A;
    using element_type     = value_type;
    using reference        = element_type&;
    using const_reference  = element_type const&;
    using difference_type  = ptrdiff_t;
    using index_type       = ptrdiff_t;
    using index_tuple_type = tuple<index_type, index_type>;
//    using span_type          = basic_mdspan<T, detail::dyn_mat_extents, detail::dyn_mat_layout>;
//    using const_span_type    = basic_mdspan<T const, detail::dyn_mat_extents, detail::dyn_mat_layout>;

  public:
    inline ~matrix_storage_engine() = default;

    //- Construction.
    //
    inline constexpr
    matrix_storage_engine()
    :   m_data()
    {}

    inline constexpr
    matrix_storage_engine(matrix_storage_engine&& rhs) noexcept
    :   m_data(std::move(rhs))
    {}

    inline constexpr
    matrix_storage_engine(matrix_storage_engine const& rhs)
    :   m_data(rhs)
    {}

    inline constexpr
    matrix_storage_engine(index_type rows, index_type cols, index_type rowcap, index_type colcap)
        requires detail::resizable_columns_and_rows<engine_interface>
    :   m_data()
    {
        reshape(rows, cols, rowcap, colcap);
    }

    //- Assignment.
    //
    inline constexpr matrix_storage_engine&
    operator =(matrix_storage_engine&& rhs) noexcept
    {
        m_data = std::move(rhs.m_data);
        return *this;
    }

    inline constexpr matrix_storage_engine&
    operator =(matrix_storage_engine const& rhs)
    {
        m_data = rhs.m_data;
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
        requires detail::resizable_columns<engine_interface>
    {
        reshape_columns(cols, m_data.m_colcap);
    }

    inline void
    resize_columns(index_type cols, index_type colcap)
        requires detail::resizable_columns<engine_interface>
    {
        reshape_columns(cols, colcap);
    }

    inline void
    reserve_columns(index_type colcap)
        requires detail::resizable_columns<engine_interface>
    {
        reshape_columns(m_data.m_cols, colcap);
    }

    //- Setting row size and capacity.
    //
    inline void
    resize_rows(index_type rows)
        requires detail::resizable_rows<engine_interface>
    {
        reshape_rows(rows, m_data.m_rowcap);
    }

    inline void
    resize_rows(index_type rows, index_type rowcap)
        requires detail::resizable_rows<engine_interface>
    {
        reshape_rows(rows, rowcap);
    }

    inline void
    reserve_rows(index_type rowcap)
        requires detail::resizable_rows<engine_interface>
    {
        reshape_rows(m_data.m_rows, rowcap);
    }

    //- Setting overall size and capacity.
    //
    inline void
    resize(index_type rows, index_type cols)
        requires detail::resizable_columns_and_rows<engine_interface>
    {
        reshape(rows, cols, m_data.m_rowcap, m_data.m_colcap);
    }

    inline void
    resize(index_type rows, index_type cols, index_type rowcap, index_type colcap)
        requires detail::resizable_columns_and_rows<engine_interface>
    {
        reshape(rows, cols, rowcap, colcap);
    }

    inline void
    reserve(index_type rowcap, index_type colcap)
        requires detail::resizable_columns_and_rows<engine_interface>
    {
        reshape(m_data.m_rows, m_data.m_cols, rowcap, colcap);
    }

    //- Element access
    //
    inline constexpr reference
    operator ()(index_type i)
        requires detail::column_or_row_matrix<engine_category>
    {
        return m_data.m_elems[i];
    }

    inline constexpr const_reference
    operator ()(index_type i) const
        requires detail::column_or_row_matrix<engine_category>
    {
        return m_data.m_elems[i];
    }

    inline constexpr reference
    operator ()(index_type i, index_type j)
    {
        if constexpr (is_same_v<element_layout, engine_attribute::row_major>)
        {
            return m_data.m_elems[i*m_data.m_colcap + j];
        }
        else
        {
            return m_data.m_elems[i + j*m_data.m_rowcap];
        }
    }

    inline constexpr const_reference
    operator ()(index_type i, index_type j) const
    {
        if constexpr (detail::row_major<element_layout>)
        {
            return m_data.m_elems[i*m_data.m_colcap + j];
        }
        else
        {
            return m_data.m_elems[i + j*m_data.m_rowcap];
        }
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

    inline constexpr void
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

    inline constexpr void
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

    static constexpr void
    move_elements(this_type& dst, this_type const& src, index_type i_lo, index_type j_lo, index_type i_hi, index_type j_hi)
    {
        if constexpr (detail::row_major<element_layout>)
        {
            for (index_type i = i_lo;  i < i_hi;  ++i)
            {
                for (index_type j = j_lo;  j < j_hi;  ++j)
                {
                    dst(i, j) = std::move(src(i, j));
                }
            }
        }
        else    //- column_major
        {
            for (index_type j = j_lo;  j < j_hi;  ++j)
            {
                for (index_type i = i_lo;  i < i_hi;  ++i)
                {
                    dst(i, j) = std::move(src(i, j));
                }
            }
        }
    }

    static constexpr void
    zero(this_type& dst, index_type i_lo, index_type j_lo, index_type i_hi, index_type j_hi)
    {
        value_type  t{};

        if constexpr (detail::row_major<element_layout>)
        {
            for (index_type i = i_lo;  i < i_hi;  ++i)
            {
                for (index_type j = j_lo;  j < j_hi;  ++j)
                {
                    dst(i, j) = t;
                }
            }
        }
        else    //- column_major
        {
            for (index_type j = j_lo;  j < j_hi;  ++j)
            {
                for (index_type i = i_lo;  i < i_hi;  ++i)
                {
                    dst(i, j) = t;
                }
            }
        }
    }

    void
    reshape_columns(index_type cols, index_type colcap)
        requires detail::resizable_columns<engine_interface>
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

            move_elements(tmp, *this, 0, 0, dst_rows, dst_cols);
            detail::la_swap(m_data, tmp.m_data);
        }
        else
        {
            if (cols < m_data.m_cols)
            {
                zero(*this, 0, cols, m_data.m_rows, min(cols, m_data.m_cols));
            }
            m_data.m_cols = cols;
        }
    }

    void
    reshape_rows(index_type rows, index_type rowcap)
        requires detail::resizable_rows<engine_interface>
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

            move_elements(tmp, *this, 0, 0, dst_rows, dst_cols);
            detail::la_swap(m_data, tmp.m_data);
        }
        else
        {
            if (rows < m_data.m_rows)
            {
                zero(*this, rows, 0, m_data.m_rows, m_data.m_cols);
            }
            m_data.m_rows = rows;
        }
    }

    void
    reshape(index_type rows, index_type cols, index_type rowcap, index_type colcap)
        requires detail::resizable_columns_and_rows<engine_interface>
    {
        detail::check_sizes(rows, cols);
        detail::check_capacities(rowcap, colcap);

        //- Only reallocate new storage if we have to.
        //
        if (rows   >  m_data.m_rowcap  ||  cols   >  m_data.m_colcap  ||
            rowcap != m_data.m_rowcap  ||  colcap != m_data.m_colcap)
        {
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

            move_elements(tmp, *this, 0, 0, dst_rows, dst_cols);
            detail::la_swap(m_data, tmp.m_data);
        }
        else
        {
            if (rows < m_data.m_rows)
            {
                zero(*this, rows, 0, m_data.m_rows, m_data.m_cols);
            }
            if (cols < m_data.m_cols)
            {
                zero(*this, 0, cols, min(rows, m_data.m_rows), m_data.m_cols);
            }
            m_data.m_rows = rows;
            m_data.m_cols = cols;
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
