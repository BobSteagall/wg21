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
//------
//
template<class T>
struct is_1d_mdspan : public false_type
{};

template<class T, class L, class A, ptrdiff_t N>
struct is_1d_mdspan<basic_mdspan<T, extents<N>, L, A>> : public true_type
{};

template<class T> inline constexpr
bool    is_1d_mdspan_v = is_1d_mdspan<T>::value;

//------
//
template<class T>
struct is_2d_mdspan : public false_type
{};

template<class T, class L, class A, ptrdiff_t R, ptrdiff_t C>
struct is_2d_mdspan<basic_mdspan<T, extents<R, C>, L, A>> : public true_type
{};

template<class T> inline constexpr
bool    is_2d_mdspan_v = is_2d_mdspan<T>::value;

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

template<class DST, class SRC>
concept constructible_from_1d_list = constructible_from<DST, initializer_list<SRC>>;

template<class DST, class SRC>
concept constructible_from_2d_list = constructible_from<DST, initializer_list<initializer_list<SRC>>>;


template<class DST, class SRC>
concept assignable_from_engine = assignable_from<DST, SRC>;

template<class DST, class SRC>
concept assignable_from_1d_list = assignable_from<DST, initializer_list<SRC>>;

template<class DST, class SRC>
concept assignable_from_2d_list = assignable_from<DST, initializer_list<initializer_list<SRC>>>;


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
        requires is_1d_mdspan_v<typename ET::span_type>;
        requires is_1d_mdspan_v<typename ET::const_span_type>;

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
        requires is_2d_mdspan_v<typename ET::span_type>;
        requires is_2d_mdspan_v<typename ET::const_span_type>;

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


