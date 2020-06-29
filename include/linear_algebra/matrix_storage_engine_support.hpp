//==================================================================================================
//  File:       matrix_storage_engine_support.hpp
//
//  Summary:    This header defines an owning, dense, rectangular engine that owns and manages
//              the elements of a matrix.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_MATRIX_STORAGE_ENGINE_SUPPORT_HPP_DEFINED
#define LINEAR_ALGEBRA_MATRIX_STORAGE_ENGINE_SUPPORT_HPP_DEFINED

namespace STD_LA {
namespace detail {
//--------------------------------------------------------------------------------------------------
//  Concept:    valid_mse_extents<X>
//
//  This private concept is used to validate the second template argument of a specialization of
//  matrix_storage_engine, the engine's extents type.  It must be a one- or two-dimensional extents
//  type, and each dimension's template argument may have only a certain set of values.
//--------------------------------------------------------------------------------------------------
//
template<class X>
struct is_valid_storage_extents : public false_type {};

//- Valid for 1-D / vector
//
template<ptrdiff_t N>
struct is_valid_storage_extents<extents<N>>
{
    static constexpr bool   value = (N == dynamic_extent || N > 0);
};

//- Valid for 2-D / matrix
//
template<ptrdiff_t R, ptrdiff_t C>
struct is_valid_storage_extents<extents<R,C>>
{
    static constexpr bool   value = (R == dynamic_extent || R > 0) && (C == dynamic_extent || C > 0);
};

//- Concept definition.
//
template<typename X>
concept valid_mse_extents = is_valid_storage_extents<X>::value;


//--------------------------------------------------------------------------------------------------
//  Concept:    valid_mse_allocator<A, T>
//
//  This private concept is used to validate the third template argument of matrix_storage_engine,
//  the allocator type.  It must be void, or it must be possible to instantiate a specialization
//  of allocator_traits<A> that meets certain requirements relative to element type T, such as
//  allocator_traits<A>::value_type is the same type as T.
//--------------------------------------------------------------------------------------------------
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

//- Concept definition.
//
template<typename A, typename T>
concept valid_mse_allocator = no_allocator<A> or valid_allocator_traits<allocator_traits<A>, T>;


//--------------------------------------------------------------------------------------------------
//  Concept:    valid_mse_matrix_layout<L>, valid_mse_vector_layout<L>
//
//  This private concept is used to validate the fourth template argument of a specialization of
//  matrix_storage_engine, the engine's layout type.  It must be either row_major or column_major.
//--------------------------------------------------------------------------------------------------
//
template<typename EL>
concept valid_mse_layout = (is_same_v<EL, row_major> || is_same_v<EL, column_major>);

template<typename EL>
concept valid_mse_matrix_layout = (is_same_v<EL, row_major> || is_same_v<EL, column_major>);

template<typename EL>
concept valid_mse_vector_layout = is_same_v<EL, unoriented>;


//--------------------------------------------------------------------------------------------------
//  MDSPAN Support:     fixed_matrix_mdspan_t<T, R, C, L>
//                      dynamic_matrix_mdspan_t<T>
//                      make_matrix_mdspan()
//
//                      fixed_vector_mdspan_t<T, N>
//                      dynamic_vector_mdspan_t<T>
//                      make_vector_mdspan()
//
//  These alias templates and function templates, are used by mse_traits<T, X, A, L> and
//  matrix_storage_engine<T, X, A, L> to declare appropriate mdspan types and return correct
//  mdspan instances, respectively.
//--------------------------------------------------------------------------------------------------
//
//- Simple traits type and alias template to convert row_major/column_major tags into
//  corresponding mdspan layout types.
//
template<class L>
struct mdspan_fixed_layout;

template<>
struct mdspan_fixed_layout<row_major>
{
    using type = layout_right;
};

template<>
struct mdspan_fixed_layout<column_major>
{
    using type = layout_left;
};


//- Alias templates for fixed/dynamic matrix/vector mdspan types, used by mse_data<T,X,A,L>.
//
template<class T, ptrdiff_t R, ptrdiff_t C, class L>
using fixed_matrix_mdspan_t = basic_mdspan<T, extents<R,C>, typename mdspan_fixed_layout<L>::type>;

template<class T>
using dynamic_matrix_mdspan_t = basic_mdspan<T,
                                             extents<dynamic_extent, dynamic_extent>,
                                             layout_stride<dynamic_extent, dynamic_extent>>;

template<class T, ptrdiff_t N>
using fixed_vector_mdspan_t = mdspan<T, N>;

template<class T>
using dynamic_vector_mdspan_t = mdspan<T, dynamic_extent>;


//- Functions for creating matrix/vector mdspan objects, used by matrix_storage_engine<T,X,A,L>.
//
template<class SpanType, class EngineTraits, class MseData> inline constexpr
SpanType
make_matrix_mdspan(MseData& rep)
{
    using dyn_extents = extents<dynamic_extent, dynamic_extent>;
    using dyn_layout  = layout_stride<dynamic_extent, dynamic_extent>;
    using dyn_mapping = typename dyn_layout::template mapping<dyn_extents>;
    using dyn_strides = array<typename dyn_extents::index_type, 2>;

    if constexpr (EngineTraits::is_column_resizable  ||  EngineTraits::is_row_resizable)
    {
        if constexpr (EngineTraits::is_row_major)
        {
            dyn_extents     extents(rep.m_rows, rep.m_cols);
            dyn_strides     strides{rep.m_rowcap, 1};
            dyn_mapping     mapping(extents, strides);

            return SpanType(rep.m_elems.data(), mapping);
        }
        else if constexpr (EngineTraits::is_column_major)
        {
            dyn_extents     extents(rep.m_rows, rep.m_cols);
            dyn_strides     strides{1, rep.m_colcap};
            dyn_mapping     mapping(extents, strides);

            return SpanType(rep.m_elems.data(), mapping);
        }
    }
    else
    {
        return SpanType(rep.m_elems.data());
    }
}


template<class SpanType, class EngineTraits, class MseData> inline constexpr
SpanType
make_vector_mdspan(MseData& rep)
{
    if constexpr (EngineTraits::is_resizable)
    {
        return SpanType(rep.m_elems.data(), rep.m_size);
    }
    else
    {
        return SpanType(rep.m_elems.data());
    }
}


//--------------------------------------------------------------------------------------------------
//  Traits Type:    mse_traitx<T, X, A, L>
//
//  This supporting, private traits type is used to ...
//--------------------------------------------------------------------------------------------------
//
template<class T, class X, class A, class L>
struct mse_traits
{
    struct null_type;

