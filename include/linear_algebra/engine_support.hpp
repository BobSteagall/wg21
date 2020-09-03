//==================================================================================================
//  File:       engine_support.hpp
//
//  Summary:    This header defines a number of private aliases, traits, concepts, types, and
//              functions used across the entire library to support engines and the required
//              operations upon them.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_ENGINE_SUPPORT_HPP_DEFINED
#define LINEAR_ALGEBRA_ENGINE_SUPPORT_HPP_DEFINED

namespace STD_LA {
//==================================================================================================
//  PUBLIC TAG TYPES
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//  Class:      matrix_layout
//
//  This public type is a container of tag sub-types whose purpose is to specify the layout
//  of matrix elements when used as a template argument to matrix_storage_engine<T, X, A, L>.
//--------------------------------------------------------------------------------------------------
//
struct matrix_layout
{
    struct row_major {};
    struct column_major {};
};


//--------------------------------------------------------------------------------------------------
//  Class:      matrix_view
//
//  This public type is a container of tag sub-types whose purpose is to specify the functionality
//  of a view engine when used as the second template argument to matrix_view_engine<ET, MVT>.
//--------------------------------------------------------------------------------------------------
//
struct matrix_view
{
    struct const_negation {};
    struct const_conjugate {};
    struct const_hermitian {};

    struct identity {};
    struct const_identity {};

    struct transpose {};
    struct const_transpose {};

    struct column {};
    struct const_column {};

    struct row {};
    struct const_row {};

    struct submatrix {};
    struct const_submatrix {};

