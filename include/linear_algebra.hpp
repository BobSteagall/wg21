//==================================================================================================
//  File:       linear_algebra.hpp
//
//  Summary:    This is a driver header for including all of the linear algebra facilities
//              defined by the library.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_HPP_DEFINED
#define LINEAR_ALGEBRA_HPP_DEFINED

//- Namespace alternatives for testing and also for detecting ADL issues.  Pick a pair
//  and attempt to build.
//
//#define STD_LA      la
//#define USING_STD   using namespace std; using namespace std::experimental;

#define STD_LA      std::math
#define USING_STD   using namespace std; using namespace std::experimental;

//#define STD_LA      std::experimental::math
//#define USING_STD   using namespace std::experimental;

#include <version>
#include <cstdint>
#include <array>
#include <complex>
#include <initializer_list>
#include <memory>
#include <tuple>
#include <type_traits>

//- Disable some unnecessary compiler warnings (i.e., noise) coming from mdspan.
//
#if defined __clang__
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#elif defined __GNUG__
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-parameter"
#elif defined _MSC_VER
    #pragma warning(push)
    #pragma warning(disable: 4100)
#endif

#include <experimental/mdspan>
using std::experimental::dynamic_extent;

//- Restore the compiler's diagnostic state.
//
#if defined __clang__
    #pragma clang diagnostic pop
#elif defined __GNUG__
    #pragma GCC diagnostic pop
#elif defined _MSC_VER
    #pragma warning(pop)
#endif

//- Implementation headers.
//
#include "linear_algebra/forward_declarations.hpp"
#include "linear_algebra/private_support.hpp"
#include "linear_algebra/public_support.hpp"
#include "linear_algebra/fixed_size_vector_engine.hpp"
#include "linear_algebra/fixed_size_matrix_engine.hpp"
#include "linear_algebra/dynamic_vector_engine.hpp"
#include "linear_algebra/dynamic_matrix_engine.hpp"
#include "linear_algebra/vector_view_engine.hpp"
#include "linear_algebra/matrix_view_engine.hpp"
#include "linear_algebra/vector_iterators.hpp"
#include "linear_algebra/vector.hpp"
#include "linear_algebra/matrix.hpp"

#include "linear_algebra/debug_helpers.hpp"     //- Helpers for debug/test -- not for production.

#include "linear_algebra/addition_traits.hpp"
#include "linear_algebra/subtraction_traits.hpp"
#include "linear_algebra/negation_traits.hpp"
#include "linear_algebra/multiplication_traits.hpp"
#include "linear_algebra/operation_traits.hpp"
#include "linear_algebra/arithmetic_operators.hpp"

#endif  //- LINEAR_ALGEBRA_HPP_DEFINED