    static constexpr bool   is_column_matrix    = false;
    static constexpr bool   is_row_matrix       = false;
    static constexpr bool   is_matrix           = false;
    static constexpr bool   is_vector           = false;

    static constexpr bool   is_readable         = false;
    static constexpr bool   is_writable         = false;
    static constexpr bool   is_initable         = false;
    static constexpr bool   is_column_resizable = false;
    static constexpr bool   is_row_resizable    = false;
    static constexpr bool   is_resizable        = false;

    static constexpr bool   is_column_major     = false;
    static constexpr bool   is_row_major        = false;

    using span_type       = null_type;
    using const_span_type = null_type;
};


//-----------------------
//- Vector engine (1 x N)
//
template<class T, ptrdiff_t N, class L>
struct mse_traits<T, extents<N>, void, L>
{
    static constexpr bool   is_column_matrix    = false;
    static constexpr bool   is_row_matrix       = false;
    static constexpr bool   is_matrix           = false;
    static constexpr bool   is_vector           = true;

    static constexpr bool   is_readable         = true;
    static constexpr bool   is_writable         = true;
    static constexpr bool   is_initable         = true;
    static constexpr bool   is_resizable        = false;

    using span_type       = fixed_vector_mdspan_t<T, N>;
    using const_span_type = fixed_vector_mdspan_t<T const, N>;
};

template<class T, ptrdiff_t N, class A, class L>
struct mse_traits<T, extents<N>, A, L>
{
    static constexpr bool   is_column_matrix    = false;
    static constexpr bool   is_row_matrix       = false;
    static constexpr bool   is_matrix           = false;
    static constexpr bool   is_vector           = true;

