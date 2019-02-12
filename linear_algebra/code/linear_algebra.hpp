#ifndef LINEAR_ALGEBRA_HPP_DEFINED
#define LINEAR_ALGEBRA_HPP_DEFINED

#include "matrix_test_impl.hpp"

namespace STD_LA {
//- Aliases for column_vector/row_vector/matrix objects based on dynamic engines.
//
template<class T, class A = std::allocator<T>>
using dyn_column_vector = column_vector<dr_matrix_engine<T, A>>;

template<class T, class A = std::allocator<T>>
using dyn_col_vector = dyn_column_vector<T, A>;

template<class T, class A = std::allocator<T>>
using dyn_row_vector = row_vector<dr_matrix_engine<T, A>>;

template<class T, class A = std::allocator<T>>
using dyn_matrix = matrix<dr_matrix_engine<T, A>>;


//- Aliases for column_vector/row_vector/matrix objects based on fixed-size engines.
//
template<class T, size_t R>
using fs_column_vector = column_vector<fs_matrix_engine<T, R, 1>>;

template<class T, size_t R>
using fs_col_vector = fs_column_vector<T, R>;

template<class T, size_t C>
using fs_row_vector = row_vector<fs_matrix_engine<T, 1, C>>;

template<class T, size_t R, size_t C>
using fs_matrix = matrix<fs_matrix_engine<T, R, C>>;

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_HPP_DEFINED
