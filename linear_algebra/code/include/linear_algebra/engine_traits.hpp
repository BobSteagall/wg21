//==================================================================================================
//  File:       engine_traits.hpp
//
//  Summary:    This header defines several private traits types, used by the vector and matrix
//              class templates, that report properties of an engine.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_ENGINE_TRAITS_HPP_DEFINED
#define LINEAR_ALGEBRA_ENGINE_TRAITS_HPP_DEFINED

namespace STD_LA {
namespace detail {

struct row_column_tag {};

template<class TAG>
struct tag_traits;

template<>
struct tag_traits<const_vector_engine_tag>
{
    static constexpr bool   is_vector_engine = true;
    static constexpr bool   is_matrix_engine = false;
    static constexpr bool   is_mutable       = false;
    static constexpr bool   is_resizable     = false;
};

template<>
struct tag_traits<mutable_vector_engine_tag>
{
    static constexpr bool   is_vector_engine = true;
    static constexpr bool   is_matrix_engine = false;
    static constexpr bool   is_mutable       = true;
    static constexpr bool   is_resizable     = false;
};

template<>
struct tag_traits<resizable_vector_engine_tag>
{
    static constexpr bool   is_vector_engine = true;
    static constexpr bool   is_matrix_engine = false;
    static constexpr bool   is_mutable       = true;
    static constexpr bool   is_resizable     = true;
};

template<>
struct tag_traits<const_matrix_engine_tag>
{
    static constexpr bool   is_vector_engine = false;
    static constexpr bool   is_matrix_engine = true;
    static constexpr bool   is_mutable       = false;
    static constexpr bool   is_resizable     = false;
};

template<>
struct tag_traits<mutable_matrix_engine_tag>
{
    static constexpr bool   is_vector_engine = false;
    static constexpr bool   is_matrix_engine = true;
    static constexpr bool   is_mutable       = true;
    static constexpr bool   is_resizable     = false;
};

template<>
struct tag_traits<resizable_matrix_engine_tag>
{
    static constexpr bool   is_vector_engine = false;
    static constexpr bool   is_matrix_engine = true;
    static constexpr bool   is_mutable       = true;
    static constexpr bool   is_resizable     = true;
};

template<bool is_vector, class ET> 
struct vector_iterator_traits;

template<class ET>
struct vector_iterator_traits<true, ET>
{
    using const_iterator = typename ET::const_iterator;
    using iterator       = typename ET::iterator;
};

template<class ET>
struct vector_iterator_traits<false, ET>
{
    using const_iterator = void;
    using iterator       = void;
};

template<bool is_vector, class ET>
using iterator_of_t = typename vector_iterator_traits<is_vector, ET>::iterator;

template<bool is_vector, class ET>
using const_iterator_of_t = typename vector_iterator_traits<is_vector, ET>::const_iterator;

template<class ET>
struct matrix_view_traits
{
    using traits_type = tag_traits<typename ET::engine_category>;

    static constexpr bool   is_vector_engine = traits_type::is_vector_engine;
    static constexpr bool   is_matrix_engine = traits_type::is_matrix_engine;
    static constexpr bool   is_mutable       = traits_type::is_mutable;
    static constexpr bool   is_resizable     = traits_type::is_resizable;

    using vector_view_category = conditional_t<is_mutable, mutable_vector_engine_tag, const_vector_engine_tag>;
    using matrix_view_category = conditional_t<is_mutable, mutable_matrix_engine_tag, const_matrix_engine_tag>;

    using reference       = conditional_t<is_mutable, typename ET::reference, typename ET::const_reference>;
    using const_reference = typename ET::const_reference;

    using mutable_iterator = iterator_of_t<is_vector_engine, ET>;
    using const_iterator   = const_iterator_of_t<is_vector_engine, ET>;
    using iterator         = conditional_t<is_mutable, mutable_iterator, const_iterator>;
};

template<class ET>
struct matrix_view_traits<ET const>
{
    using traits_type = tag_traits<typename ET::engine_category>;

    static constexpr bool   is_vector_engine = traits_type::is_vector_engine;
    static constexpr bool   is_matrix_engine = traits_type::is_matrix_engine;
    static constexpr bool   is_mutable       = false;
    static constexpr bool   is_resizable     = false;

    using vector_view_category = const_vector_engine_tag;
    using matrix_view_category = const_matrix_engine_tag;

