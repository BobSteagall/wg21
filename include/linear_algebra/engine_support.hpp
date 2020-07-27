//==================================================================================================
//  File:       engine_support.hpp
//
//  Summary:    This header defines various types, traits, concepts, and functions used across
//              the entire library.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_ENGINE_SUPPORT_HPP_DEFINED
#define LINEAR_ALGEBRA_ENGINE_SUPPORT_HPP_DEFINED

namespace STD_LA {
namespace detail {

template<class T, class X, class A, class L>    struct mse_data;

//------
//
template<class T>
struct is_one_d_mdspan : public false_type
{};

template<class T, class L, class A, ptrdiff_t N>
struct is_one_d_mdspan<basic_mdspan<T, extents<N>, L, A>> : public true_type
{};

template<class T> inline constexpr
bool    is_one_d_mdspan_v = is_one_d_mdspan<T>::value;

//------
//
template<class T>
struct is_two_d_mdspan : public false_type
{};

template<class T, class L, class A, ptrdiff_t R, ptrdiff_t C>
struct is_two_d_mdspan<basic_mdspan<T, extents<R, C>, L, A>> : public true_type
{};

template<class T> inline constexpr
bool    is_two_d_mdspan_v = is_two_d_mdspan<T>::value;

//------
//
template<class ET, class = void>
struct engine_mdspan_types
{
    using span_type       = void;
    using const_span_type = void;
};

template<class ET>
struct engine_mdspan_types<ET, void_t<typename ET::span_type, typename ET::const_span_type>>
{
    using span_type       = typename ET::span_type;
    using const_span_type = typename ET::const_span_type;
};

template<class ET>
using engine_mdspan_t = typename engine_mdspan_types<ET>::span_type;

template<class ET>
using engine_const_mdspan_t = typename engine_mdspan_types<ET>::const_span_type;


//--------------------------------------------------------------------------------------------------
//  Concepts:   same_types<T, U>,  same_as<T, U>,  convertible_element<T, U>
//
//  These are some simple private utility concepts, used by the other concepts below.
//--------------------------------------------------------------------------------------------------
//
template<class DST, class SRC>
concept convertible_from = convertible_to<SRC, DST>;


template<class DST, class SRC>
concept constructible_from_engine = constructible_from<DST, SRC>;

template<class DST, class T, ptrdiff_t X0, class L, class A>
concept constructible_from_1d_mdspan = constructible_from<DST, basic_mdspan<T, extents<X0>, L, A>>;

template<class DST, class T, ptrdiff_t X0, ptrdiff_t X1, class L, class A>
concept constructible_from_2d_mdspan = constructible_from<DST, basic_mdspan<T, extents<X0, X1>, L, A>>;

template<class DST, class SRC>
concept constructible_from_1d_list = constructible_from<DST, initializer_list<SRC>>;

template<class DST, class SRC>
concept constructible_from_2d_list = constructible_from<DST, initializer_list<initializer_list<SRC>>>;


template<class DST, class SRC>
concept assignable_from_engine = assignable_from<DST, SRC>;

template<class DST, class T, ptrdiff_t X0, class L, class A>
concept assignable_from_1d_mdspan = assignable_from<DST, basic_mdspan<T, extents<X0>, L, A>>;

template<class DST, class T, ptrdiff_t X0, ptrdiff_t X1, class L, class A>
concept assignable_from_2d_mdspan = assignable_from<DST, basic_mdspan<T, extents<X0, X1>, L, A>>;

template<class DST, class SRC>
concept assignable_from_1d_list = assignable_from<DST, initializer_list<SRC>>;

template<class DST, class SRC>
concept assignable_from_2d_list = assignable_from<DST, initializer_list<initializer_list<SRC>>>;


template<class ET>
concept one_d_indexable =
    requires (ET& eng, typename ET::index_type i)
    {
        { eng(i) };
    };

template<class ET>
concept two_d_indexable =
    requires (ET& eng, typename ET::index_type i)
    {
        { eng(i, i) };
    };

template<class ET>
concept matrix_engine_lifetime = default_initializable<ET> and copyable<ET>;


template<class T1, class T2>
concept comparable_types =
    requires (T1 const& t1, T2 const& t2)
    {
    #ifdef LA_COMPOUND_REQUIREMENT_SYNTAX_SUPPORTED
        { t1 == t2 } -> same_as<bool>;
    #else
        requires is_same_v<decltype(t1 == t2), bool>;
    #endif
    };

//--------------------------------------------------------------------------------------------------
//  Concepts:   readable_vector_engine<ET>
//              readable_matrix_engine<ET>
//              readable_engine<ET>
//
//  These concepts describe the fundamental element readability interface that must be provided
//  by all vector and matrix engine types in order to function correctly with the most basic
//  subset of services provided by basic_vector<ET, OT> and basic_matrix<ET, OT>, respectively.
//
//  Engines that fulfill these concepts may have the value of their elements read via indexing,
//  their element sizes and capacities read, and publicly declare several critical nested type
//  aliases.
//--------------------------------------------------------------------------------------------------
//
template<class ET>
concept readable_vector_engine =
    requires (ET const& eng, typename ET::index_type i)
    {
        typename ET::element_type;
        typename ET::index_type;
        typename ET::reference;
        typename ET::const_reference;
        requires is_lvalue_reference_v<typename ET::reference>;

    #ifdef LA_COMPOUND_REQUIREMENT_SYNTAX_SUPPORTED
        { eng.size()     } -> same_as<typename ET::index_type>;
        { eng.capacity() } -> same_as<typename ET::index_type>;
        { eng(i)         } -> same_as<typename ET::const_reference>;
    #else
        requires is_same_v<decltype(eng.size()), typename ET::index_type>;
        requires is_same_v<decltype(eng.capacity()), typename ET::index_type>;
        requires is_same_v<decltype(eng(i)), typename ET::const_reference>;
    #endif
    };

template<class ET>
concept readable_matrix_engine =
    requires (ET const& eng, typename ET::index_type i)
    {
        typename ET::element_type;
        typename ET::index_type;
        typename ET::reference;
        typename ET::const_reference;
        requires is_lvalue_reference_v<typename ET::reference>;

    #ifdef LA_COMPOUND_REQUIREMENT_SYNTAX_SUPPORTED
        { eng.columns()         } -> same_as<typename ET::index_type>;
        { eng.rows()            } -> same_as<typename ET::index_type>;
        { eng.size()            } -> same_as<typename ET::index_type>;
        { eng.column_capacity() } -> same_as<typename ET::index_type>;
        { eng.row_capacity()    } -> same_as<typename ET::index_type>;
        { eng.capacity()        } -> same_as<typename ET::index_type>;
        { eng(i, i)             } -> same_as<typename ET::const_reference>;
    #else
        requires is_same_v<decltype(eng.columns()), typename ET::index_type>;
        requires is_same_v<decltype(eng.rows()), typename ET::index_type>;
        requires is_same_v<decltype(eng.size()), typename ET::index_type>;
        requires is_same_v<decltype(eng.column_capacity()), typename ET::index_type>;
        requires is_same_v<decltype(eng.row_capacity()), typename ET::index_type>;
        requires is_same_v<decltype(eng.capacity()), typename ET::index_type>;
        requires is_same_v<decltype(eng(i, i)), typename ET::const_reference>;
    #endif
    };

template<class ET>
concept readable_engine = readable_matrix_engine<ET> or readable_vector_engine<ET>;

template<class ET>
concept readable_vector_engine_only = readable_vector_engine<ET> and (not two_d_indexable<ET>);

template<class ET>
concept readable_matrix_engine_only = readable_matrix_engine<ET> and (not one_d_indexable<ET>);


//--------------------------------------------------------------------------------------------------
//  Concepts:   writable_vector_engine<ET>
//              writable_matrix_engine<ET>
//              writable_engine<ET>
//
//  These concepts describe the element writability interface that may be provided by vector
//  and matrix engine types in order to function correctly the basic_vector<ET, OT> and
//  basic_matrix<ET, OT> member functions that support element writability.
//
//  Engines that fulfill these concepts fulfill the corresponding readable_*_engine concepts,
//  and additionally may have the value of their elements changed via indexing.
//--------------------------------------------------------------------------------------------------
//
template<class ET>
concept writable_vector_engine =
    readable_vector_engine<ET>  and
    requires (ET& eng, typename ET::index_type i, typename ET::element_type v)
    {
        requires is_same_v<typename ET::reference, typename ET::element_type&>;

    #ifdef LA_COMPOUND_REQUIREMENT_SYNTAX_SUPPORTED
        { eng(i) } -> same_as<typename ET::reference>;
    #else
        requires is_same_v<decltype(eng(i)), typename ET::reference>;
    #endif

        { eng(i) = v };
    };

template<class ET>
concept writable_matrix_engine =
    readable_matrix_engine<ET>  and
    requires (ET& eng, typename ET::index_type i, typename ET::element_type v)
    {
        requires is_same_v<typename ET::reference, typename ET::element_type&>;

    #ifdef LA_COMPOUND_REQUIREMENT_SYNTAX_SUPPORTED
        { eng(i, i) } -> same_as<typename ET::reference>;
    #else
        requires is_same_v<decltype(eng(i, i)), typename ET::reference>;
    #endif

        { eng(i, i) = v };
    };

template<class ET>
concept writable_engine = writable_matrix_engine<ET> or writable_vector_engine<ET>;

template<class ET>
concept row_major_engine = true;

//--------------------------------------------------------------------------------------------------
//  Concepts:   spannable_vector_engine<ET>
//              spannable_matrix_engine<ET>
//              spannable_engine<ET>
//
//  These concepts describe the mdspan interface that may be provided by vector and matrix engine
//  types in order to function correctly the basic_vector<ET, OT> and basic_matrix<ET, OT> member
//  functions that return mdspan objects.
//--------------------------------------------------------------------------------------------------
//
template<class ET>
concept spannable_vector_engine =
    readable_vector_engine<ET>  and
    requires (ET const& ceng, ET& meng)
    {
        typename ET::span_type;
        typename ET::const_span_type;
        requires is_one_d_mdspan_v<typename ET::span_type>;
        requires is_one_d_mdspan_v<typename ET::const_span_type>;

    #ifdef LA_COMPOUND_REQUIREMENT_SYNTAX_SUPPORTED
        { meng.span() } -> same_as<typename ET::span_type>;
        { ceng.span() } -> same_as<typename ET::const_span_type>;
    #else
        requires is_same_v<decltype(meng.span()), typename ET::span_type>;
        requires is_same_v<decltype(ceng.span()), typename ET::const_span_type>;
    #endif
    };

template<class ET>
concept spannable_matrix_engine =
    readable_matrix_engine<ET>  and
    requires (ET const& ceng, ET& meng)
    {
        typename ET::span_type;
        typename ET::const_span_type;
        requires is_two_d_mdspan_v<typename ET::span_type>;
        requires is_two_d_mdspan_v<typename ET::const_span_type>;

    #ifdef LA_COMPOUND_REQUIREMENT_SYNTAX_SUPPORTED
        { meng.span() } -> same_as<typename ET::span_type>;
        { ceng.span() } -> same_as<typename ET::const_span_type>;
    #else
        requires is_same_v<decltype(meng.span()), typename ET::span_type>;
        requires is_same_v<decltype(ceng.span()), typename ET::const_span_type>;
    #endif
    };

template<class ET>
concept spannable_engine = spannable_matrix_engine<ET> or spannable_vector_engine<ET>;


//--------------------------------------------------------------------------------------------------
//  Concepts:   reshapable_vector_engine<ET>
//              reshapable_matrix_engine<ET>
//              column_reshapable_matrix_engine<ET>
//              row_reshapable_matrix_engine<ET>
//
//  These concepts specify the interfaces of vector and matrix engines that provde reshaping.
//--------------------------------------------------------------------------------------------------
//
template<class ET>
concept reshapable_vector_engine =
    writable_vector_engine<ET>  and
    requires (ET& eng, typename ET::index_type i)
    {
        { eng.reshape(i, i) };
    };

template<class ET>
concept reshapable_matrix_engine =
    writable_matrix_engine<ET>  and
    requires (ET& eng, typename ET::index_type i)
    {
        { eng.reshape(i, i, i, i) };
    };

template<class ET>
concept column_reshapable_matrix_engine =
    writable_matrix_engine<ET>  and
    requires (ET& eng, typename ET::index_type i)
    {
        { eng.reshape_columns(i, i) };
    };

template<class ET>
concept row_reshapable_matrix_engine =
    writable_matrix_engine<ET>  and
    requires (ET& eng, typename ET::index_type i)
    {
        { eng.reshape_rows(i, i) };
    };


//---------------------------------------------------//--------------------------------------------------------------------------------------------------
//  Class Template:     common_engine_support
//
//  Policy/traits base type that provides various types and functions for verification.
//--------------------------------------------------------------------------------------------------
//
struct common_engine_support
{
    template<class IT1, class IT2>
    static inline constexpr bool
    sizes_differ(IT1 n1, IT2 n2)
    {
        using cmp_type = common_type_t<IT1, IT2, ptrdiff_t>;

        return (static_cast<cmp_type>(n1) != static_cast<cmp_type>(n2));
    }

