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


template<class ET>
constexpr bool  is_fixed_size_engine_v = ET::is_fixed_size::value;


}       //- detail namespace
}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_ENGINE_TRAITS_HPP_DEFINED
