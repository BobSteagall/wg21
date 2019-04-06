#ifndef LINEAR_ALGEBRA_HPP_DEFINED
#define LINEAR_ALGEBRA_HPP_DEFINED

#include <cstdint>
#include <complex>
#include <memory>
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

//- Some helpers
//
#include "linear_algebra/debug_helpers.hpp"

//- OK, now include implementation headers.
//
#include "linear_algebra/forward_declarations.hpp"
#include "linear_algebra/number_traits.hpp"
#include "linear_algebra/element_traits.hpp"
#include "linear_algebra/engine_traits.hpp"
#include "linear_algebra/arithmetic_traits.hpp"
#include "linear_algebra/operation_traits.hpp"
#include "linear_algebra/dynamic_engines.hpp"
#include "linear_algebra/fixed_size_engines.hpp"
#include "linear_algebra/view_based_engines.hpp"
#include "linear_algebra/vector.hpp"
#include "linear_algebra/matrix.hpp"
#include "linear_algebra/arithmetic_operators.hpp"
#include "linear_algebra/library_aliases.hpp"

#endif  //- LINEAR_ALGEBRA_HPP_DEFINED
