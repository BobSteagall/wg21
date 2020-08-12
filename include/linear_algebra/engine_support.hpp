//==================================================================================================
//  File:       engine_support.hpp
//
//  Summary:    This header defines various traits, concepts, types, and functions used across
//              the entire library to support the provided engines.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_ENGINE_SUPPORT_HPP_DEFINED
#define LINEAR_ALGEBRA_ENGINE_SUPPORT_HPP_DEFINED

namespace STD_LA {
namespace detail {
//==================================================================================================
//  TRAITS DEFINITIONS
//==================================================================================================
//  Trait:      is_valid_engine_extents<X>
//  Alias:      is_valid_engine_extents_v<X>
//
//  This private traits type is used to validate an engine's extents template argument.  The
//  extents parameter must be one- or two-dimensional, and each dimension's template argument
//  may have only a specific set of values.
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

template<class T> inline constexpr
bool    is_valid_engine_extents_v = is_valid_engine_extents<T>::value;


//--------------------------------------------------------------------------------------------------
//  Trait:      is_1d_mdspan<T>
//  Alias:      is_1d_mdspan_v<T>
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

template<class T> inline constexpr
bool    is_1d_mdspan_v = is_1d_mdspan<T>::value;


//--------------------------------------------------------------------------------------------------
//  Trait:      is_2d_mdspan<T>
//  Alias:      is_2d_mdspan_v<T>
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

template<class T> inline constexpr
bool    is_2d_mdspan_v = is_2d_mdspan<T>::value;


//--------------------------------------------------------------------------------------------------
//  Trait:      nested_mdspan_types<T>
//  Aliases:    engine_mdspan_t<T> and const_engine_mdspan_t<T>
//
//  This private traits type and the associated alias templates determine whether or not the
//  template parameter type has nested type aliases "span_type" and "const_span_type".  If both
//  aliases are present, then the associated aliases templates refer to the corresponding types.
//  Otherwise, the alias templates refer to the void type.
//--------------------------------------------------------------------------------------------------
//
template<class ET, class = void>
struct nested_mdspan_types
{
    using span_type       = void;
    using const_span_type = void;
};

template<class ET>
struct nested_mdspan_types<ET, void_t<typename ET::span_type, typename ET::const_span_type>>
{
    using span_type       = typename ET::span_type;
    using const_span_type = typename ET::const_span_type;
};

template<class ET>
using engine_mdspan_t = typename nested_mdspan_types<ET>::span_type;

template<class ET>
using engine_const_mdspan_t = typename nested_mdspan_types<ET>::const_span_type;


//--------------------------------------------------------------------------------------------------
//  Trait:      is_random_access_standard_container<X>
//  Alias:      is_random_access_standard_container_v<X>
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

template<class T> inline constexpr
bool    is_random_access_standard_container_v = is_random_access_standard_container<T>::value;


//==================================================================================================
//  CONCEPT DEFINITIONS
//==================================================================================================
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
//  exist, then we fake it.  The first reason for wrapping is consistencey and readability; the
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
//  Concept:    valid_engine_allocator<A, T>
//
//  This private concept determines whether a given type A is an acceptable allocator of type T.
//  Prospective allocator type A must either: be void; or, it must be possible to instantiate a
//  specialization of allocator_traits<A> that meets certain requirements relative to allocated
//  type T, such as allocator_traits<A>::value_type is the same type as T, etc.
//--------------------------------------------------------------------------------------------------
//
template<typename T>
concept no_allocator = same_as<T, void>;

template<typename AT, typename T>
concept valid_allocator =
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

template<typename A, typename T>
concept valid_engine_allocator = no_allocator<A> or valid_allocator<allocator_traits<A>, T>;

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
        requires is_lvalue_reference_v<typename ET::reference>;
        { eng.size()     } -> same_as<typename ET::index_type>;
        { eng.capacity() } -> same_as<typename ET::index_type>;
    };


//--------------------------------------------------------------------------------------------------
//  Concept:    readable_vector_engine<ET>
//
//  This private concept determines whether a prospective vector engine type provides the
//  readability interface required to function correctly with basic_vector<ET, OT>.
//
//  Engine types that fulfill this concept may have the value of their elements read via one-
//  dimensional indexing, their element sizes and capacities read, and publicly declare several
//  important nested type aliases.
//
//  Note that certain matrix engine types might also fulfill the requirements of
//  readable_vector_engine<ET>.  For example, this could occur with matrix engines having
//  one row/column fixed at compile-time that offer one-dimensional indexing.
//--------------------------------------------------------------------------------------------------
//
template<class ET>
concept readable_vector_engine =
    readable_engine_fundamentals<ET>
    and
    requires (ET const& eng, typename ET::index_type i)
    {
        { eng(i) } -> same_as<typename ET::const_reference>;
    };


//--------------------------------------------------------------------------------------------------
//  Concept:    readable_1d_vector_engine<ET>
//
//  This private concept determines whether a prospective vector engine type provides only the
//  readability interface required to function correctly with basic_vector<ET, OT>.  Most
//  importantly, engine types that fulfill this concept perform one-dimensional indexing only,
//  and therefore cannot also be matrix engines.
//--------------------------------------------------------------------------------------------------
//
template<class ET>
concept readable_1d_vector_engine =
    readable_vector_engine<ET>
    and
    not requires (ET const& eng, typename ET::index_type i)
    {
        { eng(i, i) };
    };


//--------------------------------------------------------------------------------------------------
//  Concept:    spannable_vector_engine<ET>
//
//  This private concept determines whether a prospective vector engine type provides the
//  correct nested mdspan types optionally required by basic_vector<ET, OT>.
//--------------------------------------------------------------------------------------------------
//
template<class ET>
concept spannable_vector_engine =
    readable_1d_vector_engine<ET>
    and
    requires (ET const& ceng, ET& meng)
    {
        typename ET::span_type;
        typename ET::const_span_type;
        requires is_1d_mdspan_v<typename ET::span_type>;
        requires is_1d_mdspan_v<typename ET::const_span_type>;
        { meng.span() } -> same_as<typename ET::span_type>;
        { ceng.span() } -> same_as<typename ET::const_span_type>;
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
    requires (ET& eng, typename ET::index_type i, typename ET::element_type v)
    {
        requires std::is_same_v<typename ET::reference, typename ET::element_type&>;
        { eng(i)     } -> same_as<typename ET::reference>;
        { eng(i) = v } -> same_as<typename ET::reference>;
    };


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
    requires (ET const& eng, typename ET::index_type i)
    {
        { eng.columns()         } -> same_as<typename ET::index_type>;
        { eng.rows()            } -> same_as<typename ET::index_type>;
        { eng.column_capacity() } -> same_as<typename ET::index_type>;
        { eng.row_capacity()    } -> same_as<typename ET::index_type>;
        { eng(i, i)             } -> same_as<typename ET::const_reference>;
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
    requires (ET const& eng, typename ET::index_type i)
    {
        { eng(i) } -> same_as<typename ET::const_reference>;
    };


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
        { ceng.span() } -> same_as<typename ET::const_span_type>;
    };


//--------------------------------------------------------------------------------------------------
//  Concept:    writable_matrix_engine<ET>
//
//  This private concept determines whether a prospective matrix engine type provides the
//  writability interface required to function correctly with basic_matrix<ET, OT>.
//
//  Engine types that fulfill this concept fulfill the corresponding readable_matrix_engine<ET>
//  concept, and also permit the value of their elements to be changed via two-dimentionsl
//  indexing.
//--------------------------------------------------------------------------------------------------
//
template<class ET>
concept writable_matrix_engine =
    readable_matrix_engine<ET>
    and
    requires (ET& eng, typename ET::index_type i, typename ET::element_type v)
    {
        requires std::is_same_v<typename ET::reference, typename ET::element_type&>;
        { eng(i, i)     } -> same_as<typename ET::reference>;
        { eng(i, i) = v } -> same_as<typename ET::reference>;
    };


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
    requires (ET const& ceng, ET& meng, typename ET::index_type i, typename ET::element_type v)
    {
        { ceng(i)     } -> same_as<typename ET::const_reference>;
        { meng(i)     } -> same_as<typename ET::reference>;
        { meng(i) = v } -> same_as<typename ET::reference>;
    };


template<class ET>
concept row_major_engine = true;

template<class ET>
concept spannable_engine = spannable_matrix_engine<ET> or spannable_vector_engine<ET>;


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
//  Concepts:   valid_engine_extents<X>
//
//  This private concept is used to validate the second template parameter of a specialization
//  of matrix_storage_engine, the engine's extents type.
//--------------------------------------------------------------------------------------------------
//
template<typename X>
concept valid_engine_extents = is_valid_engine_extents_v<X>;


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
    (std::is_same_v<L, row_major> || std::is_same_v<L, column_major>);

template<typename L>
concept valid_layout_for_1d_storage_engine = std::is_same_v<L, unoriented>;


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
    template<class IT1, class IT2>
    static constexpr bool
    sizes_differ(IT1 n1, IT2 n2) noexcept
    {
        using cmp_type = common_type_t<IT1, IT2, ptrdiff_t>;

        return (static_cast<cmp_type>(n1) != static_cast<cmp_type>(n2));
    }

    template<class ITR1, class ITC1, class ITR2, class ITC2>
    static constexpr bool
    sizes_differ(ITR1 r1, ITC1 c1, ITR2 r2, ITC2 c2) noexcept
    {
        using cmp_type = common_type_t<ITR1, ITC1, ITR2, ITC2, ptrdiff_t>;

        return ((static_cast<cmp_type>(r1) != static_cast<cmp_type>(r2)) ||
                (static_cast<cmp_type>(c1) != static_cast<cmp_type>(c2)));
    }

    template<class IT>
    static constexpr void
    verify_capacity(IT c)
    {
        if (c < static_cast<IT>(0))
        {
            throw runtime_error("invalid capacity parameter");
        }
    }

    template<class U>
    static constexpr tuple<ptrdiff_t, ptrdiff_t>
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

    template<class IT>
    static constexpr void
    verify_size(IT s)
    {
        if (s < static_cast<IT>(1))
        {
            throw runtime_error("invalid size parameter");
        }
    }

    template<class IT1, class IT2>
    static constexpr void
    verify_size(IT1 s1, IT2 s2)
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
//  Class Template:     vector_engine_support
//
//  Policy/traits base type that provides various types and functions for verification.
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
        readable_vector_engine<ET>
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
//  Class Template:     matrix_engine_support
//
//  Policy/traits base type that provides various types and functions for verification.
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
        readable_1d_vector_engine<ET2>
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

        if constexpr (row_major_engine<ET>)
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

        if constexpr (row_major_engine<ET>)
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

        if constexpr (row_major_engine<ET>)
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
        readable_1d_vector_engine<ET2>
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
