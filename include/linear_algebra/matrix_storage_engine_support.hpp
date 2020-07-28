//==================================================================================================
//  File:       matrix_storage_engine_support.hpp
//
//  Summary:    This header defines several concepts and types whose purpose is to support the
//              mse_data and matrix_storage_engine types.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_MATRIX_STORAGE_ENGINE_SUPPORT_HPP_DEFINED
#define LINEAR_ALGEBRA_MATRIX_STORAGE_ENGINE_SUPPORT_HPP_DEFINED

namespace STD_LA {
namespace detail {
//- Some forward declarations.
//
//template<class MSE>                             struct mse_support;
//template<class T, class X, class A, class L>    struct mse_data;

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
concept no_allocator = same_as<T, void>;

#if defined(LA_COMPILER_GCC) && ((LA_GCC_VERSION == 9) || (LA_GCC_VERSION == 10))
    //- Neither GCC 9 nor GCC 10 can parse the type requirement 'AT::template rebind_alloc<T>',
    //  so we add a level of indirection and hoist it up into its own alias template.
    //
    template<class AT, class T>
    using at_rebind_alloc_t = typename AT::template rebind_alloc<T>;

    template<typename AT, typename T>
    concept valid_allocator_traits =
        requires (typename AT::allocator_type a, typename AT::pointer p, typename AT::size_type n)
        {
            typename AT::allocator_type;
            typename AT::value_type;
            typename AT::size_type;
            typename AT::pointer;
            typename at_rebind_alloc_t<AT, T>;
            requires is_same_v<T, typename AT::value_type>;
            { AT::deallocate(a, p, n) };
            { AT::allocate(a, n) } -> same_as<typename AT::pointer>;
            { static_cast<T*>(p) };
        #ifdef LA_COMPOUND_REQUIREMENT_SYNTAX_SUPPORTED
            { *p   } -> same_as<T&>;
            { p[n] } -> same_as<T&>;
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
        requires (typename AT::allocator_type a, typename AT::pointer p, typename AT::size_type n)
        {
            typename AT::allocator_type;
            typename AT::value_type;
            typename AT::size_type;
            typename AT::pointer;
            typename AT::template rebind_alloc<T>;
            requires is_same_v<T, typename AT::value_type>;
            { AT::deallocate(a, p, n) };
            { AT::allocate(a, n) } -> same_as<typename AT::pointer>;
            { static_cast<T*>(p) };
            { *p   } -> same_as<T&>;
            { p[n] } -> same_as<T&>;
        };
#endif

//- Concept definition.
//
template<typename A, typename T>
concept valid_mse_allocator = no_allocator<A> or valid_allocator_traits<allocator_traits<A>, T>;


//--------------------------------------------------------------------------------------------------
//  Concepts:   valid_mse_matrix_layout<L>
//              valid_mse_vector_layout<L>
//
//  These private concepts are used to validate the fourth template argument of a specialization
//  of matrix_storage_engine, the engine's layout type.  It must be row_major or column_major for
//  matrix engines, and unoriented for vector engines.
//--------------------------------------------------------------------------------------------------
//
template<typename L>
concept valid_mse_matrix_layout = (is_same_v<L, row_major> || is_same_v<L, column_major>);

template<typename L>
concept valid_mse_vector_layout = is_same_v<L, unoriented>;


//--------------------------------------------------------------------------------------------------
//  Concepts:   linearly_indexable<MSD>
//              reshapable<MSD>
//              column_reshapable<MSD>
//              row_reshapable<MSD>
//
//  These private concepts are used to probe various properties of matrix_storage_data
//  specializations.
//--------------------------------------------------------------------------------------------------
//
template<typename MSD>
concept linearly_indexable_msd = MSD::is_linearly_indexable;

template<typename MSD>
concept reshapable_msd = MSD::is_reshapable;

template<typename MSD>
concept column_reshapable_msd = MSD::is_column_reshapable;

template<typename MSD>
concept row_reshapable_msd = MSD::is_row_reshapable;

}       //- detail namespace
}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_MATRIX_STORAGE_ENGINE_SUPPORT_HPP_DEFINED