    struct subvector {};
    struct const_subvector {};
};


//==================================================================================================
//  PUBLIC TYPE FORWARD DECLARATIONS
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//  Class Template:     matrix_storage_engine<T, X, A, L>
//
//  This class template implements an owning engine for use by the math object class templates
//  basic_matrix<ET, OT> and basic_vector<ET, OT>.  Specifically, it provides storage suitable
//  for modeling a mathematical matrix or vector, having dimensions specified by X, employing
//  allocator A, and having element layout L.
//--------------------------------------------------------------------------------------------------
//
template<class T, class X, class A, class L>    class matrix_storage_engine;


//--------------------------------------------------------------------------------------------------
//  Class Template:     matrix_view_engine<ET, MVT>
//
//  This class template implements represents a non-owning engine type, which references another
//  engine object, and which presents a specific "view" of that object.
//--------------------------------------------------------------------------------------------------
//
template<class ET, class MVT>    class matrix_view_engine;


namespace detail {
//==================================================================================================
//  TYPES AND TRAITS DEFINITIONS -- GENERAL
//==================================================================================================
//
struct special_ctor_tag{};


//--------------------------------------------------------------------------------------------------
//  Trait:      has_size_type<T>
//  Alias:      get_size_type_t<T>
//
//  This private traits type finds the nested alias size_type if it is present; otherwise, it
//  returns size_t.
//--------------------------------------------------------------------------------------------------
//
template<class T, typename = void>
struct has_size_type_alias
{
    using size_type = std::size_t;
};

template<class T>
struct has_size_type_alias<T, std::void_t<typename T::size_type>>
{
    using size_type = typename T::size_type;
};

//------
//
template<class T>
using get_size_type_t = typename has_size_type_alias<T>::size_type;


//--------------------------------------------------------------------------------------------------
//  Trait:      is_specialization_of<T, PT>
//  Variable:   is_specialization_of_v<T, PT>
//
//  This private traits type determines whether a given type T is a specialization of a primary
//  template type PT.
//--------------------------------------------------------------------------------------------------
//
template<class T, template<class...> class PT>
struct is_specialization_of : public std::false_type
{};

template<template<class...> class PT, class... ARGS>
struct is_specialization_of<PT<ARGS...>, PT> : public std::true_type
{};

//------
//
template<class T, template<class...> class PT> inline constexpr
bool    is_specialization_of_v = is_specialization_of<T, PT>::value;

template<class T> inline constexpr
bool    is_complex_v = is_specialization_of_v<T, std::complex>;


//--------------------------------------------------------------------------------------------------
//  Trait:      is_random_access_standard_container<X>
//  Variable:   is_random_access_standard_container_v<X>
//
//  This private traits type determines whether the template parameter is a specialization
//  of std::array, std::deque, or std::vector.
//--------------------------------------------------------------------------------------------------
//
template<class T>
struct is_random_access_standard_container : public false_type
{};

template<class T, std::size_t N>
struct is_random_access_standard_container<std::array<T, N>> : public true_type
{};

template<class T, class A>
struct is_random_access_standard_container<std::deque<T, A>> : public true_type
{};

template<class T, class A>
struct is_random_access_standard_container<std::vector<T, A>> : public true_type
{};

//------
//
template<class T> inline constexpr
bool    is_random_access_standard_container_v = is_random_access_standard_container<T>::value;


//==================================================================================================
//  TYPES AND TRAITS DEFINITIONS -- MDSPAN-RELATED
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//  Trait:      is_1d_mdspan<T>
//  Variable:   is_1d_mdspan_v<T>
//
//  This private traits type determines whether its template parameter is a specialization of
//  basic_mdspan<T, X, L, A> with a one-dimensional extents template parameter.
//--------------------------------------------------------------------------------------------------
//
template<class T>
struct is_1d_mdspan : public false_type
{};

template<class T, ptrdiff_t X0, class SL, class SA>
struct is_1d_mdspan<basic_mdspan<T, extents<X0>, SL, SA>> : public true_type
{};

//------
//
template<class T> inline constexpr
bool    is_1d_mdspan_v = is_1d_mdspan<T>::value;


//--------------------------------------------------------------------------------------------------
//  Trait:      is_2d_mdspan<T>
//  Variable:   is_2d_mdspan_v<T>
//
//  This private traits type determines whether its template parameter is a specialization of
//  basic_mdspan<T, X, L, A> with a two-dimensional extents template parameter.
//--------------------------------------------------------------------------------------------------
//
template<class T>
struct is_2d_mdspan : public false_type
{};

template<class T, ptrdiff_t X0, ptrdiff_t X1, class SL, class SA>
struct is_2d_mdspan<basic_mdspan<T, extents<X0, X1>, SL, SA>> : public true_type
{};

//------
//
template<class T> inline constexpr
bool    is_2d_mdspan_v = is_2d_mdspan<T>::value;


//--------------------------------------------------------------------------------------------------
//  Trait:      mdspan_layout_mapper<T>
//  Alias:      get_mdspan_layout_t<T>
//
//  This private traits type maps a linear algebra element layout (row_major, etc.) into a
//  corresponding mdspan layout policy (layout_right, etc.).
//--------------------------------------------------------------------------------------------------
//
template<class L>   struct mdspan_layout_mapper;

template<>
struct mdspan_layout_mapper<matrix_layout::row_major>
{
    using layout_type = MDSPAN_NS::layout_right;
};

template<>
struct mdspan_layout_mapper<matrix_layout::column_major>
{
    using layout_type = MDSPAN_NS::layout_left;
};

template<>
struct mdspan_layout_mapper<void>
{
    using layout_type = MDSPAN_NS::layout_right;
};

//------
//
template<class L>
using get_mdspan_layout_t = typename mdspan_layout_mapper<L>::layout_type;


//--------------------------------------------------------------------------------------------------
//  Aliases:    dyn_mat_extents
//              dyn_mat_strides
//              dyn_mat_layout
//              dyn_mat_mapping
//              dyn_vec_extents
//              dyn_vec_strides
//              dyn_vec_layout
//              dyn_vec_mapping
//
//  These private aliases are used to reduce verbosity in subsequent code pertaining to mdspan
//  types and objects.
//--------------------------------------------------------------------------------------------------
//
using dyn_mat_extents = extents<dynamic_extent, dynamic_extent>;
using dyn_mat_strides = array<typename dyn_mat_extents::index_type, 2>;
using dyn_mat_layout  = layout_stride<dynamic_extent, dynamic_extent>;
using dyn_mat_mapping = typename dyn_mat_layout::template mapping<dyn_mat_extents>;

using dyn_vec_extents = extents<dynamic_extent>;
using dyn_vec_strides = array<typename dyn_vec_extents::index_type, 1>;
using dyn_vec_layout  = layout_stride<dynamic_extent>;
using dyn_vec_mapping = typename dyn_mat_layout::template mapping<dyn_vec_extents>;


//==================================================================================================
//  TYPES AND TRAITS DEFINITIONS -- ENGINE-RELATED
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//  Trait:      is_valid_engine_extents<X>
//  Variable:   is_valid_engine_extents_v<X>
//
//  This private traits type is used to validate an engine's extents template argument.  The
//  extents parameter must be one- or two-dimensional, and each dimension's template argument
//  must have a value greater than zero or equal to dynamic_extent.
//--------------------------------------------------------------------------------------------------
//
template<class X>
struct is_valid_engine_extents : public false_type {};

template<ptrdiff_t N>
struct is_valid_engine_extents<extents<N>>
{
    static constexpr bool   value = (N == dynamic_extent || N > 0);
};

template<ptrdiff_t R, ptrdiff_t C>
struct is_valid_engine_extents<extents<R, C>>
{
    static constexpr bool   value = (R == dynamic_extent || R > 0) && (C == dynamic_extent || C > 0);
};

//------
//
template<class T> inline constexpr
bool    is_valid_engine_extents_v = is_valid_engine_extents<T>::value;


//--------------------------------------------------------------------------------------------------
//  Trait:      is_valid_fixed_engine_extents<X>
//  Variable:   is_valid_fixed_engine_extents_v<X>
//
//  This private traits type is used to validate an engine's extents template argument.  The
//  extents parameter must be one- or two-dimensional, and each dimension's template argument
//  must have a value greater than zero.
//--------------------------------------------------------------------------------------------------
//
template<class X>
struct is_valid_fixed_engine_extents : public std::false_type {};

template<ptrdiff_t N>
struct is_valid_fixed_engine_extents<extents<N>>
{
    static constexpr bool   value = (N > 0);
};

template<ptrdiff_t R, ptrdiff_t C>
struct is_valid_fixed_engine_extents<extents<R, C>>
{
    static constexpr bool   value = (R > 0) && (C > 0);
};

//------
//
template<class X> inline constexpr
bool    is_valid_fixed_engine_extents_v = is_valid_fixed_engine_extents<X>::value;


//--------------------------------------------------------------------------------------------------
//  Trait:      has_owning_engine_type_alias<ET>
//  Alias:      get_owning_engine_type_t<ET>
//  Variable:   is_owning_engine_type_v<ET>
//
//  This private type detector and variable template are used to determine whether or not an
//  engine type is an owning engine.  An owning engine is one that owns (manages the lifetime of)
//  the elements it contains.  An owning engine is indicated by the fact that it and does not
//  have a public nested type alias owning_engine_type.
//
//  A non-owning engine refers to elements managed by another engine and has a public nested type
//  alias owning_engine_type.  Non-owning engines (views) may refer to other non-owning engines,
//  but ultimately, at the end of the "view chain", the bottom-most non-owning engine must refer
//  to an owning engine.  The owning_engine_type alias indicates the type of the owning engine.
//--------------------------------------------------------------------------------------------------
//
template<class ET, typename = void>
struct has_owning_engine_type_alias
:   public std::false_type
{
    static constexpr bool   is_owning = true;
    using owning_engine_type = ET;
};

template<class ET>
struct has_owning_engine_type_alias<ET, void_t<typename ET::owning_engine_type>>
:   public std::true_type
{
    static constexpr bool   is_owning = false;
    using owning_engine_type = typename ET::owning_engine_type;
};

//------
//
template<class ET>
using get_owning_engine_type_t = typename has_owning_engine_type_alias<ET>::owning_engine_type;

template<class ET> inline constexpr
bool    is_owning_engine_type_v = has_owning_engine_type_alias<ET>::is_owning;


//--------------------------------------------------------------------------------------------------
//  Trait:      has_nested_mdspan_types<T>
//  Aliases:    get_mdspan_type_t<T> and get_const_mdspan_type_t<T>
//
//  This private traits type and the associated alias templates determine whether or not the
//  template parameter type has nested type aliases "span_type" and "const_span_type".  If both
//  aliases are present, then the associated aliases templates refer to the corresponding types.
//  Otherwise, the alias templates refer to the void type.
//--------------------------------------------------------------------------------------------------
//
template<class ET, class = void>
struct has_nested_mdspan_types
{
    using span_type       = void;
    using const_span_type = void;
};

template<class ET>
struct has_nested_mdspan_types<ET, void_t<typename ET::span_type, typename ET::const_span_type>>
{
    using span_type       = typename ET::span_type;
    using const_span_type = typename ET::const_span_type;
};

//------
//
template<class ET>
using get_mdspan_type_t = typename has_nested_mdspan_types<ET>::span_type;

template<class ET>
using get_const_mdspan_type_t = typename has_nested_mdspan_types<ET>::const_span_type;


//--------------------------------------------------------------------------------------------------
//  Traits:     get_row_major_value_helper<bool, ET>
//              determine_indexing_order<ET>
//  Variable:   use_row_wise_indexing_v<ET>
//
//  These private traits types and the associated variable template are used to help determine
//  the order in which matrix elements should be accessed when iterating over them in two
//  dimensions.
//--------------------------------------------------------------------------------------------------
//
template<bool P, class ET> struct get_row_major_value_helper;

template<class ET>
struct get_row_major_value_helper<false, ET>
{
    static constexpr bool   is_row_major = true;
};

template<class ET>
struct get_row_major_value_helper<true, ET>
{
    static constexpr bool   is_row_major = static_cast<bool>(ET::is_row_major);
};

//------
//
template<class ET, class = void>
struct determine_indexing_order
{
    static constexpr bool   use_row_wise = true;
};

template<class ET>
struct determine_indexing_order<ET, std::void_t<decltype(ET::is_row_major)>>
{
    static constexpr bool   is_suitable  = std::is_convertible_v<decltype(ET::is_row_major), bool>;
    static constexpr bool   use_row_wise = get_row_major_value_helper<is_suitable, ET>::is_row_major;
};

template<class T, ptrdiff_t R, ptrdiff_t C, class A>
struct determine_indexing_order<matrix_storage_engine<T, extents<R, C>, A, matrix_layout::column_major>, void>
{
    static constexpr bool   use_row_wise = false;
};

template<class T, ptrdiff_t R, ptrdiff_t C, class A>
struct determine_indexing_order<matrix_storage_engine<T, extents<R, C>, A, matrix_layout::row_major>, void>
{
    static constexpr bool   use_row_wise = true;
};

//------
//
template<class ET> inline constexpr
bool    use_row_wise_indexing_v = determine_indexing_order<ET>::use_row_wise;


//==================================================================================================
//  CONCEPT DEFINITIONS
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//  Concept:    class_type<T>
//
//  This private concept determines whether its parameter is a non-union class type.
//--------------------------------------------------------------------------------------------------
//
template<class T>
concept class_type = std::is_class_v<T>;


//--------------------------------------------------------------------------------------------------
//  Concepts:   convertible_from<DST, SRC>
//              constructible_from<DST, SRC>
//              assignable_from<DST, SRC>
//
//              not_convertible_from<DST, SRC>
//              not_constructible_from<DST, SRC>
//              not_assignable_from<DST, SRC>
//
//  These are some simple private concepts, and their logical complements, that simply wrap
//  corresponding standard concepts from namespace std.  This wrapping occurs because some of
//  the tested compilers do not yet implement the standard concepts library in <concepts>.
//
//  If <concepts> exists, this implementation wraps the concepts it uses; if <concepts> does not
//  exist, then we fake it.  The first reason for wrapping is consistency and readability; the
//  second reason is to keep the temporary "fake" concepts out of the std namespace.
//--------------------------------------------------------------------------------------------------
//
#if defined(LA_STD_CONCEPTS_HEADER_SUPPORTED)

    template<class T, class U>
    concept same_as = std::same_as<T, U>;

    template<class T>
    concept default_initializable = std::default_initializable<T>;

    template<class T>
    concept copyable = std::copyable<T>;

    template<class DST, class SRC>
    concept convertible_from = std::convertible_to<SRC, DST>;

