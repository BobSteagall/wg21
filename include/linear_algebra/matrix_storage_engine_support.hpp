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
        #ifdef LA_COMPOUND_REQUIREMENT_SYNTAX_SUPPORTED
            { *p   } -> returns<T&>;
            { p[n] } -> returns<T&>;
        #else
            requires is_same_v<decltype(*p), T&>;
            requires is_same_v<decltype(p[n]), T&>;
        #endif
        };
#else
    //- Clang 10 and VC++ 16.5 accept the following without any problems.
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
            dyn_strides     strides{rep.m_colcap, 1};
            dyn_mapping     mapping(extents, strides);

            return SpanType(rep.m_elems.data(), mapping);
        }
        else if constexpr (EngineTraits::is_column_major)
        {
            dyn_extents     extents(rep.m_rows, rep.m_cols);
            dyn_strides     strides{1, rep.m_rowcap};
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
//- Case 0: primary template
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
//- Case 1: vector engine (1 x N)
//
template<class T, ptrdiff_t N, class L>
struct mse_traits<T, extents<N>, void, L>                       //- Case 1A
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
struct mse_traits<T, extents<N>, A, L>                          //- Case 1B
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
struct mse_traits<T, extents<dynamic_extent>, A, L>             //- Case 1C
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
//- Case 2: row matrix engine (1 x C)
//
template<class T, ptrdiff_t C, class L>
struct mse_traits<T, extents<1, C>, void, L>                    //- Case 2A
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
struct mse_traits<T, extents<1, C>, A, L>                       //- Case 2B
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
struct mse_traits<T, extents<1, dynamic_extent>, A, L>          //- Case 2C
{
    static constexpr bool   is_column_matrix    = false;
    static constexpr bool   is_row_matrix       = true;
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
//- Case 3: column matrix engine (R x 1)
//
template<class T, ptrdiff_t R, class L>
struct mse_traits<T, extents<R, 1>, void, L>                    //- Case 3A
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
struct mse_traits<T, extents<R, 1>, A, L>                       //- Case 3B
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
struct mse_traits<T, extents<dynamic_extent, 1>, A, L>          //- Case 3C
{
    static constexpr bool   is_column_matrix    = true;
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

//------------------------------
//- Case 4: general matrix engine (R, C)
//
template<class T, ptrdiff_t R, ptrdiff_t C, class L>
struct mse_traits<T, extents<R, C>, void, L>                    //- Case 4A
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
struct mse_traits<T, extents<R, C>, A, L>                       //- Case 4B
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
struct mse_traits<T, extents<dynamic_extent, C>, A, L>          //- Case 4C
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
struct mse_traits<T, extents<R, dynamic_extent>, A, L>          //- Case 4D
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
struct mse_traits<T, extents<dynamic_extent, dynamic_extent>, A, L>     //- Case 4E
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

template<class MSE>                             struct mse_support;
template<class T, class X, class A, class L>    struct mse_data;

//--------------------------------------------------------------------------------------------------
//  Class Template:     mse_support<T, X, A, L>
//
//  Policy/traits type that performs element maniuplation on behalof of mse_data<T, X, A, L>.
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
struct mse_support_base
{
    static inline constexpr void
    verify_capacity(ptrdiff_t c)
    {
        if (c < 0)
        {
            throw runtime_error("invalid capacity parameter");
        }
    }

    static inline constexpr void
    verify_size(ptrdiff_t s)
    {
        if (s < 1)
        {
            throw runtime_error("invalid size parameter");
        }
    }

    static inline constexpr void
    verify_size(ptrdiff_t s1, ptrdiff_t s2)
    {
        if (s1 != s2)
        {
            throw runtime_error("invalid size parameter");
        }
    }

    template<class U>
    static constexpr tuple<ptrdiff_t, ptrdiff_t>
    verify_list(initializer_list<initializer_list<U>> list)
    {
        size_t  rows = list.size();
        size_t  cols = list.begin()->size();

        if (std::is_constant_evaluated())
        {
            for (auto const row : list)
            {
                static_assert(row.size() != cols, "matrix initializer_list has invalid shape");
            }
        }
        else
        {
            for (auto const row : list)
            {
                if (row.size() != cols)
                {
                    throw runtime_error("matrix initializer_list has invalid shape");
                }
            }
        }

        return {static_cast<ptrdiff_t>(rows), static_cast<ptrdiff_t>(cols)};
    }
};

template<class T, ptrdiff_t R, ptrdiff_t C, class A, class L>
struct mse_support<mse_data<T, extents<R, C>, A, L>>  : public mse_support_base
{
    //- Engine representation type.
    //
    using mse_type = mse_data<T, extents<R, C>, A, L>;

    template<class FN>
    static constexpr void
    apply(mse_type& dst, ptrdiff_t i0, ptrdiff_t j0, ptrdiff_t i1, ptrdiff_t j1, FN fn)
    {
        if constexpr (is_same_v<L, row_major>)
        {
            for (ptrdiff_t i = i0;  i < i1;  ++i)
            {
                for (ptrdiff_t j = j0;  j < j1;  ++j)
                {
                    dst.at(i, j) = fn(i, j);
                }
            }
        }
        else
        {
            for (ptrdiff_t j = j0;  j < j1;  ++j)
            {
                for (ptrdiff_t i = i0;  i < i1;  ++i)
                {
                    dst.at(i, j) = fn(i, j);
                }
            }
        }
    }

    template<class ET>
    static inline constexpr void
    copy_engine(mse_type& dst, ET const& src)
    {
        apply(dst, 0, 0, dst.m_rows, dst.m_cols,
                [&src](ptrdiff_t i, ptrdiff_t j){  return static_cast<T>(src(i, j));  });
    }

    static inline constexpr void
    fill_columns(mse_type& dst, ptrdiff_t c0, ptrdiff_t c1, T const& t)
    {
        apply(dst, 0, c0, dst.m_rows, c1,
                [&t](ptrdiff_t, ptrdiff_t){  return t;  });
    }

    static inline constexpr void
    fill_rows(mse_type& dst, ptrdiff_t r0, ptrdiff_t r1, T const& t)
    {
        apply(dst, r0, 0, r1, dst.m_cols,
                [&t](ptrdiff_t, ptrdiff_t){  return t;  });
    }

    static inline constexpr void
    move_elements(mse_type& dst, mse_type const& src, ptrdiff_t rows, ptrdiff_t cols)
    {
        apply(dst, src, 0, 0, rows, cols,
                [&src](ptrdiff_t i, ptrdiff_t j){  return std::move(src.at(i, j));  });
    }

    template<class U>
    static constexpr void
    copy_list(mse_type& dst, initializer_list<U> src)
    {
        transform(src.begin(), src.end(), dst.m_data.begin(),
                    [](U const& u){  return static_cast<T>(u);  });
    }

    template<class T2>
    static constexpr void
    copy_list(mse_type& dst, initializer_list<initializer_list<T2>> src)
    {
        using row_iter = decltype(src.begin());
        using col_iter = decltype(src.begin()->begin());

        ptrdiff_t   di = 0;
        row_iter    rp = src.begin();

        for (;  di < dst.m_rows;  ++di, ++rp)
        {
            ptrdiff_t   dj = 0;
            col_iter    cp = rp->begin();

            for (;  dj < dst.m_cols;  ++dj, ++cp)
            {
                dst.at(di, dj) = static_cast<T>(*cp);
            }
        }
    }

    static inline constexpr void
    swap(mse_type& m1, mse_type& m2)
    {
        mse_type    m0(std::move(m1));
        m1 = std::move(m2);
        m2 = std::move(m0);
    }

    //- Support for mdspan -- types and functions.
    //
    static constexpr bool   use_dyn_span = (R == dynamic_extent || C == dynamic_extent);

    using fixed_extents = extents<R, C>;
    using fixed_layout  = conditional_t<is_same_v<L, row_major>, layout_right, layout_left>;
    using fixed_mdspan  = basic_mdspan<T, fixed_extents, fixed_layout>;
    using fixed_cmdspan = basic_mdspan<T const, fixed_extents, fixed_layout>;

    using dyn_extents = extents<dynamic_extent, dynamic_extent>;
    using dyn_layout  = layout_stride<dynamic_extent, dynamic_extent>;
    using dyn_mapping = typename dyn_layout::template mapping<dyn_extents>;
    using dyn_strides = array<typename dyn_extents::index_type, 2>;
    using dyn_mdspan  = basic_mdspan<T, dyn_extents, dyn_layout>;
    using dyn_cmdspan = basic_mdspan<T const, dyn_extents, dyn_layout>;

    using span_type       = conditional_t<use_dyn_span, dyn_mdspan, fixed_mdspan>;
    using const_span_type = conditional_t<use_dyn_span, dyn_cmdspan, fixed_cmdspan>;

    static inline constexpr span_type
    make_span(mse_type& rep)
    {
        if constexpr (use_dyn_span)
        {
            if constexpr (is_same_v<L, row_major>)
            {
                dyn_extents     extents(rep.m_rows, rep.m_cols);
                dyn_strides     strides{rep.m_colcap, 1};
                dyn_mapping     mapping(extents, strides);

                return span_type(rep.m_elems.data(), mapping);
            }
            else
            {
                dyn_extents     extents(rep.m_rows, rep.m_cols);
                dyn_strides     strides{1, rep.m_rowcap};
                dyn_mapping     mapping(extents, strides);

                return span_type(rep.m_elems.data(), mapping);
            }
        }
        else
        {
            return span_type(rep.m_elems.data());
        }
    }

    static inline constexpr const_span_type
    make_const_span(mse_type const& rep)
    {
        if constexpr (use_dyn_span)
        {
            if constexpr (is_same_v<L, row_major>)
            {
                dyn_extents     extents(rep.m_rows, rep.m_cols);
                dyn_strides     strides{rep.m_colcap, 1};
                dyn_mapping     mapping(extents, strides);

                return const_span_type(rep.m_elems.data(), mapping);
            }
            else
            {
                dyn_extents     extents(rep.m_rows, rep.m_cols);
                dyn_strides     strides{1, rep.m_rowcap};
                dyn_mapping     mapping(extents, strides);

                return const_span_type(rep.m_elems.data(), mapping);
            }
        }
        else
        {
            return const_span_type(rep.m_elems.data());
        }
    }
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
    using this_type       = mse_data;
    using array_type      = std::array<T, R*C>;
    using support         = mse_support<this_type>;
    using span_type       = typename support::span_type;
    using const_span_type = typename support::const_span_type;

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

    inline constexpr T&
    at(ptrdiff_t i)
        requires this_type::is_linear_matrix
    {
        return m_elems[i];
    }

    inline constexpr T const&
    at(ptrdiff_t i) const
        requires this_type::is_linear_matrix
    {
        return m_elems[i];
    }

    inline constexpr T&
    at(ptrdiff_t i, ptrdiff_t j)
    {
        if constexpr (is_same_v<L, row_major>)
            return m_elems[i*m_colcap + j];
        else
            return m_elems[i + j*m_rowcap];
    }

    inline constexpr T const&
    at(ptrdiff_t i, ptrdiff_t j) const
    {
        if constexpr (is_same_v<L, row_major>)
            return m_elems[i*m_colcap + j];
        else
            return m_elems[i + j*m_rowcap];
    }

    template<class U>
    constexpr void
    assign(initializer_list<U> src)
        requires this_type::is_linear_matrix
    {
        ptrdiff_t   size = static_cast<ptrdiff_t>(src.size());
        static_assert((m_rows == 1  &&  m_cols == size) || (m_rows == size  &&  m_cols == 1));
        support::copy_list(*this, src);
    }

    template<class U>
    constexpr void
    assign(initializer_list<initializer_list<U>> src)
    {
        auto    [rows, cols] = support::verify_list(src);
        static_assert(rows == m_rows);
        static_assert(cols == m_cols);
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

    inline span_type
    span() noexcept
    {
        return support::make_span(*this);
    }

    inline const_span_type
    span() const noexcept
    {
        return support::make_const_span(*this);
    }
};

//-------------------------------------------------------------------------------
//- Fixed rows / fixed columns.  Elements dynamically allocated in a std::vector.
//
template<class T, ptrdiff_t R, ptrdiff_t C, class A, class L>
struct mse_data<T, extents<R, C>, A, L>
{
    using this_type       = mse_data;
    using array_type      = std::vector<T, A>;
    using support         = mse_support<this_type>;
    using span_type       = typename support::span_type;
    using const_span_type = typename support::const_span_type;

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

    inline T&
    at(ptrdiff_t i)
        requires this_type::is_linear_matrix
    {
        return m_elems[i];
    }

    inline T const&
    at(ptrdiff_t i) const
        requires this_type::is_linear_matrix
    {
        return m_elems[i];
    }

    inline T&
    at(ptrdiff_t i, ptrdiff_t j)
    {
        if constexpr (is_same_v<L, row_major>)
            return m_elems[i*m_colcap + j];
        else
            return m_elems[i + j*m_rowcap];
    }

    inline T const&
    at(ptrdiff_t i, ptrdiff_t j) const
    {
        if constexpr (is_same_v<L, row_major>)
            return m_elems[i*m_colcap + j];
        else
            return m_elems[i + j*m_rowcap];
    }

    template<class U>
    constexpr void
    assign(initializer_list<U> src)
        requires this_type::is_linear_matrix
    {
        ptrdiff_t   size = static_cast<ptrdiff_t>(src.size());
        static_assert((m_rows == 1  &&  m_cols == size) || (m_rows == size  &&  m_cols == 1));
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

    inline span_type
    span() noexcept
    {
        return support::make_span(*this);
    }

    inline const_span_type
    span() const noexcept
    {
        return support::make_const_span(*this);
    }
};

//---------------------------------------------------------------------------------
//- Fixed rows / dynamic columns.  Elements dynamically allocated in a std::vector.
//
template<class T, ptrdiff_t R, class A, class L>
struct mse_data<T, extents<R, dynamic_extent>, A, L>
{
    using this_type       = mse_data;
    using array_type      = std::vector<T, A>;
    using support         = mse_support<this_type>;
    using span_type       = typename support::span_type;
    using const_span_type = typename support::const_span_type;

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

    constexpr void
    update_extents(ptrdiff_t, ptrdiff_t cols, ptrdiff_t, ptrdiff_t colcap) noexcept
    {
        m_cols   = cols;
        m_colcap = colcap;
    }

    inline T&
    at(ptrdiff_t i)
        requires this_type::is_row_matrix
    {
        return m_elems[i];
    }

    inline T const&
    at(ptrdiff_t i) const
        requires this_type::is_row_matrix
    {
        return m_elems[i];
    }

    inline T&
    at(ptrdiff_t i, ptrdiff_t j)
    {
        if constexpr (this_type::is_row_major)
            return m_elems[i*m_colcap + j];
        else
            return m_elems[i + j*m_rowcap];
    }

    inline T const&
    at(ptrdiff_t i, ptrdiff_t j) const
    {
        if constexpr (this_type::is_row_major)
            return m_elems[i*m_colcap + j];
        else
            return m_elems[i + j*m_rowcap];
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
        support::verify_size(static_cast<ptrdiff_t>(eng.columns()), m_cols);
        reshape_columns(eng.rows(), m_rowcap);
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

    inline span_type
    span() noexcept
    {
        return support::make_span(*this);
    }

    inline const_span_type
    span() const noexcept
    {
        return support::make_const_span(*this);
    }
};

//---------------------------------------------------------------------------------
//- Dynamic rows / fixed columns.  Elements dynamically allocated in a std::vector.
//
template<class T, ptrdiff_t C, class A, class L>
struct mse_data<T, extents<dynamic_extent, C>, A, L>
{
    using this_type       = mse_data;
    using array_type      = std::vector<T, A>;
    using support         = mse_support<this_type>;
    using span_type       = typename support::span_type;
    using const_span_type = typename support::const_span_type;

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

    constexpr void
    update_extents(ptrdiff_t rows, ptrdiff_t, ptrdiff_t rowcap, ptrdiff_t) noexcept
    {
        m_rows   = rows;
        m_rowcap = rowcap;
    }

    inline T&
    at(ptrdiff_t i)
        requires this_type::is_column_matrix
    {
        return m_elems[i];
    }

    inline T const&
    at(ptrdiff_t i) const
        requires this_type::is_column_matrix
    {
        return m_elems[i];
    }

    inline T&
    at(ptrdiff_t i, ptrdiff_t j)
    {
        if constexpr (this_type::is_row_major)
            return m_elems[i*m_colcap + j];
        else
            return m_elems[i + j*m_rowcap];
    }

    inline T const&
    at(ptrdiff_t i, ptrdiff_t j) const
    {
        if constexpr (this_type::is_row_major)
            return m_elems[i*m_colcap + j];
        else
            return m_elems[i + j*m_rowcap];
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
        verify_size(cols, m_cols);
        reshape_rows(rows, m_rowcap);
        support::copy_list(*this, src);
    }

    template<class ET>
    void
    assign(ET const& eng)
    {
        support::verify_size(static_cast<ptrdiff_t>(eng.columns()), m_cols);
        reshape_rows(eng.rows(), m_rowcap);
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

    inline span_type
    span() noexcept
    {
        return support::make_span(*this);
    }

    inline const_span_type
    span() const noexcept
    {
        return support::make_const_span(*this);
    }
};

//-----------------------------------------------------------------------------------
//- Dynamic rows / dynamic columns.  Elements dynamically allocated in a std::vector.
//
template<class T, class A, class L>
struct mse_data<T, extents<dynamic_extent, dynamic_extent>, A, L>
{
    using this_type       = mse_data;
    using array_type      = std::vector<T, A>;
    using support         = mse_support<this_type>;
    using span_type       = typename support::span_type;
    using const_span_type = typename support::const_span_type;

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

    inline void
    update_extents(ptrdiff_t rows, ptrdiff_t cols, ptrdiff_t rowcap, ptrdiff_t colcap) noexcept
    {
        m_rows   = rows;
        m_cols   = cols;
        m_rowcap = rowcap;
        m_colcap = colcap;
    }

    inline T&
    at(ptrdiff_t i, ptrdiff_t j)
    {
        if constexpr (this_type::is_row_major)
            return m_elems[i*m_colcap + j];
        else
            return m_elems[i + j*m_rowcap];
    }

    inline T const&
    at(ptrdiff_t i, ptrdiff_t j) const
    {
        if constexpr (this_type::is_row_major)
            return m_elems[i*m_colcap + j];
        else
            return m_elems[i + j*m_rowcap];
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

    inline span_type
    span() noexcept
    {
        return support::make_span(*this);
    }

    inline const_span_type
    span() const noexcept
    {
        return support::make_const_span(*this);
    }
};

}       //- detail namespace
}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_MATRIX_STORAGE_ENGINE_SUPPORT_HPP_DEFINED
