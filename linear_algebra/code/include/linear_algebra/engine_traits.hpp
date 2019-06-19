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
template<class ET>
constexpr bool  is_scalar_engine_v = (ET::engine_category::value == scalar_engine_tag::value);

template<class ET>
constexpr bool  is_vector_engine_v =  (ET::engine_category::value >= const_vector_engine_tag::value)
                                   && (ET::engine_category::value <  const_matrix_engine_tag::value);

template<typename ET>
constexpr bool  is_matrix_engine_v = (ET::engine_category::value >= const_matrix_engine_tag::value);


template<class ET1, class ET2>
constexpr bool  engines_match_v =  (is_vector_engine_v<ET1> && is_vector_engine_v<ET2>)
                                || (is_matrix_engine_v<ET1> && is_matrix_engine_v<ET2>);


//--------------------------------------------------------------------------------------------------
//- Tests to determine if an engine has mutable element indexing.
//
template<class ET>
constexpr bool  has_mutable_tag_v = ET::engine_category::value >= mutable_matrix_engine_tag::value;

template<class ET1, class ET2>
constexpr bool  is_mutable_engine_v = is_same_v<ET1, ET2> && has_mutable_tag_v<ET1>;

//------
//
template<class ET1, class ET2>
using enable_if_mutable = enable_if_t<is_mutable_engine_v<ET1, ET2>, bool>;

//--------------------------------------------------------------------------------------------------
//- Tests to determine if an engine is resizable.
//
template<class ET>
constexpr bool  has_resizable_tag_v = ET::engine_category::value >= resizable_matrix_engine_tag::value;

template<class ET>
constexpr bool  is_resizable_engine_v = ET::is_resizable::value;

//------
//
template<class ET1, class ET2>
using enable_if_resizable = enable_if_t<is_same_v<ET1, ET2> && is_resizable_engine_v<ET1>, bool>;


//--------------------------------------------------------------------------------------------------
//- Tests/facilites for fixed-size engines.
//
template<class ET>
constexpr bool  is_fixed_size_engine_v = ET::is_fixed_size::value;

template<class ET>
constexpr auto  engine_size_v = ET().size();

//------
//
template<class ET1, class ET2>
using enable_if_fixed_size = enable_if_t<is_same_v<ET1, ET2> && is_fixed_size_engine_v<ET1>, bool>;



//--------------------------------------------------------------------------------------------------
//- Temporary replacement for std::swap (which is constexpr in C++20)
//
template<class T>
static constexpr bool   is_movable_v =  std::is_move_constructible_v<T> 
                                     && std::is_move_assignable_v<T>;

template<class T>
static constexpr bool   is_nothrow_movable_v =  std::is_nothrow_move_constructible_v<T> 
                                             && std::is_nothrow_move_assignable_v<T>;

template<class T> inline 
constexpr std::enable_if_t<is_movable_v<T>, void>
la_swap(T& t0, T& t1) noexcept(is_nothrow_movable_v<T>)
{
    T   t2(std::move(t0));
    t0 = std::move(t1);
    t1 = std::move(t2);
}


}       //- detail namespace
}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_ENGINE_TRAITS_HPP_DEFINED