    template<class DST, class SRC>
    concept constructible_from = std::constructible_from<DST, SRC>;

    template<class DST, class SRC>
    concept assignable_from = std::assignable_from<DST&, SRC>;

    template<class DST, class SRC>
    concept not_convertible_from = not std::convertible_to<SRC, DST>;

    template<class DST, class SRC>
    concept not_constructible_from = not std::constructible_from<DST, SRC>;

    template<class DST, class SRC>
    concept not_assignable_from = not std::assignable_from<DST&, SRC>;

#else

    template<class T, class U>
    concept same_as_helper = std::is_same_v<T, U>;

    template<class T, class U>
    concept same_as = same_as_helper<T, U> and same_as_helper<U, T>;

    template <class From, class To>
    concept convertible_to =
        std::is_convertible_v<From, To> and
        requires(add_rvalue_reference_t<From>(&f)())
        {
            static_cast<To>(f());
        };

    template<class DST, class SRC>
    concept convertible_from = convertible_to<SRC, DST>;

    //- Rather than try to re-implement these complex library concepts here, there are instead
    //  implemented as tautologies for compilers which don't yet have complete implementations
    //  of the standard concepts defined in <concepts>.
    //
    template<class T>
    concept default_initializable = true;

    template<class T>
    concept copyable = true;

    template <class T>
    concept destructible = std::is_nothrow_destructible_v<T>;

    template<class T, class... Args>
    concept constructible_from = destructible<T> and std::is_constructible_v<T, Args...>;

    template<class DST, class SRC>
    concept assignable_from = std::is_assignable_v<DST&, SRC>;

    template<class DST, class SRC>
    concept not_convertible_from = not convertible_from<DST, SRC>;

    template<class DST, class SRC>
    concept not_constructible_from = not constructible_from<DST, SRC>;