    static constexpr bool   is_readable         = true;
    static constexpr bool   is_writable         = true;
    static constexpr bool   is_initable         = true;
    static constexpr bool   is_resizable        = false;

    using span_type       = fixed_vector_mdspan_t<T, N>;
    using const_span_type = fixed_vector_mdspan_t<T const, N>;
};

template<class T, class A, class L>
struct mse_traits<T, extents<dynamic_extent>, A, L>
{
    static constexpr bool   is_vector           = true;
    static constexpr bool   is_matrix           = false;

    static constexpr bool   is_readable         = true;
    static constexpr bool   is_writable         = true;
    static constexpr bool   is_initable         = true;
    static constexpr bool   is_resizable        = true;

    using span_type       = dynamic_vector_mdspan_t<T>;
    using const_span_type = dynamic_vector_mdspan_t<T const>;
};

//---------------------------
//- Row matrix engine (1 x C)
//
template<class T, ptrdiff_t C, class L>
struct mse_traits<T, extents<1, C>, void, L>
{
    static constexpr bool   is_column_matrix    = false;
    static constexpr bool   is_row_matrix       = true;
    static constexpr bool   is_matrix           = true;
    static constexpr bool   is_vector           = false;

    static constexpr bool   is_readable         = true;
    static constexpr bool   is_writable         = true;
    static constexpr bool   is_initable         = true;
    static constexpr bool   is_column_resizable = false;
    static constexpr bool   is_row_resizable    = false;
    static constexpr bool   is_resizable        = false;

    static constexpr bool   is_column_major     = is_same_v<L, column_major>;
    static constexpr bool   is_row_major        = is_same_v<L, row_major>;

    using span_type       = fixed_matrix_mdspan_t<T, 1, C, L>;
    using const_span_type = fixed_matrix_mdspan_t<T const, 1, C, L>;
};

template<class T, ptrdiff_t C, class A, class L>
struct mse_traits<T, extents<1, C>, A, L>
{
    static constexpr bool   is_column_matrix    = false;
    static constexpr bool   is_row_matrix       = true;
    static constexpr bool   is_matrix           = true;
    static constexpr bool   is_vector           = false;

    static constexpr bool   is_readable         = true;
    static constexpr bool   is_writable         = true;
    static constexpr bool   is_initable         = true;
    static constexpr bool   is_column_resizable = false;
    static constexpr bool   is_row_resizable    = false;
    static constexpr bool   is_resizable        = false;

    static constexpr bool   is_column_major     = is_same_v<L, column_major>;
    static constexpr bool   is_row_major        = is_same_v<L, row_major>;

    using span_type       = fixed_matrix_mdspan_t<T, 1, C, L>;
    using const_span_type = fixed_matrix_mdspan_t<T const, 1, C, L>;
};

template<class T, class A, class L>
struct mse_traits<T, extents<1, dynamic_extent>, A, L>
{
    static constexpr bool   is_column_matrix    = false;
    static constexpr bool   is_row_matrix       = true;
    static constexpr bool   is_matrix           = true;
    static constexpr bool   is_vector           = false;

    static constexpr bool   is_readable         = true;
    static constexpr bool   is_writable         = true;
    static constexpr bool   is_initable         = true;
    static constexpr bool   is_column_resizable = false;
    static constexpr bool   is_row_resizable    = true;
    static constexpr bool   is_resizable        = false;

    static constexpr bool   is_column_major     = is_same_v<L, column_major>;
    static constexpr bool   is_row_major        = is_same_v<L, row_major>;

    using span_type       = dynamic_matrix_mdspan_t<T>;
    using const_span_type = dynamic_matrix_mdspan_t<T const>;
};

//------------------------------
//- Column matrix engine (R x 1)
//
template<class T, ptrdiff_t R, class L>
struct mse_traits<T, extents<R, 1>, void, L>
{
    static constexpr bool   is_column_matrix    = true;
    static constexpr bool   is_row_matrix       = false;
    static constexpr bool   is_matrix           = true;
    static constexpr bool   is_vector           = false;

