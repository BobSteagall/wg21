#ifndef LINEAR_ALGEBRA_HPP_DEFINED
#define LINEAR_ALGEBRA_HPP_DEFINED

#include <cstdint>
#include <complex>
#include <memory>
#include <tuple>
#include <type_traits>

//-------------------------------------------------------------------------------------------------
//- Namespace alternatives for testing and also for detecting/avoiding ADL issues.
//
#define STD_LA  la
//#define STD_LA  std
//#define STD_LA  std::math

//- Some helpers
//
#include "linear_algebra/debug_helpers.hpp"

//- OK, now include implementation headers.
//
#include "linear_algebra/forward_declarations.hpp"
#include "linear_algebra/element_promotion_traits.hpp"
#include "linear_algebra/dynamic_engines.hpp"
#include "linear_algebra/fixed_size_engines.hpp"
#include "linear_algebra/view_based_engines.hpp"
#include "linear_algebra/engine_promotion_traits.hpp"
#include "linear_algebra/helper_traits.hpp"
#include "linear_algebra/vector.hpp"
#include "linear_algebra/matrix.hpp"
#include "linear_algebra/arithmetic_traits.hpp"
#include "linear_algebra/operator_traits.hpp"
#include "linear_algebra/operator_promotion_traits.hpp"
#include "linear_algebra/arithmetic_operators.hpp"
#include "linear_algebra/library_aliases.hpp"

#endif  //- LINEAR_ALGEBRA_HPP_DEFINED
