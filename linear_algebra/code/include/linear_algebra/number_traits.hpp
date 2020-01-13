//==================================================================================================
//  File:       number_traits.hpp
//
//  Summary:    This header defines several public traits types for reporting the properties of
//              numeric types.  It also provides the primary template for the number_traits
//              customization point, intended to be specialize-able by users.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_NUMBER_TRAITS_HPP_DEFINED
#define LINEAR_ALGEBRA_NUMBER_TRAITS_HPP_DEFINED

namespace STD_LA {
namespace detail {
//--------------------------------------------------------------------------------------------------
//- New trait "is_complex<T>" to detect if a type is complex<T>.  Only here for now as a
//  convenience, as it is being proposed in a separate paper.
//--------------------------------------------------------------------------------------------------
//
template<class T>
struct is_complex : public false_type
{};

template<class T>
struct is_complex<complex<T>> : public true_type
{};

template<class T> inline constexpr 
bool    is_complex_v = is_complex<T>::value;

}       //- detail namespace
}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_NUMBER_TRAITS_HPP_DEFINED