    template<class DST, class SRC>
    concept not_assignable_from = not assignable_from<DST, SRC>;

#endif


template<class T, class U1, class U2>
concept same_as_either = same_as<T, U1> or same_as<T, U2>;

//--------------------------------------------------------------------------------------------------
//  Concept:    comparable_types<T1, T2>
//
//  This private concept determines whether two objects, of potentially different types, may
//  be compared using equality operator.
//--------------------------------------------------------------------------------------------------
//
template<class T1, class T2>
concept comparable_types_helper =
    requires (T1 t1, T2 t2)
    {
        { t1 == t2 } -> same_as<bool>;
    };

template<class T1, class T2>
concept comparable_types = comparable_types_helper<T1, T2> and comparable_types_helper<T2, T1>;


//--------------------------------------------------------------------------------------------------
//  Concept:    random_access_standard_container<CT>
//
//  This private concept determines whether a type is a specialization of std::array, std::deque,
//  or std::vector.
//--------------------------------------------------------------------------------------------------
//
template<class CT>
concept random_access_standard_container = is_random_access_standard_container_v<CT>;


//--------------------------------------------------------------------------------------------------
//  Concepts:   valid_engine_extents<X>
//
//  This private concept is used to validate the second template parameter of a specialization
//  of matrix_storage_engine, the engine's extents type.
//--------------------------------------------------------------------------------------------------
//
template<typename X>
concept valid_engine_extents = is_valid_engine_extents_v<X>;


//--------------------------------------------------------------------------------------------------
//  Concepts:   valid_fixed_engine_extents<X>
//
//  This private concept is used to validate the second template parameter of a specialization
//  of matrix_storage_engine, the engine's extents type for non-resizable engines
//--------------------------------------------------------------------------------------------------
//
template<typename X>
concept valid_fixed_engine_extents = is_valid_fixed_engine_extents_v<X>;


//--------------------------------------------------------------------------------------------------
//  Concept:    valid_allocator<T, A>
//
//  This private concept determines whether a given type A is an acceptable allocator of type T.
//  Prospective allocator type A must either: be void; or, it must be possible to instantiate a
//  specialization of allocator_traits<A> that meets certain requirements relative to allocated
//  type T, such as allocator_traits<A>::value_type is the same type as T, etc.
//--------------------------------------------------------------------------------------------------
//
template<typename T, typename A>
concept valid_allocator_interface =
    requires (A a, get_size_type_t<A> n)
    {
        typename A::value_type;
        requires std::is_same_v<T, typename A::value_type>;
        { *(a.allocate(n))               } -> same_as<T&>;
        { a.deallocate(a.allocate(n), n) };
    };


template<typename T, typename AT>
concept valid_allocator_traits =
    requires
    {
        typename AT::allocator_type;
        typename AT::value_type;
        typename AT::size_type;
        typename AT::pointer;
        typename AT::template rebind_alloc<T>;
        requires std::is_same_v<T, typename AT::value_type>;
    }
    and
    requires (typename AT::allocator_type a, typename AT::pointer p, typename AT::size_type n)
    {
        { AT::deallocate(a, p, n) };
        { AT::allocate(a, n) } -> same_as<typename AT::pointer>;
        { static_cast<T*>(p) };
        { *p   } -> same_as<T&>;
        { p[n] } -> same_as<T&>;
    };

template<typename AT>
concept no_allocator = same_as<AT, void>;

template<typename T, typename A>
concept valid_allocator_arg = no_allocator<A> or valid_allocator_interface<T, A>;


//--------------------------------------------------------------------------------------------------
//  Concept:    valid_engine_extents_and_allocator<T, X, A>
//
//  This private concept determines whether a given combination of element type T, allocator A,
//  and extents X is acceptable for matrix_storage_engine.  There are valid two possibilities:
//  1. There is no allocator and the extents are greater than zero in all dimensions;
//  2. There is a valid allocator, the extent arguments are valid for fixed-size or dynamic-sized.
//--------------------------------------------------------------------------------------------------
//
template<typename T, typename X, typename A>
concept valid_engine_extents_and_allocator =
    (no_allocator<A> and valid_fixed_engine_extents<X>)
    or
    (valid_allocator_interface<T, A> and valid_engine_extents<X>);


//--------------------------------------------------------------------------------------------------
//  Concepts:   valid_layout_for_2d_storage_engine<L>
//              valid_layout_for_1d_storage_engine<L>
//
//  These private concepts are used to validate the fourth template parameter of a specialization
//  of matrix_storage_engine, the engine's layout type.  It must be row_major or column_major for
//  matrix storage engines, and unoriented for vector storage engines.
//--------------------------------------------------------------------------------------------------
//
template<typename L>
concept valid_layout_for_2d_storage_engine =
    (std::is_same_v<L, matrix_layout::row_major> || std::is_same_v<L, matrix_layout::column_major>);

template<typename L>
concept valid_layout_for_1d_storage_engine = std::is_same_v<L, void>;


//--------------------------------------------------------------------------------------------------
//  Concept:    readable_engine_fundamentals<ET>
//
//  This private concept determines whether a prospective vector/matrix engine type provides the
//  minimum set of public nested type aliases and member functions required by all engines.
//--------------------------------------------------------------------------------------------------
//
template<class ET>
concept readable_engine_fundamentals =
    requires (ET const& eng)
    {
        typename ET::element_type;
        typename ET::index_type;
        typename ET::reference;
        typename ET::const_reference;
        requires std::is_convertible_v<typename ET::reference, typename ET::element_type>;
        requires std::is_convertible_v<typename ET::const_reference, typename ET::element_type>;
        { eng.size()     } -> same_as<typename ET::index_type>;
        { eng.capacity() } -> same_as<typename ET::index_type>;
    };


//--------------------------------------------------------------------------------------------------
//  Concept:    valid_const_indexing_result<ET>
//
//  This private concept determines whether a given type is equal to one of the three possible
//  result types of an engine's const indexing operator(s).
//
//  Note that non-const references are acceptable as an indexing return type.  This is in order
//  to support view engines, which may have a const indexing operator that returns a non-const
//  reference to an element.
//--------------------------------------------------------------------------------------------------
//
template<class T, class ET>
concept valid_const_indexing_result =
    same_as<T, typename ET::const_reference>
    or
    same_as<T, typename ET::reference>
    or
    same_as<T, typename ET::element_type>;


//--------------------------------------------------------------------------------------------------
//  Concept:    valid_mutable_indexing_result<ET>
//
//  This private concept determines whether a given type is an acceptable result types of an
//  engine's mutable indexing operator(s).
//--------------------------------------------------------------------------------------------------
//
template<class T, class ET>
concept valid_mutable_indexing_result =
    same_as<typename ET::reference, typename ET::element_type&>
    and
    same_as<T, typename ET::reference>;


//--------------------------------------------------------------------------------------------------
//  Concept:    engine_has_valid_1d_const_indexing<ET>
//
//  This private concept determines whether a given engine type has one-dimensional indexing
//  for const objects of that type, and that the return type fulfills the requirements of the
//  valid_const_indexing_result<ET> concept.
//--------------------------------------------------------------------------------------------------
//
template<class ET>
concept engine_has_valid_1d_const_indexing =
    requires (ET const& eng, typename ET::index_type i)
    {
        { eng(i) } -> valid_const_indexing_result<ET>;
    };


//--------------------------------------------------------------------------------------------------
//  Concept:    engine_has_valid_1d_mutable_indexing<ET>
//
//  This private concept determines whether a given engine type has one-dimensional indexing
//  for non-const objects of that type, and that the return type is a reference.
//--------------------------------------------------------------------------------------------------
//
template<class ET>
concept engine_has_valid_1d_mutable_indexing =
    requires (ET& eng, typename ET::index_type i)
    {
        { eng(i) } -> valid_mutable_indexing_result<ET>;
    };


//--------------------------------------------------------------------------------------------------
//  Concept:    concept engine_is_not_1d_indexable<ET>
//
//  This private concept determines whether a given engine type does not have one-dimensional
//  indexing for const objects of that type.
//--------------------------------------------------------------------------------------------------
//
template<class ET>
concept engine_is_not_1d_indexable =
    not requires(ET eng, typename ET::index_type i)
    {
        { eng(i) };
    };


//--------------------------------------------------------------------------------------------------
//  Concept:    engine_has_valid_2d_const_indexing<ET>
//
//  This private concept determines whether a given engine type has two-dimensional indexing
//  for const objects of that type, and that the return type fulfills the requirements of the
//  valid_const_indexing_result<ET> concept.
//--------------------------------------------------------------------------------------------------
//
template<class ET>
concept engine_has_valid_2d_const_indexing =
    requires (ET const& eng, typename ET::index_type i)
    {
        { eng(i, i) } -> valid_const_indexing_result<ET>;
    };


//--------------------------------------------------------------------------------------------------
//  Concept:    engine_has_valid_2d_mutable_indexing<ET>
//
//  This private concept determines whether a given engine type has two-dimensional indexing
//  for non-const objects of that type, and that the return type is a reference.
//--------------------------------------------------------------------------------------------------
//
template<class ET>
concept engine_has_valid_2d_mutable_indexing =
    requires (ET& eng, typename ET::index_type i)
    {
        { eng(i, i) } -> valid_mutable_indexing_result<ET>;
    };

//--------------------------------------------------------------------------------------------------
//  Concept:    concept engine_is_not_2d_indexable<ET>
//
//  This private concept determines whether a given engine type does not have two-dimensional
//  indexing for const objects of that type.
//--------------------------------------------------------------------------------------------------
//
template<class ET>
concept engine_is_not_2d_indexable =
    not requires(ET eng, typename ET::index_type i)
    {
        { eng(i, i) };
    };


//--------------------------------------------------------------------------------------------------
//  Concept:    readable_vector_engine<ET>
//
//  This private concept determines whether a prospective vector engine type provides the
//  readability interface required to function correctly with basic_vector<ET, OT>.
//
//  Engine types that fulfill this concept may have the value of their elements read via one-
//  dimensional indexing, their element sizes and capacities read, and publicly declare several
//  important nested type aliases.  They do not have two-dimensional indexing.
//--------------------------------------------------------------------------------------------------
//
template<class ET>
concept readable_vector_engine =
    readable_engine_fundamentals<ET>
    and
    engine_has_valid_1d_const_indexing<ET>
    and
    engine_is_not_2d_indexable<ET>;


//--------------------------------------------------------------------------------------------------
//  Concept:    spannable_vector_engine<ET>
//
//  This private concept determines whether a prospective vector engine type provides the
//  correct nested mdspan types optionally required by basic_vector<ET, OT>.
//--------------------------------------------------------------------------------------------------
//
template<class ET>
concept spannable_vector_engine =
    readable_vector_engine<ET>
    and
    requires (ET const& ceng, ET& meng)
    {
        typename ET::span_type;
        typename ET::const_span_type;
        requires is_1d_mdspan_v<typename ET::span_type>;
        requires is_1d_mdspan_v<typename ET::const_span_type>;
        { meng.span() } -> same_as<typename ET::span_type>;
        { ceng.span() } -> same_as_either<typename ET::const_span_type, typename ET::span_type>;
    };


//--------------------------------------------------------------------------------------------------
//  Concept:    writable_vector_engine<ET>
//
//  This private concept determines whether a prospective vector engine type provides the
//  writability interface required to function correctly with basic_vector<ET, OT>.
//
//  Engine types that fulfill this concept fulfill the corresponding readable_vector_engine<ET>
//  concept, and also permit the value of their elements to be changed via one-dimensional
//  indexing.
//--------------------------------------------------------------------------------------------------
//
template<class ET>
concept writable_vector_engine =
    readable_vector_engine<ET>
    and
    engine_has_valid_1d_mutable_indexing<ET>;


//--------------------------------------------------------------------------------------------------
//  Concept:    readable_matrix_engine<ET>
//
//  This private concept determines whether a prospective matrix engine type provides the
//  readability interface required to function correctly with basic_matrix<ET, OT>.
//
//  Engine types that fulfill this concept may have the value of their elements read via two-
//  dimensional indexing, their row and column sizes and capacities read, their overall sizes
//  and capacities read, and publicly declare several important nested type aliases.  They may
//  also support one-dimensional indexing, if the number of rows/columns is fixed at 1 at
//  compile-time.
//--------------------------------------------------------------------------------------------------
//
template<class ET>
concept readable_matrix_engine =
    readable_engine_fundamentals<ET>
    and
    engine_has_valid_2d_const_indexing<ET>
    and
    requires (ET const& eng)
    {
        { eng.columns()          } -> same_as<typename ET::index_type>;
        { eng.rows()             } -> same_as<typename ET::index_type>;
        { eng.column_capacity()  } -> same_as<typename ET::index_type>;
        { eng.row_capacity()     } -> same_as<typename ET::index_type>;
    };


//--------------------------------------------------------------------------------------------------
//  Concept:    readable_and_1d_indexable_matrix_engine<ET>
//
//  This private concept determines whether a prospective matrix engine type fulfills all the
//  requirements of readable_matrix_engine<ET> and additionally provides one-dimensional read
//  indexing.  Such engine types may be used to represent matrices whose number of rows/columns
//  is fixed at 1 at compile-time.
//--------------------------------------------------------------------------------------------------
//
template<class ET>
concept readable_and_1d_indexable_matrix_engine =
    readable_matrix_engine<ET>
    and
    engine_has_valid_1d_const_indexing<ET>;


//--------------------------------------------------------------------------------------------------
//  Concept:    spannable_matrix_engine<ET>
//
//  This private concept determines whether a prospective matrix engine type provides the
//  correct nested mdspan types optionally required by basic_matrix<ET, OT>.
//--------------------------------------------------------------------------------------------------
//
template<class ET>
concept spannable_matrix_engine =
    readable_matrix_engine<ET>
    and
    requires (ET const& ceng, ET& meng)
    {
        typename ET::span_type;
        typename ET::const_span_type;
        requires is_2d_mdspan_v<typename ET::span_type>;
        requires is_2d_mdspan_v<typename ET::const_span_type>;
        { meng.span() } -> same_as<typename ET::span_type>;
        { ceng.span() } -> same_as_either<typename ET::const_span_type, typename ET::span_type>;
    };


//--------------------------------------------------------------------------------------------------
//  Concept:    writable_matrix_engine<ET>
//
//  This private concept determines whether a prospective matrix engine type provides the
//  writability interface required to function correctly with basic_matrix<ET, OT>.
//
//  Engine types that fulfill this concept fulfill the corresponding readable_matrix_engine<ET>
//  concept, and also permit the value of their elements to be changed via two-dimensional
//  indexing.
//--------------------------------------------------------------------------------------------------
//
template<class ET>
concept writable_matrix_engine =
    readable_matrix_engine<ET>
    and
    engine_has_valid_2d_mutable_indexing<ET>;


//--------------------------------------------------------------------------------------------------
//  Concept:    writable_and_1d_indexable_matrix_engine<ET>
//
//  This private concept determines whether a prospective matrix engine type fulfills all the
//  requirements of writable_matrix_engine<ET>, readable_and_1d_indexable_matrix_engine<ET>,
//  and additionally provides one-dimensional write indexing.  Such engine types may be used
//  to represent matrices whose number of rows/columns is fixed at 1 at compile-time.
//--------------------------------------------------------------------------------------------------
//
template<class ET>
concept writable_and_1d_indexable_matrix_engine =
    writable_matrix_engine<ET>
    and
    engine_has_valid_1d_const_indexing<ET>
    and
    engine_has_valid_1d_mutable_indexing<ET>;


//--------------------------------------------------------------------------------------------------
//  Concepts:   reshapable_vector_engine<ET>
//
//  This private concept determines whether a prospective vector engine type provides an interface
//  for overall reshaping.
//--------------------------------------------------------------------------------------------------
//
template<class ET>
concept reshapable_vector_engine =
    writable_vector_engine<ET>
    and
    requires (ET& eng, typename ET::index_type i)
    {
        { eng.reshape(i, i) };
    };

//--------------------------------------------------------------------------------------------------
//  Concepts:   reshapable_matrix_engine<ET>
//              column_reshapable_matrix_engine<ET>
//              row_reshapable_matrix_engine<ET>
//
//  These private concepts determine whether a prospective matrix engine type provides interfaces
//  for overall reshaping, column reshaping, and/or row reshaping.
//--------------------------------------------------------------------------------------------------
//
template<class ET>
concept reshapable_matrix_engine =
    writable_matrix_engine<ET>
    and
    requires (ET& eng, typename ET::index_type i)
    {
        { eng.reshape(i, i, i, i) };
    };

template<class ET>
concept column_reshapable_matrix_engine =
    writable_matrix_engine<ET>
    and
    requires (ET& eng, typename ET::index_type i)
    {
        { eng.reshape_columns(i, i) };
    };

template<class ET>
concept row_reshapable_matrix_engine =
    writable_matrix_engine<ET>
    and
    requires (ET& eng, typename ET::index_type i)
    {
        { eng.reshape_rows(i, i) };
    };


//--------------------------------------------------------------------------------------------------
//  Concept:    do_row_wise_indexing<ET>
//
//  This private concept determines whether to use row-wise indexing for the inner loop when
//  performing two-dimensional iteration over the elements of a matrix engine.
//--------------------------------------------------------------------------------------------------
//
template<class ET>
concept do_row_wise_indexing = readable_matrix_engine<ET>  and  use_row_wise_indexing_v<ET>;



//--------------------------------------------------------------------------------------------------
//  Concept:    similar_engines<ET1, ET2>
//
//  This private concept determines whether both prospective engine types represent matrix engines
//  or vector engines.  The motivation for this concept is matrix/vector addition/subtraction where
//  engine arities must match.
//--------------------------------------------------------------------------------------------------
//
template<class ET1, class ET2>
concept similar_engines =
    (readable_matrix_engine<ET1> and readable_matrix_engine<ET2>)
    or
    (readable_vector_engine<ET1> and readable_vector_engine<ET2>);


//==================================================================================================
//  SUPPORT TYPE DEFINITIONS
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//  Class Template:     common_engine_support
//
//  Policy/traits base type that provides swapping and several functions for verification.
//--------------------------------------------------------------------------------------------------
//
struct common_engine_support
{
    template<class N1, class N2>
    static constexpr bool
    sizes_differ(N1 n1, N2 n2) noexcept
    {
        using cmp_type = common_type_t<N1, N2, ptrdiff_t>;

        return (static_cast<cmp_type>(n1) != static_cast<cmp_type>(n2));
    }