    template<class IT1, class IT2>
    static inline constexpr bool
    sizes_differ(IT1 r1, IT1 c1, IT2 r2, IT2 c2)
    {
        using cmp_type = common_type_t<IT1, IT2, ptrdiff_t>;

        return ((static_cast<cmp_type>(r1) != static_cast<cmp_type>(r2)) ||
                (static_cast<cmp_type>(c1) != static_cast<cmp_type>(c2)));
    }

    template<class IT>
    static inline constexpr void
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

        for (auto&& row : list)
        {
            if (row.size() != cols)
            {
                throw runtime_error("matrix initializer_list has invalid shape");
            }
        }
        return {static_cast<ptrdiff_t>(rows), static_cast<ptrdiff_t>(cols)};
    }

    template<class IT>
    static inline constexpr void
    verify_size(IT s)
    {
        if (s < static_cast<IT>(1))
        {
            throw runtime_error("invalid size parameter");
        }
    }

    template<class IT1, class IT2>
    static inline constexpr void
    verify_size(IT1 s1, IT2 s2)
    {
        if (sizes_differ(s1, s2))
        {
            throw runtime_error("invalid size parameter");
        }
    }

    template<class T>
    static inline constexpr void
    swap(T& t0, T& t1) noexcept(is_nothrow_movable_v<T>)
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
    //  These member functions perform reshaping where possible and needed; otherwise they peform
    //  size verifications.
    //----------------------------------------------------------------------------------------------
    //
    template<class ET, class IT>
    static inline constexpr void
    verify_and_reshape(ET& dst, IT src_elems)
    requires
        writable_vector_engine<ET>
    {
        auto    elems = static_cast<typename ET::index_type>(src_elems);

        if constexpr (reshapable_matrix_engine<ET>)
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
    //  engines, 1-D mdspans, and 1-D initializer lists.
    //----------------------------------------------------------------------------------------------
    //
    template<class ET1, class ET2>
    static constexpr void
    vector_assign_from(ET1& dst, ET2 const& src)
    requires
        writable_vector_engine<ET1> and
        readable_vector_engine<ET2>
    {
        using index_type_dst = typename ET1::index_type;
        using index_type_src = typename ET2::index_type;

        index_type_src  sn = src.size();

        verify_and_reshape(dst, sn);

        index_type_dst  di = 0;
        index_type_src  si = 0;

        for (;  si < sn;  ++di, ++si)
        {
            dst(di) = static_cast<typename ET1::element_type>(src(si));
        }
    }

    template<class ET, class T, ptrdiff_t X0, class L, class A>
    static constexpr void
    vector_assign_from(ET const& dst, basic_mdspan<T, extents<X0>, L, A> const& src)
    requires
        readable_vector_engine<ET>  and
        convertible_from<typename ET::element_type, T>
    {
        using index_type_dst = typename ET::index_type;
        using index_type_src = typename basic_mdspan<T, extents<X0>, L, A>::index_type;

        index_type_src  sn = src.extent(0);

        verify_and_reshape(dst, sn);

        index_type_dst  di = 0;
        index_type_src  si = 0;

        for (;  si < sn;  ++di, ++si)
        {
            dst(di) = static_cast<typename ET::element_type>(src(si));
        }
    }


    template<class ET, class T>
    static constexpr void
    vector_assign_from(ET& dst, initializer_list<T> src)
    requires
        writable_vector_engine<ET> and
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
    //  These member functions compare the contents of vector engines with those of other vector
    //  engines, 1-D mdspans, and 1-D initializer lists.
    //----------------------------------------------------------------------------------------------
    //
    template<class ET1, class ET2>
    static constexpr bool
    vector_compare(ET1 const& lhs, ET2 const& rhs)
    requires
        readable_vector_engine<ET1> and
        readable_vector_engine<ET2> and
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
            if (lhs(i1) != rhs(i2)) return false;
        }
        return true;
    }

