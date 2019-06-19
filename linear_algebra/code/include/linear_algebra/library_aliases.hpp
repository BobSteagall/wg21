//==================================================================================================
//  File:       library_aliases.hpp
//
//  Summary:    This header defines several public type aliases, intended as a convenience for
//              the user.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_LIBRARY_ALIASES_HPP_DEFINED
#define LINEAR_ALGEBRA_LIBRARY_ALIASES_HPP_DEFINED

//- Convenience aliases.
//
namespace STD_LA {

//- Aliases for column_vector/row_vector/matrix objects based on dynamic engines.
//
template<class T, class A = allocator<T>>
using dyn_vector = vector<dr_vector_engine<T, A>>;

template<class T, class A = allocator<T>>
using dyn_matrix = matrix<dr_matrix_engine<T, A>>;


//- Aliases for column_vector/row_vector/matrix objects based on fixed-size engines.
//
template<class T, int32_t N>
using fs_vector = vector<fs_vector_engine<T, N>>;

template<class T, int32_t R, int32_t C>
using fs_matrix = matrix<fs_matrix_engine<T, R, C>>;

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_LIBRARY_ALIASES_HPP_DEFINED