    template<class R1, class C1, class R2, class C2>
    static constexpr bool
    sizes_differ(R1 r1, C1 c1, R2 r2, C2 c2) noexcept
    {
        using cmp_type = common_type_t<R1, C1, R2, C2, ptrdiff_t>;

        return ((static_cast<cmp_type>(r1) != static_cast<cmp_type>(r2)) ||
                (static_cast<cmp_type>(c1) != static_cast<cmp_type>(c2)));
    }

    template<class N>
    static constexpr void
    verify_capacity(N c)
    {
        if (c < static_cast<N>(0))
        {
            throw runtime_error("invalid capacity parameter");
        }
    }

    template<class U>
    static constexpr std::tuple<ptrdiff_t, ptrdiff_t>
    verify_list(initializer_list<initializer_list<U>> list)
    {
        size_t  rows = list.size();
        size_t  cols = list.begin()->size();

        for (auto row : list)
        {
            if (row.size() != cols)
            {
                throw runtime_error("matrix initializer_list has invalid shape");
            }
        }
        return {static_cast<ptrdiff_t>(rows), static_cast<ptrdiff_t>(cols)};
    }

    template<class N>
    static constexpr void
    verify_size(N s)
    {
        if (s < static_cast<N>(1))
        {
            throw runtime_error("invalid size parameter");
        }
    }

    template<class N1, class N2>
    static constexpr void
    verify_size(N1 s1, N2 s2)
    {
        if (sizes_differ(s1, s2))
        {
            throw runtime_error("invalid size parameter");
        }
    }

    template<class T>
    static constexpr void
    swap(T& t0, T& t1)
        noexcept(std::is_nothrow_move_constructible_v<T>  &&  std::is_nothrow_move_assignable_v<T>)
    {
        if (&t0 != & t1)
        {
            T   t2(std::move(t0));
            t0 = std::move(t1);
            t1 = std::move(t2);
        }
    }

};


//--------------------------------------------------------------------------------------------------
//  Class:      vector_engine_support
//
//  Provides various utility functions for vector engines.
//--------------------------------------------------------------------------------------------------
//
struct vector_engine_support : public common_engine_support
{
    //----------------------------------------------------------------------------------------------
    //  This member function performs reshaping where requested and possible; otherwise, it
    //  peforms size verification.
    //----------------------------------------------------------------------------------------------
    //
    template<class ET, class IT>
    static constexpr void
    verify_and_reshape(ET& dst, IT src_elems)
    requires
        writable_vector_engine<ET>
    {
        auto    elems = static_cast<typename ET::index_type>(src_elems);

        if constexpr (reshapable_vector_engine<ET>)
        {
            dst.reshape(elems, dst.capacity());
        }
        else
        {
            verify_size(dst.size(), elems);
        }
    }

    //----------------------------------------------------------------------------------------------
    //  These member functions assign to the contents of vector engines from those of other vector
    //  engines, random-access standard containers, 1-D mdspans, and 1-D initializer lists.
    //----------------------------------------------------------------------------------------------
    //
    template<class ET1, class ET2>
    static constexpr void
    assign_from(ET1& dst, ET2 const& src)
    requires
        writable_vector_engine<ET1>
        and
        readable_vector_engine<ET2>
        and
        convertible_from<typename ET1::element_type, typename ET2::element_type>
    {
        using index_type_dst = typename ET1::index_type;
        using index_type_src = typename ET2::index_type;

        index_type_dst  di = 0;
        index_type_src  si = 0;
        index_type_src  sn = src.size();

        verify_and_reshape(dst, sn);

        for (;  si < sn;  ++di, ++si)
        {
            dst(di) = static_cast<typename ET1::element_type>(src(si));
        }
    }

    template<class ET, class CT>
    static constexpr void
    assign_from(ET& dst, CT const& src)
    requires
        writable_vector_engine<ET>
        and
        random_access_standard_container<CT>
        and
        convertible_from<typename ET::element_type, typename CT::value_type>
    {
        using index_type_dst = typename ET::index_type;
        using index_type_src = typename CT::size_type;

        index_type_dst  di = 0;
        index_type_src  si = 0;
        index_type_src  sn = src.size();

        verify_and_reshape(dst, sn);

        for (;  si < sn;  ++di, ++si)
        {
            dst(di) = static_cast<typename CT::value_type>(src[si]);
        }
    }