//--------------------------------------------------------------------------------------------------
//  Class Template:     engine_support
//
//  Policy/traits base type that provides various types and functions for verification.
//--------------------------------------------------------------------------------------------------
//
struct engine_support
{
    static inline constexpr void
    verify_capacity(ptrdiff_t c)
    {
        if (c < 0)
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

    template<class ET1, class ET2>
    static constexpr void
    assign_from_vector_engine(ET1& dst, ET2 const& src)
        requires
            writable_vector_engine<ET1>     and
            readable_vector_engine<ET2>
    {
        using elem_type_dst  = typename ET1::element_type;
        using index_type_dst = typename ET1::index_type;
        using index_type_src = typename ET2::index_type;

        index_type_dst  di = 0;
        index_type_src  si = 0;
        index_type_src  ni = src.size();

        for (;  si < ni;  ++di, ++si)
        {
            dst(di) = static_cast<elem_type_dst>(src(si));
        }
    }

    template<class ET1, class ET2>
    static constexpr void
    assign_from_matrix_engine(ET1& dst, ET2 const& src)
        requires
            writable_matrix_engine<ET1>     and
            readable_matrix_engine<ET2>
    {
        using elem_type_dst  = typename ET1::element_type;
        using index_type_dst = typename ET1::index_type;
        using index_type_src = typename ET2::index_type;

        index_type_dst  di = 0;
        index_type_src  si = 0;
        index_type_src  rows = src.rows();
        index_type_src  cols = src.columns();

        for (; si < rows;  ++di, ++si)
        {
            index_type_dst  dj = 0;
            index_type_src  sj = 0;

            for (; sj < cols;  ++dj, ++sj)
            {
                dst(di, dj) = static_cast<elem_type_dst>(src(si, sj));
            }
        }
    }

    template<class ET, class T>
    static constexpr void
    assign_from_vector_initlist(ET& dst, initializer_list<T> src)
        requires
            writable_vector_engine<ET>      and
            convertible_from<typename ET::element_type, T>
    {
        using index_type_dst = typename ET::index_type;
        using elem_iter_src  = decltype(src.begin());

        index_type_dst  di = 0;
        index_type_dst  dn = dst.size();
        elem_iter_src   ep = src.begin();

        for (;  di < dn;  ++di, ++ep)
        {
            dst(di) = static_cast<typename ET::element_type>(*ep);
        }
    }

    template<class ET, class T>
    static constexpr void
    assign_from_matrix_initlist(ET& engine, initializer_list<initializer_list<T>> rhs)
        requires
            writable_matrix_engine<ET>      and
            convertible_from<typename ET::element_type, T>
    {
        using index_type_dst = typename ET::index_type;
        using row_iter_src   = decltype(rhs.begin());
        using col_iter_src   = decltype(rhs.begin()->begin());

        index_type_dst  di = 0;
        row_iter_src    rp = rhs.begin();

        for (;  di < engine.rows();  ++di, ++rp)
        {
            index_type_dst  dj = 0;
            col_iter_src    cp = rp->begin();

            for (;  dj < engine.columns();  ++dj, ++cp)
            {
                engine(di, dj) = static_cast<typename ET::element_type>(*cp);
            }
        }
    }

    template<class ET1, class ET2>
    static constexpr bool
    vector_compare(ET1 const& lhs, ET2 const& rhs)
        requires
            readable_vector_engine<ET1>     and
            readable_vector_engine<ET2>
    {
        using index_type_lhs = typename ET1::index_type;
        using index_type_rhs = typename ET2::index_type;

        index_type_lhs  i1 = 0;
        index_type_lhs  n1 = lhs.size();

        index_type_rhs  i2 = 0;
        index_type_rhs  n2 = rhs.size();

        if (n1 != static_cast<index_type_lhs>(n2)) return false;

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
            readable_vector_engine<ET>
    {
        if (static_cast<size_t>(lhs.size()) != rhs.size()) return false;

        using index_type_lhs = typename ET::index_type;
        using elem_iter_rhs  = decltype(rhs.begin());

        index_type_lhs  di = 0;
        index_type_lhs  dn = lhs.size();
        elem_iter_rhs   ep = rhs.begin();

        for (;  di < dn;  ++di, ++ep)
        {
            if (lhs(di) != *ep) return false;
        }
        return true;
    }

    template<class ET, class T, ptrdiff_t X0, class L, class A>
    static constexpr bool
    vector_compare(ET const& lhs, basic_mdspan<T, extents<X0>, L, A> const& rhs)
        requires
            readable_vector_engine<ET>
    {
        using index_type_lhs = typename ET::index_type;
        using index_type_rhs = typename basic_mdspan<T, extents<X0>, L, A>::index_type;

        index_type_lhs  i1 = 0;
        index_type_lhs  e1 = lhs.size();

        index_type_rhs  i2 = 0;
        index_type_rhs  e2 = rhs.extent(0);

        if (e1 != static_cast<index_type_lhs>(e2)) return false;

        for (;  i1 < e1;  ++i1, ++i2)
        {
            if (lhs(i1) != rhs(i2)) return false;
        }
        return true;
    }

    template<class ET1, class ET2>
    static constexpr bool
    matrix_compare(ET1 const& lhs, ET2 const& rhs)
        requires
            readable_matrix_engine<ET1>     and
            readable_matrix_engine<ET2>
    {
        using index_type_lhs = typename ET1::index_type;
        using index_type_rhs = typename ET2::index_type;

        index_type_lhs  r1 = lhs.rows();
        index_type_lhs  c1 = lhs.columns();
        index_type_rhs  r2 = rhs.rows();
        index_type_rhs  c2 = rhs.columns();

        if (r1 != static_cast<index_type_lhs>(r2)  ||  c1 != static_cast<index_type_lhs>(c2))
            return false;

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
    matrix_compare(ET const& lhs, initializer_list<initializer_list<U>> rhs)
        requires
            readable_matrix_engine<ET>
    {
        size_t  first_row_size = rhs.begin()->size();

        for (auto row : rhs)
        {
            if (row.size() != first_row_size)
            {
                return false;
            }
        }

        if (static_cast<size_t>(lhs.rows()) != rhs.size()) return false;
        if (static_cast<size_t>(lhs.columns()) != first_row_size) return false;

        using index_type_lhs = typename ET::index_type;
        using row_iter_rhs   = decltype(rhs.begin());
        using col_iter_rhs   = decltype(rhs.begin()->begin());

        index_type_lhs  ei = 0;
        index_type_lhs  er = lhs.rows();
        index_type_lhs  ec = lhs.columns();
        row_iter_rhs    rp = rhs.begin();

        for (;  ei < er;  ++ei, ++rp)
        {
            index_type_lhs  ej = 0;
            col_iter_rhs    cp = rp->begin();

            for (;  ej < ec;  ++ej, ++cp)
            {
                if (not (lhs(ei, ej) == *cp)) return false;
            }
        }
        return true;
    }

    template<class ET, class T, ptrdiff_t X0, ptrdiff_t X1, class L, class A>
    static constexpr bool
    matrix_compare(ET const& lhs, basic_mdspan<T, extents<X0, X1>, L, A> const& rhs)
        requires
            readable_matrix_engine<ET>
    {
        using index_type_lhs = typename ET::index_type;
        using index_type_rhs = typename basic_mdspan<T, extents<X0, X1>, L, A>::index_type;

        index_type_lhs  r1 = lhs.rows();
        index_type_lhs  c1 = lhs.columns();
        index_type_rhs  r2 = rhs.extent(0);
        index_type_rhs  c2 = rhs.extent(1);

        if (r1 != static_cast<index_type_lhs>(r2)  ||  c1 != static_cast<index_type_lhs>(c2))
            return false;

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
};

}       //- detail namespace
}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_ENGINE_SUPPORT_HPP_DEFINED
