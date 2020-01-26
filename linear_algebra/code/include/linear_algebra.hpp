//==================================================================================================
//  File:       linear_algebra.hpp
//
//  Summary:    This is a driver header for including all of the linear algebra facilities
//              defined by the library.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_HPP_DEFINED
#define LINEAR_ALGEBRA_HPP_DEFINED

//- Some build options for dev/debug/test.
//
//#define LA_USE_MDSPAN

//- Namespace alternatives for testing and also for detecting  ADL issues.  Pick a pair
//  and attempt to build.
//
//#define STD_LA      la
//#define USING_STD   using namespace std; using namespace std::experimental;

//#define STD_LA      std::math
//#define USING_STD

#define STD_LA      std::experimental::math
#define USING_STD   using namespace std::experimental;

#include <cstdint>
#include <algorithm>
#include <array>
#include <complex>
#include <initializer_list>
#include <memory>
#include <numeric>
#include <tuple>
#include <type_traits>

//- Disable some compiler warnings (noise) coming from mdspan.
//
#if defined _MSC_VER
    #undef LA_USE_MDSPAN
#elif defined __clang__
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#elif defined __GNUG__
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

#ifdef LA_USE_MDSPAN
    #include <experimental/mdspan>
#endif

//- Restore the compiler's diagnostic state.
//
#if defined _MSC_VER
#elif defined __clang__
    #pragma clang diagnostic pop
#elif defined __GNUG__
    #pragma GCC diagnostic pop
#endif

//- Implementation headers.
//
#include "linear_algebra/forward_declarations.hpp"
#include "linear_algebra/private_support.hpp"
#include "linear_algebra/public_support.hpp"
#include "linear_algebra/dynamic_vector_engine.hpp"
#include "linear_algebra/dynamic_matrix_engine.hpp"
#include "linear_algebra/fixed_size_vector_engine.hpp"
#include "linear_algebra/fixed_size_matrix_engine.hpp"
#include "linear_algebra/subvector_engine.hpp"
#include "linear_algebra/column_engine.hpp"
#include "linear_algebra/row_engine.hpp"
#include "linear_algebra/submatrix_engine.hpp"
#include "linear_algebra/transpose_engine.hpp"
#include "linear_algebra/vector_iterators.hpp"
#include "linear_algebra/vector.hpp"
#include "linear_algebra/matrix.hpp"

#include "linear_algebra/debug_helpers.hpp"     //- helpers for debug/test.  Not for production.

#include "linear_algebra/addition_traits.hpp"
#include "linear_algebra/addition_traits_impl.hpp"
#include "linear_algebra/subtraction_traits.hpp"
#include "linear_algebra/subtraction_traits_impl.hpp"
#include "linear_algebra/negation_traits.hpp"
#include "linear_algebra/negation_traits_impl.hpp"
#include "linear_algebra/multiplication_traits.hpp"
#include "linear_algebra/multiplication_traits_impl.hpp"
#include "linear_algebra/operation_traits.hpp"
#include "linear_algebra/arithmetic_operators.hpp"
#include "linear_algebra/library_aliases.hpp"

#endif  //- LINEAR_ALGEBRA_HPP_DEFINED
