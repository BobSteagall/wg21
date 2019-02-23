#ifndef LINEAR_ALGEBRA_HPP_DEFINED
#define LINEAR_ALGEBRA_HPP_DEFINED

#include <cstdint>
#include <complex>
#include <memory>
#include <tuple>
#include <type_traits>
#include <typeinfo>
#include <string_view>

//- These are some helpers, used for printing useful information.
//
#include <iostream>

template <class T>
constexpr std::string_view
type_name()                //- Thanks to Howard Hinnant for this!
{
    using namespace std;
#ifdef __clang__
    string_view p = __PRETTY_FUNCTION__;
    return string_view(p.data() + 34, p.size() - 34 - 1);
#elif defined(__GNUC__)
    string_view p = __PRETTY_FUNCTION__;
#  if __cplusplus < 201402
    return string_view(p.data() + 36, p.size() - 36 - 1);
#  else
    return string_view(p.data() + 49, p.find(';', 49) - 49);
#  endif
#elif defined(_MSC_VER)
    string_view p = __FUNCSIG__;
    return string_view(p.data() + 84, p.size() - 84 - 7);
#endif
}

template<class T>
inline std::string
get_type_name()
{
    auto    view = type_name<T>();
    return std::string(view.data(), view.size());
}

template<class T>
inline std::string
get_type_name(T const&)
{
    auto    view = type_name<T>();
    return std::string(view.data(), view.size());
}

using std::cout;    //- Yes, we're cheating....
using std::endl;

#define PRINT_FN_NAME(name) \
    std::cout \
        << "********************************************************************************\n" \
        << "********************************************************************************\n" \
        << "entering test function " << #name << "()" << std::endl << std::endl


//- Namespace alternatives for testing and avoiding ADL issues.
//
//#define STD_LA  std
#define STD_LA  la
//#define STD_LA  std::experimental::la


//- OK, now include implementation headers.
//
#include "matrix_fwd.hpp"
#include "matrix_element_traits.hpp"
#include "matrix_engines.hpp"
#include "matrix_engine_traits.hpp"
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