    static constexpr bool   is_readable         = true;
    static constexpr bool   is_writable         = true;
    static constexpr bool   is_initable         = true;
    static constexpr bool   is_column_resizable = false;
    static constexpr bool   is_row_resizable    = false;
    static constexpr bool   is_resizable        = false;

    static constexpr bool   is_column_major     = is_same_v<L, column_major>;
    static constexpr bool   is_row_major        = is_same_v<L, row_major>;

    using span_type       = fixed_matrix_mdspan_t<T, R, 1, L>;
    using const_span_type = fixed_matrix_mdspan_t<T const, R, 1, L>;
};

template<class T, ptrdiff_t R, class A, class L>
struct mse_traits<T, extents<R, 1>, A, L>
{
    static constexpr bool   is_column_matrix    = true;
    static constexpr bool   is_row_matrix       = false;
    static constexpr bool   is_matrix           = true;
    static constexpr bool   is_vector           = false;

    static constexpr bool   is_readable         = true;
    static constexpr bool   is_writable         = true;
    static constexpr bool   is_initable         = true;
    static constexpr bool   is_column_resizable = false;
    static constexpr bool   is_row_resizable    = false;
    static constexpr bool   is_resizable        = false;

    static constexpr bool   is_column_major     = is_same_v<L, column_major>;
    static constexpr bool   is_row_major        = is_same_v<L, row_major>;

    using span_type       = fixed_matrix_mdspan_t<T, R, 1, L>;
    using const_span_type = fixed_matrix_mdspan_t<T const, R, 1, L>;
};

template<class T, class A, class L>
struct mse_traits<T, extents<dynamic_extent, 1>, A, L>
{
    static constexpr bool   is_column_matrix    = true;
    static constexpr bool   is_row_matrix       = false;
    static constexpr bool   is_matrix           = true;
    static constexpr bool   is_vector           = false;

    static constexpr bool   is_readable         = true;
    static constexpr bool   is_writable         = true;
    static constexpr bool   is_initable         = true;
    static constexpr bool   is_column_resizable = true;
    static constexpr bool   is_row_resizable    = false;
    static constexpr bool   is_resizable        = false;

    static constexpr bool   is_column_major     = is_same_v<L, column_major>;
    static constexpr bool   is_row_major        = is_same_v<L, row_major>;

    using span_type       = dynamic_matrix_mdspan_t<T>;
    using const_span_type = dynamic_matrix_mdspan_t<T const>;
};

//------------------------------
//- General matrix engine (R, C)
//
template<class T, ptrdiff_t R, ptrdiff_t C, class L>
struct mse_traits<T, extents<R, C>, void, L>
{
    static constexpr bool   is_column_matrix    = false;
    static constexpr bool   is_row_matrix       = false;
    static constexpr bool   is_matrix           = true;
    static constexpr bool   is_vector           = false;

    static constexpr bool   is_readable         = true;
    static constexpr bool   is_writable         = true;
    static constexpr bool   is_initable         = true;
    static constexpr bool   is_column_resizable = false;
    static constexpr bool   is_row_resizable    = false;
    static constexpr bool   is_resizable        = false;

    static constexpr bool   is_column_major     = is_same_v<L, column_major>;
    static constexpr bool   is_row_major        = is_same_v<L, row_major>;

    using span_type       = fixed_matrix_mdspan_t<T, R, C, L>;
    using const_span_type = fixed_matrix_mdspan_t<T const, R, C, L>;
};

template<class T, ptrdiff_t R, ptrdiff_t C, class A, class L>
struct mse_traits<T, extents<R, C>, A, L>
{
    static constexpr bool   is_column_matrix    = false;
    static constexpr bool   is_row_matrix       = false;
    static constexpr bool   is_matrix           = true;
    static constexpr bool   is_vector           = false;

    static constexpr bool   is_readable         = true;
    static constexpr bool   is_writable         = true;
    static constexpr bool   is_initable         = true;
    static constexpr bool   is_column_resizable = false;
    static constexpr bool   is_row_resizable    = false;
    static constexpr bool   is_resizable        = false;

    static constexpr bool   is_column_major     = is_same_v<L, column_major>;
    static constexpr bool   is_row_major        = is_same_v<L, row_major>;