    template<class ET, class U, ptrdiff_t X0, class SL, class SA>
    static constexpr void
    assign_from(ET& dst, basic_mdspan<U, extents<X0>, SL, SA> const& src)
    requires
        writable_vector_engine<ET>
        and
        convertible_from<typename ET::element_type, U>
    {
        using index_type_dst = typename ET::index_type;
        using index_type_src = typename basic_mdspan<U, extents<X0>, SL, SA>::index_type;

        index_type_dst  di = 0;
        index_type_src  si = 0;
        index_type_src  sn = src.extent(0);

        verify_and_reshape(dst, sn);

        for (;  si < sn;  ++di, ++si)
        {
            dst(di) = static_cast<typename ET::element_type>(src(si));
        }
    }


    template<class ET, class U>
    static constexpr void
    assign_from(ET& dst, initializer_list<U> src)
    requires
        writable_vector_engine<ET>
        and
        convertible_from<typename ET::element_type, U>
    {
        using index_type_dst = typename ET::index_type;

        verify_and_reshape(dst, src.size());

        index_type_dst  di = 0;
        index_type_dst  dn = dst.size();
        auto            si = src.begin();

        for (;  di < dn;  ++di, ++si)
        {
            dst(di) = static_cast<typename ET::element_type>(*si);
        }
    }

    //----------------------------------------------------------------------------------------------
    //  These member functions fill and move ranges of elements, usually when reshaping.
    //----------------------------------------------------------------------------------------------
    //
    template<class ET, class JT0, class JT1, class T>
    static constexpr void
    fill(ET& dst, JT0 e0, JT1 e1, T const& t)
    requires
        writable_vector_engine<ET>
        and
        convertible_from<typename ET::element_type, T>
    {
        using index_type = typename ET::index_type;

        auto    i0 = static_cast<index_type>(e0);
        auto    i1 = static_cast<index_type>(e1);

        for (auto i = i0;  i < i1;  ++i)
        {
            dst(i) = static_cast<typename ET::element_type>(t);
        }
    }

    template<class ET, class IT>
    static constexpr void
    move_elements(ET& dst, ET& src, IT size)
    requires
        writable_vector_engine<ET>
    {
        using index_type = typename ET::index_type;

        auto    i0 = static_cast<index_type>(0);
        auto    i1 = static_cast<index_type>(size);

        for (auto i = i0;  i < i1;  ++i)
        {
            dst(i) = std::move(src(i));
        }
    }

    //----------------------------------------------------------------------------------------------
    //  These member functions compare the contents of vector engines with those of other vector
    //  engines, random-access standard containers, 1-D mdspans, and 1-D initializer lists.
    //----------------------------------------------------------------------------------------------
    //
    template<class ET1, class ET2>
    static constexpr bool
    compare(ET1 const& lhs, ET2 const& rhs)
    requires
        readable_vector_engine<ET1>
        and
        readable_vector_engine<ET2>
        and
        comparable_types<typename ET1::element_type, typename ET2::element_type>
    {
        using index_type_lhs = typename ET1::index_type;
        using index_type_rhs = typename ET2::index_type;

        index_type_lhs  n1 = lhs.size();
        index_type_rhs  n2 = rhs.size();

        if (sizes_differ(n1, n2)) return false;

        index_type_lhs  i1 = 0;
        index_type_rhs  i2 = 0;

        for (;  i1 < n1;  ++i1, ++i2)
        {
            if (not (lhs(i1) == rhs(i2))) return false;
        }
        return true;
    }

    template<class ET, class CT>
    static constexpr bool
    compare(ET const& lhs, CT const& rhs)
    requires
        readable_vector_engine<ET>
        and
        random_access_standard_container<CT>
        and
        comparable_types<typename ET::element_type, typename CT::value_type>
    {
        using index_type_lhs = typename ET::index_type;
        using index_type_rhs = typename CT::size_type;

        index_type_lhs  n1 = lhs.size();
        index_type_rhs  n2 = rhs.size();

        if (sizes_differ(n1, n2)) return false;

        index_type_lhs  i1 = 0;
        index_type_rhs  i2 = 0;

        for (;  i1 < n1;  ++i1, ++i2)
        {
            if (not (lhs(i1) == rhs[i2])) return false;
        }
        return true;
    }

    template<class ET, class U, ptrdiff_t X0, class SL, class SA>
    static constexpr bool
    compare(ET const& lhs, basic_mdspan<U, extents<X0>, SL, SA> const& rhs)
    requires
        readable_vector_engine<ET>
        and
        comparable_types<typename ET::element_type, U>
    {
        using index_type_lhs = typename ET::index_type;
        using index_type_rhs = typename basic_mdspan<U, extents<X0>, SL, SA>::index_type;

        index_type_lhs  n1 = lhs.size();
        index_type_rhs  n2 = rhs.extent(0);

        if (sizes_differ(n1, n2)) return false;

        index_type_lhs  i1 = 0;
        index_type_rhs  i2 = 0;

        for (;  i1 < n1;  ++i1, ++i2)
        {
            if (not (lhs(i1) == rhs(i2))) return false;
        }
        return true;
    }

    template<class ET, class U>
    static constexpr bool
    compare(ET const& lhs, initializer_list<U> rhs)
    requires
        readable_vector_engine<ET>
        and
        comparable_types<typename ET::element_type, U>
    {
        using index_type_lhs = typename ET::index_type;

        index_type_lhs  n1 = lhs.size();
        auto            n2 = rhs.size();

        if (sizes_differ(n1, n2)) return false;

        index_type_lhs  i1 = 0;
        auto            i2 = rhs.begin();

        for (;  i1 < n1;  ++i1, ++i2)
        {
            if (not (lhs(i1) == *i2)) return false;
        }
        return true;
    }
};


//--------------------------------------------------------------------------------------------------
//  Class:      matrix_engine_support
//
//  Provides various utility functions for matrix engines.
//--------------------------------------------------------------------------------------------------
//
struct matrix_engine_support : public common_engine_support
{
    //----------------------------------------------------------------------------------------------
    //  These member functions perform reshaping where requested and possible; otherwise, they
    //  peform size verifications.
    //----------------------------------------------------------------------------------------------
    //
    template<class ET, class IT1, class IT2>
    static constexpr void
    verify_and_reshape(ET& dst, IT1 src_rows, IT2 src_cols)
    requires
        writable_matrix_engine<ET>
    {
        auto    rows = static_cast<typename ET::index_type>(src_rows);
        auto    cols = static_cast<typename ET::index_type>(src_cols);

        if constexpr (reshapable_matrix_engine<ET>)
        {
            if (rows != dst.rows()  ||  cols != dst.columns())
            {
                dst.reshape(rows, cols, dst.row_capacity(), dst.column_capacity());
            }
        }
        else if constexpr (column_reshapable_matrix_engine<ET>)
        {
            if (verify_size(dst.rows(), rows);  cols != dst.columns())
            {
                dst.reshape_columns(cols, dst.column_capacity());
            }
        }
        else if constexpr (row_reshapable_matrix_engine<ET>)
        {
            if (verify_size(dst.columns(), cols);  rows != dst.rows())
            {
                dst.reshape_rows(rows, dst.row_capacity());
            }
        }
        else
        {
            verify_size(dst.rows(), rows);
            verify_size(dst.columns(), cols);
        }
    }

    template<class ET, class IT>
    static constexpr void
    verify_and_reshape(ET& dst, IT src_size)
    requires
        writable_and_1d_indexable_matrix_engine<ET>
    {
        auto    size = static_cast<typename ET::index_type>(src_size);

        if constexpr (column_reshapable_matrix_engine<ET>)
        {
            if (verify_size(dst.rows(), 1);  size != dst.columns())
            {
                dst.reshape_columns(size, dst.column_capacity());
            }
        }
        else if constexpr (row_reshapable_matrix_engine<ET>)
        {
            if (verify_size(dst.columns(), 1);  size != dst.rows())
            {
                dst.reshape_rows(size, dst.row_capacity());
            }
        }
        else
        {
            verify_size(dst.size(), size);
        }
    }

