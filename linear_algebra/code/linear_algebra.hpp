#ifndef LINEAR_ALGEBRA_HPP_DEFINED
#define LINEAR_ALGEBRA_HPP_DEFINED

#include <cstdint>
#include <complex>
#include <memory>
#include <tuple>
#include <type_traits>

//- These are some helpers, used for printing useful information.
//
#include <iostream>
#include <cxxabi.h>

inline std::string
GetTypeName(std::type_info const& info)
{
    using CrtlStrPtr = std::unique_ptr<char, decltype(&::free)>;

    int         status = 0;
    CrtlStrPtr  pName{abi::__cxa_demangle(info.name(), nullptr, nullptr, &status), &::free};

    return pName.get();
}

template<class T>
inline std::string
GetTypeName()
{
    return GetTypeName(typeid(T));
}

template<class T>
inline std::string
GetTypeName(T const& t)
{
    return GetTypeName(typeid(t));
}

using std::cout;    //- Yes, we're cheating....
using std::endl;


//- Namespace alternatives to for testing and avoiding ADL issues.
//
//#define STD_LA  std
#define STD_LA  la
//#define STD_LA  std::experimental::la


//- OK, now include implementation headers.
//
#include "matrix_fwd.hpp"
#include "matrix_element_traits.hpp"
#include "matrix_engines.hpp"
#include "matrix_element_traits.hpp"
#include "matrix.hpp"
#include "matrix_arithmetic_traits.hpp"
#include "matrix_operator_traits.hpp"
#include "matrix_operators.hpp"
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