    template<class ET, class T, ptrdiff_t X0, class L, class A>
    static constexpr bool
    vector_compare(ET const& lhs, basic_mdspan<T, extents<X0>, L, A> const& rhs)
    requires
        readable_vector_engine<ET>  and
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
            if (lhs(i1) != rhs(i2)) return false;
        }
        return true;
    }

    template<class ET, class U>
    static constexpr bool
    vector_compare(ET const& lhs, initializer_list<U> rhs)
    requires
        readable_vector_engine<ET>  and
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
            if (lhs(i1) != *i2) return false;
        }
        return true;
    }

    template<class ET, class IT, class FN>
    static constexpr void
    apply(ET& dst, IT i0, IT j0, IT i1, IT j1, FN fn)
    requires
        writable_matrix_engine<ET>
    {
        if constexpr (row_major_engine<ET>)
        {
            for (auto i = i0;  i < i1;  ++i)
            {
                for (auto j = j0;  j < j1;  ++j)
                {
                    dst(i, j) = fn(i, j);
                }
            }
        }
        else
        {
            for (auto j = j0;  j < j1;  ++j)
            {
                for (auto i = i0;  i < i1;  ++i)
                {
                    dst(i, j) = fn(i, j);
                }
            }
        }
    }

    //- Fill the specified range of columns of an MSE with some value.
    //
    template<class ET, class JT0, class JT1, class T>
    static inline constexpr void
    fill_columns(ET& dst, JT0 c0, JT1 c1, T const& t)
    requires
        writable_matrix_engine<ET>  and
        same_as<typename ET::element_type, T>
    {
        using index_type = typename ET::index_type;

        auto    i0 = static_cast<index_type>(0);
        auto    j0 = static_cast<index_type>(c0);
        auto    i1 = dst.rows();
        auto    j1 = static_cast<index_type>(c1);

        apply(dst, i0, j0, i1, j1, [&t](index_type, index_type){  return t;  });
    }

    //- Fill the specified range of rows of an MSE with some value.
    //
    template<class ET, class IT0, class IT1, class T>
    static inline constexpr void
    fill_rows(ET& dst, IT0 r0, IT1 r1, T const& t)
    requires
        writable_matrix_engine<ET>  and
        same_as<typename ET::element_type, T>
    {
        using index_type = typename ET::index_type;

        auto    i0 = static_cast<index_type>(r0);
        auto    j0 = static_cast<index_type>(0);
        auto    i1 = static_cast<index_type>(r1);
        auto    j1 = dst.columns();

        apply(dst, i0, j0, i1, j1, [&t](index_type, index_type){  return t;  });
    }

    //- Move elements from a source MSE into a destination MSE.
    //
    template<class ET, class IT, class JT>
    static inline constexpr void
    move_elements(ET& dst, ET& src, IT rows, JT cols)
    {
        using index_type = typename ET::index_type;

        auto    i0 = static_cast<index_type>(0);
        auto    j0 = static_cast<index_type>(0);
        auto    i1 = static_cast<index_type>(rows);
        auto    j1 = static_cast<index_type>(cols);

        apply(dst, i0, j0, i1, j1,
              [&src](index_type i, index_type j){  return std::move(src(i, j));  });
    }

    using dyn_extents = extents<dynamic_extent, dynamic_extent>;
    using dyn_layout  = layout_stride<dynamic_extent, dynamic_extent>;
    using dyn_strides = array<typename dyn_extents::index_type, 2>;
    using dyn_mapping = typename dyn_layout::template mapping<dyn_extents>;

    template<class T, class ST> inline constexpr
    basic_mdspan<T, dyn_mat_extents, dyn_mat_layout>
    make_dynamic_span(T* pdata, ST rows, ST cols, ST row_stride, ST col_stride)
    {
        using idx_t = typename dyn_mat_extents::index_type;

        dyn_mat_extents     extents(static_cast<idx_t>(rows), static_cast<idx_t>(cols));
        dyn_mat_strides     strides{static_cast<idx_t>(row_stride), static_cast<idx_t>(col_stride)};
        dyn_mat_mapping     mapping(extents, strides);

        return basic_mdspan<T, dyn_mat_extents, dyn_mat_layout>(pdata, mapping);
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
    //  These member functions perform reshaping where possible and needed; otherwise they peform
    //  size verifications.
    //----------------------------------------------------------------------------------------------
    //
    template<class ET, class IT1, class IT2>
    static inline constexpr void
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
    static inline constexpr void
    verify_and_reshape(ET& dst, IT src_elems)
    requires
        writable_matrix_engine<ET>  and
        writable_vector_engine<ET>
    {
        auto    elems = static_cast<typename ET::index_type>(src_elems);

        if constexpr (column_reshapable_matrix_engine<ET>)
        {
            if (verify_size(dst.rows(), 1);  elems != dst.columns())
            {
                dst.reshape_columns(elems, dst.column_capacity());
            }
        }
        else if constexpr (row_reshapable_matrix_engine<ET>)
        {
            if (verify_size(dst.columns(), 1);  elems != dst.rows())
            {
                dst.reshape_rows(elems, dst.row_capacity());
            }
        }
        else
        {
            verify_size(dst.size(), elems);
        }
    }

    //----------------------------------------------------------------------------------------------
    //  These member functions assign to the contents of matrix engines from those of other matrix
    //  engines, 2-D mdspans, and 2-D initializer lists.
    //----------------------------------------------------------------------------------------------
    //
    template<class ET1, class ET2>
    static constexpr void
    matrix_assign_from(ET1& dst, ET2 const& src)
    requires
        writable_matrix_engine<ET1> and
        readable_matrix_engine<ET2> and
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

    template<class ET1, class ET2>
    static constexpr void
    matrix_assign_from(ET1& dst, ET2 const& src)
    requires
        writable_matrix_engine<ET1>
        and writable_vector_engine<ET1>
        and readable_vector_engine_only<ET2>
        and convertible_from<typename ET1::element_type, typename ET2::element_type>
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

    template<class ET, class T, ptrdiff_t X0, ptrdiff_t X1, class L, class A>
    static constexpr void
    matrix_assign_from(ET& dst, basic_mdspan<T, extents<X0, X1>, L, A> const& src)
    requires
        writable_matrix_engine<ET>
        and convertible_from<typename ET::element_type, T>
    {
        using index_type_dst = typename ET::index_type;
        using index_type_src = typename basic_mdspan<T, extents<X0, X1>, L, A>::index_type;

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

    template<class ET, class T, ptrdiff_t X0, class L, class A>
    static constexpr void
    matrix_assign_from(ET const& dst, basic_mdspan<T, extents<X0>, L, A> const& src)
    requires
        writable_matrix_engine<ET>
        and writable_vector_engine<ET>
        and convertible_from<typename ET::element_type, T>
    {
        using index_type_dst = typename ET::index_type;
        using index_type_src = typename basic_mdspan<T, extents<X0>, L, A>::index_type;

        index_type_src  sn = src.extent(0);

        verify_and_reshape(dst, sn);

        index_type_dst  di = 0;
        index_type_src  si = 0;

        for (;  si < sn;  ++di, ++si)
        {
            dst(di) = static_cast<typename ET::element_type>(src(si));
        }
    }

    template<class ET, class T>
    static constexpr void
    matrix_assign_from(ET& dst, initializer_list<initializer_list<T>> src)
    requires
        writable_matrix_engine<ET>  and
        convertible_from<typename ET::element_type, T>
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

    template<class ET, class T>
    static constexpr void
    matrix_assign_from(ET& dst, initializer_list<T> src)
    requires
        writable_matrix_engine<ET> and
        writable_vector_engine<ET> and
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
    //  These member functions compare the contents of matrix engines with those of other matrix
    //  engines, mdspans, and initializer lists.
    //----------------------------------------------------------------------------------------------
    //
    template<class ET1, class ET2>
    static constexpr bool
    matrix_compare(ET1 const& lhs, ET2 const& rhs)
    requires
        readable_matrix_engine<ET1> and
        readable_matrix_engine<ET2> and
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

    template<class ET1, class ET2>
    static constexpr bool
    matrix_compare(ET1 const& lhs, ET2 const& rhs)
    requires
        readable_matrix_engine<ET1> and
        readable_vector_engine<ET1> and
        readable_vector_engine_only<ET2> and
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
            if (lhs(i1) != rhs(i2)) return false;
        }
        return true;
    }

    template<class ET, class T, ptrdiff_t X0, ptrdiff_t X1, class L, class A>
    static constexpr bool
    matrix_compare(ET const& lhs, basic_mdspan<T, extents<X0, X1>, L, A> const& rhs)
    requires
        readable_matrix_engine<ET>  and
        comparable_types<typename ET::element_type, T>
    {
        using index_type_lhs = typename ET::index_type;
        using index_type_rhs = typename basic_mdspan<T, extents<X0, X1>, L, A>::index_type;

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

    template<class ET, class T, ptrdiff_t X0, class L, class A>
    static constexpr bool
    matrix_compare(ET const& lhs, basic_mdspan<T, extents<X0>, L, A> const& rhs)
    requires
        readable_matrix_engine<ET>  and
        readable_vector_engine<ET>  and
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
            if (lhs(i1) != rhs(i2)) return false;
        }
        return true;
    }

    template<class ET, class U>
    static constexpr bool
    matrix_compare(ET const& lhs, initializer_list<initializer_list<U>> rhs)
    requires
        readable_matrix_engine<ET>  and
        comparable_types<typename ET::element_type, U>
    {
        using index_type_lhs = typename ET::index_type;

        index_type_lhs  r1 = lhs.rows();
        index_type_lhs  c1 = lhs.columns();
        auto            [r2, c2] = verify_list(rhs);

        if (sizes_differ(r1, c1, r2, c2)) return false;

        index_type_lhs  i1 = 0;
        auto            i2 = rhs.begin();

        for (;  i1 < r1;  ++i1, ++i2)
        {
            index_type_lhs  j1 = 0;
            auto            j2 = i1->begin();

            for (;  j1 < jn;  ++j1, ++j2)
            {
                if (not (lhs(i1, j1) == *j2)) return false;
            }
        }
        return true;
    }

    template<class ET, class U>
    static constexpr bool
    matrix_compare(ET const& lhs, initializer_list<U> rhs)
    requires
        readable_matrix_engine<ET>  and
        readable_vector_engine<ET>  and
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
            if (lhs(i1) != *i2) return false;
        }
        return true;
    }


    //- Fill the specified range of columns of a writable engine with some value.
    //
    template<class ET, class JT0, class JT1, class T>
    static inline constexpr void
    fill_columns(ET& dst, JT0 c0, JT1 c1, T const& t)
    requires
        writable_matrix_engine<ET>  and
        same_as<typename ET::element_type, T>
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
                    dst(i, j) = t;
                }
            }
        }
        else
        {
            for (auto j = j0;  j < j1;  ++j)
            {
                for (auto i = i0;  i < i1;  ++i)
                {
                    dst(i, j) = t;
                }
            }
        }
    }

    //- Fill the specified range of rows of a writable engine with some value.
    //
    template<class ET, class IT0, class IT1, class T>
    static inline constexpr void
    fill_rows(ET& dst, IT0 r0, IT1 r1, T const& t)
    requires
        writable_matrix_engine<ET>  and
        same_as<typename ET::element_type, T>
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
                    dst(i, j) = t;
                }
            }
        }
        else
        {
            for (auto j = j0;  j < j1;  ++j)
            {
                for (auto i = i0;  i < i1;  ++i)
                {
                    dst(i, j) = t;
                }
            }
        }
    }

    //- Move elements from a source writable engine into a destination writable engine.
    //
    template<class ET, class IT, class JT>
    static inline constexpr void
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
};

}       //- detail namespace
}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_ENGINE_SUPPORT_HPP_DEFINED