    using const_reference = typename ET::const_reference;
    using reference       = typename ET::const_reference;

    using const_iterator = const_iterator_of_t<is_vector_engine, ET>;
    using iterator       = const_iterator;
};

//--------------------------------------------------------------------------------------------------
//- Internally-used tag type to facilitate distinguishing elements from vectors/matrices.
//
template<class T>
struct element_tag
{
    using engine_category = scalar_engine_tag;
};


//--------------------------------------------------------------------------------------------------
//- Internally-used constexpr variables for describe an engine's category.
//
template<class ET> inline constexpr 
bool    is_scalar_engine_v = (ET::engine_category::value == scalar_engine_tag::value);

template<class ET> inline constexpr 
bool    is_vector_engine_v = matrix_view_traits<ET>::is_vector_engine;

template<typename ET> inline constexpr 
bool    is_matrix_engine_v = matrix_view_traits<ET>::is_matrix_engine;

template<class ET1, class ET2> inline constexpr 
bool    engines_match_v = (is_matrix_engine_v<ET1> == is_matrix_engine_v<ET2>);


//--------------------------------------------------------------------------------------------------
//- Tests to determine if an engine has mutable element indexing.
//
template<class ET> inline constexpr 
bool    is_mutable_engine_v = matrix_view_traits<ET>::is_mutable;

template<class ET1, class ET2>
using enable_if_mutable = enable_if_t<is_same_v<ET1, ET2> && is_mutable_engine_v<ET1>, bool>;


//--------------------------------------------------------------------------------------------------
//- Tests to determine if an engine is resizable.
//
template<class ET> inline constexpr 
bool    is_resizable_engine_v = matrix_view_traits<ET>::is_resizable;

template<class ET1, class ET2>
using enable_if_resizable = enable_if_t<is_same_v<ET1, ET2> && is_resizable_engine_v<ET1>, bool>;


//--------------------------------------------------------------------------------------------------
//- Tests/facilites for fixed-size engines.
//
template<class ET> inline constexpr 
bool    is_fixed_size_engine_v = !is_resizable_engine_v<ET>;

template<class ET> inline constexpr 
auto    engine_size_v = ET().size();

//------
//
template<class ET1, class ET2>
using enable_if_fixed_size = enable_if_t<is_same_v<ET1, ET2> && is_fixed_size_engine_v<ET1>, bool>;


//--------------------------------------------------------------------------------------------------
//- Temporary replacement for std::swap (which is constexpr in C++20)
//
template<class T> inline constexpr 
bool    is_movable_v = std::is_move_constructible_v<T> && std::is_move_assignable_v<T>;

template<class T> inline constexpr 
bool    is_nothrow_movable_v = std::is_nothrow_move_constructible_v<T> &&  
                               std::is_nothrow_move_assignable_v<T>;

template<class T> constexpr 
std::enable_if_t<is_movable_v<T>, void>
la_swap(T& t0, T& t1) noexcept(is_nothrow_movable_v<T>)
{
    T   t2(std::move(t0));
    t0 = std::move(t1);
    t1 = std::move(t2);
}

template<class ET>
using index_t = typename ET::size_type;

template<class ET>
using vector_view_reference_t = decltype(declval<ET>()(declval<typename ET::size_type>()));

template<class ET>
using matrix_view_reference_t = decltype(declval<ET>()(declval<typename ET::size_type>(),
                                                       declval<typename ET::size_type>()));

template<class ET>
using vector_view_iterator_t = decltype(declval<ET>().begin());

template<class ET>
using matrix_view_iterator_t = decltype(declval<ET>().begin());
/*
template<class DST_TAG, class ET>
struct dest_view_traits;

template<class ET>
struct dest_view_traits<const_vector_engine_tag, ET>
{
    using tag_type  = const_vector_engine_tag;
    using reference = typename ET::const_reference;
    using iterator  = typename ET::const_iterator;
};

template<class ET>
struct dest_view_traits<mutable_vector_engine_tag, ET>
{
    using tag_type  = mutable_vector_engine_tag;
    using reference = typename ET::reference;
    using iterator  = typename ET::iterator;
};

template<class ET>
struct dest_view_traits<resizable_vector_engine_tag, ET>
{
    using tag_type  = resizable_vector_engine_tag;
    using reference = typename ET::reference;
    using iterator  = typename ET::iterator;
};
*/
}       //- detail namespace
}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_ENGINE_TRAITS_HPP_DEFINED