    //----------------------------------------------------------------------------------------------
    //  These member functions assign to the contents of matrix engines from those of other matrix
    //  engines, 2-D mdspans, and 2-D initializer lists.
    //----------------------------------------------------------------------------------------------
    //
    template<class ET1, class ET2>
    static constexpr void
    assign_from(ET1& dst, ET2 const& src)
    requires
        writable_matrix_engine<ET1>
        and
        readable_matrix_engine<ET2>
        and
        convertible_from<typename ET1::element_type, typename ET2::element_type>
    {
        using index_type_dst = typename ET1::index_type;
        using index_type_src = typename ET2::index_type;

        index_type_src  rows = src.rows();
        index_type_src  cols = src.columns();

        verify_and_reshape(dst, rows, cols);

        index_type_dst  di = 0;
        index_type_src  si = 0;

        for (; si < rows;  ++di, ++si)
        {
            index_type_dst  dj = 0;
            index_type_src  sj = 0;

            for (; sj < cols;  ++dj, ++sj)
            {
                dst(di, dj) = static_cast<typename ET1::element_type>(src(si, sj));
            }
        }
    }

    template<class ET, class U, ptrdiff_t X0, ptrdiff_t X1, class SL, class SA>
    static constexpr void
    assign_from(ET& dst, basic_mdspan<U, extents<X0, X1>, SL, SA> const& src)
    requires
        writable_matrix_engine<ET>
        and
        convertible_from<typename ET::element_type, U>
    {
        using index_type_dst = typename ET::index_type;
        using index_type_src = typename basic_mdspan<U, extents<X0, X1>, SL, SA>::index_type;

        index_type_src  rows = src.extent(0);
        index_type_src  cols = src.extent(1);

        verify_and_reshape(dst, rows, cols);

        index_type_dst  di = 0;
        index_type_src  si = 0;

        for (; si < rows;  ++di, ++si)
        {
            index_type_dst  dj = 0;
            index_type_src  sj = 0;

            for (; sj < cols;  ++dj, ++sj)
            {
                dst(di, dj) = static_cast<typename ET::element_type>(src(si, sj));
            }
        }
    }

    template<class ET, class U>
    static constexpr void
    assign_from(ET& dst, initializer_list<initializer_list<U>> src)
    requires
        writable_matrix_engine<ET>
        and
        convertible_from<typename ET::element_type, U>
    {
        using index_type_dst = typename ET::index_type;

        auto    [rows, cols] = verify_list(src);

        verify_and_reshape(dst, rows, cols);

        index_type_dst  di = 0;
        auto            si = src.begin();

        for (;  di < dst.rows();  ++di, ++si)
        {
            index_type_dst  dj = 0;
            auto            sj = si->begin();

            for (;  dj < dst.columns();  ++dj, ++sj)
            {
                dst(di, dj) = static_cast<typename ET::element_type>(*sj);
            }
        }
    }

    //----------------------------------------------------------------------------------------------
    //  These member functions assign to the contents of 1-D indexable matrix engines from those
    //  of 1-D vector engines, random-access standard containers, 1-D mdspans, and 1-D initializer
    //  lists.
    //----------------------------------------------------------------------------------------------
    //
    template<class ET1, class ET2>
    static constexpr void
    assign_from(ET1& dst, ET2 const& src)
    requires
        writable_and_1d_indexable_matrix_engine<ET1>
        and
        readable_vector_engine<ET2>
        and
        convertible_from<typename ET1::element_type, typename ET2::element_type>
    {
        using index_type_dst = typename ET1::index_type;
        using index_type_src = typename ET2::index_type;

        index_type_dst  di = 0;
        index_type_src  si = 0;
        index_type_src  sn = src.size();

        verify_and_reshape(dst, sn);

        for (;  si < sn;  ++di, ++si)
        {
            dst(di) = static_cast<typename ET1::element_type>(src(si));
        }
    }

    template<class ET, class CT>
    static constexpr void
    assign_from(ET& dst, CT const& src)
    requires
        writable_and_1d_indexable_matrix_engine<ET>
        and
        random_access_standard_container<CT>
        and
        convertible_from<typename ET::element_type, typename CT::value_type>
    {
        using index_type_dst = typename ET::index_type;
        using index_type_src = typename CT::size_type;

        index_type_dst  di = 0;
        index_type_src  si = 0;
        index_type_src  sn = src.size();

        verify_and_reshape(dst, sn);

        for (;  si < sn;  ++di, ++si)
        {
            dst(di) = static_cast<typename CT::value_type>(src[si]);
        }
    }

    template<class ET, class T, ptrdiff_t X0, class SL, class SA>
    static constexpr void
    assign_from(ET& dst, basic_mdspan<T, extents<X0>, SL, SA> const& src)
    requires
        writable_and_1d_indexable_matrix_engine<ET>
        and
        convertible_from<typename ET::element_type, T>
    {
        using index_type_dst = typename ET::index_type;
        using index_type_src = typename basic_mdspan<T, extents<X0>, SL, SA>::index_type;

        index_type_dst  di = 0;
        index_type_src  si = 0;
        index_type_src  sn = src.extent(0);

        verify_and_reshape(dst, sn);

        for (;  si < sn;  ++di, ++si)
        {
            dst(di) = static_cast<typename ET::element_type>(src(si));
        }
    }

    template<class ET, class T>
    static constexpr void
    assign_from(ET& dst, initializer_list<T> src)
    requires
        writable_and_1d_indexable_matrix_engine<ET>
        and
        convertible_from<typename ET::element_type, T>
    {
        using index_type_dst = typename ET::index_type;

        verify_and_reshape(dst, src.size());

        index_type_dst  di = 0;
        index_type_dst  dn = dst.size();
        auto            si = src.begin();

        for (;  di < dn;  ++di, ++si)
        {
            dst(di) = static_cast<typename ET::element_type>(*si);
        }
    }

    //----------------------------------------------------------------------------------------------
    //  These member functions fill ranges of rows, fill ranges of columns, and move ranges of
    //  elements, usually when reshaping.
    //----------------------------------------------------------------------------------------------
    //
    template<class ET, class JT0, class JT1, class T>
    static constexpr void
    fill_columns(ET& dst, JT0 c0, JT1 c1, T const& t)
    requires
        writable_matrix_engine<ET>
        and
        convertible_from<typename ET::element_type, T>
    {
        using index_type = typename ET::index_type;

        auto    i0 = static_cast<index_type>(0);
        auto    j0 = static_cast<index_type>(c0);
        auto    i1 = dst.rows();
        auto    j1 = static_cast<index_type>(c1);

        if constexpr (do_row_wise_indexing<ET>)
        {
            for (auto i = i0;  i < i1;  ++i)
            {
                for (auto j = j0;  j < j1;  ++j)
                {
                    dst(i, j) = static_cast<typename ET::element_type>(t);
                }
            }
        }
        else
        {
            for (auto j = j0;  j < j1;  ++j)
            {
                for (auto i = i0;  i < i1;  ++i)
                {
                    dst(i, j) = static_cast<typename ET::element_type>(t);
                }
            }
        }
    }

    //- Fill the specified range of rows of a writable engine with some value.
    //
    template<class ET, class IT0, class IT1, class T>
    static constexpr void
    fill_rows(ET& dst, IT0 r0, IT1 r1, T const& t)
    requires
        writable_matrix_engine<ET>
        and
        convertible_from<typename ET::element_type, T>
    {
        using index_type = typename ET::index_type;

        auto    i0 = static_cast<index_type>(r0);
        auto    j0 = static_cast<index_type>(0);
        auto    i1 = static_cast<index_type>(r1);
        auto    j1 = dst.columns();

        if constexpr (do_row_wise_indexing<ET>)
        {
            for (auto i = i0;  i < i1;  ++i)
            {
                for (auto j = j0;  j < j1;  ++j)
                {
                    dst(i, j) = static_cast<typename ET::element_type>(t);
                }
            }
        }
        else
        {
            for (auto j = j0;  j < j1;  ++j)
            {
                for (auto i = i0;  i < i1;  ++i)
                {
                    dst(i, j) = static_cast<typename ET::element_type>(t);
                }
            }
        }
    }

