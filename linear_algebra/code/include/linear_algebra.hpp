//==================================================================================================
//  File:       linear_algebra.hpp
//
//  Summary:    This is a driver header for including all of the linear algebra facilities
//              defined by the library.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_HPP_DEFINED
#define LINEAR_ALGEBRA_HPP_DEFINED

#include <cstdint>
#include <algorithm>
#include <complex>
#include <initializer_list>
#include <memory>
#include <numeric>
#include <tuple>
#include <type_traits>

//-------------------------------------------------------------------------------------------------
//- Namespace alternatives for testing and also for detecting/avoiding ADL issues.  Pick a pair
//  and attempt to build.
//
#define STD_LA      la
#define USING_STD   using namespace std;

//#define STD_LA      std
//#define USING_STD

//#define STD_LA  std::math
//#define USING_STD

//- Implementation headers.
//
#include "linear_algebra/forward_declarations.hpp"
#include "linear_algebra/number_traits.hpp"
#include "linear_algebra/engine_traits.hpp"
#include "linear_algebra/vector_iterators.hpp"
#include "linear_algebra/row_column_views.hpp"
#include "linear_algebra/transpose_views.hpp"
#include "linear_algebra/dynamic_engines.hpp"
#include "linear_algebra/fixed_size_engines.hpp"
#include "linear_algebra/vector.hpp"
#include "linear_algebra/matrix.hpp"
#include "linear_algebra/library_aliases.hpp"

//- Some helpers for debugging/testing.  Not for production.
//
#include "linear_algebra/debug_helpers.hpp"

//- Some more implementation headers.
//
#include "linear_algebra/arithmetic_traits.hpp"
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

#endif  //- LINEAR_ALGEBRA_HPP_DEFINED
