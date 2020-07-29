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
concept valid_mse_extents = is_valid_engine_extents_v<X>;


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


}       //- detail namespace
}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_MATRIX_STORAGE_ENGINE_SUPPORT_HPP_DEFINED
