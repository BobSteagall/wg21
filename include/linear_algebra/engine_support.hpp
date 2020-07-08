//==================================================================================================
//  File:       engine_support.hpp
//
//  Summary:    This header defines various common types, traits, concepts, and functions used
//              by matrix_storage_engine and matrix_view_engine.
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
//  Concepts:   same_types<T, U>,  returns<T, U>,  convertible_element<T, U>
//
//  These are some very simple private utility concepts, used by the other concepts below.
//--------------------------------------------------------------------------------------------------
//
template<typename T, typename U>
concept same_types = is_same_v<T, U>;

template<typename T, typename U>
concept returns = is_same_v<T, U>;

template<typename T, typename U>
concept convertibility = is_convertible_v<T, U>;


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
    requires
    {
        typename ET::element_type;
        typename ET::index_type;
        typename ET::reference;
        typename ET::const_reference;
        requires is_lvalue_reference_v<typename ET::reference>;
    }
    and
    requires (ET const& eng, typename ET::index_type i)
    {
    #ifdef LA_COMPOUND_REQUIREMENT_SYNTAX_SUPPORTED
        { eng.size() } -> returns<typename ET::index_type>;
        { eng(i)     } -> returns<typename ET::const_reference>;
    #else
        requires is_same_v<decltype(eng.size()), typename ET::index_type>;
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
        { eng(i)     } -> returns<typename ET::reference>;
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
        { meng.span() } -> returns<typename ET::span_type>;
        { ceng.span() } -> returns<typename ET::const_span_type>;
    #else
        requires is_same_v<decltype(meng.span()), typename ET::span_type>;
        requires is_same_v<decltype(ceng.span()), typename ET::const_span_type>;
    #endif
    };

//--------------------------------------------------------------------------------------------------
//  Concepts:   readable_matrix_engine<ET>
//              writable_matrix_engine<ET>
//              initable_matrix_engine<ET>
//              column_resizable_matrix_engine<ET>
//              row_resizable_matrix_engine<ET>
//              resizable_matrix_engine<ET>
//              spannable_matrix_engine<ET>
//
//  These concepts describe the capabilities that must be provided by each matrix engine category
//  in order to function correctly with some subset of services provided by class template
//  basic_matrix<ET, OT>.
//--------------------------------------------------------------------------------------------------
//
template<class ET>
concept readable_matrix_engine =
    requires
    {
        typename ET::element_type;
        typename ET::index_type;
        typename ET::index_tuple_type;
        typename ET::reference;
        typename ET::const_reference;
        requires is_lvalue_reference_v<typename ET::reference>;
    }
    and
    requires (ET const& eng, typename ET::index_type i)
    {
    #ifdef LA_COMPOUND_REQUIREMENT_SYNTAX_SUPPORTED
        { eng.columns() } -> returns<typename ET::index_type>;
        { eng.rows()    } -> returns<typename ET::index_type>;
        { eng.size()    } -> returns<typename ET::index_tuple_type>;
        { eng(i, i)     } -> returns<typename ET::const_reference>;
    #else
        requires is_same_v<decltype(eng.columns()), typename ET::index_type>;
        requires is_same_v<decltype(eng.rows()), typename ET::index_type>;
        requires is_same_v<decltype(eng.size()), typename ET::index_tuple_type>;
        requires is_same_v<decltype(eng(i, i)), typename ET::const_reference>;
    #endif
    };

template<class ET>
concept writable_matrix_engine =
    readable_matrix_engine<ET>
    and
    requires (ET& eng, typename ET::index_type i, typename ET::element_type v)
    {
        requires is_same_v<typename ET::reference, typename ET::element_type&>;
        { eng(i, i) = v };
    #ifdef LA_COMPOUND_REQUIREMENT_SYNTAX_SUPPORTED
        { eng(i, i)     } -> returns<typename ET::reference>;
    #else
        requires is_same_v<decltype(eng(i, i)), typename ET::reference>;
    #endif
    };

template<class ET>
concept initable_matrix_engine =
    writable_matrix_engine<ET>  
    and
    requires (ET& eng, initializer_list<initializer_list<typename ET::element_type>> l)
    {
        { ET::ET(l) };
        { eng = l   };
    };

template<class ET>
concept column_resizable_matrix_engine =
    initable_matrix_engine<ET>  
    and
    requires (ET& eng, typename ET::element_type i)
    {
        { eng.reshape_columns(i, i) };
    };

template<class ET>
concept row_resizable_matrix_engine =
    initable_matrix_engine<ET>  
    and
    requires (ET& eng, typename ET::element_type i)
    {
        { eng.reshape_rows(i, i) };
    };

template<class ET>
concept resizable_matrix_engine =
    initable_matrix_engine<ET>  
    and
    requires (ET& eng, typename ET::element_type i)
    {
        { eng.reshape(i, i, i, i) };
    };

template<class ET>
concept spannable_matrix_engine =
    readable_matrix_engine<ET>  
    and
    requires (ET const& ceng, ET& meng)
    {
        typename ET::span_type;
        typename ET::const_span_type;
        requires is_mdspan_v<typename ET::span_type>;
        requires is_mdspan_v<typename ET::const_span_type>;
    #ifdef LA_COMPOUND_REQUIREMENT_SYNTAX_SUPPORTED
        { meng.span() } -> returns<typename ET::span_type>;
        { ceng.span() } -> returns<typename ET::const_span_type>;
    #else
        requires is_same_v<decltype(meng.span()), typename ET::span_type>;
        requires is_same_v<decltype(ceng.span()), typename ET::const_span_type>;
    #endif
    };


}       //- detail namespace
}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_ENGINE_SUPPORT_HPP_DEFINED