    template<class ET, class IT, class JT>
    static constexpr void
    move_elements(ET& dst, ET& src, IT rows, JT cols)
    requires
        writable_matrix_engine<ET>
    {
        using index_type = typename ET::index_type;

        auto    i0 = static_cast<index_type>(0);
        auto    j0 = static_cast<index_type>(0);
        auto    i1 = static_cast<index_type>(rows);
        auto    j1 = static_cast<index_type>(cols);

        if constexpr (do_row_wise_indexing<ET>)
        {
            for (auto i = i0;  i < i1;  ++i)
            {
                for (auto j = j0;  j < j1;  ++j)
                {
                    dst(i, j) = std::move(src(i, j));
                }
            }
        }
        else
        {
            for (auto j = j0;  j < j1;  ++j)
            {
                for (auto i = i0;  i < i1;  ++i)
                {
                    dst(i, j) = std::move(src(i, j));
                }
            }
        }
    }

    //----------------------------------------------------------------------------------------------
    //  These member functions compare the contents of matrix engines with those of other matrix
    //  engines, 2-D mdspans, and 2-D initializer lists.
    //----------------------------------------------------------------------------------------------
    //
    template<class ET1, class ET2>
    static constexpr bool
    compare(ET1 const& lhs, ET2 const& rhs)
    requires
        readable_matrix_engine<ET1>
        and
        readable_matrix_engine<ET2>
        and
        comparable_types<typename ET1::element_type, typename ET2::element_type>
    {
        using index_type_lhs = typename ET1::index_type;
        using index_type_rhs = typename ET2::index_type;

        index_type_lhs  r1 = lhs.rows();
        index_type_lhs  c1 = lhs.columns();
        index_type_rhs  r2 = rhs.rows();
        index_type_rhs  c2 = rhs.columns();

        if (sizes_differ(r1, c1, r2, c2)) return false;

        index_type_lhs  i1 = 0;
        index_type_rhs  i2 = 0;

        for (;  i1 < r1;  ++i1, ++i2)
        {
            index_type_lhs  j1 = 0;
            index_type_rhs  j2 = 0;

            for (;  j1 < c1;  ++j1, ++j2)
            {
                if (not (lhs(i1, j1) == rhs(i2, j2))) return false;
            }
        }
        return true;
    }

    template<class ET, class T, ptrdiff_t X0, ptrdiff_t X1, class SL, class SA>
    static constexpr bool
    compare(ET const& lhs, basic_mdspan<T, extents<X0, X1>, SL, SA> const& rhs)
    requires
        readable_matrix_engine<ET>
        and
        comparable_types<typename ET::element_type, T>
    {
        using index_type_lhs = typename ET::index_type;
        using index_type_rhs = typename basic_mdspan<T, extents<X0, X1>, SL, SA>::index_type;

        index_type_lhs  r1 = lhs.rows();
        index_type_lhs  c1 = lhs.columns();
        index_type_rhs  r2 = rhs.extent(0);
        index_type_rhs  c2 = rhs.extent(1);

        if (sizes_differ(r1, c1, r2, c2)) return false;

        index_type_lhs  i1 = 0;
        index_type_rhs  i2 = 0;

        for (;  i1 < r1;  ++i1, ++i2)
        {
            index_type_lhs  j1 = 0;
            index_type_rhs  j2 = 0;

            for (;  j1 < c1;  ++j1, ++j2)
            {
                if (not (lhs(i1, j1) == rhs(i2, j2))) return false;
            }
        }
        return true;
    }

    template<class ET, class U>
    static constexpr bool
    compare(ET const& lhs, initializer_list<initializer_list<U>> rhs)
    requires
        readable_matrix_engine<ET>
        and
        comparable_types<typename ET::element_type, U>
    {
        using index_type_lhs = typename ET::index_type;
        using index_type_rhs = typename initializer_list<initializer_list<U>>::size_type;

        index_type_lhs  r1 = lhs.rows();
        index_type_lhs  c1 = lhs.columns();
        auto            [r2, c2] = verify_list(rhs);

        if (sizes_differ(r1, c1, r2, c2)) return false;

        index_type_lhs  i1 = 0;
        auto            i2 = rhs.begin();

        for (;  i1 < r1;  ++i1, ++i2)
        {
            index_type_lhs  j1 = 0;
            auto            j2 = i2->begin();

            for (;  j1 < c1;  ++j1, ++j2)
            {
                if (not (lhs(i1, j1) == *j2)) return false;
            }
        }
        return true;
    }

    //----------------------------------------------------------------------------------------------
    //  These member functions compare the contents of 1-D indexable matrix engines with those of
    //  vector engines, random-access standard containers, 1-D mdspans, and 1-D initializer lists.
    //----------------------------------------------------------------------------------------------
    //
    template<class ET1, class ET2>
    static constexpr bool
    compare(ET1 const& lhs, ET2 const& rhs)
    requires
        readable_and_1d_indexable_matrix_engine<ET1>
        and
        readable_vector_engine<ET2>
        and
        comparable_types<typename ET1::element_type, typename ET2::element_type>
    {
        using index_type_lhs = typename ET1::index_type;
        using index_type_rhs = typename ET2::index_type;

        index_type_lhs  n1 = lhs.size();
        index_type_rhs  n2 = rhs.size();

        if (sizes_differ(n1, n2)) return false;

        index_type_lhs  i1 = 0;
        index_type_rhs  i2 = 0;

        for (;  i1 < n1;  ++i1, ++i2)
        {
            if (not (lhs(i1) == rhs(i2))) return false;
        }
        return true;
    }

    template<class ET, class CT>
    static constexpr bool
    compare(ET const& lhs, CT const& rhs)
    requires
        readable_and_1d_indexable_matrix_engine<ET>
        and
        random_access_standard_container<CT>
        and
        comparable_types<typename ET::element_type, typename CT::value_type>
    {
        using index_type_lhs = typename ET::index_type;
        using index_type_rhs = typename CT::size_type;

        index_type_lhs  n1 = lhs.size();
        index_type_rhs  n2 = rhs.size();

        if (sizes_differ(n1, n2)) return false;

        index_type_lhs  i1 = 0;
        index_type_rhs  i2 = 0;

        for (;  i1 < n1;  ++i1, ++i2)
        {
            if (not (lhs(i1) == rhs[i2])) return false;
        }
        return true;
    }

    template<class ET, class T, ptrdiff_t X0, class L, class A>
    static constexpr bool
    compare(ET const& lhs, basic_mdspan<T, extents<X0>, L, A> const& rhs)
    requires
        readable_and_1d_indexable_matrix_engine<ET>
        and
        comparable_types<typename ET::element_type, T>
    {
        using index_type_lhs = typename ET::index_type;
        using index_type_rhs = typename basic_mdspan<T, extents<X0>, L, A>::index_type;

        index_type_lhs  n1 = lhs.size();
        index_type_rhs  n2 = rhs.extent(0);

        if (sizes_differ(n1, n2)) return false;

        index_type_lhs  i1 = 0;
        index_type_rhs  i2 = 0;

        for (;  i1 < n1;  ++i1, ++i2)
        {
            if (not (lhs(i1) == rhs(i2))) return false;
        }
        return true;
    }

    template<class ET, class U>
    static constexpr bool
    compare(ET const& lhs, initializer_list<U> rhs)
    requires
        readable_and_1d_indexable_matrix_engine<ET>
        and
        comparable_types<typename ET::element_type, U>
    {
        using index_type_lhs = typename ET::index_type;
        using index_type_rhs = typename initializer_list<U>::size_type;

        index_type_lhs  n1 = lhs.size();
        index_type_rhs  n2 = rhs.size();

        if (sizes_differ(n1, n2)) return false;

        index_type_lhs  i1 = 0;
        auto            i2 = rhs.begin();

        for (;  i1 < n1;  ++i1, ++i2)
        {
            if (not (lhs(i1) == *i2)) return false;
        }
        return true;
    }
};

}       //- detail namespace
}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_ENGINE_SUPPORT_HPP_DEFINED
