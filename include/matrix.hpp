//==================================================================================================
//  File:       linear_algebra.hpp
//
//  Summary:    This is a driver header for including all of the linear algebra facilities
//              defined by the library.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_MATRIX_HPP_DEFINED
#define LINEAR_ALGEBRA_MATRIX_HPP_DEFINED

//- Detect the compiler.
//
#if defined __clang__
    #if (__clang_major__ < 10)
        #error unsupported version of Clang
    #endif
    #define LA_COMPILER_CLANG

#elif defined __GNUG__
    #if (__GNUG__ < 10)
        #error unsupported version of GCC
    #endif
    #define LA_COMPILER_GCC
    #define LA_GCC_VERSION  __GNUG__
    #if (__GNUG__ >= 10)
        #define LA_STD_CONCEPTS_HEADER_SUPPORTED
    #endif

#elif defined _MSC_VER
    #if (_MSC_VER < 1925)
        #error unsupported version of VC++
    #endif
    #define LA_COMPILER_MS
    #if (_MSC_VER >= 1925)
        #define LA_STD_CONCEPTS_HEADER_SUPPORTED
    #endif
#endif

//- Namespace alternatives for testing and also for detecting ADL issues.  Pick a pair
//  and attempt to build.
//
//#define STD_LA      la
//#define USING_STD   using namespace std; using namespace std::experimental;

#define STD_LA      std::math
#define USING_STD   using namespace std::experimental;

//#define STD_LA      std::experimental::math
//#define USING_STD   using namespace std::experimental;

#include <cstdint>
#include <array>
#include <complex>
#include <deque>
#include <initializer_list>
#include <memory>
#include <tuple>
#include <type_traits>
#include <vector>

#if defined(LA_STD_CONCEPTS_HEADER_SUPPORTED)
    #include <concepts>
#endif

//- Disable some unnecessary compiler warnings coming from mdspan.
//
#if defined LA_COMPILER_CLANG
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#elif defined LA_COMPILER_GCC
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-parameter"
#elif defined LA_COMPILER_MS
    #pragma warning(push)
    #pragma warning(disable: 4100)
#endif

#include <experimental/mdspan>

#define MDSPAN_NS   std::experimental

namespace STD_LA
{
    USING_STD

    using MDSPAN_NS::extents;
    using MDSPAN_NS::dynamic_extent;
    using MDSPAN_NS::layout_left;
    using MDSPAN_NS::layout_left;
    using MDSPAN_NS::layout_stride;
}

//- Restore the compiler's diagnostic state.
//
#if defined LA_COMPILER_CLANG
    #pragma clang diagnostic pop
#elif defined LA_COMPILER_GCC
    #pragma GCC diagnostic pop
#elif defined LA_COMPILER_MS
    #pragma warning(pop)
#endif

//- Implementation headers.
//
#include "linear_algebra/engine_support.hpp"
#include "linear_algebra/matrix_storage_engine.hpp"
#include "linear_algebra/matrix_view_engine.hpp"
#include "linear_algebra/basic_matrix.hpp"
#include "linear_algebra/operation_traits.hpp"
#include "linear_algebra/addition_traits.hpp"
//#include "linear_algebra/subtraction_traits.hpp"
//#include "linear_algebra/multiplication_traits.hpp"
//#include "linear_algebra/division_traits.hpp"
#include "linear_algebra/arithmetic_operators.hpp"

#include "linear_algebra/debug_helpers.hpp"     //- Helpers for edit/debug/test -- not for production.

#endif  //- LINEAR_ALGEBRA_MATRIX_HPP_DEFINED