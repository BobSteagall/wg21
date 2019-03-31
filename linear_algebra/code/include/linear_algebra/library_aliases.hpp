#ifndef LINEAR_ALGEBRA_LIBRARY_ALIASES_HPP_DEFINED
#define LINEAR_ALGEBRA_LIBRARY_ALIASES_HPP_DEFINED

#include "arithmetic_operators.hpp"

//- Convenience aliases.
//
namespace STD_LA {

//- Aliases for column_vector/row_vector/matrix objects based on dynamic engines.
//
template<class T, class A = std::allocator<T>>
using dyn_vector = STD_LA::vector<STD_LA::dr_matrix_engine<T, A>>;

template<class T, class A = std::allocator<T>>
using dyn_matrix = STD_LA::matrix<STD_LA::dr_matrix_engine<T, A>>;


//- Aliases for column_vector/row_vector/matrix objects based on fixed-size engines.
//
template<class T, size_t N>
using fs_vector = STD_LA::vector<STD_LA::fs_vector_engine<T, N>>;

template<class T, size_t R, size_t C>
using fs_matrix = STD_LA::matrix<STD_LA::fs_matrix_engine<T, R, C>>;

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_LIBRARY_ALIASES_HPP_DEFINED
