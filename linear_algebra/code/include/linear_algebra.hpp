#ifndef LINEAR_ALGEBRA_HPP_DEFINED
#define LINEAR_ALGEBRA_HPP_DEFINED

#include <cstdint>
#include <complex>
#include <memory>
#include <tuple>
#include <type_traits>

//-------------------------------------------------------------------------------------------------
//  Helper functions and utilities for testing the interfacce; not part of the final proposal!
//-------------------------------------------------------------------------------------------------
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

using std::cout;    //- Yes, we're cheating, but only for convenience, we promise....
using std::endl;

#define PRINT_FN_NAME(name) \
    std::cout \
        << "********************************************************************************\n" \
        << "********************************************************************************\n" \
        << "entering test function " << #name << "()" << std::endl << std::endl


//-------------------------------------------------------------------------------------------------
//- Namespace alternatives for testing and also for detecting/avoiding ADL issues.
//
#define STD_LA  la
//#define STD_LA  std
//#define STD_LA  std::math


//- OK, now include implementation headers.
//
#include "linear_algebra/forward_declarations.hpp"
#include "linear_algebra/element_promotion_traits.hpp"
#include "linear_algebra/dynamic_engines.hpp"
#include "linear_algebra/fixed_size_engines.hpp"
#include "linear_algebra/view_based_engines.hpp"
#include "linear_algebra/engine_promotion_traits.hpp"
#include "linear_algebra/vector.hpp"
#include "linear_algebra/matrix.hpp"
#include "linear_algebra/arithmetic_traits.hpp"
#include "linear_algebra/operator_traits.hpp"
#include "linear_algebra/operator_promotion_traits.hpp"
#include "linear_algebra/arithmetic_operators.hpp"

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_HPP_DEFINED
