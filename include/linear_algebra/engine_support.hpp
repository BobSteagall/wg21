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

template<class T>
struct is_mdspan : public false_type
{};

template<class T, class L, class A, ptrdiff_t... x>
struct is_mdspan<basic_mdspan<T, extents<x...>, L, A>> : public true_type
{};

template<class T> inline constexpr
bool    is_mdspan_v = is_mdspan<T>::value;


//--------------------------------------------------------------------------------------------------
//  Concepts:   same_types<T, U>,  same_as<T, U>,  convertible_element<T, U>
//
//  These are some very simple private utility concepts, used by the other concepts below.
//--------------------------------------------------------------------------------------------------
//
template<typename T, typename U>
concept element_convertibility = convertible_to<T, U>;


//--------------------------------------------------------------------------------------------------
//  Concepts:   readable_vector_engine<ET>
//              writable_vector_engine<ET>
//              initable_vector_engine<ET>
//              resizable_vector_engine<ET>
//              spannable_vector_engine<ET>
//
//  These concepts describe the capabilities that must be provided by each vector engine category
//  in order to function correctly with some subset of services provided by class template
//  basic_vector<ET, OT>.
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
concept writable_vector_engine =
    readable_vector_engine<ET>
    and
    requires (ET& eng, typename ET::index_type i, typename ET::element_type v)
    {
        requires is_same_v<typename ET::reference, typename ET::element_type&>;
        { eng(i) = v };

    #ifdef LA_COMPOUND_REQUIREMENT_SYNTAX_SUPPORTED
        { eng(i) } -> same_as<typename ET::reference>;
    #else
        requires is_same_v<decltype(eng(i)), typename ET::reference>;
    #endif
    };

template<class ET>
concept initable_vector_engine =
    writable_vector_engine<ET>
    and
    requires (ET& eng, initializer_list<typename ET::element_type> l)
    {
        { ET::ET(l) };
        { eng = l   };
    };

template<class ET, class U>
concept initlist_constructible_vector_engine =
    writable_vector_engine<ET>  and
    requires (void* pdata, initializer_list<U> list)
    {
        { ::new (pdata) ET(list) };
    };

template<class ET, class U>
concept initlist_assignable_vector_engine =
    writable_vector_engine<ET>  and
    requires (ET& eng, initializer_list<U> list)
    {
        { eng = list };
    };

template<class ET>
concept resizable_vector_engine =
    readable_vector_engine<ET>
    and
    requires (ET& eng, typename ET::index_type i)
    {
        { eng.resize(i)     };
        { eng.reserve(i)    };
        { eng.reshape(i, i) };
    };

template<class ET>
concept spannable_vector_engine =
    readable_vector_engine<ET>
    and
    requires (ET const& ceng, ET& meng)
    {
        typename ET::span_type;
        typename ET::const_span_type;
        requires is_mdspan_v<typename ET::span_type>;
        requires is_mdspan_v<typename ET::const_span_type>;

    #ifdef LA_COMPOUND_REQUIREMENT_SYNTAX_SUPPORTED
        { meng.span() } -> same_as<typename ET::span_type>;
        { ceng.span() } -> same_as<typename ET::const_span_type>;
    #else
        requires is_same_v<decltype(meng.span()), typename ET::span_type>;
        requires is_same_v<decltype(ceng.span()), typename ET::const_span_type>;
    #endif
    };

//--------------------------------------------------------------------------------------------------
//  Concepts:   readable_matrix_engine<ET>
//              writable_matrix_engine<ET>
//              initable_matrix_engine<ET>
//              column_reshapable_matrix_engine<ET>
//              row_reshapable_matrix_engine<ET>
//              reshapable_matrix_engine<ET>
//              spannable_matrix_engine<ET>
//
//  These concepts describe the capabilities that must be provided by each matrix engine category
//  in order to function correctly with some subset of services provided by class template
//  basic_matrix<ET, OT>.
//--------------------------------------------------------------------------------------------------
//
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
concept spannable_matrix_engine =
    readable_matrix_engine<ET>  and
    requires (ET const& ceng, ET& meng)
    {
        typename ET::span_type;
        typename ET::const_span_type;
        requires is_mdspan_v<typename ET::span_type>;
        requires is_mdspan_v<typename ET::const_span_type>;

    #ifdef LA_COMPOUND_REQUIREMENT_SYNTAX_SUPPORTED
        { meng.span() } -> same_as<typename ET::span_type>;
        { ceng.span() } -> same_as<typename ET::const_span_type>;
    #else
        requires is_same_v<decltype(meng.span()), typename ET::span_type>;
        requires is_same_v<decltype(ceng.span()), typename ET::const_span_type>;
    #endif
    };

template<class ET>
concept writable_matrix_engine =
    readable_matrix_engine<ET>  and
    requires (ET& eng, typename ET::index_type i, typename ET::element_type v)
    {
        requires is_same_v<typename ET::reference, typename ET::element_type&>;
        { eng(i, i) = v };

    #ifdef LA_COMPOUND_REQUIREMENT_SYNTAX_SUPPORTED
        { eng(i, i) } -> same_as<typename ET::reference>;
    #else
        requires is_same_v<decltype(eng(i, i)), typename ET::reference>;
    #endif
    };

template<class ET>
concept initable_matrix_engine =
    writable_matrix_engine<ET>  and
    requires (ET& eng, initializer_list<initializer_list<typename ET::element_type>> l)
    {
        { ET::ET(l) };
        { eng = l   };
    };

template<class ET, class U>
concept initlist_constructible_matrix_engine =
    writable_matrix_engine<ET>
    and
    requires (void* pdata, initializer_list<initializer_list<U>> list)
    {
        { ::new (pdata) ET(list) };
    };

template<class ET, class U>
concept initlist_assignable_matrix_engine =
    writable_matrix_engine<ET>
    and
    requires (ET& eng, initializer_list<initializer_list<U>> list)
    {
        { eng = list };
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

template<class ET>
concept reshapable_matrix_engine =
    writable_matrix_engine<ET>  and
    requires (ET& eng, typename ET::index_type i)
    {
        { eng.reshape(i, i, i, i) };
    };


//--------------------------------------------------------------------------------------------------
//  Class Template:     engine_support_base
//
//  Policy/traits base type that provides various types and functions for verification.
//--------------------------------------------------------------------------------------------------
//
struct engine_support_base
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
};

}       //- detail namespace
}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_ENGINE_SUPPORT_HPP_DEFINED