    using span_type       = fixed_matrix_mdspan_t<T, R, C, L>;
    using const_span_type = fixed_matrix_mdspan_t<T const, R, C, L>;
};

template<class T, ptrdiff_t C, class A, class L>
struct mse_traits<T, extents<dynamic_extent, C>, A, L>
{
    static constexpr bool   is_column_matrix    = false;
    static constexpr bool   is_row_matrix       = false;
    static constexpr bool   is_matrix           = true;
    static constexpr bool   is_vector           = false;

    static constexpr bool   is_readable         = true;
    static constexpr bool   is_writable         = true;
    static constexpr bool   is_initable         = true;
    static constexpr bool   is_column_resizable = false;
    static constexpr bool   is_row_resizable    = true;
    static constexpr bool   is_resizable        = false;

    static constexpr bool   is_column_major     = is_same_v<L, column_major>;
    static constexpr bool   is_row_major        = is_same_v<L, row_major>;

    using span_type       = dynamic_matrix_mdspan_t<T>;
    using const_span_type = dynamic_matrix_mdspan_t<T const>;
};

template<class T, ptrdiff_t R, class A, class L>
struct mse_traits<T, extents<R, dynamic_extent>, A, L>
{
    static constexpr bool   is_column_matrix    = false;
    static constexpr bool   is_row_matrix       = false;
    static constexpr bool   is_matrix           = true;
    static constexpr bool   is_vector           = false;

    static constexpr bool   is_readable         = true;
    static constexpr bool   is_writable         = true;
    static constexpr bool   is_initable         = true;
    static constexpr bool   is_column_resizable = true;
    static constexpr bool   is_row_resizable    = false;
    static constexpr bool   is_resizable        = false;

    static constexpr bool   is_column_major     = is_same_v<L, column_major>;
    static constexpr bool   is_row_major        = is_same_v<L, row_major>;

    using span_type       = dynamic_matrix_mdspan_t<T>;
    using const_span_type = dynamic_matrix_mdspan_t<T const>;
};

template<class T, class A, class L>
struct mse_traits<T, extents<dynamic_extent, dynamic_extent>, A, L>
{
    static constexpr bool   is_column_matrix    = false;
    static constexpr bool   is_row_matrix       = false;
    static constexpr bool   is_matrix           = true;
    static constexpr bool   is_vector           = false;

    static constexpr bool   is_readable         = true;
    static constexpr bool   is_writable         = true;
    static constexpr bool   is_initable         = true;
    static constexpr bool   is_column_resizable = true;
    static constexpr bool   is_row_resizable    = true;
    static constexpr bool   is_resizable        = true;

    static constexpr bool   is_column_major     = is_same_v<L, column_major>;
    static constexpr bool   is_row_major        = is_same_v<L, row_major>;

    using span_type       = dynamic_matrix_mdspan_t<T>;
    using const_span_type = dynamic_matrix_mdspan_t<T const>;
};


//--------------------------------------------------------------------------------------------------
//  Class Template:     mse_data<T, X, A, L>
//
//  Type that contains and manages elements on behalf of matrix_storage_engine<T,X,A,L>.
//
//  Partial specializations of this class template are tailored to specific corresponding partial
//  specializations of matrix_storage_engine.  They provide only the special member functions,
//  and all other manipulation of their internal state is performed by matrix_storage_engine.
//
//  The implementation assumes that all dynamically-allocated memory is default-constructed,
//  with the consequence that elements lying in unused capacity are also constructed.  This
//  assumption makes implementation easy, but may be absent in the final version.
//--------------------------------------------------------------------------------------------------
//
template<class T, class X, class A, class L>    struct mse_data;

//-----------------------------------------------------------------
//- Fixed size.  Elements contained as member data in a std::array.
//
template<class T, ptrdiff_t N, class L>
struct mse_data<T, extents<N>, void, L>
{
    using array_type = std::array<T, N>;

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
};

//------------------------------------------------------------------
//- Fixed size.  Elements contained as member data in a std::vector.
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

//--------------------------------------------------------------------
//- Dynamic size.  Elements contained as member data in a std::vector.
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

}       //- detail namespace
}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_MATRIX_STORAGE_ENGINE_SUPPORT_HPP_DEFINED
