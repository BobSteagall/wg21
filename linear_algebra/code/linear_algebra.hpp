#ifndef LINEAR_ALGEBRA_HPP_DEFINED
#define LINEAR_ALGEBRA_HPP_DEFINED

#include "matrix_operators.hpp"

namespace std::la {
//- Aliases for matrix/row_vector/column_vector objects based on dynamic engines.
//
template<class T, class A = std::allocator<T>>
using dyn_column_vector = column_vector<dr_matrix_engine<T, A>>;

template<class T, class A = std::allocator<T>>
using dyn_col_vector = dyn_column_vector<T, A>;

template<class T, class A = std::allocator<T>>
using dyn_row_vector = row_vector<dr_matrix_engine<T, A>>;

template<class T, class A = std::allocator<T>>
using dyn_matrix = matrix<dr_matrix_engine<T, A>>;


//- Aliases for matrix/row_vector/column_vector objects based on fixed-size engines.
//
template<class T, size_t R>
using fs_column_vector = column_vector<fs_matrix_engine<T, R, 1>>;

template<class T, size_t R>
using fs_col_vector = fs_column_vector<T, R>;

template<class T, size_t C>
using fs_row_vector = row_vector<fs_matrix_engine<T, 1, C>>;

template<class T, size_t R, size_t C>
using fs_matrix = matrix<fs_matrix_engine<T, R, C>>;

}       //- std::la namespace
#endif  //- LINEAR_ALGEBRA_HPP_DEFINED
